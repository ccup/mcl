#include "mcl/task/task_scheduler.h"
#include "mcl/task/task_queue.h"
#include "mcl/task/thread_pool.h"
#include "mcl/task/task.h"
#include "mcl/mem/memory.h"
#include "mcl/assert.h"

#define MCL_TASK_SCHEDULER_THREAD_MAX 8
#define MCL_TASK_SCHEDULER_LEVEL_MAX  3

MCL_TYPE(MclTaskScheduler) {
	MclThreadPool *threadPool;
	MclTaskQueue  *taskQueue;
};

MCL_PRIVATE MclStatus MclTaskScheduler_Init(MclTaskScheduler *self, MclSize threadCount, MclSize priorities, MclSize *thresholds) {
	self->threadPool = MclThreadPool_Create("TaskScheduler", threadCount);
	MCL_ASSERT_VALID_PTR(self->threadPool);

	self->taskQueue = MclTaskQueue_Create(priorities, thresholds);
	if (!self->taskQueue) {
		MclThreadPool_Delete(self->threadPool);
		MCL_LOG_ERR("Create task queue failed!");
		return MCL_FAILURE;
	}

	if (MCL_FAILED(MclThreadPool_SubmitTaskQueue(self->threadPool, self->taskQueue))) {
		MclTaskQueue_Delete(self->taskQueue);
		MclThreadPool_Delete(self->threadPool);
		MCL_LOG_ERR("Submit task queue to thread pool failed!");
		return MCL_FAILURE;
	}
	return MCL_SUCCESS;
}

MclTaskScheduler* MclTaskScheduler_Create(MclSize threadCount, MclSize priorities, MclSize *thresholds) {
	MCL_ASSERT_TRUE_NIL(threadCount <= MCL_TASK_SCHEDULER_THREAD_MAX);
	MCL_ASSERT_TRUE_NIL(priorities <= MCL_TASK_SCHEDULER_LEVEL_MAX);

	MclTaskScheduler *self = MCL_MALLOC(sizeof(MclTaskScheduler));
	MCL_ASSERT_VALID_PTR_NIL(self);

	if (MCL_FAILED(MclTaskScheduler_Init(self, threadCount, priorities, thresholds))) {
		MCL_LOG_ERR("Task scheduler init failed!");
		MCL_FREE(self);
		return NULL;
	}

    MCL_LOG_DBG("Task scheduler create OK!");
	return self;
}

MCL_PRIVATE void MclTaskScheduler_Destroy(MclTaskScheduler *self) {
	MclThreadPool_Delete(self->threadPool);
	MclTaskQueue_Delete(self->taskQueue);
}

void MclTaskScheduler_Delete(MclTaskScheduler *self) {
	MCL_ASSERT_VALID_PTR_VOID(self);

	MclTaskScheduler_Destroy(self);
	MCL_FREE(self);

    MCL_LOG_DBG("Task scheduler delete OK!");
}

bool MclTaskScheduler_IsRunning(const MclTaskScheduler *self){
	MCL_ASSERT_VALID_PTR_BOOL(self);
	return MclThreadPool_IsRunning(self->threadPool);
}

MclStatus MclTaskScheduler_Start(MclTaskScheduler *self) {
	MCL_ASSERT_VALID_PTR(self);
    MCL_ASSERT_SUCC_CALL(MclThreadPool_Start(self->threadPool));
	return MCL_SUCCESS;
}

MclStatus MclTaskScheduler_Stop(MclTaskScheduler *self) {
	MCL_ASSERT_VALID_PTR(self);
	MCL_ASSERT_SUCC_CALL(MclThreadPool_Stop(self->threadPool));
    return MCL_SUCCESS;
}

MclStatus MclTaskScheduler_SubmitTask(MclTaskScheduler *self, MclTask *task, MclTaskPriority priority) {
	MCL_ASSERT_VALID_PTR(self);
	MCL_ASSERT_VALID_PTR(task);
	MCL_ASSERT_TRUE(priority < MCL_TASK_SCHEDULER_LEVEL_MAX);

	MCL_ASSERT_SUCC_CALL(MclTaskQueue_AddTask(self->taskQueue, task, priority));

    MCL_LOG_DBG("Task scheduler submit task (%u) of pri (%u).", task->key, priority);
	return MCL_SUCCESS;
}

MclStatus MclTaskScheduler_RemoveTask(MclTaskScheduler *self, MclTaskKey key, MclTaskPriority priority) {
	MCL_ASSERT_VALID_PTR(self);
	MCL_ASSERT_TRUE(MclTaskKey_IsValid(key));
	MCL_ASSERT_TRUE(priority < MCL_TASK_SCHEDULER_LEVEL_MAX);

	MCL_ASSERT_SUCC_CALL(MclTaskQueue_DelTask(self->taskQueue, key, priority));

    MCL_LOG_DBG("Task scheduler remove task (%u) of pri (%u).", key, priority);
	return MCL_SUCCESS;
}

void MclTaskScheduler_LocalExecute(MclTaskScheduler *self) {
	MCL_ASSERT_VALID_PTR_VOID(self);
	MclThreadPool_LocalExecute(self->threadPool);
}

void MclTaskScheduler_WaitDone(MclTaskScheduler *self) {
    MCL_ASSERT_VALID_PTR_VOID(self);
    MclThreadPool_WaitDone(self->threadPool);
}
