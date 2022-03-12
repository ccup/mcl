#include "entity/mcl_entity.h"
#include "entity/private/mcl_entity_private.h"
#include "mcl/assert.h"

MclStatus MclEntity_Init(MclEntity *self, MclEntityId id, void *cfg) {
	self->id = id;
	MclInteger_Clear(&self->value);
	self->aggregatorId = MCL_AGGREGATOR_ID_INVALID;
	return MCL_SUCCESS;
}

void MclEntity_Destroy(MclEntity *self) {
	MclEntity_ClearValue(self);
	self->id = MCL_ENTITY_ID_INVALID;
	self->aggregatorId = MCL_AGGREGATOR_ID_INVALID;
}

MclEntityId MclEntity_GetId(const MclEntity *self) {
	return self ? self->id : MCL_ENTITY_ID_INVALID;
}

MclInteger MclEntity_GetValue(const MclEntity *self) {
	return self ? self->value : 0;
}

MclAggregatorId MclEntity_GetAggregatorId(const MclEntity *self) {
	return self ? self->aggregatorId : MCL_AGGREGATOR_ID_INVALID;
}

void MclEntity_OnAddToAggregator(MclEntity *self, MclAggregatorId aggregatorId) {
	MCL_ASSERT_VALID_PTR_VOID(self);
	MCL_ASSERT_TRUE_VOID(MclAggregatorId_IsValid(aggregatorId));
	MCL_ASSERT_TRUE_VOID(!MclAggregatorId_IsValid(self->aggregatorId));

	self->aggregatorId = aggregatorId;
}

void MclEntity_OnRemoveFromAggregator(MclEntity *self) {
	MCL_ASSERT_VALID_PTR_VOID(self);

	self->aggregatorId = MCL_AGGREGATOR_ID_INVALID;
	MclInteger_Clear(&self->value);
}

MclStatus MclEntity_UpdateValue(MclEntity *self, MclInteger value) {
	MCL_ASSERT_VALID_PTR(self);

	if (self->value == value) return MCL_STATUS_NOTHING_CHANGED;

	self->value = value;
	return MCL_SUCCESS;
}

MCL_PRIVATE const MclSize DOUBLE_TIME = 2;

MclStatus MclEntity_DoubleValue(MclEntity *self) {
	MCL_ASSERT_VALID_PTR(self);

	if (self->value == 0) return MCL_STATUS_NOTHING_CHANGED;

	if (MclEntityPrivate_IsOverflow(self, DOUBLE_TIME)) return MCL_FAILURE;

	self->value *= DOUBLE_TIME;
	return MCL_SUCCESS;
}

void MclEntity_ClearValue(MclEntity *self) {
	MclInteger_Clear(&self->value);
}
