#include <cctest/cctest.h>
#include "mcl/task/mutex.h"
#include "mcl/link/link.h"
#include <string>

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
            return id;
        }

    private:
        int id{0};
        MclMutex mutex;
    };

    struct FooFactory {
        static Foo* create(int id) {
            return new Foo{id};
        }

        static void Delete(Foo* foo) {
            if (foo) delete foo;
        }
    };

    bool Foo_IsEq(MclLinkData data, void *arg) {
        Foo *foo = (Foo*)data;
        return foo->getId() == *(int*)arg;
    }

    struct FooRepo {
        FooRepo() {
            foos = MclLink_Create();
            MCL_MUTEX_INIT(mutex);
        }

        ~FooRepo() {
            MclLink_Delete(foos, (MclLinkDataDeleter) FooFactory::Delete);
            MclMutex_Destroy(&mutex);
        }

        void insert(int id) {
            MCL_LOCK_AUTO(mutex);
            auto foo = FooFactory::create(id);
            MclLink_PushBack(foos, foo);
        }

        void remove(int id) {
            MCL_LOCK_AUTO(mutex);
            MclLink_RemoveBy(foos, Foo_IsEq, &id, (MclLinkDataDeleter) FooFactory::Delete);
        }

        Foo* get(int id) {
            MCL_LOCK_AUTO(mutex);
            MclLink result;
            MclLink_Init(&result);
            MclLink_FindBy(foos, Foo_IsEq, &id, &result);
            Foo *f = MclLink_IsEmpty(&result)? NULL : (Foo*)MclLinkNode_GetData(MclLink_GetFirst(&result));
            MclLink_Clear(&result, NULL);
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

    private:
        MclLink *foos;
        MclMutex mutex;
    };
}

FIXTURE(TestLockDestroy) {
    FooRepo repo;

    TEST("") {
    }
};
