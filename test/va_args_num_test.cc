#include <cctest/cctest.h>
#include "mcl/va_args_num.h"

FIXTURE(VaArgsNumTest) {
	TEST("should count the parameter count of VA_ARGS") {
		ASSERT_EQ(0, MCL_VA_ARGS_NUM());
		ASSERT_EQ(1, MCL_VA_ARGS_NUM(1));
		ASSERT_EQ(2, MCL_VA_ARGS_NUM(a, b));
		ASSERT_EQ(3, MCL_VA_ARGS_NUM(1, 2, 3));
		ASSERT_EQ(4, MCL_VA_ARGS_NUM(a, b, c, d));
		ASSERT_EQ(5, MCL_VA_ARGS_NUM(1, 2, 3, 4, 5));
		ASSERT_EQ(6, MCL_VA_ARGS_NUM(1, 2, 3, 4, 5, e));
	}
};
