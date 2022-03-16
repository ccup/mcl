#include "mcl/lock/lock_counter.h"
#include "mcl/lock/atomic.h"
#include "mcl/keyword.h"

typedef struct {
	MclAtomic mutexLock;
	MclAtomic mutexUnlock;
	MclAtomic readLock;
	MclAtomic writeLock;
	MclAtomic rwUnlock;
} MclLockCounter;

MCL_PRIVATE MclLockCounter counter = {0};

void MclLockCounter_CountMutexLock() {
	MclAtomic_AddFetch(&counter.mutexLock, 1);
}

void MclLockCounter_CountMutexUnlock() {
	MclAtomic_AddFetch(&counter.mutexUnlock, 1);
}

void MclLockCounter_CountReadLock() {
	MclAtomic_AddFetch(&counter.readLock, 1);
}

void MclLockCounter_CountWriteLock() {
	MclAtomic_AddFetch(&counter.writeLock, 1);
}

void MclLockCounter_CountRwUnlock() {
	MclAtomic_AddFetch(&counter.rwUnlock, 1);
}

MclSize MclLockCounter_GetMutexLockCount() {
	return MclAtomic_Get(&counter.mutexLock);
}

MclSize MclLockCounter_GetMutexUnlockCount() {
	return MclAtomic_Get(&counter.mutexUnlock);
}

MclSize MclLockCounter_GetReadLockCount() {
	return MclAtomic_Get(&counter.readLock);
}

MclSize MclLockCounter_GetWriteLockCount() {
	return MclAtomic_Get(&counter.writeLock);
}

MclSize MclLockCounter_GetRwUnlockCount() {
	return MclAtomic_Get(&counter.rwUnlock);
}
