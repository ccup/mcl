#ifndef H498CE2EC_9C3F_48BA_ADA8_247B2AAD587E
#define H498CE2EC_9C3F_48BA_ADA8_247B2AAD587E

#include "mcl/stdtype.h"
#include "mcl/status.h"

MCL_STDC_BEGIN

typedef MclStatus (*MclLockObj_ObjInit)(void *obj, void *arg);
typedef void (*MclLockObj_ObjDestroy)(void *obj, void *arg);

void* MclLockObj_Create(uint32_t size, MclLockObj_ObjInit, void *arg);
void  MclLockObj_Delete(void *obj, MclLockObj_ObjDestroy, void *arg);

MclStatus MclLockObj_Lock(void *obj);
MclStatus MclLockObj_Unlock(void *obj);

MCL_STDC_END

#endif
