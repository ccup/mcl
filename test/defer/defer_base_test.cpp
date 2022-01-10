#include <cctest/cctest.h>
#include "mcl/defer/defer.h"

namespace {
    void increase(void *ctxt) {
        auto v = (int*)ctxt;
        (*v)++;
    }

    void decrease(void *ctxt) {
        auto v = (int*)ctxt;
        (*v)--;
    }
}

FIXTURE(DeferBaseTest) {
    int v {0};

    BEFORE {
        v = 0;
    };

    AFTER {
        ASSERT_EQ(0, v);
    };

    TEST("should execute defer by add node and execute explicitly") {
        MclDefer defer = MCL_DEFER(defer, NULL);
        MclDeferNode node = MCL_DEFER_NODE(decrease, &v);
        MclDefer_AddNode(&defer, &node);

        v++;
        ASSERT_EQ(1, v);

        MclDefer_Exec(&defer);
        ASSERT_EQ(0, v);
    }

    TEST("should auto execute defer") {
        MCL_DEFER_ENABLE();
        MCL_DEFER_ADD(decrease, &v);

        v++;
        ASSERT_EQ(1, v);
    }

    TEST("should define auto defer with func") {
        MCL_DEFER_ENABLE_WITH(decrease, &v);
        v++;
        MCL_DEFER_ADD(increase, &v);
        MCL_DEFER_ADD(decrease, &v);
        ASSERT_EQ(1, v);
    }
};
