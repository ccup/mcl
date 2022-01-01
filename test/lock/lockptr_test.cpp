#include <cctest/cctest.h>
#include "mcl/lock/lockptr.h"
#include "lock/foo_utils/foo_factory.h"
#include "mcl/lock/rwlock.h"
#include "mcl/thread/thread.h"
#include "mcl/list/list.h"
#include <unistd.h>
#include <string>

using namespace std::string_literals;

namespace {
    void FooLockPtr_DeletePtr(Foo *f, void* arg) {
        FooFactory<FooCreateType::NORMAL>::destroy(f);
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
            MclListDataDeleter fooDeleter{.destroy = Foo_ListDelete<FooCreateType::NORMAL>};
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
            MCL_ASSERT_TRUE_VOID(id != FOO_ID_INVALID);
            MCL_LOG_DBG("FooRepo: enter remove foo of id %d", id);
            MCL_LOCK_WRITE_SCOPE(rwlock) {
                MCL_LOG_DBG("FooRepo: begin remove foo of id %d", id);
                auto result = removeById(id);
                MCL_ASSERT_VALID_PTR_VOID(result);
                MCL_LOG_DBG("FooRepo: begin delete foo of id %d", id);
                MclLockPtr_Delete(result, (MclLockPtrDeleter)FooLockPtr_DeletePtr, NULL);
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
            MCL_ASSERT_VALID_PTR_NIL(node);
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
            MCL_ASSERT_VALID_PTR_R(node, FOO_ID_INVALID);
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
            auto f = FooFactory<FooCreateType::NORMAL>::create(id);
            MCL_ASSERT_VALID_PTR_NIL(f);
            fooRepo.insert(f);
            MCL_LOG_INFO("service end insert foo of id %d", id);
            sleep(1);
        }
        MCL_LOG_SUCC("Create service  exit!");
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
        MCL_LOG_SUCC("Delete service  exit!");
        return NULL;
    }

    void* FooVisitService1(void*) {
        uint16_t tryCount = 0;
        while (true) {
            MCL_LOG_INFO("service 1 begin visit foo");
            MCL_UNLOCK_PTR_AUTO MclLockPtr *fptr = fooRepo.getFirst();
            if (!fptr) {
            	sleep(1);
            	if (++tryCount >= 3) {
            		break;
            	}
            	continue;
            }
            sleep(2);
            uint16_t tryCount = 0;
            auto f = (Foo*)MclLockPtr_Get(fptr);
            sleep(2);
            auto id = f->getId();
            if ((id < 0) || (id >= MAX_ID)) {
                throw std::runtime_error("thread error");
            }
            MCL_LOG_INFO("service 1 end visit foo of id %d", id);
        }
        MCL_LOG_SUCC("Visit service 1 exit!");
        return NULL;
    }

    void* FooVisitService2(void*) {
        for (int i = 0; i < MAX_ID; i++)  {
            MCL_LOG_INFO("service 2 begin visit foo");
            MCL_UNLOCK_PTR_AUTO MclLockPtr *fptr = fooRepo.get(i);
            if (!fptr) {
            	sleep(1);
            	continue;
            }
            auto f = (Foo*)MclLockPtr_Get(fptr);
            sleep(1);
            auto id = f->getId();
            if ((id < 0) || (id >= MAX_ID)) {
                throw std::runtime_error("thread error");
            }
            MCL_LOG_INFO("service 2 end visit foo of id %d", id);
        }
        MCL_LOG_SUCC("Visit service 2 exit!");
        return NULL;
    }
}

FIXTURE(LockPtrTest) {
    BEFORE {
        Foo::FOO_COUNT = 0;
    }

    AFTER {
        ASSERT_EQ(0, Foo::FOO_COUNT.load());
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
