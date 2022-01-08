#ifndef HC1A219BE_19EE_4506_B7BD_C39433B086B0
#define HC1A219BE_19EE_4506_B7BD_C39433B086B0

#include "mcl/keyword.h"

MCL_STDC_BEGIN

typedef uint32_t MclTaskKey;

static const MclTaskKey MCL_TASK_KEY_INVALID = 0xffffffff;

MCL_INLINE bool MclTaskKey_IsValid(MclTaskKey self) {
	return self != MCL_TASK_KEY_INVALID;
}

MCL_STDC_END

#endif
