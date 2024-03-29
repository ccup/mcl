#include <cctest/cctest.h>
#include "mcl/lock/atomic.h"
#include "mcl/thread/thread.h"

namespace {
    MclAtomic sum = 0;

    constexpr int PATCH_COUNT = 100000;

    void* add(void *) {
        for (int i = 0; i < PATCH_COUNT; i++) {
            MclAtomic_AddFetch(&sum, 1);
            MclThread_Yield();
        }
        return NULL;
    }

    void* sub(void*) {
        for (int i = 0; i < PATCH_COUNT; i++) {
            MclAtomic_SubFetch(&sum, 1);
            MclThread_Yield();
        }
        return NULL;
    }
}

FIXTURE(AtomTest) {
    constexpr static int THREAD_COUNT{10};

    AFTER {
        MclAtomic_Set(&sum, 0);
    };

    TEST("should add to expect value when use atom variable") {
        MclAtomic_Clear(&sum);

        MclThread threads[THREAD_COUNT];

        for (int i = 0; i < THREAD_COUNT; i++) {
            MclThread_Create(&threads[i], NULL, add, NULL);
        }

        for (int i = 0; i < THREAD_COUNT; i++) {
            MclThread_Join(threads[i], NULL);
        }

        ASSERT_EQ(PATCH_COUNT * THREAD_COUNT, sum);
    }

    TEST("should sub to expect value when use atom variable") {
        MclAtomic_Set(&sum, PATCH_COUNT * THREAD_COUNT);

        MclThread threads[THREAD_COUNT];

        for (int i = 0; i < THREAD_COUNT; i++) {
            MclThread_Create(&threads[i], NULL, sub, NULL);
        }

        for (int i = 0; i < THREAD_COUNT; i++) {
            MclThread_Join(threads[i], NULL);
        }

        ASSERT_EQ(0, sum);
    }
};
