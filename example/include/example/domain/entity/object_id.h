#ifndef H98A0A9BD_7F09_489A_87CE_6F26DBC34E48
#define H98A0A9BD_7F09_489A_87CE_6F26DBC34E48

#include "mcl/keyword.h"

MCL_STDC_BEGIN

typedef uint32_t MclObjectId;

static const MclObjectId MCL_OBJECT_ID_INVALID = 0xFFFFFFFF;

MCL_INLINE bool MclObjectId_IsValid(MclObjectId self) {
	return self != MCL_OBJECT_ID_INVALID;
}

MCL_STDC_END

#endif
