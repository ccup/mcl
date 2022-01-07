#include "entity/private/mcl_entity_private.h"

MCL_PRIVATE void MclEntity_Clear(MclEntity *self) {
	MclInteger_Clear(&self->value);
}

MclStatus MclEntity_Init(MclEntity *self, MclEntityId id) {
	self->id = id;
	MclEntity_Clear(self);
	return MCL_SUCCESS;
}

void MclEntity_Destroy(MclEntity *self) {
	MclEntity_Clear(self);
	self->id = MCL_ENTITY_ID_INVALID;
}
