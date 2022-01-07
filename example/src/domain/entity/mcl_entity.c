#include "entity/mcl_entity.h"
#include "entity/private/mcl_entity_private.h"
#include "mcl/assert.h"

MCL_PRIVATE const uint32_t DOUBLE_TIME = 2;

MCL_PRIVATE bool MclEntity_IsOverflowFor(const MclEntity *self, uint32_t time) {
	return  ((uint64_t)self->value) * time > MCL_INTEGER_MAX;
}

MclEntityId MclEntity_GetId(const MclEntity *self) {
	return self ? self->id : MCL_ENTITY_ID_INVALID;
}

MclInteger MclEntity_GetValue(const MclEntity *self) {
	return self ? self->value : 0;
}

MclStatus MclEntity_DoubleValue(MclEntity *self) {
	MCL_ASSERT_VALID_PTR(self);

	if (self->value == 0) return MCL_SUCCESS;

	if (MclEntity_IsOverflowFor(self, DOUBLE_TIME)) return MCL_FAILURE;

	self->value *= DOUBLE_TIME;
	return MCL_SUCCESS;
}
