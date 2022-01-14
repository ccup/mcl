#include "mcl/task/task_scheduler.h"
#include "mcl/task/task_queue.h"
#include "mcl/thread/thread.h"
#include "mcl/task/task.h"
#include "mcl/lock/atom.h"
#include "mcl/mem/malloc.h"
#include "mcl/assert.h"

#define MCL_TASK_SCHEDULER_THREAD_MAX 8
#define MCL_TASK_SCHEDULER_LEVEL_MAX  3

MCL_TYPE(MclTaskScheduler) {
	MclThread threads[MCL_TASK_SCHEDULER_THREAD_MAX];
	MclSize threadCount;
	MclAtom isRunning;
	MclTaskQueue *taskQueue;
};

MCL_PRIVATE void MclTaskScheduler_ExecuteTaskInThread(MclTaskScheduler *self) {
    MCL_LOG_DBG("Task thread try to pop task...");
    MclTask *task = MclTaskQueue_PopTask(self->taskQueue);
    if (task) {
        MCL_LOG_DBG("Task thread popped task (%u).", task->key);
        MCL_ASSERT_SUCC_CALL_VOID(MclTask_Execute(task));
        MclTask_Destroy(task);
    } else {
        MCL_LOG_WARN("Task Thread popped none task.");
    }
}

MCL_PRIVATE void* MclTaskScheduler_ThreadExecute(void *data) {
    MclTaskScheduler *self = (MclTaskScheduler*)data;
    if (!self) return NULL;

    MclThread tid = MclThread_GetId();
    MCL_ASSERT_SUCC_CALL_NIL(MclThread_SetName(tid, "TaskScheduler"));

    MCL_LOG_DBG("Task thread in scheduler begin!");

    while (MclAtom_IsTrue(&self->isRunning)) {
        MclTaskScheduler_ExecuteTaskInThread(self);
    }
    MCL_LOG_DBG("Task thread in scheduler done!");
    return NULL;
}

MCL_PRIVATE MclStatus MclTaskScheduler_StartThreads(MclTaskScheduler *self) {
	for (MclSize i = 0; i < self->threadCount; i++) {
		MCL_ASSERT_SUCC_CALL(MclThread_Create(&self->threads[i], NULL, MclTaskScheduler_ThreadExecute, self));
	}
	return MCL_SUCCESS;
}

MCL_PRIVATE MclStatus MclTaskScheduler_JoinThreads(MclTaskScheduler *self) {
	for (MclSize i = 0; i < self->threadCount; i++) {
		MclThread_Join(self->threads[i], NULL);
	}
	return MCL_SUCCESS;
}

MCL_PRIVATE MclStatus MclTaskScheculer_Launch(MclTaskScheduler *self) {
    MCL_LOG_DBG("Task scheduler try to launch...");

    MclAtom_Set(&self->isRunning, 1);
    MclTaskQueue_Start(self->taskQueue);

    if (MCL_FAILED(MclTaskScheduler_StartThreads(self))) {
        MCL_LOG_ERR("Task scheduler start threads failed!");
        MclAtom_Set(&self->isRunning, 0);
        return MCL_FAILURE;
    }
    MCL_LOG_DBG("Task scheduler launch OK!");
    return MCL_SUCCESS;
}

MCL_PRIVATE MclStatus MclTaskScheduler_Quit(MclTaskScheduler *self) {
    MCL_LOG_DBG("Task scheduler try to quit...");

    MclAtom_Set(&self->isRunning, 0);
    MclTaskQueue_Stop(self->taskQueue);

    if (MCL_FAILED(MclTaskScheduler_JoinThreads(self))) {
        MCL_LOG_ERR("Task scheduler join threads failed!");
        MclAtom_Set(&self->isRunning, 1);
        return MCL_FAILURE;
    }
    MCL_LOG_DBG("Task scheduler quit OK!");
    return MCL_SUCCESS;
}

MCL_PRIVATE MclStatus MclTaskScheduler_Init(MclTaskScheduler *self, MclSize threadCount, MclSize priorities, MclSize *thresholds) {
    self->taskQueue = MclTaskQueue_Create(priorities, thresholds);
    MCL_ASSERT_VALID_PTR(self->taskQueue);

    self->threadCount = threadCount;
    MclAtom_Clear(&self->isRunning);
    return MCL_SUCCESS;
}

bool MclTaskScheduler_IsRunning(const MclTaskScheduler *self){
	MCL_ASSERT_VALID_PTR_BOOL(self);
	return MclAtom_IsTrue(&((MclTaskScheduler*)self)->isRunning);
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

void MclTaskScheduler_Delete(MclTaskScheduler *self) {
	MCL_ASSERT_VALID_PTR_VOID(self);

	if (MclAtom_IsTrue(&self->isRunning)) {
        (void) MclTaskScheduler_Quit(self);
	}
	MclTaskQueue_Delete(self->taskQueue);
	MCL_FREE(self);

    MCL_LOG_DBG("Task scheduler delete OK!");
}

MclStatus MclTaskScheduler_Start(MclTaskScheduler *self) {
	MCL_ASSERT_VALID_PTR(self);

	if (MclAtom_IsTrue(&self->isRunning)) return MCL_SUCCESS;

    MCL_ASSERT_SUCC_CALL(MclTaskScheculer_Launch(self));
	return MCL_SUCCESS;
}

MclStatus MclTaskScheduler_Stop(MclTaskScheduler *self) {
	MCL_ASSERT_VALID_PTR(self);

	if (!MclAtom_IsTrue(&self->isRunning)) return MCL_SUCCESS;

	MCL_ASSERT_SUCC_CALL(MclTaskScheduler_Quit(self));
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
	MCL_ASSERT_VALID_PTR_VOID(self->taskQueue);

    MCL_LOG_DBG("Task thread execute in local begin!");

	while (!MclTaskQueue_IsEmpty(self->taskQueue)) {
        MclTaskScheduler_ExecuteTaskInThread(self);
	}
	MCL_LOG_DBG("Task thread executed in local done!");
}

void MclTaskScheduler_WaitDone(MclTaskScheduler *self) {
    MCL_ASSERT_VALID_PTR_VOID(self);
    MCL_ASSERT_VALID_PTR_VOID(self->taskQueue);
    MCL_ASSERT_TRUE_VOID(MclAtom_IsTrue(&self->isRunning));

    MCL_LOG_DBG("Task scheduler wait begin.");

    while (!MclTaskQueue_IsEmpty(self->taskQueue)) {
        if (self->threadCount == 0) {
            MclTaskScheduler_ExecuteTaskInThread(self);
        } else {
            MclThread_Yield();
        }
    }
    MCL_LOG_DBG("Task scheduler wait done!");
}
