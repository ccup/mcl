#include "../../../example/src/domain/entity/object.h"
#include "../../../example/src/domain/entity/private/object_private.h"
#include "mcl/mem/malloc.h"
#include "mcl/assert.h"

MclObjectId MclObject_GetId(const MclObject *self) {
	return self ? self->id : MCL_OBJECT_ID_INVALID;
}

uint64_t MclObject_GetValue(const MclObject *self) {
	return self ? self->value : 0;
}

MclStatus MclObject_DoubleValue(MclObject *self) {
	MCL_ASSERT_VALID_PTR(self);

	if (self->value == 0) return MCL_SUCCESS;

	self->value *= 2;
	return MCL_SUCCESS;
}
