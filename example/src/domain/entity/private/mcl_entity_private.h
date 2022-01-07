#ifndef HDBD96DB5_DE71_44D3_97D8_D392FF059647
#define HDBD96DB5_DE71_44D3_97D8_D392FF059647

#include "mcl/domain/entity/mcl_entity_id.h"
#include "mcl/domain/value/mcl_integer.h"

MCL_STDC_BEGIN

MCL_TYPE(MclEntity) {
	MclEntityId id;
	MclInteger value;
};

MclStatus MclEntity_Init(MclEntity*, MclEntityId);
void MclEntity_Destroy(MclEntity*);

///////////////////////////////////////////////////////////
#define MCL_ENTITY(ID) {.id = ID, .value = 0}

MCL_STDC_END

#endif
