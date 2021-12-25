#ifndef MCL_ED99144E686E49A1ADF6F1BE984D14E1
#define MCL_ED99144E686E49A1ADF6F1BE984D14E1

#include "mcl/task/mutex.h"

MCL_STDC_BEGIN

typedef void (*MclLockPtrDeleter)(void*);

MCL_TYPE(MclLockPtr) {
    void *ptr;
    MclMutex mutex;
    MclLockPtrDeleter deleter;
};

MCL_INLINE void* MclLockPtr_Get(MclLockPtr *self) {
    return self ? self->ptr : NULL;
}

MCL_INLINE bool MclLockPtr_IsValid(const MclLockPtr *self) {
    return self && self->ptr;
}

MclLockPtr* MclLockPtr_Create(void *ptr, MclLockPtrDeleter);
void MclLockPtr_Delete(MclLockPtr*);

MclStatus MclLockPtr_Init(MclLockPtr*, void *ptr, MclLockPtrDeleter);
void MclLockPtr_Destroy(MclLockPtr*);
void MclLockPtr_UniqueDestroy(MclLockPtr*);

MclStatus MclLockPtr_Lock(MclLockPtr*);
MclStatus MclLockPtr_Unlock(MclLockPtr*);

MCL_STDC_END

#endif
