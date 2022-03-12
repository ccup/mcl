#include "factory/mcl_aggregator_factory.h"
#include "allocator/mcl_allocator.h"
#include "aggregator/mcl_aggregator.h"
#include "mcl/lock/atom.h"
#include "mcl/assert.h"

///////////////////////////////////////////////////////////
typedef struct {
	MclFactoryAllocator allocator;
	MclAtom count;
} MclAggregatorFactory;

MCL_PRIVATE MclAggregatorFactory factory;

MCL_PRIVATE void MclAggregatorFactory_DestroyAggregator(void *obj, void *arg) {
	MclAggregator_Destroy((MclAggregator*)obj);
}

MCL_PRIVATE void MclAggregatorFactory_InitAllocator(MclFactoryAllocator *allocator, MclAllocatorType type) {
	MCL_ASSERT_TRUE_VOID(type != MCL_ALLOCATOR_POOL);

	allocator->destroyObj = MclAggregatorFactory_DestroyAggregator;
	allocator->ctxt = NULL;
	MclFactoryAllocator_Init(allocator, type);
}

void MclAggregatorFactory_Init(MclAllocatorType type) {
	MclAggregatorFactory_InitAllocator(&factory.allocator, type);
	MclAtom_Clear(&factory.count);
}

MclAggregator* MclAggregatorFactory_Create(MclAggregatorId id, void *cfg) {
	MclAggregator *aggregator = MclFactoryAllocator_Create(&factory.allocator, MCL_AGGREGATOR_SIZE);
	MCL_ASSERT_VALID_PTR_NIL(aggregator);

	if (MCL_FAILED(MclAggregator_Init(aggregator, id, cfg))) {
		MCL_LOG_ERR("Initialize aggregator (%u) failed!", id);
		MclFactoryAllocator_Delete(&factory.allocator, aggregator);
		return NULL;
	}

	MclAtom_AddFetch(&factory.count, 1);
	return aggregator;
}

void MclAggregatorFactory_Delete(MclAggregator *aggregator) {
	MCL_ASSERT_VALID_PTR_VOID(aggregator);
	MclFactoryAllocator_Delete(&factory.allocator, aggregator);
	MclAtom_SubFetch(&factory.count, 1);
}

MclSize MclAggregatorFactory_GetUnreleasedCount() {
	return factory.count;
}
