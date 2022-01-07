#ifndef HDBD96DB5_DE71_44D3_97D8_D392FF059647
#define HDBD96DB5_DE71_44D3_97D8_D392FF059647

#include "example/domain/entity/object_id.h"
#include "mcl/typedef.h"

MCL_STDC_BEGIN

MCL_TYPE(MclObject) {
	MclObjectId id;
	uint32_t value;
};

MclStatus MclObject_Init(MclObject*, MclObjectId);
void MclObject_Destroy(MclObject*);

///////////////////////////////////////////////////////////
#define MCL_OBJECT(ID) {.id = ID, .value = 0}

MCL_STDC_END

#endif
