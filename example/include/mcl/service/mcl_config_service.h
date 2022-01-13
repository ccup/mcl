#ifndef H2F3EA299_6B04_45CA_802F_4B66F0591B4E
#define H2F3EA299_6B04_45CA_802F_4B66F0591B4E

#include "mcl/domain/entity/mcl_entity_id.h"
#include "mcl/domain/aggregator/mcl_aggregator_id.h"
#include "mcl/status.h"

MCL_STDC_BEGIN

MCL_TYPE_DECL(MclEntityConfig);

MclStatus MclConfigService_CreateEntity(MclEntityId, const MclEntityConfig*);
MclStatus MclConfigService_DeleteEntity(MclEntityId);

MclStatus MclConfigService_CreateAggregator(MclAggregatorId);
MclStatus MclConfigService_DeleteAggregator(MclAggregatorId);

MCL_STDC_END

#endif
