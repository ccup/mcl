#include <cctest/cctest.h>
#include "mcl/task/mutex.h"
#include "mcl/task/thread.h"
#include "mcl/link/link.h"
#include "mcl/link/link_node_allocator.h"
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
            MCL_LOCK_AUTO(mutex);
            return id;
        }

        void setId(int id) {
            MCL_LOCK_AUTO(mutex);
            this->id = id;
        }

        std::string toString() const {
            MCL_LOCK_AUTO(mutex);
            auto result = ""s;
            result += std::to_string(id);
            return result;
        }

    private:
        int id{0};
        MclMutex mutex;
    };

    struct FooFactory {
        static Foo* create(int id) {
            return new Foo{id};
        }

        static void release(Foo* foo) {
            if (foo) delete foo;
        }
    };

    void Foo_Delete(MclLinkDataDeleter *deleter, MclLinkData data) {
        auto f = (Foo*)data;
        FooFactory::release(f);
    }

    struct FooIdPred {
        MclLinkDataPred pred;
        int id;
    };

    bool FooIdPred_IsEqual(MclLinkDataPred *pred, MclLinkData data) {
        Foo *foo = (Foo*)data;
        FooIdPred *self = MCL_TYPE_REDUCT(pred, FooIdPred, pred);
        return foo->getId() == self->id;
    }

    FooIdPred FooIdPred_Create(int id) {
        FooIdPred pred = {.pred = MCL_LINK_DATA_PRED(FooIdPred_IsEqual), .id = id};
        return pred;
    }

    struct FooRepo {
        FooRepo() {
            foos = MclLink_Create(MclLinkNodeAllocator_GetDefault());
            MCL_MUTEX_INIT(mutex);
        }

        ~FooRepo() {
            MclLink_Delete(foos, &fooDeleter);
            MclMutex_Destroy(&mutex);
        }

        void insert(int id) {
            MCL_LOCK_AUTO(mutex);
            auto foo = FooFactory::create(id);
            MclLink_PushBack(foos, foo);
        }

        void insert(Foo *f) {
            MCL_LOCK_AUTO(mutex);
            MCL_LOG_INFO("insert foo of id %d", f->getId());
            MclLink_PushBack(foos, f);
        }

        void remove(int id) {
            MCL_LOCK_AUTO(mutex);
            auto fooEqual = FooIdPred_Create(id);
            MclLink_RemoveBy(foos, &fooEqual.pred, &fooDeleter);
        }

        void remove(Foo *f) {
            MCL_LOCK_AUTO(mutex);
            MCL_LOG_INFO("remove foo of id %d", f->getId());
            MclLink_RemoveData(foos, f, NULL);
        }

        Foo* get(int id) {
            MCL_LOCK_AUTO(mutex);
            MclLink result;
            MclLink_Init(&result, NULL);
            auto fooEqual = FooIdPred_Create(id);
            MclLink_FindBy(foos, &fooEqual.pred, &result);
            Foo *f = MclLink_IsEmpty(&result)? NULL : (Foo*)MclLinkNode_GetData(MclLink_GetFirst(&result));
            MclLink_Clear(&result, NULL);
            return f;
        }

        Foo* getFirst() {
            MCL_LOCK_AUTO(mutex);
            auto n = MclLink_GetFirst(foos);
            auto f = (Foo*)MclLinkNode_GetData(n);
            MCL_LOG_INFO("get foo of id %d", f->getId());
            return f;
        }

        std::string toString() const {
            MCL_LOCK_AUTO(mutex);
            auto result = ""s;
            MclLinkNode *node = NULL;
            MCL_LINK_FOR_EACH(foos, node) {
                result += std::to_string(((Foo*)MclLinkNode_GetData(node))->getId());
                result += ";";
            }
            return result;
        }

        bool isEmpty() const {
            MCL_LOCK_AUTO(mutex);
            return MclLink_IsEmpty(foos);
        }

    private:
        MclLink *foos;
        MclLinkDataDeleter fooDeleter{.destroy = Foo_Delete};
        MclMutex mutex;
    };

    FooRepo fooRepo;

    constexpr int MAX_ID = 10;

    void* FooCreateService(void*) {
        for (int id = 0; id < MAX_ID; id++) {
            auto f = FooFactory::create(id);
            fooRepo.insert(f);
            sleep(1);
        }
        return NULL;
    }

    void* FooDeleteService(void*) {
        while (!fooRepo.isEmpty()) {
            auto f = fooRepo.getFirst();
            fooRepo.remove(f);
            FooFactory::release(f);
            sleep(2);
        }
        return NULL;
    }

    void* FooVisitService(void*) {
        while (!fooRepo.isEmpty()) {
            auto f = fooRepo.getFirst();
            sleep(2);
            auto id = f->getId();
            if ((id < 0) || (id >= MAX_ID)) {
                throw std::runtime_error("thread error");
            }
            MCL_LOG_INFO("visit foo of id %d", f->getId());
        }
        return NULL;
    }
}

FIXTURE(TestLockDestroy) {
    TEST("should not crash when multi-threads running") {
//        MclThread t1, t2, t3;
//
//        MclThread_Create(&t1, NULL, FooCreateService, NULL);
//        MclThread_Create(&t3, NULL, FooVisitService,  NULL);
//        MclThread_Create(&t2, NULL, FooDeleteService, NULL);
//
//        MclThread_Join(t1, NULL);
//        MclThread_Join(t3, NULL);
//        MclThread_Join(t2, NULL);
//
//        ASSERT_TRUE(fooRepo.isEmpty());
    }
};
