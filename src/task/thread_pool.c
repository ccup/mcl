#include "mcl/task/thread_pool.h"
#include "mcl/thread/thread_launcher.h"
#include "mcl/task/task_queue.h"
#include "mcl/thread/thread.h"
#include "mcl/task/task.h"
#include "mcl/algo/loop.h"
#include "mcl/mem/memory.h"
#include "mcl/assert.h"

MCL_TYPE(MclThreadPool) {
	const char *name;
	MclTaskQueue *taskQueue;
	MclSize threadCount;
	MclThreadInfo threads[];
};

MCL_PRIVATE void MclThreadPool_ExecuteTask(MclTaskQueue *taskQueue) {
    MCL_LOG_DBG("Task thread try to pop task...");
    MclTask *task = MclTaskQueue_PopTask(taskQueue);
    if (task) {
        MCL_LOG_DBG("Task thread popped task (%u).", task->key);
        MCL_ASSERT_SUCC_CALL_VOID(MclTask_Execute(task));
        MclTask_Destroy(task);
    } else {
        MCL_LOG_WARN("Task Thread popped none task.");
    }
}

MCL_PRIVATE void MclThreadPool_RunThread(void *ctxt) {
	MclTaskQueue *taskQueue = (MclTaskQueue*)ctxt;
	MCL_ASSERT_VALID_PTR_VOID(taskQueue);

    while (MclTaskQueue_IsRunning(taskQueue)) {
        MclThreadPool_ExecuteTask(taskQueue);
    }
}

MCL_PRIVATE void MclThreadPool_StopThread(void *ctxt) {
	MclTaskQueue *taskQueue = (MclTaskQueue*)ctxt;
	MCL_ASSERT_VALID_PTR_VOID(taskQueue);

	MclTaskQueue_Stop(taskQueue);
}

MCL_PRIVATE MclStatus MclThreadPool_Launch(MclThreadPool *self) {
    MCL_LOG_DBG("%s launch begin...", self->name);

    MclTaskQueue_Start(self->taskQueue);

    if (MCL_FAILED(MclThreadLauncher_Launch(self->threads, self->threadCount))) {
        MCL_LOG_ERR("%s launch threads failed!", self->name);
        MclTaskQueue_Stop(self->taskQueue);
        return MCL_FAILURE;
    }
    MCL_LOG_DBG("%s launch OK!", self->name);
    return MCL_SUCCESS;
}

MCL_PRIVATE MclStatus MclThreadPool_Quit(MclThreadPool *self) {
    MCL_LOG_DBG("%s quit begin...", self->name);
    MclThreadLauncher_WaitDone(self->threads, self->threadCount);
    MCL_LOG_DBG("%s quit OK!", self->name);
    return MCL_SUCCESS;
}

MCL_PRIVATE void MclThreadPool_InitThread(MclThreadPool *self, MclSize index)
{
	self->threads[index].name = self->name;
	self->threads[index].run = MclThreadPool_RunThread;
	self->threads[index].stop = (index == 0) ? MclThreadPool_StopThread : NULL;
	self->threads[index].ctxt = self->taskQueue;
}

MCL_PRIVATE MclStatus MclThreadPool_InitThreads(MclThreadPool *self, MclSize threadCount)
{
	MCL_LOOP_FOREACH_INDEX(i, threadCount) {
		MclThreadPool_InitThread(self, i);
	}
    self->threadCount = threadCount;
    return MCL_SUCCESS;
}

MCL_PRIVATE MclStatus MclThreadPool_Init(MclThreadPool *self, const char *name, MclSize threadCount) {
	self->name = name ? name : "MclThreadPool";
    self->taskQueue = NULL;
    MCL_ASSERT_SUCC_CALL(MclThreadPool_InitThreads(self, threadCount));
    return MCL_SUCCESS;
}

MclThreadPool* MclThreadPool_Create(const char *name, MclSize threadCount) {
	MCL_ASSERT_TRUE_NIL(threadCount > 0);

	MclThreadPool *self = MCL_MALLOC(sizeof(MclThreadPool) + sizeof(MclThreadInfo) * threadCount);
	MCL_ASSERT_VALID_PTR_NIL(self);

	if (MCL_FAILED(MclThreadPool_Init(self, name, threadCount))) {
		MCL_LOG_ERR("%s init failed!", self->name);
		MCL_FREE(self);
		return NULL;
	}

    MCL_LOG_DBG("%s create OK!", self->name);
	return self;
}

void MclThreadPool_Delete(MclThreadPool *self) {
	MCL_ASSERT_VALID_PTR_VOID(self);
	MCL_ASSERT_VALID_PTR_VOID(self->taskQueue);

	if (MclTaskQueue_IsRunning(self->taskQueue)) {
        (void) MclThreadPool_Quit(self);
	}
//	MclTaskQueue_Delete(self->taskQueue); // TODO : should delete out or by args?
    MCL_LOG_DBG("%s delete OK!", self->name);
	MCL_FREE(self);
}

MclStatus MclThreadPool_Start(MclThreadPool *self) {
	MCL_ASSERT_VALID_PTR(self);
	MCL_ASSERT_VALID_PTR(self->taskQueue);

	if (MclTaskQueue_IsRunning(self->taskQueue)) return MCL_SUCCESS;

    MCL_ASSERT_SUCC_CALL(MclThreadPool_Launch(self));
    MCL_LOG_DBG("%s start OK!", self->name);
	return MCL_SUCCESS;
}

MclStatus MclThreadPool_Stop(MclThreadPool *self) {
	MCL_ASSERT_VALID_PTR(self);
	MCL_ASSERT_VALID_PTR(self->taskQueue);

	if (!MclTaskQueue_IsRunning(self->taskQueue)) return MCL_SUCCESS;

	MCL_ASSERT_SUCC_CALL(MclThreadPool_Quit(self));
	MCL_LOG_DBG("%s stop OK!", self->name);
    return MCL_SUCCESS;
}

bool MclThreadPool_IsRunning(const MclThreadPool *self){
	MCL_ASSERT_VALID_PTR_BOOL(self);
	MCL_ASSERT_VALID_PTR_BOOL(self->taskQueue);

	return MclTaskQueue_IsRunning(self->taskQueue);
}

MclStatus MclThreadPool_SubmitTaskQueue(MclThreadPool *self, MclTaskQueue *queue)
{
	MCL_ASSERT_VALID_PTR(self);
	MCL_ASSERT_VALID_PTR(queue);
	MCL_ASSERT_TRUE(self->taskQueue == NULL);

	self->taskQueue = queue;
	MCL_LOOP_FOREACH_INDEX(i, self->threadCount) {
		self->threads[i].ctxt = self->taskQueue;
	}

	MCL_LOG_DBG("%s submit task queue OK!", self->name);
	return MCL_SUCCESS;
}

MclTaskQueue* MclThreadPool_RemoveTaskQueue(MclThreadPool *self)
{
	MCL_ASSERT_VALID_PTR_NIL(self);

	MclTaskQueue *queue = self->taskQueue;
	self->taskQueue = NULL;

	MCL_LOG_DBG("%s remove task queue OK!", self->name);
	return queue;
}

void MclThreadPool_LocalExecute(MclThreadPool *self) {
	MCL_ASSERT_VALID_PTR_VOID(self);
	MCL_ASSERT_VALID_PTR_VOID(self->taskQueue);

    MCL_LOG_DBG("%s execute in local begin!", self->name);

	while (!MclTaskQueue_IsEmpty(self->taskQueue)) {
        MclThreadPool_ExecuteTask(self->taskQueue);
	}
	MCL_LOG_DBG("%s executed in local done!", self->name);
}

void MclThreadPool_WaitDone(MclThreadPool *self) {
    MCL_ASSERT_VALID_PTR_VOID(self);
    MCL_ASSERT_VALID_PTR_VOID(self->taskQueue);
    MCL_ASSERT_TRUE_VOID(MclTaskQueue_IsRunning(self->taskQueue));

    MCL_LOG_DBG("%s wait begin.", self->name);

    while (!MclTaskQueue_IsEmpty(self->taskQueue)) {
        if (self->threadCount == 0) {
            MclThreadPool_ExecuteTask(self->taskQueue);
        } else {
            MclThread_Yield();
        }
    }
    MCL_LOG_DBG("%s wait done!", self->name);
}
