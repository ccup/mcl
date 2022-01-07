#include <cctest/cctest.h>
#include "mcl/mem/align.h"

FIXTURE(AlignTest) {
    TEST("should get correct align size") {
        ASSERT_EQ(sizeof(void*), MclAlign_GetSizeOf(1));
        ASSERT_EQ(2 * sizeof(void*), MclAlign_GetSizeOf(2 * sizeof(void*) - 2));
        ASSERT_EQ(3 * sizeof(void*), MclAlign_GetSizeOf(2 * sizeof(void*) + 1));
    }
};
