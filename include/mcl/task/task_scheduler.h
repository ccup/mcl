#ifndef HACFCFAB7_879C_4A1E_938B_C292D5B85DC3
#define HACFCFAB7_879C_4A1E_938B_C292D5B85DC3

#include "mcl/typedef.h"
#include "mcl/status.h"
#include "mcl/task/task_key.h"
#include "mcl/task/task_priority.h"

MCL_STDC_BEGIN

MCL_TYPE_DECL(MclTask);
MCL_TYPE_DECL(MclTaskScheduler);

MclTaskScheduler* MclTaskScheduler_Create(MclSize threadCount, MclSize priorities, MclSize *thresholds);
void MclTaskScheduler_Delete(MclTaskScheduler*);

MclStatus MclTaskScheduler_Start(MclTaskScheduler*);
MclStatus MclTaskScheduler_Stop(MclTaskScheduler*);

bool MclTaskScheduler_IsRunning(const MclTaskScheduler*);

MclStatus MclTaskScheduler_SubmitTask(MclTaskScheduler*, MclTask*, MclTaskPriority);
MclStatus MclTaskScheduler_RemoveTask(MclTaskScheduler*, MclTaskKey, MclTaskPriority);

void MclTaskScheduler_LocalExecute(MclTaskScheduler*);
void MclTaskScheduler_WaitDone(MclTaskScheduler*);

MCL_STDC_END

#endif
