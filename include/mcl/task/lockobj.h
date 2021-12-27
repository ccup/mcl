#ifndef H498CE2EC_9C3F_48BA_ADA8_247B2AAD587E
#define H498CE2EC_9C3F_48BA_ADA8_247B2AAD587E

#include "mcl/status.h"

MCL_STDC_BEGIN

typedef void (*MclLockObjDestructor)(void *obj, void *arg);

void* MclLockObj_Create(size_t size);
void  MclLockObj_Delete(void *obj, MclLockObjDestructor, void *arg);

MclStatus MclLockObj_Lock(void *obj);
MclStatus MclLockObj_Unlock(void *obj);

///////////////////////////////////////////////////////
MCL_INLINE void MclLockObj_AutoUnlock(void *ppobj) {
    if (!ppobj) return;
    void** pObj =  (void**)ppobj;
    (void)MclLockObj_Unlock(*pObj);
}

#define MCL_UNLOCK_OBJ_AUTO  MCL_RAII(MclLockObj_AutoUnlock)

MCL_STDC_END

#endif
