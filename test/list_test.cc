#include <cctest/cctest.h>
#include "mcl/link/link.h"

namespace
{
    struct Foo{
		MCL_LIST_NODE(Foo) link;
		int x;
	};
}

FIXTURE(ListTest)
{
	MCL_LIST(Foo) foos;

	BEFORE {
		MCL_LIST_INIT(&foos, Foo, link);
	}

	AFTER {
		Foo *f1, *f2;
		MCL_LIST_FOREACH_SAFE(&foos, Foo, link, f1, f2) {
			MCL_LIST_REMOVE(f1, link);
		}
	}

	TEST("should be empty when init")
	{
		ASSERT_TRUE(MCL_LIST_EMPTY(&foos, Foo, link));
	}

	TEST("should insert tail after init")
	{
		Foo f = {.x = 5};

		MCL_LIST_INSERT_TAIL(&foos, &f, Foo, link);

		ASSERT_FALSE(MCL_LIST_EMPTY(&foos, Foo, link));
		ASSERT_EQ(5, MCL_LIST_FIRST(&foos)->x);
		ASSERT_EQ(5, MCL_LIST_LAST(&foos)->x);
	}

	TEST("should instert before specified element")
	{
		Foo f1 = {.x = 5};
		MCL_LIST_INSERT_TAIL(&foos, &f1, Foo, link);

		Foo f2 = {.x = 6};
		MCL_LIST_INSERT_BEFORE(&f1, &f2, link);

		ASSERT_EQ(6, MCL_LIST_FIRST(&foos)->x);
		ASSERT_EQ(5, MCL_LIST_LAST(&foos)->x);
	}

	TEST("should remove single element")
	{
		Foo f1 = {.x = 5};
		MCL_LIST_INSERT_TAIL(&foos, &f1, Foo, link);

		MCL_LIST_REMOVE(&f1, link);

		ASSERT_TRUE(MCL_LIST_EMPTY(&foos, Foo, link));
	}	

	TEST("should remove specified element")
	{
		Foo f1 = {.x = 5};
		MCL_LIST_INSERT_TAIL(&foos, &f1, Foo, link);

		Foo f2 = {.x = 6};
		MCL_LIST_INSERT_BEFORE(&f1, &f2, link);

		MCL_LIST_REMOVE(&f1, link);

		ASSERT_FALSE(MCL_LIST_EMPTY(&foos, Foo, link));
		ASSERT_EQ(6, MCL_LIST_FIRST(&foos)->x);
		ASSERT_EQ(6, MCL_LIST_LAST(&foos)->x);
	}

	TEST("should foreach all elements")
	{
		Foo f1 = {.x = 5};
		MCL_LIST_INSERT_TAIL(&foos, &f1, Foo, link);

		Foo f2 = {.x = 6};
		MCL_LIST_INSERT_BEFORE(&f1, &f2, link);

		int sum = 0;
		Foo *f;
		MCL_LIST_FOREACH(&foos, Foo, link, f) {
			sum += f->x;
		}

		ASSERT_EQ(11, sum);
	}	
};
