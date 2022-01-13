#ifndef H72BE1BCA_46B7_4EBE_AFBF_117CCC6EA21F
#define H72BE1BCA_46B7_4EBE_AFBF_117CCC6EA21F

#include "mcl/domain/value/mcl_integer.h"
#include "mcl/domain/entity/mcl_entity_id.h"
#include "mcl/domain/aggregator/mcl_aggregator_id.h"

MCL_STDC_BEGIN

MCL_TYPE_DECL(MclStatistics);

size_t MclQueryService_QueryEntityCount();
size_t MclQueryService_QueryAggregatorCount();
size_t MclQueryService_QueryEntityCountOfAggregator(MclAggregatorId);

MclInteger MclQueryService_QueryValueOf(MclEntityId);
MclInteger MclQueryService_QuerySumValueOf(MclAggregatorId);

MclStatistics MclQueryService_QueryStatistics();

MCL_STDC_END

#endif
