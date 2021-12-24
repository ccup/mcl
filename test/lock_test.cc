#include <cctest/cctest.h>
#include "mcl/task/mutex.h"
#include "mcl/task/rwlock.h"
#include "mcl/task/thread.h"
#include "mcl/list/list.h"
#include "mcl/list/list_node_allocator.h"
#include <string>
#include <unistd.h>

using namespace std::string_literals;

namespace {
    struct Foo {
        Foo(int id) : id{id} {
            MCL_MUTEX_INIT(mutex);
        }

        ~Foo() {
            MclMutex_Destroy(&mutex);
        }

        int getId() const {
//            MCL_LOCK_AUTO(mutex);
            return id;
        }

        std::string toString() const {
//            MCL_LOCK_AUTO(mutex);
            auto result = ""s;
            result += std::to_string(id);
            return result;
        }

        void lock() {
            MclMutex_Lock(&mutex);
        }

        void unlock() {
            MclMutex_Unlock(&mutex);
        }

    private:
        int id{0};
        MclMutex mutex;
    };

    void Foo_Delete(MclListDataDeleter *deleter, MclListData data) {
        auto f = (Foo*)data;
        if (f) delete f;
    }

    struct FooIdPred {
        MclListDataPred pred;
        int id;
    };

    bool FooIdPred_IsEqual(MclListDataPred *pred, MclListData data) {
        Foo *foo = (Foo*)data;
        FooIdPred *self = MCL_TYPE_REDUCT(pred, FooIdPred, pred);
        return foo->getId() == self->id;
    }

    class FooRepo {
        MclList *foos;
        MclRwLock rwlock;

        MclListDataDeleter fooDeleter{.destroy = Foo_Delete};

    public:
        FooRepo() {
            foos = MclList_Create(MclListNodeAllocator_GetDefault());
            MCL_RWLOCK_INIT(rwlock);
        }

        ~FooRepo() {
            MclList_Delete(foos, &fooDeleter);
            MclRwLock_Destroy(&rwlock);
        }

        void insert(int id) {
            MCL_WRLOCK_AUTO(rwlock);
            auto foo = new Foo{id};
            MclList_PushBack(foos, foo);
        }

        void remove(int id) {
            MCL_WRLOCK_AUTO(rwlock);
            FooIdPred fooEqual = {.pred = MCL_LIST_DATA_PRED(FooIdPred_IsEqual), .id = id};

            MclList_RemoveBy(foos, &fooEqual.pred, &fooDeleter);
        }

        Foo* get(int id) {
            MCL_RDLOCK_AUTO(rwlock);
            MclListNode *node = NULL;
            MCL_LIST_FOREACH(foos, node) {
                auto f = (Foo*)MclListNode_GetData(node);
                if (id == f->getId()) {
                    return f;
                }
            }
            return nullptr;
        }

        Foo* getFirst() {
            MCL_RDLOCK_AUTO(rwlock);
            auto node = MclList_GetFirst(foos);
            auto f = (Foo*)MclListNode_GetData(node);
            MCL_LOG_INFO("get foo of id %d", f->getId());
            return f;
        }

        int getFirstId() {
            MCL_RDLOCK_AUTO(rwlock);
            auto node = MclList_GetFirst(foos);
            auto f = (Foo*)MclListNode_GetData(node);
            MCL_LOG_INFO("get foo of id %d", f->getId());
            return f->getId();
        }

        std::string toString() const {
            MCL_RDLOCK_AUTO(rwlock);
            auto result = ""s;
            MclListNode *node = NULL;
            MCL_LIST_FOREACH(foos, node) {
                result += std::to_string(((Foo*)MclListNode_GetData(node))->getId());
                result += ";";
            }
            return result;
        }

        bool isEmpty() const {
            MCL_RDLOCK_AUTO(rwlock);
            return MclList_IsEmpty(foos);
        }
    } fooRepo;

    constexpr int MAX_ID = 10;

    void* FooCreateService(void*) {
        for (int id = 0; id < MAX_ID; id++) {
            fooRepo.insert(id);
            sleep(1);
        }
        return NULL;
    }

    void* FooDeleteService(void*) {
        while (!fooRepo.isEmpty()) {
            int id = fooRepo.getFirstId();
            fooRepo.remove(id);
            sleep(2);
        }
        return NULL;
    }

    void* FooVisitService(void*) {
        while (!fooRepo.isEmpty()) {
            auto f = fooRepo.getFirst();
            sleep(2);
            auto id = f->getId();
//            auto id = fooRepo.getFirstId();
            if ((id < 0) || (id >= MAX_ID)) {
                throw std::runtime_error("thread error");
            }
            MCL_LOG_INFO("visit foo of id %d", id);
        }
        return NULL;
    }
}

FIXTURE(LockTest) {
    TEST("should not crash when multi-threads running") {
        MclThread t1, t2, t3;

        MclThread_Create(&t1, NULL, FooCreateService, NULL);
        MclThread_Create(&t3, NULL, FooVisitService,  NULL);
        MclThread_Create(&t2, NULL, FooDeleteService, NULL);

        MclThread_Join(t1, NULL);
        MclThread_Join(t3, NULL);
        MclThread_Join(t2, NULL);

        ASSERT_TRUE(fooRepo.isEmpty());
    }
};
