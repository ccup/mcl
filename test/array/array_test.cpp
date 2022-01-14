#include <cctest/cctest.h>
#include "mcl/array/array.h"

namespace {
    struct Foo {
        int id;
        int value;
    };
}

FIXTURE(ArrayTest) {
    constexpr static MclSize ARRAY_SIZE = 10;
    MclArray *array;

    BEFORE {
        array = MclArray_Create(ARRAY_SIZE, sizeof(Foo));
    };

    AFTER {
        MclArray_Delete(array);
    };

    TEST("should create array with buffer") {
        ASSERT_TRUE(array != NULL);
        ASSERT_TRUE(array->buff != NULL);
        ASSERT_EQ(ARRAY_SIZE, MclArray_GetCapacity(array));
        ASSERT_EQ(sizeof(Foo), array->elemBytes);
    }

    TEST("should set value to array") {
        Foo f{.id = 1, .value = 10};
        ASSERT_TRUE(!MCL_FAILED(MclArray_Set(array, 0, &f)));

        auto result = (Foo*)MclArray_Get(array, 0);
        ASSERT_TRUE(result != NULL);
        ASSERT_EQ(1, result->id);
        ASSERT_EQ(10, result->value);

        MclArray_Reset(array, 0);
        ASSERT_EQ(0, result->id);
        ASSERT_EQ(0, result->value);
    }

    TEST("should not set value to array when index overflow") {
        MclArray_Clear(array);

        Foo f{.id = 1, .value = 10};
        ASSERT_TRUE(MCL_FAILED(MclArray_Set(array, ARRAY_SIZE, &f)));

        auto result = (Foo*)MclArray_Get(array, 0);
        ASSERT_TRUE(result != NULL);
        ASSERT_EQ(0, result->id);
        ASSERT_EQ(0, result->value);
    }

    TEST("should not get value from array when index overflow") {
        MclArray_Clear(array);

        auto result = (Foo*)MclArray_Get(array, ARRAY_SIZE);
        ASSERT_TRUE(result == NULL);
    }

    TEST("should travel array") {
        MclArray_Clear(array);

        Foo f1{.id = 1, .value = 10};
        ASSERT_TRUE(!MCL_FAILED(MclArray_Set(array, 3, &f1)));

        Foo f2{.id = 2, .value = 20};
        ASSERT_TRUE(!MCL_FAILED(MclArray_Set(array, 9, &f2)));

        int sum = 0;
        Foo *foo {nullptr};
        MCL_ARRAY_FOREACH(array, Foo, foo) {
            sum += foo->value;
        }
        ASSERT_EQ(30, sum);
    }
};
