#include <cctest/cctest.h>
#include "mcl/link/link.h"

namespace
{
    struct Foo{
		MCL_LINK_NODE(Foo) link;
		int x;
	};
}

FIXTURE(LinkTest)
{
	MCL_LINK(Foo) foos;

	BEFORE {
		MCL_LINK_INIT(&foos, Foo, link);
	}

	AFTER {
		Foo *f1, *f2;
		MCL_LINK_FOREACH_SAFE(&foos, Foo, link, f1, f2) {
			MCL_LINK_REMOVE(f1, link);
		}
	}

	TEST("should be empty when init")
	{
		ASSERT_TRUE(MCL_LINK_EMPTY(&foos, Foo, link));
	}

	TEST("should insert tail after init")
	{
		Foo f = {.x = 5};

		MCL_LINK_INSERT_TAIL(&foos, &f, Foo, link);

		ASSERT_FALSE(MCL_LINK_EMPTY(&foos, Foo, link));
		ASSERT_EQ(5, MCL_LINK_FIRST(&foos)->x);
		ASSERT_EQ(5, MCL_LINK_LAST(&foos)->x);
	}

	TEST("should insert before specified element")
	{
		Foo f1 = {.x = 5};
		MCL_LINK_INSERT_TAIL(&foos, &f1, Foo, link);

		Foo f2 = {.x = 6};
		MCL_LINK_INSERT_BEFORE(&f1, &f2, link);

		ASSERT_EQ(6, MCL_LINK_FIRST(&foos)->x);
		ASSERT_EQ(5, MCL_LINK_LAST(&foos)->x);
	}

	TEST("should remove single element")
	{
		Foo f1 = {.x = 5};
		MCL_LINK_INSERT_TAIL(&foos, &f1, Foo, link);

		MCL_LINK_REMOVE(&f1, link);

		ASSERT_TRUE(MCL_LINK_EMPTY(&foos, Foo, link));
	}	

	TEST("should remove specified element")
	{
		Foo f1 = {.x = 5};
		MCL_LINK_INSERT_TAIL(&foos, &f1, Foo, link);

		Foo f2 = {.x = 6};
		MCL_LINK_INSERT_BEFORE(&f1, &f2, link);

		MCL_LINK_REMOVE(&f1, link);

		ASSERT_FALSE(MCL_LINK_EMPTY(&foos, Foo, link));
		ASSERT_EQ(6, MCL_LINK_FIRST(&foos)->x);
		ASSERT_EQ(6, MCL_LINK_LAST(&foos)->x);
	}

	TEST("should foreach all elements")
	{
		Foo f1 = {.x = 5};
		MCL_LINK_INSERT_TAIL(&foos, &f1, Foo, link);

		Foo f2 = {.x = 6};
		MCL_LINK_INSERT_BEFORE(&f1, &f2, link);

		int sum = 0;
		Foo *f;
		MCL_LINK_FOREACH(&foos, Foo, link, f) {
			sum += f->x;
		}

		ASSERT_EQ(11, sum);
	}	
};
