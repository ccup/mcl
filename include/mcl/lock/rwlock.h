#ifndef MCL_6D987B89B5EA4C54972D4C7EE3DBED36
#define MCL_6D987B89B5EA4C54972D4C7EE3DBED36

#include "mcl/typedef.h"
#include "mcl/keyword.h"
#include "mcl/assert.h"
#include "mcl/symbol.h"
#include <pthread.h>

MCL_STDC_BEGIN

typedef pthread_rwlock_t MclRwLock;
typedef pthread_rwlockattr_t MclRwLockAttr;

#define MCL_RWLOCK_INIT(RWLOCK) RWLOCK=PTHREAD_RWLOCK_INITIALIZER

MCL_INLINE MclStatus MclRwLock_Init(MclRwLock *rwlock, const MclRwLockAttr *attr) {
    return pthread_rwlock_init(rwlock, attr) ?  MCL_FAILURE : MCL_SUCCESS;
}

MCL_INLINE MclStatus MclRwLock_Destroy(MclRwLock *rwlock) {
    return pthread_rwlock_destroy(rwlock) ?  MCL_FAILURE : MCL_SUCCESS;
}

MCL_INLINE MclStatus MclRwLock_RdLock(MclRwLock *rwlock) {
    return pthread_rwlock_rdlock(rwlock) ?  MCL_FAILURE : MCL_SUCCESS;
}

MCL_INLINE MclStatus MclRwLock_TryRdLock(MclRwLock *rwlock) {
    return pthread_rwlock_tryrdlock(rwlock) ?  MCL_FAILURE : MCL_SUCCESS;
}

MCL_INLINE MclStatus MclRwLock_WrLock(MclRwLock *rwlock) {
    return pthread_rwlock_wrlock(rwlock) ?  MCL_FAILURE : MCL_SUCCESS;
}

MCL_INLINE MclStatus MclRwLock_TryWrLock(MclRwLock *rwlock) {
    return pthread_rwlock_trywrlock(rwlock) ?  MCL_FAILURE : MCL_SUCCESS;
}

MCL_INLINE MclStatus MclRwLock_Unlock(MclRwLock *rwlock) {
    return pthread_rwlock_unlock(rwlock) ?  MCL_FAILURE : MCL_SUCCESS;
}

///////////////////////////////////////////////////////////
MCL_TYPE(MclAutoRwLock) {
    MclRwLock *rwlock;
};

MCL_INLINE MclAutoRwLock MclRwLock_AutoRdLock(MclRwLock *rwlock) {
    if (!rwlock) {
        MCL_LOG_FATAL("Auto lock a NULL rwlock!");
    }
    if (MCL_FAILED(MclRwLock_RdLock(rwlock))) {
        MCL_LOG_FATAL("Auto read lock failed!");
    }
    MclAutoRwLock lock = {.rwlock = rwlock};
    return lock;
}

MCL_INLINE MclAutoRwLock MclRwLock_AutoWrLock(MclRwLock *rwlock) {
    if (!rwlock) {
        MCL_LOG_FATAL("Auto lock a NULL rwlock!");
    }
    if (MCL_FAILED(MclRwLock_WrLock(rwlock))) {
        MCL_LOG_FATAL("Auto write lock failed!");
    }
    MclAutoRwLock lock = {.rwlock = rwlock};
    return lock;
}

MCL_INLINE void MclRwLock_AutoUnlock(MclAutoRwLock *lock) {
    if (lock && lock->rwlock) {
        if (MCL_FAILED(MclRwLock_Unlock(lock->rwlock))) {
            MCL_LOG_FATAL("Auto unlock rwlock failed!");
        }
        lock->rwlock = NULL;
    }
}

MCL_INLINE bool MclRwLock_IsLocked(const MclAutoRwLock *lock) {
    return (lock && lock->rwlock);
}

#define MCL_LOCK_READ_AUTO(RWLOCK) MCL_RAII(MclRwLock_AutoUnlock) MclAutoRwLock MCL_SYMBOL_UNIQUE(MCL_RDLOCK) = MclRwLock_AutoRdLock((MclRwLock*)&RWLOCK)

#define MCL_LOCK_WRITE_AUTO(RWLOCK) MCL_RAII(MclRwLock_AutoUnlock) MclAutoRwLock MCL_SYMBOL_UNIQUE(MCL_WRLOCK) = MclRwLock_AutoWrLock((MclRwLock*)&RWLOCK)

#define MCL_LOCK_READ_SCOPE(RWLOCK)           \
for (MCL_RAII(MclRwLock_AutoUnlock) MclAutoRwLock mclRdLock=MclRwLock_AutoRdLock((MclRwLock*)&RWLOCK); MclRwLock_IsLocked(&mclRdLock); MclRwLock_AutoUnlock(&mclRdLock))

#define MCL_LOCK_WRITE_SCOPE(RWLOCK)           \
for (MCL_RAII(MclRwLock_AutoUnlock) MclAutoRwLock mclWrLock=MclRwLock_AutoWrLock((MclRwLock*)&RWLOCK); MclRwLock_IsLocked(&mclWrLock); MclRwLock_AutoUnlock(&mclWrLock))

MCL_STDC_END

#endif
