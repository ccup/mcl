#include <cctest/cctest.h>
#include "mcl/algo/sort.h"
#include "mcl/array/array_size.h"
#include "mcl/mem/malloc.h"

namespace {
	struct Object {
		int id;
	};
}

FIXTURE(SortTest) {
	TEST ("should sort for one element") {
		MclSortObj sortObjs[] = {MCL_SORT_OBJ(1, NULL)};

		MclSort_QuickSort(sortObjs, MCL_ARRAY_SIZE(sortObjs));

		ASSERT_EQ(1, sortObjs[0].key);
	}

	TEST ("should sort for multiple elements") {
		MclSortObj sortObjs[] = {MCL_SORT_OBJ(4, NULL), MCL_SORT_OBJ(1, NULL), MCL_SORT_OBJ(3, NULL), MCL_SORT_OBJ(2, NULL)};

		MclSort_QuickSort(sortObjs, MCL_ARRAY_SIZE(sortObjs));

		ASSERT_EQ(1, sortObjs[0].key);
		ASSERT_EQ(2, sortObjs[1].key);
		ASSERT_EQ(3, sortObjs[2].key);
		ASSERT_EQ(4, sortObjs[3].key);
	}

	TEST ("should sort for range") {
		MclSortObj sortObjs[] = {MCL_SORT_OBJ(4, NULL), MCL_SORT_OBJ(1, NULL), MCL_SORT_OBJ(3, NULL), MCL_SORT_OBJ(2, NULL)};

		MclSort_QuickSortRange(sortObjs, 1, 3);

		ASSERT_EQ(4, sortObjs[0].key);
		ASSERT_EQ(1, sortObjs[1].key);
		ASSERT_EQ(2, sortObjs[2].key);
		ASSERT_EQ(3, sortObjs[3].key);
	}

	TEST ("should sort for objects") {
		Object objs[] = {{.id = 0}, {.id = 1}, {.id = 2}};

		MclSortObj sortObjs[] = {MCL_SORT_OBJ(4, &objs[0]), MCL_SORT_OBJ(1, &objs[1]), MCL_SORT_OBJ(3, &objs[2])};

		MclSort_QuickSort(sortObjs, MCL_ARRAY_SIZE(sortObjs));

		ASSERT_EQ(1, sortObjs[0].key);
		ASSERT_EQ(3, sortObjs[1].key);
		ASSERT_EQ(4, sortObjs[2].key);

		ASSERT_EQ(1, ((Object*)sortObjs[0].obj)->id);
		ASSERT_EQ(2, ((Object*)sortObjs[1].obj)->id);
		ASSERT_EQ(0, ((Object*)sortObjs[2].obj)->id);

		ASSERT_EQ(0, objs[0].id);
		ASSERT_EQ(1, objs[1].id);
		ASSERT_EQ(2, objs[2].id);
	}

	TEST ("should sort stable") {
		Object objs[] = {{.id = 0}, {.id = 1}, {.id = 2}, {.id = 3}, {.id = 4}};

		MclSortObj sortObjs[] = {
				MCL_SORT_OBJ(4, &objs[0]),
				MCL_SORT_OBJ(3, &objs[1]),
				MCL_SORT_OBJ(3, &objs[2]),
				MCL_SORT_OBJ(3, &objs[3]),
				MCL_SORT_OBJ(1, &objs[4]),
		};

		MclSort_QuickSort(sortObjs, MCL_ARRAY_SIZE(sortObjs));

		ASSERT_EQ(1, sortObjs[0].key);
		ASSERT_EQ(3, sortObjs[1].key);
		ASSERT_EQ(3, sortObjs[2].key);
		ASSERT_EQ(3, sortObjs[3].key);
		ASSERT_EQ(4, sortObjs[4].key);

		ASSERT_EQ(4, ((Object*)sortObjs[0].obj)->id);
		ASSERT_EQ(1, ((Object*)sortObjs[1].obj)->id);
		ASSERT_EQ(2, ((Object*)sortObjs[2].obj)->id);
		ASSERT_EQ(3, ((Object*)sortObjs[3].obj)->id);
		ASSERT_EQ(0, ((Object*)sortObjs[4].obj)->id);
	}
};

