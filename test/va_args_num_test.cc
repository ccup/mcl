#include <cctest/cctest.h>
#include "mcl/macro.h"
#include "mcl/array.h"

FIXTURE(VaArgsNumTest) {
	TEST("should count the parameter count of VA_ARGS") {
		ASSERT_EQ(0, MCL_MARCO_ARGS_NUM());
		ASSERT_EQ(1, MCL_MARCO_ARGS_NUM(1));
		ASSERT_EQ(2, MCL_MARCO_ARGS_NUM(a, b));
		ASSERT_EQ(3, MCL_MARCO_ARGS_NUM(1, 2, 3));
		ASSERT_EQ(4, MCL_MARCO_ARGS_NUM(a, b, c, d));
		ASSERT_EQ(5, MCL_MARCO_ARGS_NUM(1, 2, 3, 4, 5));
		ASSERT_EQ(6, MCL_MARCO_ARGS_NUM(1, 2, 3, 4, 5, e));
	}

	TEST("should take n arg from given pos of VA_ARGS") {
		int a[] = {MCL_MACRO_ARG_TAKE(2, 4, MCL_MACRO_ARGS(1, 2, 3, 4, 5, 6))};
		ASSERT_EQ(4, ARRAY_SIZE(a));
		ASSERT_EQ(3, a[0]);
		ASSERT_EQ(4, a[1]);
		ASSERT_EQ(5, a[2]);
		ASSERT_EQ(6, a[3]);
	}
};
