#ifndef H425D83DB_AA41_4C87_BD61_B1496B874AA1
#define H425D83DB_AA41_4C87_BD61_B1496B874AA1

#include "mcl/typedef.h"
#include "mcl/status.h"

MCL_STDC_BEGIN

typedef uint32_t MclSortKey;

static const MclSortKey MCK_SORT_KEY_MAX = 0xFFFFFFFF;

MCL_TYPE(MclSortObj) {
	void *obj;
	MclSortKey key;
};

MclStatus MclSort_QuickSortRange(MclSortObj*, uint32_t minNum, uint32_t maxNum);
MclStatus MclSort_QuickSort(MclSortObj*, uint32_t count);

MCL_STDC_END

#endif
