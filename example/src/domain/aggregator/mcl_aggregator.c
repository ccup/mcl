#include "aggregator/mcl_aggregator.h"
#include "entity/mcl_entity_list.h"
#include "entity/mcl_entity.h"
#include "mcl/assert.h"

MCL_TYPE(MclAggregator) {
	MclAggregatorId id;
	MclEntityList entities;
};

const MclSize MCL_AGGREGATOR_SIZE = sizeof(MclAggregator);

MclStatus MclAggregator_Init(MclAggregator *self, MclAggregatorId id, void *cfg) {
	MCL_ASSERT_VALID_PTR(self);
	MCL_ASSERT_TRUE(MclAggregatorId_IsValid(id));

	self->id = id;
	MclEntityList_Init(&self->entities);
	return MCL_SUCCESS;
}

void MclAggregator_Destroy(MclAggregator *self) {
	MCL_ASSERT_VALID_PTR_VOID(self);

	self->id = MCL_AGGREGATOR_ID_INVALID;
	MclEntityList_Destroy(&self->entities, MclEntity_OnRemoveFromAggregator);
}

MclAggregatorId MclAggregator_GetId(const MclAggregator *self) {
	return self ? self->id : MCL_AGGREGATOR_ID_INVALID;
}

MclSize MclAggregator_GetEntityCount(const MclAggregator *self) {
	MCL_ASSERT_VALID_PTR_NIL(self);
	return MclEntityList_GetSize(&self->entities);
}

MclStatus MclAggregator_AddEntity(MclAggregator *self, MclEntity *entity) {
	MCL_ASSERT_VALID_PTR(self);
	MCL_ASSERT_VALID_PTR(entity);

	if (MclEntityList_HasEntity(&self->entities, MclEntity_GetId(entity))) {
		return MCL_STATUS_NOTHING_CHANGED;
	}

	MCL_ASSERT_SUCC_CALL(MclEntityList_Insert(&self->entities, entity));

	MclEntity_OnAddToAggregator(entity, self->id);
	return MCL_SUCCESS;
}

MclStatus MclAggregator_RemoveEntity(MclAggregator *self, MclEntityId entityId) {
	MCL_ASSERT_VALID_PTR(self);
	MCL_ASSERT_TRUE(MclEntityId_IsValid(entityId));

	if (MclEntityList_HasEntity(&self->entities, entityId)) {
		return MCL_STATUS_NOTHING_CHANGED;
	}

	MclEntity *entity = MclEntityList_Remove(&self->entities, entityId);
	if (!entity) {
		return MCL_STATUS_NOTHING_CHANGED;
	}

	MclEntity_OnRemoveFromAggregator(entity);
	return MCL_SUCCESS;
}

bool MclAggregator_HasEntity(const MclAggregator *self, MclEntityId entityId) {
	MCL_ASSERT_VALID_PTR_BOOL(self);
	MCL_ASSERT_TRUE_BOOL(MclEntityId_IsValid(entityId));

	return MclEntityList_HasEntity(&self->entities, entityId);
}

MCL_PRIVATE MclStatus MclAggregator_DoubleEntity(MclEntity *entity, void *arg) {
	return MclEntity_DoubleValue(entity);
}

MclStatus MclAggregator_DoubleEntities(MclAggregator *self) {
	MCL_ASSERT_VALID_PTR(self);

	if (MclEntityList_IsEmpty(&self->entities)) {
		return MCL_STATUS_NOTHING_CHANGED;
	}
	MCL_ASSERT_SUCC_CALL(MclEntityList_Accept(&self->entities, MclAggregator_DoubleEntity, NULL));
	return MCL_SUCCESS;
}

MCL_PRIVATE MclStatus MclAggregator_SumEntityValue(const MclEntity *entity, void *arg) {
	MclInteger *sum = (MclInteger*)arg;

	(*sum) += MclEntity_GetValue(entity);
	return MCL_SUCCESS;
}

MclInteger MclAggregator_GetSumValue(const MclAggregator *self) {
	MCL_ASSERT_VALID_PTR_NIL(self);

	MclInteger sum = 0;
	MCL_ASSERT_SUCC_CALL(MclEntityList_AcceptConst(&self->entities, MclAggregator_SumEntityValue, &sum));
	return sum;
}
