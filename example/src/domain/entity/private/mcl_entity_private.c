#include "mcl_entity_private.h"

bool MclEntityPrivate_IsOverflow(const MclEntity *self, MclSize time) {
	return  ((uint64_t)self->value) * time > MCL_INTEGER_MAX;
}

