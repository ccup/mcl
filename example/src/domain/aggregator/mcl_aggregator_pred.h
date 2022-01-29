#ifndef HDB3FFC9B_6F5A_4E71_8DA4_CF87BF34BBAE
#define HDB3FFC9B_6F5A_4E71_8DA4_CF87BF34BBAE

#include "aggregator/mcl_aggregator.h"
#include "mcl/keyword.h"
#include "mcl/assert.h"

MCL_STDC_BEGIN

MCL_INLINE bool MclAggregatorPred_HasEntity(const MclAggregator *aggregator, void *arg) {
	MCL_ASSERT_VALID_PTR_BOOL(aggregator);
	MCL_ASSERT_VALID_PTR_BOOL(arg);
	return MclAggregator_HasEntity(aggregator, *(MclEntityId*)arg);
}

MCL_STDC_END

#endif
