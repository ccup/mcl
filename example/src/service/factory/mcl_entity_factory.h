#ifndef H3AA8B6E8_5BD8_40F3_A31F_384C5A5DF024
#define H3AA8B6E8_5BD8_40F3_A31F_384C5A5DF024

#include "mcl/domain/entity/mcl_entity_id.h"
#include "mcl/typedef.h"

MCL_STDC_BEGIN

MCL_TYPE_DECL(MclEntity);
MCL_TYPE_DECL(MclLockPtr);

MclEntity* MclEntityFactory_Create(MclEntityId);
void MclEntityFactory_Delete(MclEntity*);

MclEntity* MclEntityFactory_CreateSharedPtr(MclEntityId);
void MclEntityFactory_DeleteSharedPtr(MclEntity*);

MclEntity* MclEntityFactory_CreateLockObj(MclEntityId);
void MclEntityFactory_DeleteLockObj(MclEntity*);

MclLockPtr* MclEntityFactory_CreateLockPtr(MclEntityId);
void MclEntityFactory_DeleteLockPtr(MclLockPtr*);

size_t MclEntityFactory_GetUnreleasedCount();

MCL_STDC_END

#endif
