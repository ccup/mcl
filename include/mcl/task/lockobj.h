#ifndef H498CE2EC_9C3F_48BA_ADA8_247B2AAD587E
#define H498CE2EC_9C3F_48BA_ADA8_247B2AAD587E

#include "mcl/status.h"

MCL_STDC_BEGIN

typedef MclStatus (*MclLockObjConstructor)(void *obj, void *arg);
typedef void (*MclLockObjDestructor)(void *obj, void *arg);

void* MclLockObj_Create(uint32_t size, MclLockObjConstructor, void *arg);
void  MclLockObj_Delete(void *obj, MclLockObjDestructor, void *arg);

MclStatus MclLockObj_Lock(void *obj);
MclStatus MclLockObj_Unlock(void *obj);

///////////////////////////////////////////////////////
MCL_INLINE void MclLockObj_AutoUnlock(void *ppobj) {
    if (!ppobj) return;
    void** pObj =  (void**)ppobj;
    (void)MclLockObj_Unlock(*pObj);
}

#define MCL_AUTO_UNLOCK_OBJ  MCL_RAII(MclLockObj_AutoUnlock)

MCL_STDC_END

#endif
