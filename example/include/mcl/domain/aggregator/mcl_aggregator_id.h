#ifndef HE0D510F5_4A46_4642_BBBF_6510C8A349A7
#define HE0D510F5_4A46_4642_BBBF_6510C8A349A7

#include "mcl/keyword.h"

MCL_STDC_BEGIN

typedef uint32_t MclAggregatorId;

static const MclAggregatorId MCL_AGGREGATOR_ID_INVALID = 0xFFFFFFFF;

MCL_INLINE bool MclAggregatorId_IsValid(MclAggregatorId self) {
	return self != MCL_AGGREGATOR_ID_INVALID;
}

MCL_STDC_END

#endif
