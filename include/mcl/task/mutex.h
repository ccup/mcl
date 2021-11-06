#ifndef H90B6970A_7AC3_43F4_9432_0647CA0925A1
#define H90B6970A_7AC3_43F4_9432_0647CA0925A1

#include "mcl/keyword.h"
#include "mcl/typedef.h"
#include "mcl/assert.h"
#include "mcl/symbol.h"
#include <pthread.h>

MCL_STDC_BEGIN

typedef pthread_mutex_t MclMutex;
typedef pthread_mutexattr_t MclMutexAttr;

#define MCL_MUTEX_INIT(MUTEX) MUTEX=PTHREAD_MUTEX_INITIALIZER

MCL_INLINE MclStatus MclMutex_InitAttr(MclMutexAttr *attr) {
    return pthread_mutexattr_init(attr) ? MCL_FAILURE : MCL_SUCCESS;
}

MCL_INLINE MclStatus MclMutex_DestroyAttr(MclMutexAttr *attr) {
    return pthread_mutexattr_destroy(attr) ? MCL_FAILURE : MCL_SUCCESS;
}

MCL_INLINE MclStatus MclMutex_SetAttrPShared(MclMutexAttr *attr, int pshared) {
    return pthread_mutexattr_setpshared(attr, pshared) ? MCL_FAILURE : MCL_SUCCESS;
}

MCL_INLINE MclStatus MclMutex_SetAttrType(MclMutexAttr *attr, int type) {
    return pthread_mutexattr_settype(attr, type) ? MCL_FAILURE : MCL_SUCCESS;
}

MCL_INLINE MclStatus MclMutex_Init(MclMutex *mutex, const MclMutexAttr *attr) {
    return pthread_mutex_init(mutex, attr) ?  MCL_FAILURE : MCL_SUCCESS;
}

MCL_INLINE MclStatus MclMutex_Destroy(MclMutex *mutex) {
    return pthread_mutex_destroy(mutex) ?  MCL_FAILURE : MCL_SUCCESS;
}

MCL_INLINE MclStatus MclMutex_InitRecursive(MclMutex *mutex) {
    MclMutexAttr attr;
    MCL_ASSERT_SUCC_CALL(MclMutex_InitAttr(&attr));
    MCL_ASSERT_SUCC_CALL(MclMutex_SetAttrType(&attr, PTHREAD_MUTEX_RECURSIVE));
    return pthread_mutex_init(mutex, &attr) ?  MCL_FAILURE : MCL_SUCCESS;
}

MCL_INLINE MclStatus MclMutex_Lock(MclMutex *mutex) {
    return pthread_mutex_lock(mutex) ?  MCL_FAILURE : MCL_SUCCESS;
}

MCL_INLINE MclStatus MclMutex_Unlock(MclMutex *mutex) {
    return pthread_mutex_unlock(mutex) ?  MCL_FAILURE : MCL_SUCCESS;
}

MCL_INLINE MclStatus MclMutex_TryLock(MclMutex *mutex) {
    return pthread_mutex_unlock(mutex) ?  MCL_FAILURE : MCL_SUCCESS;
}

MCL_TYPE_DEF(MclAutoLock) {
    MclMutex *mutex;
};

MCL_INLINE MclAutoLock MclLock_AutoLock(MclMutex *mutex) {
    MclMutex_Lock(mutex);
    MclAutoLock lock = {.mutex = mutex};
    return lock;
}

MCL_INLINE void MclLock_AutoUnlock(MclAutoLock *lock) {
    if (lock && lock->mutex) {
        (void)MclMutex_Unlock(lock->mutex);
        lock->mutex = NULL;
    }
}

MCL_INLINE bool MclLock_IsLocked(const MclAutoLock *lock) {
    return (lock && lock->mutex);
}

#define MCL_LOCK_AUTO(MUTEX) MCL_RAII(MclLock_AutoUnlock) MclAutoLock MCL_SYMBOL_UNIQUE(MCL_LOCK) = MclLock_AutoLock(&MUTEX)

#define MCL_LOCK_SCOPE(MUTEX)           \
for (MCL_RAII(MclLock_AutoUnlock) MclAutoLock mclLock=MclLock_AutoLock(&MUTEX); MclLock_IsLocked(&mclLock); MclLock_AutoUnlock(&mclLock))

MCL_STDC_END

#endif
