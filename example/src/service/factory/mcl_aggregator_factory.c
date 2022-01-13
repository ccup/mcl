#include "factory/mcl_aggregator_factory.h"
#include "aggregator/mcl_aggregator.h"
#include "mcl/mem/shared_ptr.h"
#include "mcl/mem/allocator.h"
#include "mcl/mem/malloc.h"
#include "mcl/lock/lockobj.h"
#include "mcl/lock/atom.h"
#include "mcl/assert.h"

///////////////////////////////////////////////////////////
MCL_PRIVATE MclAtom aggregatorCount = 0;

size_t MclAggregatorFactory_GetUnreleasedCount() {
	return aggregatorCount;
}

///////////////////////////////////////////////////////////
MclAggregator* MclAggregatorFactory_Create(MclAggregatorId id) {
	MclAggregator *self = MCL_MALLOC(MCL_AGGREGATOR_SIZE);
	MCL_ASSERT_VALID_PTR_NIL(self);

	if (MCL_FAILED(MclAggregator_Init(self, id))) {
		MCL_LOG_ERR("Initialize aggregator (%u) failed!", id);
		MCL_FREE(self);
		return NULL;
	}

	MclAtom_Add(&aggregatorCount, 1);
	return self;
}

void MclAggregatorFactory_Delete(MclAggregator *self) {
	MCL_ASSERT_VALID_PTR_VOID(self);
	MclAggregator_Destroy(self);
	MCL_FREE(self);

	MclAtom_Sub(&aggregatorCount, 1);
}

///////////////////////////////////////////////////////////
MCL_PRIVATE void MclAggregatorFactory_DestroyAggregator(void *obj, void *arg) {
	MclAggregator_Destroy((MclAggregator*)obj);
}

MclAggregator* MclAggregatorFactory_CreateSharedPtr(MclAggregatorId id) {
	MclAggregator *self = MclSharedPtr_Create(MCL_AGGREGATOR_SIZE, MclAggregatorFactory_DestroyAggregator, NULL);
	MCL_ASSERT_VALID_PTR_NIL(self);

	if (MCL_FAILED(MclAggregator_Init(self, id))) {
		MCL_LOG_ERR("Initialize shared ptr (%u) of aggregator failed!", id);
		MCL_FREE(self);
		return NULL;
	}

	MclAtom_Add(&aggregatorCount, 1);
	return self;
}

void MclAggregatorFactory_DeleteSharedPtr(MclAggregator *self) {
	MCL_ASSERT_VALID_PTR_VOID(self);

	MclSharedPtr_Delete(self);
	MclAtom_Sub(&aggregatorCount, 1);
}

///////////////////////////////////////////////////////////
MclAggregator* MclAggregatorFactory_CreateLockObj(MclAggregatorId id) {
	MclAggregator *self = (MclAggregator*)MclLockObj_Create(MCL_AGGREGATOR_SIZE);
	MCL_ASSERT_VALID_PTR_NIL(self);

	if (MCL_FAILED(MclAggregator_Init(self, id))) {
		MCL_LOG_ERR("Initialize lock aggregator (%u) failed!", id);
		MclLockObj_Delete(self, NULL, NULL);
		return NULL;
	}
	MclAtom_Add(&aggregatorCount, 1);
	return self;
}

void MclAggregatorFactory_DeleteLockObj(MclAggregator *self) {
	MCL_ASSERT_VALID_PTR_VOID(self);

	MclLockObj_Delete(self, MclAggregatorFactory_DestroyAggregator, NULL);
	MclAtom_Sub(&aggregatorCount, 1);
}

/////////////////////////////////////////////////////////
//#define MCL_AGGREGATOR_MEM_SIZE  8
//MCL_PRIVATE const uint16_t MCL_AGGREGATOR_CAPACITY = 16;
//
//MCL_ALLOCATOR_TYPE_DEF(MclAggregatorAllocator, MclAggregator, MCL_AGGREGATOR_CAPACITY);
//
//MCL_PRIVATE MclAggregatorAllocator aggregatorAllocator;
//
//MCL_CTOR void MclAggregatorAllocator_Ctor() {
//	if (MCL_AGGREGATOR_MEM_SIZE < MCL_AGGREGATOR_SIZE) {
//		MCL_LOG_FATAL("Size (%u) of aggregator memory in allocator is less than actual size (%lu)!"
//				, MCL_AGGREGATOR_MEM_SIZE, MCL_AGGREGATOR_SIZE);
//	}
//	MCL_ALLOCATOR_INIT(MclAggregatorAllocator, aggregatorAllocator);
//	MCL_LOG_SUCC("Aggregator allocator init OK!");
//}
//
//MclAggregator* MclAggregatorFactory_CreateStatic(MclAggregatorId id) {
//	if (MCL_AGGREGATOR_MEM_SIZE < MCL_AGGREGATOR_SIZE) return NULL;
//
//	MclAggregator* self = MCL_ALLOCATOR_ALLOC(MclAggregatorAllocator, aggregatorAllocator);
//	MCL_ASSERT_VALID_PTR_NIL(self);
//
//	if (MCL_FAILED(MclAggregator_Init(self, id))) {
//		MCL_LOG_ERR("Initialize static aggregator (%u) failed!", id);
//		MclAggregatorAllocator_Free(&aggregatorAllocator, self);
//		return NULL;
//	}
//
//	MclAtom_Add(&aggregatorCount, 1);
//	return self;
//
//}
//
//void MclAggregatorFactory_DeleteStatic(MclAggregator *self) {
//	MCL_ASSERT_VALID_PTR_VOID(self);
//
//	MCL_ALLOCATOR_FREE(MclAggregatorAllocator, aggregatorAllocator, self);
//	MclAtom_Sub(&aggregatorCount, 1);
//}
