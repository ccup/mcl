#include "mcl/mem/mem_counter.h"
#include "mcl/lock/atomic.h"
#include "mcl/keyword.h"

typedef struct {
	MclAtomic malloc;
	MclAtomic free;
} MclMemCounter;

MCL_PRIVATE MclMemCounter counter = {0};

void MclMemCounter_CountMalloc() {
	MclAtomic_AddFetch(&counter.malloc , 1);
}

void MclMemCounter_CountFree() {
	MclAtomic_AddFetch(&counter.free , 1);
}

MclSize MclMemCounter_GetMallocCount() {
	return MclAtomic_Get(&counter.malloc);
}

MclSize MclMemCounter_GetFreeCount() {
	return MclAtomic_Get(&counter.free);
}
