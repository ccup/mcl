#ifndef HA6F6E7C5_9742_4718_9877_E89ECA2963A1
#define HA6F6E7C5_9742_4718_9877_E89ECA2963A1

#include "mcl/typedef.h"
#include "mcl/status.h"

MCL_STDC_BEGIN

MCL_TYPE_DECL(MclThreadPool);
MCL_TYPE_DECL(MclTaskQueue);

MclThreadPool* MclThreadPool_Create(const char *name, MclSize threadCount);
void MclThreadPool_Delete(MclThreadPool*);

MclStatus MclThreadPool_Start(MclThreadPool*);
MclStatus MclThreadPool_Stop(MclThreadPool*);

bool MclThreadPool_IsRunning(const MclThreadPool*);

MclStatus MclThreadPool_SubmitTaskQueue(MclThreadPool*, MclTaskQueue*);
MclTaskQueue* MclThreadPool_RemoveTaskQueue(MclThreadPool*);

void MclThreadPool_LocalExecute(MclThreadPool*);
void MclThreadPool_WaitDone(MclThreadPool*);

MCL_STDC_END

#endif
