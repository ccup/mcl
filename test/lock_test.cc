#include <cctest/cctest.h>
#include "mcl/task/lockptr.h"
#include "mcl/task/rwlock.h"
#include "mcl/task/thread.h"
#include "mcl/list/list.h"
#include <unistd.h>
#include <string>

using namespace std::string_literals;

namespace {
    constexpr int INVALID_ID = 0xFFFFFFFF;

    std::atomic<uint16_t> FOO_COUNT {0};

    struct Foo {
        Foo(int id) : id{id} {
            FOO_COUNT++;
        }

        ~Foo() {
            id = INVALID_ID;
            FOO_COUNT--;
        }

        int getId() const {
            return id;
        }

        std::string toString() const {
            return std::to_string(id);
        }

    private:
        int id {INVALID_ID};
    };

    struct FooFactory {
        static Foo* create(int id) {
            return new Foo{id};
        }

        static void destroy(Foo *f) {
            delete f;
        }
    };

    void Foo_Delete(MclListDataDeleter *deleter, MclListData data) {
        auto f = (Foo*)data;
        if (f) FooFactory::destroy(f);
    }

    class FooRepo {
        MclList *foos;
        MclRwLock rwlock;

    public:
        FooRepo() {
            foos = MclList_CreateDefault();
            MCL_RWLOCK_INIT(rwlock);
        }

        ~FooRepo() {
            MclListDataDeleter fooDeleter{.destroy = Foo_Delete};
            MclList_Delete(foos, &fooDeleter);
            MclRwLock_Destroy(&rwlock);
        }

        void insert(Foo *f) {
            MCL_ASSERT_VALID_PTR_VOID(f);
            MCL_LOG_DBG("FooRepo: enter insert foo of id %d", f->getId());
            MCL_LOCK_WRITE_SCOPE(rwlock) {
                MCL_LOG_DBG("FooRepo: begin insert foo of id %d", f->getId());
                MclLockPtr *fptr = MclLockPtr_Create(f);
                MCL_ASSERT_VALID_PTR_VOID(fptr);
                MclList_PushBack(foos, fptr);
                MCL_LOG_DBG("FooRepo: end insert foo of id %d", f->getId());
            }
        }

        void remove(int id) {
            MCL_ASSERT_TRUE_VOID(id != INVALID_ID);
            MCL_LOG_DBG("FooRepo: enter remove foo of id %d", id);
            MCL_LOCK_WRITE_SCOPE(rwlock) {
                MCL_LOG_DBG("FooRepo: begin remove foo of id %d", id);
                auto result = removeById(id);
                MCL_ASSERT_VALID_PTR_VOID(result);
                MCL_LOG_DBG("FooRepo: begin delete foo of id %d", id);
                MclLockPtr_Delete(result, (MclLockPtrDeleter)FooFactory::destroy);
                MCL_LOG_DBG("FooRepo: end delete foo of id %d", id);
                MCL_LOG_DBG("FooRepo: end remove foo of id %d", id);
            }
        }

        MclLockPtr* get(int id) {
            MCL_LOG_DBG("FooRepo: enter get foo of id %d", id);
            MCL_LOCK_READ_AUTO(rwlock);
            MCL_LOG_DBG("FooRepo: begin get foo of id %d", id);
            MclListNode *node = NULL;
            MCL_LIST_FOREACH(foos, node) {
                auto fptr = (MclLockPtr*)MclListNode_GetData(node);
                auto f = (Foo*)MclLockPtr_Get(fptr);
                if (id == f->getId()) {
                    MclLockPtr_Lock(fptr);
                    MCL_LOG_DBG("FooRepo: end get foo of id %d", id);
                    return fptr;
                }
            }
            return NULL;
        }

        MclLockPtr* getFirst() {
            MCL_LOG_DBG("FooRepo: enter get first foo");
            MCL_LOCK_READ_AUTO(rwlock);
            MCL_LOG_DBG("FooRepo: begin get first foo");
            auto node = MclList_GetFirst(foos);
            auto fptr = (MclLockPtr*)MclListNode_GetData(node);
            MCL_ASSERT_VALID_PTR_R(fptr, NULL);
            auto id = ((Foo*)MclLockPtr_Get(fptr))->getId();
            MCL_LOG_DBG("FooRepo: begin lock first foo %d", id);
            MclLockPtr_Lock(fptr);
            MCL_LOG_DBG("FooRepo: end lock first foo %d", id);
            MCL_LOG_DBG("FooRepo: end get first foo %d", id);
            return fptr;
        }

        int getFirstId() {
            MCL_LOG_DBG("FooRepo: enter get first foo id");
            MCL_LOCK_READ_AUTO(rwlock);
            MCL_LOG_DBG("FooRepo: begin get first foo id");
            auto node = MclList_GetFirst(foos);
            auto fptr = (MclLockPtr*)MclListNode_GetData(node);
            auto f = (Foo*)MclLockPtr_Get(fptr);
            MCL_LOG_DBG("FooRepo: end get first foo id %d", f->getId());
            return f->getId();
        }

        std::string toString() const {
            MCL_LOCK_READ_AUTO(rwlock);
            auto result = ""s;
            MclListNode *node = NULL;
            MCL_LIST_FOREACH(foos, node) {
                auto fptr = (MclLockPtr*)MclListNode_GetData(node);
                auto f = (Foo*)MclLockPtr_Get(fptr);
                result += std::to_string(f->getId());
                result += ";";
            }
            MCL_LOG_DBG("FooRepo: toString");
            return result;
        }

        bool isEmpty() const {
            MCL_LOCK_READ_AUTO(rwlock);
            return MclList_IsEmpty(foos);
        }

    private:
        MclLockPtr* removeById(int id) {
            MclListNode *node = NULL;
            MclListNode *tmpNode = NULL;
            MCL_LIST_FOREACH_SAFE(this->foos, node, tmpNode) {
                auto fptr = (MclLockPtr*)node->data;
                if (id == ((Foo*)MclLockPtr_Get(fptr))->getId()) {
                    MclList_RemoveNode(this->foos, node, NULL);
                    return fptr;
                }
            }
            return NULL;
        }

    } fooRepo;

    constexpr int MAX_ID = 10;

    void* FooCreateService(void*) {
        for (int id = 0; id < MAX_ID; id++) {
            MCL_LOG_INFO("service begin insert foo of id %d", id);
            auto f = FooFactory::create(id);
            MCL_ASSERT_VALID_PTR_NIL(f);
            fooRepo.insert(f);
            MCL_LOG_INFO("service end insert foo of id %d", id);
            sleep(1);
        }
        return NULL;
    }

    void* FooDeleteService(void*) {
        while (!fooRepo.isEmpty()) {
            MCL_LOG_INFO("service begin remove foo");
            auto id = fooRepo.getFirstId();
            fooRepo.remove(id);
            MCL_LOG_INFO("service end remove foo of id %d", id);
            sleep(2);
        }
        return NULL;
    }

    void* FooVisitService1(void*) {
        while (!fooRepo.isEmpty()) {
            MCL_LOG_INFO("service 1 begin visit foo");
            auto fptr = fooRepo.getFirst();
            if (!fptr) continue;
            auto f = (Foo*)MclLockPtr_Get(fptr);
            sleep(2);
            auto id = f->getId();
            if ((id < 0) || (id >= MAX_ID)) {
                MclLockPtr_Unlock(fptr);
                throw std::runtime_error("thread error");
            }
            MCL_LOG_INFO("service 1 end visit foo of id %d", id);
            MclLockPtr_Unlock(fptr);
        }
        return NULL;
    }

    void* FooVisitService2(void*) {
        for (int i = 0; i < MAX_ID; i++)  {
            MCL_LOG_INFO("service 2 begin visit foo");
            auto fptr = fooRepo.get(i);
            if (!fptr) continue;
            auto f = (Foo*)MclLockPtr_Get(fptr);
            sleep(1);
            auto id = f->getId();
            if ((id < 0) || (id >= MAX_ID)) {
                MclLockPtr_Unlock(fptr);
                throw std::runtime_error("thread error");
            }
            MCL_LOG_INFO("service 2 end visit foo of id %d", id);
            MclLockPtr_Unlock(fptr);
        }
        return NULL;
    }
}


FIXTURE(LockTest) {
    BEFORE {
        FOO_COUNT = 0;
    }

    AFTER {
        ASSERT_EQ(0, FOO_COUNT.load());
    }

    TEST("should not crash when multi-threads running") {
        MclThread t1, t2, t3, t4;

        MclThread_Create(&t1, NULL, FooCreateService, NULL);
        MclThread_Create(&t3, NULL, FooVisitService1,  NULL);
        MclThread_Create(&t2, NULL, FooDeleteService, NULL);
        MclThread_Create(&t4, NULL, FooVisitService2, NULL);

        MclThread_Join(t1, NULL);
        MclThread_Join(t3, NULL);
        MclThread_Join(t2, NULL);
        MclThread_Join(t4, NULL);

        ASSERT_TRUE(fooRepo.isEmpty());
    }
};
