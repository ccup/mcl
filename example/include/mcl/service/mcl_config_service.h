#ifndef H2F3EA299_6B04_45CA_802F_4B66F0591B4E
#define H2F3EA299_6B04_45CA_802F_4B66F0591B4E

#include "mcl/domain/entity/mcl_entity_id.h"
#include "mcl/status.h"

MCL_STDC_BEGIN

MclStatus MclConfigService_CreateEntity(MclEntityId entityId);
MclStatus MclConfigService_DeleteEntity(MclEntityId entityId);

MCL_STDC_END

#endif
