#ifndef H90B6970A_7AC3_43F4_9432_0647CA0925A0
#define H90B6970A_7AC3_43F4_9432_0647CA0925A0

#include "mcl/keyword.h"
#include "mcl/assertion.h"
#include "mcl/symbol.h"
#include <pthread.h>

MCL_STDC_BEGIN

#define MCL_MUTEX pthread_mutex_t
#define MCL_MUTEX_ATTR pthread_mutexattr_t

#define MCL_MUTEX_INITIALIZE(MUTEX) MUTEX=PTHREAD_MUTEX_INITIALIZER

MCL_INLINE MclStatus Mcl_InitMutexAttr(MCL_MUTEX_ATTR *attr) {
    return pthread_mutexattr_init(attr) ? MCL_FAILURE : MCL_SUCCESS;
}

MCL_INLINE MclStatus Mcl_DestroyMutexAttr(MCL_MUTEX_ATTR *attr) {
    return pthread_mutexattr_destroy(attr) ? MCL_FAILURE : MCL_SUCCESS;
}

MCL_INLINE MclStatus Mcl_SetMutexAttrPShared(MCL_MUTEX_ATTR *attr, int pshared) {
    return pthread_mutexattr_setpshared(attr, pshared) ? MCL_FAILURE : MCL_SUCCESS;
}

MCL_INLINE MclStatus Mcl_SetMutexAttrType(MCL_MUTEX_ATTR *attr, int type) {
    return pthread_mutexattr_settype(attr, type) ? MCL_FAILURE : MCL_SUCCESS;
}

MCL_INLINE MclStatus Mcl_InitMutex(MCL_MUTEX *mutex, const MCL_MUTEX_ATTR *attr) {
    return pthread_mutex_init(mutex, attr) ?  MCL_FAILURE : MCL_SUCCESS;
}

MCL_INLINE MclStatus Mcl_DestroyMutex(MCL_MUTEX *mutex) {
    return pthread_mutex_destroy(mutex) ?  MCL_FAILURE : MCL_SUCCESS;
}

MCL_INLINE MclStatus Mcl_InitRecursiveMutex(MCL_MUTEX *mutex) {
    MCL_MUTEX_ATTR attr;
    MCL_ASSERT_SUCC_CALL(Mcl_InitMutexAttr(&attr));
    MCL_ASSERT_SUCC_CALL(Mcl_SetMutexAttrType(&attr, PTHREAD_MUTEX_RECURSIVE));
    return pthread_mutex_init(mutex, &attr) ?  MCL_FAILURE : MCL_SUCCESS;
}

MCL_INLINE MclStatus Mcl_LockMutex(MCL_MUTEX *mutex) {
    return pthread_mutex_lock(mutex) ?  MCL_FAILURE : MCL_SUCCESS;
}

MCL_INLINE MclStatus Mcl_UnlockMutex(MCL_MUTEX *mutex) {
    return pthread_mutex_unlock(mutex) ?  MCL_FAILURE : MCL_SUCCESS;
}

MCL_INLINE MclStatus Mcl_TryLockMutex(MCL_MUTEX *mutex) {
    return pthread_mutex_unlock(mutex) ?  MCL_FAILURE : MCL_SUCCESS;
}

typedef struct MclAutoLock {
    MCL_MUTEX *mutex;
} MclAutoLock;

MclAutoLock Mcl_AutoLockCreate(MCL_MUTEX *mutex) {
    Mcl_LockMutex(mutex);
    MclAutoLock lock = {.mutex = mutex};
    return lock;
}

void Mcl_AutoLockDestroy(MclAutoLock *lock) {
    if (lock && lock->mutex) {
        (void)Mcl_UnlockMutex(lock->mutex);
        lock->mutex = NULL;
    }
}

bool Mcl_IsAutoLocked(MclAutoLock *lock) {
    return (lock && lock->mutex);
}

#define MCL_AUTO_LOCK(MUTEX) MCL_RAII(Mcl_AutoLockDestroy) MclAutoLock MCL_UNIQUE_NAME(MCL_LOCK) = Mcl_AutoLockCreate(&MUTEX)

#define MCL_LOCK_SCOPE(MUTEX)           \
for (MCL_RAII(Mcl_AutoLockDestroy) MclAutoLock mclLock=Mcl_AutoLockCreate(&MUTEX); Mcl_IsAutoLocked(&mclLock); Mcl_AutoLockDestroy(&mclLock))

MCL_STDC_END

#endif