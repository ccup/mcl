#ifndef MCL_6D987B89B5EA4C54972D4C7EE3DBED36
#define MCL_6D987B89B5EA4C54972D4C7EE3DBED36

#include "mcl/typedef.h"
#include "mcl/keyword.h"
#include "mcl/assert.h"
#include "mcl/macro/symbol.h"
#include "mcl/lock/lock_config.h"
#include "mcl/lock/lock_counter.h"
#include <pthread.h>

MCL_STDC_BEGIN

typedef pthread_rwlock_t MclRwLock;
typedef pthread_rwlockattr_t MclRwLockAttr;

#define MCL_RWLOCK()  PTHREAD_RWLOCK_INITIALIZER

MCL_INLINE MclStatus MclRwLock_Init(MclRwLock *self, const MclRwLockAttr *attr) {
    int ret = pthread_rwlock_init(self, attr);
    if (ret) {
        MCL_LOG_FATAL("pthread_rwlock_init fail %d!", ret);
    }
    return ret ?  MCL_FAILURE : MCL_SUCCESS;
}

MCL_INLINE MclStatus MclRwLock_Destroy(MclRwLock *self) {
    int ret = pthread_rwlock_destroy(self);
    if (ret) {
        MCL_LOG_FATAL("MclRwLock_Destroy fail %d!", ret);
    }
    return ret ?  MCL_FAILURE : MCL_SUCCESS;
}

MCL_INLINE MclStatus MclRwLock_RdLock(MclRwLock *self) {
    int ret = pthread_rwlock_rdlock(self);
    if (ret) {
        MCL_LOG_FATAL("pthread_rwlock_rdlock fail %d!", ret);
    } else {
#if MCL_CONFIG_LOCK_COUNT_ENABLE
    	MclLockCounter_CountReadLock();
#endif
    }
    return ret ?  MCL_FAILURE : MCL_SUCCESS;
}

MCL_INLINE MclStatus MclRwLock_TryRdLock(MclRwLock *self) {
    return pthread_rwlock_tryrdlock(self) ?  MCL_FAILURE : MCL_SUCCESS;
}

MCL_INLINE MclStatus MclRwLock_WrLock(MclRwLock *self) {
    int ret = pthread_rwlock_wrlock(self);
    if (ret) {
        MCL_LOG_FATAL("pthread_rwlock_wrlock fail %d!", ret);
    } else {
#if MCL_CONFIG_LOCK_COUNT_ENABLE
    	MclLockCounter_CountWriteLock();
#endif
    }
    return ret ?  MCL_FAILURE : MCL_SUCCESS;
}

MCL_INLINE MclStatus MclRwLock_TryWrLock(MclRwLock *self) {
    return pthread_rwlock_trywrlock(self) ?  MCL_FAILURE : MCL_SUCCESS;
}

MCL_INLINE MclStatus MclRwLock_UnLock(MclRwLock *self) {
    int ret = pthread_rwlock_unlock(self);
    if (ret) {
        MCL_LOG_FATAL("pthread_rwlock_unlock fail %d!", ret);
    } else {
#if MCL_CONFIG_LOCK_COUNT_ENABLE
    	MclLockCounter_CountRwUnlock();
#endif
    }
    return ret ?  MCL_FAILURE : MCL_SUCCESS;
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

MCL_INLINE void MclRwLock_AutoUnLock(MclAutoRwLock *lock) {
    if (lock && lock->rwlock) {
        if (MCL_FAILED(MclRwLock_UnLock(lock->rwlock))) {
            MCL_LOG_FATAL("Auto unlock rwlock failed!");
        }
        lock->rwlock = NULL;
    }
}

MCL_INLINE bool MclRwLock_IsLocked(const MclAutoRwLock *lock) {
    return (lock && lock->rwlock);
}

#define MCL_LOCK_READ_AUTO(RWLOCK)							\
MCL_RAII(MclRwLock_AutoUnLock) MclAutoRwLock MCL_SYMBOL_UNIQUE(MCL_RDLOCK) = MclRwLock_AutoRdLock((MclRwLock*)&RWLOCK)

#define MCL_LOCK_WRITE_AUTO(RWLOCK)							\
MCL_RAII(MclRwLock_AutoUnLock) MclAutoRwLock MCL_SYMBOL_UNIQUE(MCL_WRLOCK) = MclRwLock_AutoWrLock((MclRwLock*)&RWLOCK)

#define MCL_LOCK_READ_SCOPE(RWLOCK)           				\
for (MCL_RAII(MclRwLock_AutoUnLock) MclAutoRwLock mclRdLock=MclRwLock_AutoRdLock((MclRwLock*)&RWLOCK); \
	MclRwLock_IsLocked(&mclRdLock); MclRwLock_AutoUnLock(&mclRdLock))

#define MCL_LOCK_WRITE_SCOPE(RWLOCK)           				\
for (MCL_RAII(MclRwLock_AutoUnLock) MclAutoRwLock mclWrLock=MclRwLock_AutoWrLock((MclRwLock*)&RWLOCK); \
	MclRwLock_IsLocked(&mclWrLock); MclRwLock_AutoUnLock(&mclWrLock))

MCL_STDC_END

#endif
