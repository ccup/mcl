#include <cctest/cctest.h>
#include "mcl/array.h"

FIXTURE(ArrayTest) {
	MCL_DECL_ARRAY(int, IntArray);

	TEST("array initialized") {
		int a[10] = {0, 1, 2, 3};
		IntArray arr;
		MCL_INIT_ARRAY(arr, a, 10, 4);

		ASSERT_EQ(a,  MCL_ARRAY_RAW(arr));
		ASSERT_EQ(10, MCL_ARRAY_SIZE(arr));
		ASSERT_EQ(4,  MCL_ARRAY_LEN(arr));
		ASSERT_EQ(0,  MCL_ARRAY_FIRST(arr));
		ASSERT_EQ(3,  MCL_ARRAY_LAST(arr));
	}

	TEST("array initialized in local") {
		IntArray arr;
		MCL_INIT_ARRAY_LOCAL(arr, int, 10);
		ASSERT_EQ(10, MCL_ARRAY_SIZE(arr));
		ASSERT_EQ(0,  MCL_ARRAY_LEN(arr));
		ASSERT_EQ(0,  MCL_ARRAY_ELEM(arr, 9));
	}

	TEST("array initialized in local with elements") {
		IntArray arr;
		MCL_INIT_ARRAY_LOCAL_WITH_ELEMS(arr, int, 10, 5, {0, 1, 2, 3, 4});
		ASSERT_EQ(10, MCL_ARRAY_SIZE(arr));
		ASSERT_EQ(5,  MCL_ARRAY_LEN(arr));
		ASSERT_EQ(2,  MCL_ARRAY_ELEM(arr, 2));
	}

	TEST("append elem in last position") {
		IntArray arr;
		MCL_INIT_ARRAY_LOCAL_WITH_ELEMS(arr, int, 10, 5, {0, 1, 2, 3, 4});
		MCL_ARRAY_APPEND(arr, 5);
		ASSERT_EQ(10, MCL_ARRAY_SIZE(arr));
		ASSERT_EQ(6,  MCL_ARRAY_LEN(arr));
		ASSERT_EQ(5,  MCL_ARRAY_ELEM(arr, 5));
	}

	TEST("insert elem to array") {
		IntArray arr;
		MCL_INIT_ARRAY_LOCAL_WITH_ELEMS(arr, int, 10, 5, {0, 1, 2, 3, 4});
		MCL_ARRAY_INSERT(arr, int, 2, 5);
		ASSERT_EQ(10, MCL_ARRAY_SIZE(arr));
		ASSERT_EQ(6,  MCL_ARRAY_LEN(arr));
		ASSERT_EQ(5,  MCL_ARRAY_ELEM(arr, 2));
		ASSERT_EQ(4,  MCL_ARRAY_ELEM(arr, 5));
		ASSERT_EQ(4,  MCL_ARRAY_LAST(arr));
	}

	TEST("insert elem to full array") {
		IntArray arr;
		MCL_INIT_ARRAY_LOCAL_WITH_ELEMS(arr, int, 5, 5, {0, 1, 2, 3, 4});
		MCL_ARRAY_INSERT(arr, int, 2, 5);
		ASSERT_EQ(5,  MCL_ARRAY_SIZE(arr));
		ASSERT_EQ(5,  MCL_ARRAY_LEN(arr));
		ASSERT_EQ(2,  MCL_ARRAY_ELEM(arr, 2));
		ASSERT_EQ(4,  MCL_ARRAY_ELEM(arr, 4));
		ASSERT_EQ(4,  MCL_ARRAY_LAST(arr));

		int v = 0xff;
		MCL_ARRAY_GET(arr, 5, v, -1);
		ASSERT_EQ(-1,  v);
	}

	TEST("insert elem outside of valid range") {
		IntArray arr;
		MCL_INIT_ARRAY_LOCAL_WITH_ELEMS(arr, int, 5, 5, {0, 1, 2, 3, 4});
		MCL_ARRAY_INSERT(arr, int, 5, 0xff);
		ASSERT_EQ(5,  MCL_ARRAY_SIZE(arr));
		ASSERT_EQ(5,  MCL_ARRAY_LEN(arr));
		ASSERT_EQ(0,  MCL_ARRAY_FIRST(arr));
		ASSERT_EQ(4,  MCL_ARRAY_LAST(arr));
	}

	TEST("remove elem from array") {
		IntArray arr;
		MCL_INIT_ARRAY_LOCAL_WITH_ELEMS(arr, int, 10, 5, {0, 1, 2, 3, 4});
		MCL_ARRAY_REMOVE(arr, int, 2);
		ASSERT_EQ(10, MCL_ARRAY_SIZE(arr));
		ASSERT_EQ(4,  MCL_ARRAY_LEN(arr));
		ASSERT_EQ(3,  MCL_ARRAY_ELEM(arr, 2));
		ASSERT_EQ(4,  MCL_ARRAY_LAST(arr));

		int v = 0xff;
		MCL_ARRAY_GET(arr, 5, v, -1);
		ASSERT_EQ(-1,  v);
	}

	TEST("remove elem from empty array") {
		IntArray arr;
		MCL_INIT_ARRAY_LOCAL_WITH_ELEMS(arr, int, 10, 0, {0});
		MCL_ARRAY_REMOVE(arr, int, 0);
		ASSERT_EQ(10, MCL_ARRAY_SIZE(arr));
		ASSERT_EQ(0,  MCL_ARRAY_LEN(arr));
		ASSERT_EQ(0,  MCL_ARRAY_ELEM(arr, 2));
	}

	TEST("find elem in array") {
		IntArray arr;
		int index = 0xff;

		MCL_INIT_ARRAY_LOCAL_WITH_ELEMS(arr, int, 10, 5, {0, 1, 2, 3, 4});

		MCL_ARRAY_FIND_INDEX(index, arr, 3);
		ASSERT_EQ(3, index);

		MCL_ARRAY_FIND_INDEX(index, arr, 5);
		ASSERT_TRUE(MCL_ARRAY_INDEX_INVALID(index));
	}

	TEST("array foreach") {
		IntArray arr;
		int sum = 0;
		MCL_INIT_ARRAY_BY_ELEMS(arr, int, {0, 1, 2, 3, 4});
		MCL_ARRAY_FOREACH(int*, arr, v) {
			sum += *v;
		}
		ASSERT_EQ(10, sum);
	}

	TEST("use array as stack") {
		IntArray arr;
		int v = 0xff;
		MCL_INIT_ARRAY_LOCAL(arr, int, 3);

		MCL_ARRAY_POP(arr, int, v, -1);
		ASSERT_EQ(-1, v);
		ASSERT_TRUE(MCL_ARRAY_IS_EMPTY(arr));

		MCL_ARRAY_PUSH(arr, int, 3);
		ASSERT_FALSE(MCL_ARRAY_IS_EMPTY(arr));
		ASSERT_FALSE(MCL_ARRAY_IS_FULL(arr));
		ASSERT_EQ(1, MCL_ARRAY_LEN(arr));
		ASSERT_EQ(3, MCL_ARRAY_FIRST(arr));
		ASSERT_EQ(3, MCL_ARRAY_LAST(arr));

		MCL_ARRAY_POP(arr, int, v, -1);
		ASSERT_EQ(3, v);
		ASSERT_TRUE(MCL_ARRAY_IS_EMPTY(arr));

		MCL_ARRAY_PUSH(arr, int, 3);
		MCL_ARRAY_PUSH(arr, int, 2);
		MCL_ARRAY_PUSH(arr, int, 1);
		ASSERT_FALSE(MCL_ARRAY_IS_EMPTY(arr));
		ASSERT_TRUE(MCL_ARRAY_IS_FULL(arr));
		ASSERT_EQ(3, MCL_ARRAY_LEN(arr));
		ASSERT_EQ(1, MCL_ARRAY_FIRST(arr));
		ASSERT_EQ(3, MCL_ARRAY_LAST(arr));

		MCL_ARRAY_PUSH(arr, int, 4);
		ASSERT_EQ(3, MCL_ARRAY_LEN(arr));
		ASSERT_EQ(1, MCL_ARRAY_FIRST(arr));
		ASSERT_EQ(3, MCL_ARRAY_LAST(arr));

		MCL_ARRAY_POP(arr, int, v, -1);
		ASSERT_EQ(1, v);
		ASSERT_EQ(2, MCL_ARRAY_LEN(arr));
		ASSERT_FALSE(MCL_ARRAY_IS_EMPTY(arr));
		ASSERT_FALSE(MCL_ARRAY_IS_FULL(arr));
	}

	//	TEST("should panic on compile when array length exceeds capacity") {
	//		int a[10];
	//		IntArray int_arr;
	//		MCL_INIT_ARRAY(int_arr, a, 10, 11);
	//	}
};
