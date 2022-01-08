#ifndef MCL_F2A54BFBC3EF41079397B09166E3F21A
#define MCL_F2A54BFBC3EF41079397B09166E3F21A

#include "mcl/keyword.h"

MCL_STDC_BEGIN

typedef void* MclHashValue;

static const MclHashValue MCL_HASH_VALUE_INVALID = NULL;

MCL_INLINE bool MclHashValue_IsValid(MclHashValue self) {
	return self != MCL_HASH_VALUE_INVALID;
}

typedef void (*MclHashValueDestroy)(MclHashValue);

MCL_STDC_END

#endif
