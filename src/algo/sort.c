#include "mcl/algo/sort.h"
#include "mcl/assert.h"

MCL_PRIVATE MclStatus MclSort_ExchangeBackward(MclSortObj *sortObjs, uint32_t key, uint32_t minNum, uint32_t *maxNum) {
	while ((minNum < *maxNum) && ((sortObjs + *maxNum)->key >= key)) {
		--(*maxNum);
	}
	memmove(sortObjs + minNum, sortObjs + *maxNum, sizeof(MclSortObj));
	return MCL_SUCCESS;
}

MCL_PRIVATE MclStatus MclSort_ExchangeForward(MclSortObj *sortObjs, uint32_t key, uint32_t *minNum, uint32_t maxNum) {
	while ((*minNum < maxNum) && ((sortObjs + *minNum)->key <= key)) {
		++(*minNum);
	}
	memmove(sortObjs + maxNum, sortObjs + *minNum, sizeof(MclSortObj));
	return MCL_SUCCESS;
}

MCL_PRIVATE MclStatus MclSort_SeekPosition(MclSortObj *sortObjs, uint32_t minNum, uint32_t maxNum, uint32_t *position) {
	MCL_ASSERT_VALID_PTR(sortObjs);
	MCL_ASSERT_VALID_PTR(position);

	MclSortObj keyObj = {0};
	memcpy(&keyObj, sortObjs + minNum, sizeof(MclSortObj));

	while (minNum < maxNum) {
		MCL_ASSERT_SUCC_CALL(MclSort_ExchangeBackward(sortObjs, keyObj.key, minNum, &maxNum));
		MCL_ASSERT_SUCC_CALL(MclSort_ExchangeForward(sortObjs, keyObj.key, &minNum, maxNum));
	}

	*position = minNum;
	memcpy(sortObjs + *position, &keyObj, sizeof(MclSortObj));
	return MCL_SUCCESS;
}

MclStatus MclSort_QuickSortRange(MclSortObj *sortObjs, uint32_t minNum, uint32_t maxNum) {
	MCL_ASSERT_VALID_PTR(sortObjs);
	uint32_t position = minNum;
	if (minNum < maxNum) {
		MCL_ASSERT_SUCC_CALL(MclSort_SeekPosition(sortObjs, minNum, maxNum, &position));
		if (position > minNum) MCL_ASSERT_SUCC_CALL(MclSort_QuickSortRange(sortObjs, minNum, position));
		if (position < maxNum) MCL_ASSERT_SUCC_CALL(MclSort_QuickSortRange(sortObjs, position, maxNum));
	}
	return MCL_SUCCESS;
}

MclStatus MclSort_QuickSort(MclSortObj *sortObjs, uint32_t count) {
	MCL_ASSERT_VALID_PTR(sortObjs);
	MCL_ASSERT_TRUE(count > 0);
	MCL_ASSERT_SUCC_CALL(MclSort_QuickSortRange(sortObjs, 0, count - 1));
	return MCL_SUCCESS;
}
