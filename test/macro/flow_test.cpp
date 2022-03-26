#include <cctest/cctest.h>
#include "mcl/macro/flow.h"

#define TEST_RESULT(decl, init, result)		\
MCL_MACRO_FLOW_DECLARE(1, decl)				\
MCL_MACRO_FLOW_BEFORE(2, i = init)			\
MCL_MACRO_FLOW_AFTER(3, ASSERT_EQ(i, result))

FIXTURE(MacroCtrlTest) {
	TEST("should repeat init array items") {
		TEST_RESULT(int i, 3, 6) {
			i = i * 2;
		}
	}
};
