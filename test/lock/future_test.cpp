#include <cctest/cctest.h>
#include "mcl/lock/future.h"
#include "mcl/thread/thread.h"
#include <unistd.h>

namespace {
    constexpr int MAX_COUNT = 1000;

    int result = 0;

    void* increase(void *data) {
        auto future = (MclFuture*)(data);

        for (int i = 0; i < MAX_COUNT; i++) {
            result = i;
            MclThread_Yield();
        }
//        sleep(10);
        MclFuture_Set(future, MCL_SUCCESS, &result);

        return NULL;
    }
}

FIXTURE(FutureTest) {
    TEST("should get correct result from future") {
        MclFuture *future = MclFuture_Create();

        MclThread th;

        MclThread_Create(&th, NULL, increase, future);

        MclStatus status {MCL_STATUS_DONE};
        void *value {nullptr};

        MclFuture_Get(future, &status, &value);

        ASSERT_EQ(MCL_SUCCESS, status);
        ASSERT_EQ(MAX_COUNT - 1, *(int*)(value));
    }
};
