#include "mcl/algo/sort.h"
#include "mcl/assert.h"

MCL_PRIVATE MclArrayIndex MclSort_SeekPosition(MclSortObj *sortObjs, MclArrayIndex begin, MclArrayIndex end) {
	MclSortObj keyObj = sortObjs[begin];
	MclSortKey key = keyObj.key;

	MclArrayIndex i = begin;
	MclArrayIndex j = end;
	while (i < j) {
		while (i < j && (key <= sortObjs[j].key)) {
			j--;
		}
		if (i < j) {
			sortObjs[i++] = sortObjs[j];
		}
		while (i < j && (sortObjs[i].key <= key)) {
			i++;
		}
		if (i < j) {
			sortObjs[j--] = sortObjs[i];
		}
	}

	sortObjs[i] = keyObj;
	return i;
}

void MclSort_QuickSortRange(MclSortObj *sortObjs, MclArrayIndex begin, MclArrayIndex end) {
	MCL_ASSERT_VALID_PTR_VOID(sortObjs);
	if (begin >= end) return;

	MclArrayIndex position = MclSort_SeekPosition(sortObjs, begin, end);
	if (position > begin) MclSort_QuickSortRange(sortObjs, begin, position - 1);
	if (position < end) MclSort_QuickSortRange(sortObjs, position + 1, end);
}

void MclSort_QuickSort(MclSortObj *sortObjs, MclSize size) {
	MCL_ASSERT_VALID_PTR_VOID(sortObjs);
	MCL_ASSERT_TRUE_VOID(size > 0);
	MclSort_QuickSortRange(sortObjs, 0, size - 1);
}
