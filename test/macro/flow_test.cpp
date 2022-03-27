#include <cctest/cctest.h>
#include "mcl/macro/flow.h"

#define TEST_RESULT(type, var, init, result)\
MCL_MACRO_FLOW_DECLARE(1, type var)			\
MCL_MACRO_FLOW_BEFORE(2, var = init)		\
MCL_MACRO_FLOW_AFTER(3, ASSERT_EQ(var, result))

FIXTURE(MacroCtrlTest) {
	TEST("should repeat init array items") {
		TEST_RESULT(int, i, 3, 6) {
			i = i * 2;
		}
	}
};
