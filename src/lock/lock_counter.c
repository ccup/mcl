#include "mcl/lock/lock_counter.h"
#include "mcl/lock/atom.h"
#include "mcl/keyword.h"

typedef struct {
	MclAtom mutexLock;
	MclAtom mutexUnlock;
	MclAtom readLock;
	MclAtom writeLock;
	MclAtom rwUnlock;
} MclLockCounter;

MCL_PRIVATE MclLockCounter counter = {0};

void MclLockCounter_CountMutexLock() {
	MclAtom_AddFetch(&counter.mutexLock, 1);
}

void MclLockCounter_CountMutexUnlock() {
	MclAtom_AddFetch(&counter.mutexUnlock, 1);
}

void MclLockCounter_CountReadLock() {
	MclAtom_AddFetch(&counter.readLock, 1);
}

void MclLockCounter_CountWriteLock() {
	MclAtom_AddFetch(&counter.writeLock, 1);
}

void MclLockCounter_CountRwUnlock() {
	MclAtom_AddFetch(&counter.rwUnlock, 1);
}

MclSize MclLockCounter_GetMutexLockCount() {
	return MclAtom_Get(&counter.mutexLock);
}

MclSize MclLockCounter_GetMutexUnlockCount() {
	return MclAtom_Get(&counter.mutexUnlock);
}

MclSize MclLockCounter_GetReadLockCount() {
	return MclAtom_Get(&counter.readLock);
}

MclSize MclLockCounter_GetWriteLockCount() {
	return MclAtom_Get(&counter.writeLock);
}

MclSize MclLockCounter_GetRwUnlockCount() {
	return MclAtom_Get(&counter.rwUnlock);
}
