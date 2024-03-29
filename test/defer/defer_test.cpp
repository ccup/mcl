#include <cctest/cctest.h>
#include "mcl/defer/defer.h"
#include "mcl/log/log.h"
#include <string>

namespace {
    using Records = std::vector<std::string>;
    Records records;

    void increase(void *ctxt) {
        auto v = (int*)ctxt;
        (*v)++;
        records.push_back("increase");
    }

    void decrease(void *ctxt) {
        auto v = (int*)ctxt;
        (*v)--;
        records.push_back("decrease");
    }

    void logout(void *ctxt) {
        MCL_LOG_INFO("defer exec logout!");
        records.push_back("logout");
    }
}

FIXTURE(DeferTest) {
    int v {0};

    BEFORE {
        v = 0;
        records.clear();
    };

    AFTER {
        ASSERT_EQ(0 ,v);

        ASSERT_EQ(4, records.size());
        ASSERT_EQ("logout", records[0]);
        ASSERT_EQ("decrease", records[1]);
        ASSERT_EQ("increase", records[2]);
        ASSERT_EQ("decrease", records[3]);
    };

    TEST("should execute defers by reverse order") {
        MCL_DEFER_ENABLE_WITH(decrease, &v);
        v++;
        MCL_DEFER_ADD(increase, &v);
        MCL_DEFER_ADD(decrease, &v);
        MCL_DEFER_ADD(logout, NULL);

        ASSERT_EQ(1, v);
    }
};
