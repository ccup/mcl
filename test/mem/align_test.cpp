#include <cctest/cctest.h>
#include "mcl/mem/align.h"

FIXTURE(AlignTest) {
    TEST("should get correct align size") {
        ASSERT_EQ(sizeof(void*), MclAlign_GetSizeOf(1));
        ASSERT_EQ(2 * sizeof(void*), MclAlign_GetSizeOf(13));
    }
};
