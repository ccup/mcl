#ifndef H98A0A9BD_7F09_489A_87CE_6F26DBC34E48
#define H98A0A9BD_7F09_489A_87CE_6F26DBC34E48

#include "mcl/keyword.h"

MCL_STDC_BEGIN

typedef uint32_t MclEntityId;

static const MclEntityId MCL_ENTITY_ID_INVALID = 0xFFFFFFFF;

MCL_INLINE bool MclEntityId_IsValid(MclEntityId self) {
	return self != MCL_ENTITY_ID_INVALID;
}

MCL_STDC_END

#endif
