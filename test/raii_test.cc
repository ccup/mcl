#include <cctest/cctest.h>
#include "mcl/mem/malloc.h"

namespace
{
    struct Foo {
		Foo(int* cleanCount) : cleanCount(cleanCount) {
		}

		void clean() {
			if (cleanCount) (*cleanCount)++;
		}

	private:
		int *cleanCount{nullptr};
	};

	void Mcl_AutoClean(Foo *foo) {
		if (foo) foo->clean();
	}
}

#define MCL_AUTO_CLEAN MCL_RAII(Mcl_AutoClean)

FIXTURE(RaiiTest)
{
	int cleanCount{0};

	AFTER {
		cleanCount = 0;
	}

	TEST("should auto clean aster exit scope")
	{
		ASSERT_EQ(0, cleanCount);
		{
			MCL_AUTO_CLEAN Foo foo(&cleanCount);
		}
		ASSERT_EQ(1, cleanCount);
	}

	TEST("should auto free aster exit scope")
	{
		void **p = nullptr;

		{
			MCL_AUTO_PTR auto *pf = MCL_MALLOC(sizeof(Foo));
			p = &pf;
			ASSERT_TRUE(*p != nullptr);
		}
		ASSERT_TRUE(*p == NULL);
	}
};
