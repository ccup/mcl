#include "aggregator/mcl_aggregator.h"
#include "entity/mcl_entity_list.h"
#include "mcl/assert.h"

MCL_TYPE(MclAggregator) {
	MclAggregatorId id;
	MclEntityList entities;
};

const size_t MCL_AGGREGATOR_SIZE = sizeof(MclAggregator);

MclStatus MclAggregator_Init(MclAggregator *self, MclAggregatorId id) {
	MCL_ASSERT_VALID_PTR(self);
	MCL_ASSERT_TRUE(MclAggregatorId_IsValid(id));
	return MCL_SUCCESS;
}

void MclAggregator_Destroy(MclAggregator *self) {
	MCL_ASSERT_VALID_PTR_VOID(self);
}

MclAggregatorId MclAggregator_GetId(const MclAggregator *self) {
	return self ? self->id : MCL_AGGREGATOR_ID_INVALID;
}

size_t MclAggregator_GetEntityCount(const MclAggregator *self) {
	MCL_ASSERT_VALID_PTR_NIL(self);
	return MclEntityList_GetCount(&self->entities);
}

MclStatus MclAggregator_AddEntity(MclAggregator *self, MclEntity *entity) {
	MCL_ASSERT_VALID_PTR(self);
	MCL_ASSERT_VALID_PTR(entity);
	return MCL_SUCCESS;
}

void MclAggregator_RemoveEntity(MclAggregator *self, MclEntity *entity) {
	MCL_ASSERT_VALID_PTR_VOID(self);
	MCL_ASSERT_VALID_PTR_VOID(entity);
}

MclStatus MclAggregator_DoubleAll(MclAggregator *self) {
	MCL_ASSERT_VALID_PTR(self);
	return MCL_SUCCESS;
}
