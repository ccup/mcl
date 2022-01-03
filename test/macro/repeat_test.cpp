#include <cctest/cctest.h>
#include "mcl/macro/repeat.h"
#include "mcl/array/array_size.h"

#define ARRAY_SIZE_MAX 5
#define ARRAY_ITEM_INIT(n) [n]=n,

FIXTURE(MacroRepeatTest) {
	TEST("should repeat init array items") {
		int a[] = {MCL_MACRO_REPEAT_SIMPLE(ARRAY_SIZE_MAX, ARRAY_ITEM_INIT)};

		for (int i = 0; i < MCL_ARRAY_SIZE(a); i++) {
			ASSERT_EQ(i, a[i]);
		}
	}
};
