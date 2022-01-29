#ifndef H725220C2_ED4D_40E5_8FDA_3127270649DE
#define H725220C2_ED4D_40E5_8FDA_3127270649DE

#include "entity/mcl_entity.h"
#include "mcl/keyword.h"
#include "mcl/assert.h"

MCL_STDC_BEGIN

MCL_INLINE bool MclEntityPred_IsInAggregator(const MclEntity *entity, void *arg) {
	MCL_ASSERT_VALID_PTR_BOOL(entity);
	MCL_ASSERT_VALID_PTR_BOOL(arg);
	return MclEntity_GetAggregatorId(entity) == *(MclAggregatorId*)arg;
}

MCL_STDC_END

#endif
