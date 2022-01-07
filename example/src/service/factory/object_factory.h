#ifndef H3AA8B6E8_5BD8_40F3_A31F_384C5A5DF024
#define H3AA8B6E8_5BD8_40F3_A31F_384C5A5DF024

#include "mcl/typedef.h"
#include "example/domain/entity/object_id.h"

MCL_STDC_BEGIN

MCL_TYPE_DECL(MclObject);
MCL_TYPE_DECL(MclLockPtr);

MclObject* MclObjectFactory_Create(MclObjectId);
void MclObjectFactory_Delete(MclObject*);

MclObject* MclObjectFactory_CreateSharedPtr(MclObjectId);
void MclObjectFactory_DeleteSharedPtr(MclObject*);

MclObject* MclObjectFactory_CreateLockObj(MclObjectId);
void MclObjectFactory_DeleteLockObj(MclObject*);

MclLockPtr MclObjectFactory_CreateLockPtr(MclObjectId);
void MclObjectFactory_DeleteLockObj(MclLockPtr);

MCL_STDC_END

#endif
