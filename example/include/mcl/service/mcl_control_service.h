#ifndef H72BE1BCA_46B7_4EBE_AFBF_117CCC6EA21E
#define H72BE1BCA_46B7_4EBE_AFBF_117CCC6EA21E

#include "mcl/domain/entity/mcl_entity_id.h"
#include "mcl/domain/aggregator/mcl_aggregator_id.h"
#include "mcl/status.h"

MCL_STDC_BEGIN

MclStatus MclControlService_AddEntityToAggregator(MclEntityId, MclAggregatorId);
MclStatus MclControlService_RemoveEntityFromAggregator(MclEntityId, MclAggregatorId);

MclStatus MclControlService_DoubleEntitesInAggregator(MclAggregatorId);
MclStatus MclControlService_DoubleEntity(MclEntityId);

MCL_STDC_END

#endif
