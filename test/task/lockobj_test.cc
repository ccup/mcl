#include <cctest/cctest.h>
#include "mcl/task/lockobj.h"
#include "mcl/task/rwlock.h"
#include "mcl/task/thread.h"
#include "mcl/list/list.h"
#include <unistd.h>
#include <string>

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
            return (Foo*)MclLockObj_Create(sizeof(Foo), FooFactory::fooInit, &id);
        }

        static void destroy(Foo *f) {
        	MclLockObj_Delete(f, FooFactory::fooDestroy, NULL);
        }

    private:
        static MclStatus fooInit(void *p, void *arg) {
        	if (!p) return MCL_FAILURE;
        	auto id = (int*)arg;
        	auto result = new(p) Foo(*id);
        	return result ? MCL_SUCCESS : MCL_FAILURE;
        }

        static void fooDestroy(void *p, void *arg) {
        	auto foo = (Foo*)p;
        	foo->~Foo();
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

        void insert(Foo *foo) {
            MCL_ASSERT_VALID_PTR_VOID(foo);
            MCL_LOG_DBG("FooRepo: enter insert foo of id %d", foo->getId());
            MCL_LOCK_WRITE_SCOPE(rwlock) {
                MCL_LOG_DBG("FooRepo: begin insert foo of id %d", foo->getId());
                MclList_PushBack(foos, foo);
                MCL_LOG_DBG("FooRepo: end insert foo of id %d", foo->getId());
            }
        }

        void remove(int id) {
            MCL_ASSERT_TRUE_VOID(id != INVALID_ID);
            MCL_LOG_DBG("FooRepo: enter remove foo of id %d", id);
            MCL_LOCK_WRITE_SCOPE(rwlock) {
                MCL_LOG_DBG("FooRepo: begin remove foo of id %d", id);
                auto foo = removeById(id);
                MCL_ASSERT_VALID_PTR_VOID(foo);
                MCL_LOG_DBG("FooRepo: begin delete foo of id %d", id);
                MclLockObj_Lock(foo);
                MclLockObj_Unlock(foo);
                FooFactory::destroy(foo);
                MCL_LOG_DBG("FooRepo: end delete foo of id %d", id);
                MCL_LOG_DBG("FooRepo: end remove foo of id %d", id);
            }
        }

        Foo* get(int id) {
            MCL_LOG_DBG("FooRepo: enter get foo of id %d", id);
            MCL_LOCK_READ_AUTO(rwlock);
            MCL_LOG_DBG("FooRepo: begin get foo of id %d", id);
            MclListNode *node = NULL;
            MCL_LIST_FOREACH(foos, node) {
                auto foo = (Foo*)MclListNode_GetData(node);
                if (id == foo->getId()) {
                	MclLockObj_Lock(foo);
                    MCL_LOG_DBG("FooRepo: end get foo of id %d", id);
                    return foo;
                }
            }
            return NULL;
        }

        Foo* getFirst() {
            MCL_LOG_DBG("FooRepo: enter get first foo");
            MCL_LOCK_READ_AUTO(rwlock);
            MCL_LOG_DBG("FooRepo: begin get first foo");
            auto node = MclList_GetFirst(foos);
            auto foo = (Foo*)MclListNode_GetData(node);
            MCL_ASSERT_VALID_PTR_R(foo, NULL);
            MCL_LOG_DBG("FooRepo: begin lock first foo %d", foo->getId());
            MclLockObj_Lock(foo);
            MCL_LOG_DBG("FooRepo: end lock first foo %d", foo->getId());
            MCL_LOG_DBG("FooRepo: end get first foo %d", foo->getId());
            return foo;
        }

        int getFirstId() {
            MCL_LOG_DBG("FooRepo: enter get first foo id");
            MCL_LOCK_READ_AUTO(rwlock);
            MCL_LOG_DBG("FooRepo: begin get first foo id");
            auto node = MclList_GetFirst(foos);
            auto foo = (Foo*)MclListNode_GetData(node);
            MCL_LOG_DBG("FooRepo: end get first foo id %d", foo->getId());
            return foo->getId();
        }

        std::string toString() const {
            MCL_LOCK_READ_AUTO(rwlock);
            auto result = std::string();
            MclListNode *node = NULL;
            MCL_LIST_FOREACH(foos, node) {
            	auto foo = (Foo*)MclListNode_GetData(node);
                result += std::to_string(foo->getId());
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
        Foo* removeById(int id) {
            MclListNode *node = NULL;
            MclListNode *tmpNode = NULL;
            MCL_LIST_FOREACH_SAFE(this->foos, node, tmpNode) {
                auto foo = (Foo*)node->data;
                if (id == foo->getId()) {
                    MclList_RemoveNode(this->foos, node, NULL);
                    return foo;
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
            auto foo = fooRepo.getFirst();
            if (!foo) continue;
            sleep(2);
            auto id = foo->getId();
            if ((id < 0) || (id >= MAX_ID)) {
                MclLockObj_Unlock(foo);
                throw std::runtime_error("thread error");
            }
            MCL_LOG_INFO("service 1 end visit foo of id %d", id);
            MclLockObj_Unlock(foo);
        }
        return NULL;
    }

    void* FooVisitService2(void*) {
        for (int i = 0; i < MAX_ID; i++)  {
            MCL_LOG_INFO("service 2 begin visit foo");
            auto foo = fooRepo.get(i);
            if (!foo) continue;
            sleep(1);
            auto id = foo->getId();
            if ((id < 0) || (id >= MAX_ID)) {
            	MclLockObj_Unlock(foo);
                throw std::runtime_error("thread error");
            }
            MCL_LOG_INFO("service 2 end visit foo of id %d", id);
            MclLockObj_Unlock(foo);
        }
        return NULL;
    }
}

FIXTURE(LockObjTest) {
    BEFORE {
        FOO_COUNT = 0;
    }

    AFTER {
        ASSERT_EQ(0, FOO_COUNT.load());
    }

    TEST("should not crash when multi-threads running") {
        MclThread t1, t2, t3, t4;

        MclThread_Create(&t1, NULL, FooCreateService, NULL);
        MclThread_Create(&t2, NULL, FooVisitService1, NULL);
    	sleep(1);
        MclThread_Create(&t3, NULL, FooDeleteService, NULL);
    	sleep(1);
        MclThread_Create(&t4, NULL, FooVisitService2, NULL);

        MclThread_Join(t1, NULL);
        MclThread_Join(t2, NULL);
        MclThread_Join(t3, NULL);
        MclThread_Join(t4, NULL);

        ASSERT_TRUE(fooRepo.isEmpty());
    }
};
