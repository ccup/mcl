#include "mcl/mem/mem_counter.h"
#include "mcl/lock/atom.h"
#include "mcl/keyword.h"

typedef struct {
	MclAtom malloc;
	MclAtom free;
} MclMemCounter;

MCL_PRIVATE MclMemCounter counter = {0};

void MclMemCounter_CountMalloc() {
	MclAtom_AddFetch(&counter.malloc , 1);
}

void MclMemCounter_CountFree() {
	MclAtom_AddFetch(&counter.free , 1);
}

MclSize MclMemCounter_GetMallocCount() {
	return MclAtom_Get(&counter.malloc);
}

MclSize MclMemCounter_GetFreeCount() {
	return MclAtom_Get(&counter.free);
}
