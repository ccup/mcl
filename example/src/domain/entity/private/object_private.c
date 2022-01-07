#include "../../../../example/src/domain/entity/private/object_private.h"

MCL_PRIVATE void MclObject_Clear(MclObject *self) {
	self->value = 0;
}

MclStatus MclObject_Init(MclObject *self, MclObjectId id) {
	self->id = id;
	MclObject_Clear(self);
	return MCL_SUCCESS;
}

void MclObject_Destroy(MclObject *self) {
	MclObject_Clear(self);
	self->id = MCL_OBJECT_ID_INVALID;
}
