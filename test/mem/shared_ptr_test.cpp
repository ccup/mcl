#include <cctest/cctest.h>
#include "mcl/mem/shared_ptr.h"
#include "foo_utils//foo_factory.h"

FIXTURE(SharedPtrTest) {
    TEST("should create shared ptr") {
        MCL_SHARED_AUTO int *p = MCL_SHARED_PTR(int, 3);

        ASSERT_TRUE(p != NULL);
        ASSERT_EQ(3, *p);
    }
};

namespace {
    struct Obj {
        int x;
        char y;
    };

    void Obj_Destruct(void *ptr, void* arg) {
        auto count = (uint32_t*)arg;
        (*count)++;
    }

    void Obj_DestructSum(void *ptr, void* arg) {
        auto obj = (Obj*)ptr;
        auto sum = (uint32_t*)arg;
        (*sum) += obj->x;
    }
}

FIXTURE(SharedPtrReleaseTest) {
    uint32_t releaseCount {0};
    Obj *obj {nullptr};

    SharedPtrReleaseTest() {
        obj = MCL_SHARED_PTR(Obj, {.x = 3, .y = 2}, Obj_Destruct, &releaseCount);
    }

    AFTER {
        ASSERT_EQ(0, releaseCount);
    }

    TEST("should release early when not increase ref") {
        MCL_SHARED_AUTO Obj *obj1 = MCL_SHARED_REF(Obj, obj);
        ASSERT_EQ(3, obj1->x);
    }
};

FIXTURE(SharedPtrEarlyReleaseTest) {
    uint32_t releaseCount {0};
    Obj *obj {nullptr};

    SharedPtrEarlyReleaseTest() {
        obj = MCL_SHARED_PTR(Obj, {.x = 3, .y = 2}, Obj_Destruct, &releaseCount);
    }

    AFTER {
        ASSERT_EQ(1, releaseCount);
    }

    TEST("should release early when not increase ref") {
        MCL_SHARED_AUTO Obj *obj1 = obj;
        ASSERT_EQ(3, obj1->x);
    }
};

FIXTURE(SharedPtrDestructTest) {
    uint32_t sumX = 0;

    BEFORE {
        sumX = 0;
    }

    AFTER {
        ASSERT_EQ(3, sumX);
    };

    TEST("should create shared ptr by destructor args") {
        MCL_SHARED_AUTO Obj *p = MCL_SHARED_PTR(Obj, {.x = 3, .y = 2}, Obj_DestructSum, &sumX);

        ASSERT_TRUE(p != NULL);
        ASSERT_EQ(3, p->x);
        ASSERT_EQ(2, p->y);
    }

    TEST("should ref shared ptr but not destruct when ref count is not zero") {
        MCL_SHARED_AUTO Obj *p1 = MCL_SHARED_PTR(Obj, {.x = 3, .y = 2}, Obj_DestructSum, &sumX);
        ASSERT_TRUE(p1 != NULL);
        {
            MCL_SHARED_AUTO Obj *p2 = MCL_SHARED_REF(Obj, p1);
            ASSERT_EQ(3, p2->x);
            ASSERT_EQ(2, p2->y);
        }

        ASSERT_EQ(3, p1->x);
        ASSERT_EQ(2, p1->y);
    }
};

FIXTURE(SharedPtrAutoReleaseTest) {
    BEFORE {
        Foo::FOO_COUNT = 0;
    }

    AFTER {
        ASSERT_EQ(0, Foo::FOO_COUNT.load());
    }

    TEST("should auto release memory") {
        MCL_SHARED_AUTO Foo *f = FooFactory<FooCreateType::SHARED_PTR>::create(1);
        ASSERT_EQ(1, f->getId());
        ASSERT_EQ(1, Foo::FOO_COUNT.load());
    }

    TEST("should auto release memory when ref count become zero") {
        MCL_SHARED_AUTO Foo *f1 = FooFactory<FooCreateType::SHARED_PTR>::create(1);
        MCL_SHARED_AUTO Foo *f2 = (Foo*)MclSharedPtr_Ref(f1);
        ASSERT_EQ(f1, f2);
        ASSERT_EQ(1, Foo::FOO_COUNT.load());
    }

    TEST("should not release memory when ref count is not zero") {
        MCL_SHARED_AUTO Foo *f1 = FooFactory<FooCreateType::SHARED_PTR>::create(1);
        {
            MCL_SHARED_AUTO Foo *f2 = MCL_SHARED_REF(Foo, f1);
            ASSERT_EQ(f1, f2);
            ASSERT_EQ(1, f2->getId());
        }
        ASSERT_EQ(1, f1->getId());
        ASSERT_EQ(1, Foo::FOO_COUNT.load());
    }
};
