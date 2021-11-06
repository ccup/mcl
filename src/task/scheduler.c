#include "mcl/task/scheduler.h"
#include "mcl/task/queue.h"
#include "mcl/task/thread.h"
#include "mcl/task/task.h"
#include "mcl/mem/malloc.h"
#include "mcl/assert.h"

#define MCL_TASK_SCHEDULER_THREAD_MAX 8
#define MCL_TASK_SCHEDULER_LEVEL_MAX  3

MCL_TYPE_DEF(MclTaskScheduler) {
	MclThread threads[MCL_TASK_SCHEDULER_THREAD_MAX];
	uint32_t threadCount;
	MclTaskQueue *taskQueue;
};

MCL_PRIVATE MclStatus MclTaskScheduler_StartThreads(MclTaskScheduler *self) {
	for (uint32_t i = 0; i < self->threadCount; i++) {
		MCL_ASSERT_SUCC_CALL(MclThread_Create(&self->threads[i], NULL, MclTaskQueue_Execute, self->taskQueue));
	}
	return MCL_SUCCESS;
}

MCL_PRIVATE MclStatus MclTaskScheduler_JoinThreads(MclTaskScheduler *self) {
	for (uint32_t i = 0; i < self->threadCount; i++) {
		MclThread_Join(self->threads[i], NULL);
	}
	return MCL_SUCCESS;
}

bool MclTaskScheduler_IsRunning(const MclTaskScheduler *self){
	MCL_ASSERT_VALID_PTR_BOOL(self);
	return MclTaskQueue_IsReady(self->taskQueue);
}

MclTaskScheduler* MclTaskScheduler_Create(uint32_t threadCount, uint32_t priorities, uint32_t *thresholds) {
	MCL_ASSERT_TRUE_NIL(threadCount <= MCL_TASK_SCHEDULER_THREAD_MAX);
	MCL_ASSERT_TRUE_NIL(priorities <= MCL_TASK_SCHEDULER_LEVEL_MAX);

	MclTaskScheduler *self = MCL_MALLOC(sizeof(MclTaskScheduler));
	MCL_ASSERT_VALID_PTR_NIL(self);

	self->threadCount = threadCount;
	self->taskQueue = MclTaskQueue_Create(priorities, thresholds);
	if (!self->taskQueue) {
		MCL_LOG_ERR("Task queue create failed!");
		MCL_FREE(self);
		return NULL;
	}
	return self;
}

void MclTaskScheduler_Delete(MclTaskScheduler *self) {
	MCL_ASSERT_VALID_PTR_VOID(self);

	if (MclTaskScheduler_IsRunning(self)) {
		MclTaskScheduler_Stop(self);
	}
	MclTaskQueue_Delete(self->taskQueue);
	MCL_FREE(self);
}

MclStatus MclTaskScheduler_Start(MclTaskScheduler *self) {
	MCL_ASSERT_VALID_PTR(self);

	if (MclTaskScheduler_IsRunning(self)) return MCL_SUCCESS;

	MCL_ASSERT_SUCC_CALL(MclTaskQueue_Start(self->taskQueue));
	MCL_ASSERT_SUCC_CALL(MclTaskScheduler_StartThreads(self));
	return MCL_SUCCESS;
}

MclStatus MclTaskScheduler_Stop(MclTaskScheduler *self) {
	MCL_ASSERT_VALID_PTR(self);

	if (!MclTaskScheduler_IsRunning(self)) return MCL_SUCCESS;

	MCL_ASSERT_SUCC_CALL(MclTaskQueue_Stop(self->taskQueue));
	MCL_ASSERT_SUCC_CALL(MclTaskScheduler_JoinThreads(self));
	return MCL_SUCCESS;
}

MclStatus MclTaskScheduler_AddTask(MclTaskScheduler *self, MclTask *task, uint32_t priority) {
	MCL_ASSERT_VALID_PTR(self);
	MCL_ASSERT_VALID_PTR(task);
	MCL_ASSERT_TRUE(priority < MCL_TASK_SCHEDULER_LEVEL_MAX);

	MCL_ASSERT_SUCC_CALL(MclTaskQueue_InsertTask(self->taskQueue, task, priority));
	return MCL_SUCCESS;
}

MclStatus MclTaskScheduler_DelTask(MclTaskScheduler *self, MclTaskKey key, uint32_t priority) {
	MCL_ASSERT_VALID_PTR(self);
	MCL_ASSERT_TRUE(priority < MCL_TASK_SCHEDULER_LEVEL_MAX);

	MCL_ASSERT_SUCC_CALL(MclTaskQueue_RemoveTask(self->taskQueue, key, priority));
	return MCL_SUCCESS;
}
