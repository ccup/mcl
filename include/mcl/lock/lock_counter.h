#ifndef HC5348500_6932_4FBB_BA51_F27F3CDCD8D9
#define HC5348500_6932_4FBB_BA51_F27F3CDCD8D9

#include "mcl/stdtype.h"

MCL_STDC_BEGIN

void MclLockCounter_CountMutexLock();
void MclLockCounter_CountMutexUnlock();
void MclLockCounter_CountReadLock();
void MclLockCounter_CountWriteLock();
void MclLockCounter_CountRwUnlock();

MclSize MclLockCounter_GetMutexLockCount();
MclSize MclLockCounter_GetMutexUnlockCount();
MclSize MclLockCounter_GetReadLockCount();
MclSize MclLockCounter_GetWriteLockCount();
MclSize MclLockCounter_GetRwUnlockCount();

MCL_STDC_END

#endif
