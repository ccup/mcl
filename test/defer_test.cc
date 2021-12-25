#include <cctest/cctest.h>
#include "mcl/defer/defer.h"

namespace {
    void decrease(void *data) {
        auto n = (int*)data;
        (*n)--;
    }
}

FIXTURE(DeferTest) {
    int v {0};

    AFTER {
        ASSERT_EQ(0, v);
    };

    TEST("should execute defer by calling exec explicitly") {
        MclDefer defer = MCL_DEFER(defer);
        MclDeferNode node = MCL_DEFER_NODE(decrease, &v);
        MclDefer_Add(&defer, &node);

        v++;

        MclDefer_Exec(&defer);
    }

    TEST("should auto execute defer") {
        MCL_DEFER_ENABLE();
        MCL_DEFER_ADD(decrease, &v);

        v++;
    }

    TEST("should define auto defer with func") {
        MCL_DEFER_ENABLE_WITH(decrease, &v);
        v++;
    }
};
