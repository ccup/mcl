#ifndef H3AA8B6E8_5BD8_40F3_A31F_384C5A5DF024
#define H3AA8B6E8_5BD8_40F3_A31F_384C5A5DF024

#include "mcl/domain/entity/mcl_entity_id.h"
#include "mcl/typedef.h"

MCL_STDC_BEGIN

MCL_TYPE_DECL(MclEntity);

MclSize MclEntityFactory_GetUnreleasedCount();

MclEntity* MclEntityFactory_Create(MclEntityId, void *cfg);
void MclEntityFactory_Delete(MclEntity*);

MclEntity* MclEntityFactory_CreateSharedPtr(MclEntityId, void *cfg);
void MclEntityFactory_DeleteSharedPtr(MclEntity*);

MclEntity* MclEntityFactory_CreateLockObj(MclEntityId, void *cfg);
void MclEntityFactory_DeleteLockObj(MclEntity*);

MclEntity* MclEntityFactory_CreateStatic(MclEntityId, void *cfg);
void MclEntityFactory_DeleteStatic(MclEntity*);

MCL_STDC_END

#endif
