#ifndef H72BE1BCA_46B7_4EBE_AFBF_117CCC6EA21F
#define H72BE1BCA_46B7_4EBE_AFBF_117CCC6EA21F

#include "mcl/domain/value/mcl_integer.h"
#include "mcl/domain/entity/mcl_entity_id.h"
#include "mcl/domain/aggregator/mcl_aggregator_id.h"

MCL_STDC_BEGIN

MclInteger MclQueryService_QueryValueOf(MclEntityId);
MclInteger MclQueryService_QuerySumValueOf(MclAggregatorId);

size_t MclQueryService_QueryEntityCountIn(MclAggregatorId);

MCL_STDC_END

#endif
