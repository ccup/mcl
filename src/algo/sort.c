#include "mcl/algo/sort.h"
#include "mcl/assert.h"

MCL_PRIVATE uint32_t MclSort_SeekPosition(MclSortObj *sortObjs, size_t begin, uint32_t end) {
	MclSortObj keyObj = sortObjs[begin];
	MclSortKey key = keyObj.key;

	uint32_t i = begin;
	uint32_t j = end;
	while (i < j) {
		while (i < j && (sortObjs[j].key > key)) {
			j--;
		}
		if (i < j) {
			sortObjs[i++] = sortObjs[j];
		}
		while (i < j && (sortObjs[i].key < key)) {
			i++;
		}
		if (i < j) {
			sortObjs[j--] = sortObjs[i];
		}
	}

	sortObjs[i] = keyObj;
	return i;
}

void MclSort_QuickSortRange(MclSortObj *sortObjs, uint32_t begin, uint32_t end) {
	MCL_ASSERT_VALID_PTR_VOID(sortObjs);
	if (begin >= end) return;

	uint32_t position = MclSort_SeekPosition(sortObjs, begin, end);
	if (position > begin) MclSort_QuickSortRange(sortObjs, begin, position - 1);
	if (position < end) MclSort_QuickSortRange(sortObjs, position + 1, end);
}

void MclSort_QuickSort(MclSortObj *sortObjs, size_t size) {
	MCL_ASSERT_VALID_PTR_VOID(sortObjs);
	MCL_ASSERT_TRUE_VOID(size > 0);
	return MclSort_QuickSortRange(sortObjs, 0, size - 1);
}
