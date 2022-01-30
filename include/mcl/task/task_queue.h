#ifndef H53A34707_1BAC_45DA_A91E_2B62231025E5
#define H53A34707_1BAC_45DA_A91E_2B62231025E5

#include "mcl/typedef.h"
#include "mcl/status.h"
#include "mcl/task/task_key.h"
#include "mcl/task/task_priority.h"

MCL_STDC_BEGIN

MCL_TYPE_DECL(MclTask);
MCL_TYPE_DECL(MclTaskQueue);

MclTaskQueue* MclTaskQueue_Create(MclSize priorities, MclSize *thresholds);
void MclTaskQueue_Delete(MclTaskQueue*);

void MclTaskQueue_Start(MclTaskQueue *self);
void MclTaskQueue_Stop(MclTaskQueue *self);

bool MclTaskQueue_IsRunning(const MclTaskQueue*);

bool MclTaskQueue_IsEmpty(const MclTaskQueue*);

MclStatus MclTaskQueue_AddTask(MclTaskQueue*, MclTask*, MclTaskPriority);
MclStatus MclTaskQueue_DelTask(MclTaskQueue*, MclTaskKey, MclTaskPriority);
MclTask*  MclTaskQueue_PopTask(MclTaskQueue *);

MCL_STDC_END

#endif
