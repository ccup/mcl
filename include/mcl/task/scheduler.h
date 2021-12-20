#ifndef HACFCFAB7_879C_4A1E_938B_C292D5B85DC3
#define HACFCFAB7_879C_4A1E_938B_C292D5B85DC3

#include "mcl/typedef.h"
#include "mcl/status.h"
#include "mcl/task/key.h"

MCL_STDC_BEGIN

MCL_TYPE_FWD(MclTask);
MCL_TYPE_FWD(MclTaskScheduler);

MclTaskScheduler* MclTaskScheduler_Create(uint32_t threadCount, uint32_t priorities, uint32_t *thresholds);
void MclTaskScheduler_Delete(MclTaskScheduler*);

MclStatus MclTaskScheduler_Start(MclTaskScheduler*);
MclStatus MclTaskScheduler_Stop(MclTaskScheduler*);

bool MclTaskScheduler_IsRunning(const MclTaskScheduler*);

MclStatus MclTaskScheduler_SubmitTask(MclTaskScheduler*, MclTask*, uint32_t priority);
MclStatus MclTaskScheduler_RemoveTask(MclTaskScheduler*, MclTaskKey, uint32_t priority);

void MclTaskScheduler_WaitDone(MclTaskScheduler*);

MCL_STDC_END

#endif
