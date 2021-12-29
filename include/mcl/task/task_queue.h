#ifndef H53A34707_1BAC_45DA_A91E_2B62231025E5
#define H53A34707_1BAC_45DA_A91E_2B62231025E5

#include "mcl/typedef.h"
#include "mcl/status.h"
#include "mcl/task/task_key.h"

MCL_STDC_BEGIN

MCL_TYPE_DECL(MclTask);
MCL_TYPE_DECL(MclTaskQueue);

MclTaskQueue* MclTaskQueue_Create(uint32_t priorities, uint32_t *thresholds);
void MclTaskQueue_Delete(MclTaskQueue*);

MclStatus MclTaskQueue_Start(MclTaskQueue*);
MclStatus MclTaskQueue_Stop(MclTaskQueue*);

MclStatus MclTaskQueue_SubmitTask(MclTaskQueue*, MclTask*, uint32_t priority);
MclStatus MclTaskQueue_RemoveTask(MclTaskQueue*, MclTaskKey, uint32_t priority);

bool MclTaskQueue_IsReady(const MclTaskQueue*);
bool MclTaskQueue_IsEmpty(const MclTaskQueue*);

void MclTaskQueue_ExecuteAll(MclTaskQueue*);

void* MclTaskQueue_ThreadExecute(void*);

MCL_STDC_END

#endif
