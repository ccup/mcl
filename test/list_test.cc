#include <cctest/cctest.h>
#include "mcl/list.h"

namespace
{
    struct Foo{
		MCL_LIST_ENTRY(Foo) link;
		int x;
	};
}

FIXTURE(ListTest)
{
	MCL_LIST_HEAD(Foo) elems;

	BEFORE {
		MCL_LIST_INIT(&elems, Foo, link);
	}

	AFTER {
		Foo *f1, *f2;
		MCL_LIST_FOREACH_SAFE(f1, f2, &elems, Foo, link) {
			MCL_LIST_REMOVE(f1, link);
		}
	}

	TEST("should be empty when init")
	{
		ASSERT_TRUE(MCL_LIST_EMPTY(&elems, Foo, link));
	}

	TEST("should instert tail after init")
	{
		Foo f = {.x = 5};

		MCL_LIST_INSERT_TAIL(&elems, &f, Foo, link);

		ASSERT_FALSE(MCL_LIST_EMPTY(&elems, Foo, link));
		ASSERT_EQ(5, MCL_LIST_FIRST(&elems)->x);
		ASSERT_EQ(5, MCL_LIST_LAST(&elems)->x);
	}

	TEST("should instert before specified element")
	{
		Foo f1 = {.x = 5};
		MCL_LIST_INSERT_TAIL(&elems, &f1, Foo, link);

		Foo f2 = {.x = 6};
		MCL_LIST_INSERT_BEFORE(&f1, &f2, link);

		ASSERT_EQ(6, MCL_LIST_FIRST(&elems)->x);
		ASSERT_EQ(5, MCL_LIST_LAST(&elems)->x);
	}

	TEST("should remove single element")
	{
		Foo f1 = {.x = 5};
		MCL_LIST_INSERT_TAIL(&elems, &f1, Foo, link);

		MCL_LIST_REMOVE(&f1, link);

		ASSERT_TRUE(MCL_LIST_EMPTY(&elems, Foo, link));
	}	

	TEST("should remove specified element")
	{
		Foo f1 = {.x = 5};
		MCL_LIST_INSERT_TAIL(&elems, &f1, Foo, link);

		Foo f2 = {.x = 6};
		MCL_LIST_INSERT_BEFORE(&f1, &f2, link);

		MCL_LIST_REMOVE(&f1, link);

		ASSERT_FALSE(MCL_LIST_EMPTY(&elems, Foo, link));
		ASSERT_EQ(6, MCL_LIST_FIRST(&elems)->x);
		ASSERT_EQ(6, MCL_LIST_LAST(&elems)->x);
	}

	TEST("should foreach all elements")
	{
		Foo f1 = {.x = 5};
		MCL_LIST_INSERT_TAIL(&elems, &f1, Foo, link);

		Foo f2 = {.x = 6};
		MCL_LIST_INSERT_BEFORE(&f1, &f2, link);

		int sum = 0;
		Foo *f;
		MCL_LIST_FOREACH(f, &elems, Foo, link) {
			sum += f->x;
		}

		ASSERT_EQ(11, sum);
	}	
};