#ifndef H3AA8B6E8_5BD8_40F3_A31F_384C5A5DF024
#define H3AA8B6E8_5BD8_40F3_A31F_384C5A5DF024

#include "mcl/infra/allocator/mcl_allocator_type.h"
#include "mcl/domain/entity/mcl_entity_id.h"
#include "mcl/typedef.h"

MCL_STDC_BEGIN

MCL_TYPE_DECL(MclEntity);

void MclEntityFactory_Init(MclAllocatorType);

MclEntity* MclEntityFactory_Create(MclEntityId, void *cfg);

void MclEntityFactory_Delete(MclEntity*);

MclSize MclEntityFactory_GetUnreleasedCount();

MCL_STDC_END

#endif
