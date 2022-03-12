#include "factory/mcl_aggregator_factory.h"
#include "aggregator/mcl_aggregator.h"
#include "mcl/mem/memory.h"
#include "mcl/lock/lockobj.h"
#include "mcl/lock/atom.h"
#include "mcl/assert.h"

///////////////////////////////////////////////////////////
MCL_PRIVATE MclAtom aggregatorCount = 0;

MclSize MclAggregatorFactory_GetUnreleasedCount() {
	return aggregatorCount;
}

///////////////////////////////////////////////////////////
MclAggregator* MclAggregatorFactory_Create(MclAggregatorId id, void *cfg) {
	MclAggregator *self = MCL_MALLOC(MCL_AGGREGATOR_SIZE);
	MCL_ASSERT_VALID_PTR_NIL(self);

	if (MCL_FAILED(MclAggregator_Init(self, id, cfg))) {
		MCL_LOG_ERR("Initialize aggregator (%u) failed!", id);
		MCL_FREE(self);
		return NULL;
	}

	MclAtom_AddFetch(&aggregatorCount, 1);
	return self;
}

void MclAggregatorFactory_Delete(MclAggregator *self) {
	MCL_ASSERT_VALID_PTR_VOID(self);
	MclAggregator_Destroy(self);
	MCL_FREE(self);

	MclAtom_SubFetch(&aggregatorCount, 1);
}

///////////////////////////////////////////////////////////
MclAggregator* MclAggregatorFactory_CreateLockObj(MclAggregatorId id, void *cfg) {
	MclAggregator *self = (MclAggregator*)MclLockObj_Create(MCL_AGGREGATOR_SIZE);
	MCL_ASSERT_VALID_PTR_NIL(self);

	if (MCL_FAILED(MclAggregator_Init(self, id, cfg))) {
		MCL_LOG_ERR("Initialize lock aggregator (%u) failed!", id);
		MclLockObj_Delete(self, NULL, NULL);
		return NULL;
	}
	MclAtom_AddFetch(&aggregatorCount, 1);
	return self;
}

MCL_PRIVATE void MclAggregatorFactory_DestroyAggregator(void *obj, void *arg) {
	MclAggregator_Destroy((MclAggregator*)obj);
}

void MclAggregatorFactory_DeleteLockObj(MclAggregator *self) {
	MCL_ASSERT_VALID_PTR_VOID(self);

	MclLockObj_Delete(self, MclAggregatorFactory_DestroyAggregator, NULL);
	MclAtom_SubFetch(&aggregatorCount, 1);
}
