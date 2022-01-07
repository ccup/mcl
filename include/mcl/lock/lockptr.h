#ifndef MCL_ED99144E686E49A1ADF6F1BE984D14E1
#define MCL_ED99144E686E49A1ADF6F1BE984D14E1

#include "mcl/lock/rwlock.h"

MCL_STDC_BEGIN

MCL_TYPE(MclLockPtr) {
    MclRwLock rwlock;
    void *ptr;
};

typedef void (*MclLockPtrDeleter)(void*, void*);
MclLockPtr* MclLockPtr_Create(void *ptr);
void MclLockPtr_Delete(MclLockPtr*, MclLockPtrDeleter, void *arg);

MclStatus MclLockPtr_Init(MclLockPtr*, void *ptr);
void MclLockPtr_Destroy(MclLockPtr*, MclLockPtrDeleter, void *arg);

MclStatus MclLockPtr_WrLock(MclLockPtr*);
MclStatus MclLockPtr_RdLock(MclLockPtr*);
MclStatus MclLockPtr_UnLock(MclLockPtr*);

///////////////////////////////////////////////////////////
MCL_INLINE void* MclLockPtr_Get(MclLockPtr *self) {
    return self ? self->ptr : NULL;
}

MCL_INLINE const void* MclLockPtr_GetConst(const MclLockPtr *self) {
    return self ? self->ptr : NULL;
}

MCL_INLINE bool MclLockPtr_IsValid(const MclLockPtr *self) {
    return self && self->ptr;
}

///////////////////////////////////////////////////////////
#define MCL_LOCK_PTR(PTR)  {.rwlock = MCL_RWLOCK(), .ptr = PTR}

///////////////////////////////////////////////////////////
MCL_INLINE void MclLockPtr_AutoUnLock(const MclLockPtr **ppPtr) {
    if (!ppPtr) return;
    (void)MclLockPtr_UnLock((MclLockPtr*)(*ppPtr));
}

#define MCL_LOCK_PTR_AUTO  MCL_RAII(MclLockPtr_AutoUnLock)

MCL_STDC_END

#endif
