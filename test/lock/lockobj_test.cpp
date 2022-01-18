#include <cctest/cctest.h>
#include "mcl/lock/lockobj.h"
#include "lock/foo_utils/foo.h"
#include "lock/foo_utils/foo_factory.h"
#include "mcl/lock/rwlock.h"
#include "mcl/thread/thread.h"
#include "mcl/list/list.h"
#include <unistd.h>
#include <string>

namespace {
    class FooRepo {
        MclList *foos;
        MclRwLock rwlock;

    public:
        FooRepo() {
            foos = MclList_CreateDefault();
            MclRwLock_Init(&rwlock, NULL);
        }

        ~FooRepo() {
            MclList_Delete(foos, (MclListDataDestroy)FooFactory<FooCreateType::LOCKOBJ>::destroy);
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

        void remove(FooId id) {
            MCL_ASSERT_TRUE_VOID(id != FOO_ID_INVALID);
            MCL_LOG_DBG("FooRepo: enter remove foo of id %d", id);
            MCL_LOCK_WRITE_SCOPE(rwlock) {
                MCL_LOG_DBG("FooRepo: begin remove foo of id %d", id);
                auto foo = removeById(id);
                MCL_ASSERT_VALID_PTR_VOID(foo);
                MCL_LOG_DBG("FooRepo: begin delete foo of id %d", id);
                FooFactory<FooCreateType::LOCKOBJ>::destroy(foo);
                MCL_LOG_DBG("FooRepo: end delete foo of id %d", id);
                MCL_LOG_DBG("FooRepo: end remove foo of id %d", id);
            }
        }

        Foo* get(FooId id) {
            MCL_LOG_DBG("FooRepo: enter get foo of id %d", id);
            MCL_LOCK_READ_AUTO(rwlock);
            MCL_LOG_DBG("FooRepo: begin get foo of id %d", id);
            MclListNode *node = NULL;
            MCL_LIST_FOREACH(foos, node) {
                auto foo = (Foo*)MclListNode_GetData(node);
                if (id == foo->getId()) {
                	MclLockObj_WrLock(foo);
                    MCL_LOG_DBG("FooRepo: end get foo of id %d", id);
                    return foo;
                }
            }
            return NULL;
        }

        const Foo* getConst(FooId id) {
            MCL_LOG_DBG("FooRepo: enter get const foo of id %d", id);
            MCL_LOCK_READ_AUTO(rwlock);
            MCL_LOG_DBG("FooRepo: begin get const foo of id %d", id);
            MclListNode *node = NULL;
            MCL_LIST_FOREACH(foos, node) {
                auto foo = (Foo*)MclListNode_GetData(node);
                if (id == foo->getId()) {
                	MclLockObj_RdLock(foo);
                    MCL_LOG_DBG("FooRepo: end get const foo of id %d", id);
                    return foo;
                }
            }
            return NULL;
        }

        const Foo* getFirst() {
            MCL_LOG_DBG("FooRepo: enter get first foo");
            MCL_LOCK_READ_AUTO(rwlock);
            MCL_LOG_DBG("FooRepo: begin get first foo");
            auto node = MclList_GetFirst(foos);
            auto foo = (Foo*)MclListNode_GetData(node);
            MCL_ASSERT_VALID_PTR_NIL(foo);
            MCL_LOG_DBG("FooRepo: begin lock first foo %d", foo->getId());
            MclLockObj_RdLock(foo);
            MCL_LOG_DBG("FooRepo: end lock first foo %d", foo->getId());
            MCL_LOG_DBG("FooRepo: end get first foo %d", foo->getId());
            return foo;
        }

        FooId getFirstId() {
            MCL_LOG_DBG("FooRepo: enter get first foo id");
            MCL_LOCK_READ_AUTO(rwlock);
            MCL_LOG_DBG("FooRepo: begin get first foo id");
            auto node = MclList_GetFirst(foos);
            MCL_ASSERT_VALID_PTR_R(node, FOO_ID_INVALID);
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
        Foo* removeById(FooId id) {
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

    constexpr FooId MAX_ID = 10;

    void* FooCreateService(void*) {
        for (FooId id = 0; id < MAX_ID; id++) {
            MCL_LOG_INFO("service begin insert foo of id %d", id);
            auto f = FooFactory<FooCreateType::LOCKOBJ>::create(id);
            MCL_ASSERT_VALID_PTR_NIL(f);
            fooRepo.insert(f);
            MCL_LOG_INFO("service end insert foo of id %d", id);
            sleep(1);
        }
        MCL_LOG_SUCC("Create service exit!");
        return NULL;
    }

    void* FooDeleteService(void*) {
    	uint16_t tryCount = 0;
        while (true) {
            MCL_LOG_INFO("service begin remove foo");
            auto id = fooRepo.getFirstId();
			if (id == FOO_ID_INVALID) {
				sleep(1);
				if (++tryCount >= 3) {
					break;
				}
				continue;
			}
			sleep(1);
			uint16_t tryCount = 0;
            fooRepo.remove(id);
            MCL_LOG_INFO("service end remove foo of id %d", id);
            sleep(2);
        }
        MCL_LOG_SUCC("Delete service exit!");
        return NULL;
    }

    void* FooVisitService1(void*) {
        uint16_t tryCount = 0;
        while (true) {
            MCL_LOG_INFO("service 1 begin visit foo");
            MCL_LOCK_OBJ_AUTO auto foo = fooRepo.getFirst();
            if (!foo) {
            	sleep(1);
            	if (++tryCount >= 3) {
            		break;
            	}
            	continue;
            }
            sleep(1);
            uint16_t tryCount = 0;
            auto id = foo->getId();
            if ((id < 0) || (id >= MAX_ID)) {
                throw std::runtime_error("thread error");
            }
            MCL_LOG_INFO("service 1 end visit foo of id %d", id);
        }
        MCL_LOG_SUCC("Visit Service 1 exit!");
        return NULL;
    }

    void* FooVisitService2(void*) {
        for (FooId i = 0; i < MAX_ID; i++)  {
            MCL_LOG_INFO("service 2 begin visit foo");
            MCL_LOCK_OBJ_AUTO auto foo = fooRepo.getConst(i);
            if (!foo) {
            	sleep(1);
            	continue;
            }
            sleep(1);
            auto id = foo->getId();
            if ((id < 0) || (id >= MAX_ID)) {
                throw std::runtime_error("thread error");
            }
            MCL_LOG_INFO("service 2 end visit foo of id %d", id);
        }
        MCL_LOG_SUCC("Visit Service 2 exit!");
        return NULL;
    }
}

FIXTURE(LockObjTest) {
    BEFORE {
        MclAtom_Clear(&Foo::FOO_COUNT);
    }

    AFTER {
        ASSERT_EQ(0, Foo::FOO_COUNT);
    }

    TEST("should not crash when multi-threads running") {
        MclThread t1, t2, t3, t4;

        MclThread_Create(&t1, NULL, FooCreateService, NULL);
        MclThread_Create(&t2, NULL, FooVisitService1, NULL);
        MclThread_Create(&t3, NULL, FooDeleteService, NULL);
        MclThread_Create(&t4, NULL, FooVisitService2, NULL);

        MclThread_Join(t1, NULL);
        MclThread_Join(t2, NULL);
        MclThread_Join(t3, NULL);
        MclThread_Join(t4, NULL);

        ASSERT_TRUE(fooRepo.isEmpty());
    }
};
