#ifndef H425D83DB_AA41_4C87_BD61_B1496B874AA1
#define H425D83DB_AA41_4C87_BD61_B1496B874AA1

#include "mcl/typedef.h"

MCL_STDC_BEGIN

typedef uint32_t MclSortKey;

MCL_TYPE(MclSortObj) {
	MclSortKey key;
	void *obj;
};

void MclSort_QuickSortRange(MclSortObj*, uint32_t begin, uint32_t end);
void MclSort_QuickSort(MclSortObj*, size_t size);

///////////////////////////////////////////////////////////
#define MCL_SORT_OBJ(KEY, OBJ) {.key = (KEY), .obj = (OBJ)}

MCL_STDC_END

#endif
