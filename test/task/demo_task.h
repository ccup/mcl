#ifndef HACCAD3BA_6D60_42A2_8098_99FE7FDA22E5
#define HACCAD3BA_6D60_42A2_8098_99FE7FDA22E5

#include "mcl/task/task.h"
#include "task/priority_level.h"
#include "task/task_history.h"
#include <unistd.h>

MCL_STDC_BEGIN

struct DemoTask {
	MclTask task;
	uint32_t pauseTime;
	PriorityLevel priority;
	TaskHistory *history;
};

MclStatus DemoTask_Execute(MclTask *task) {
	MCL_ASSERT_VALID_PTR(task);

	DemoTask *self = MCL_TYPE_REDUCT(task, DemoTask, task);
	if (self->priority == SLOW) {
		sleep(self->pauseTime);
	}
	self->history->onTaskExecuted(ExecutedTask(self->priority, self->task.key));
	return MCL_SUCCESS;
}

void DemoTask_Init(DemoTask *self,
		           MclTaskKey key,
				   PriorityLevel priority,
				   uint32_t pauseTime,
				   TaskHistory *history) {

	MCL_ASSERT_VALID_PTR_VOID(self);
	MCL_ASSERT_VALID_PTR_VOID(history);
	MCL_ASSERT_TRUE_VOID(priority < MAX_PRIORITY);

	self->task.key = key;
	self->task.execute = DemoTask_Execute;
	self->task.destroy = NULL;
	self->priority = priority;
	self->pauseTime = pauseTime;
	self->history = history;
}

MCL_STDC_END

#endif
