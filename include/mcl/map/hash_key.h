#ifndef HF9C79CDA_16C8_4F15_B889_599D3C3F7D49
#define HF9C79CDA_16C8_4F15_B889_599D3C3F7D49

#include "mcl/keyword.h"

MCL_STDC_BEGIN

typedef uint64_t MclHashKey;

static const MclHashKey MCL_HASH_KEY_INVALID = 0xFFFFFFFFFFFFFFFF;

MCL_INLINE bool MclHashKey_IsValid(MclHashKey self) {
	return self != MCL_HASH_KEY_INVALID;
}

MCL_STDC_END

#endif
