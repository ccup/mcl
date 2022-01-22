#include "mcl/task/task_queue.h"
#include "mcl/lock/cond.h"
#include "mcl/task/task.h"
#include "mcl/list/list.h"
#include "mcl/list/list_node_allocator.h"
#include "mcl/mem/memory.h"
#include "mcl/assert.h"

typedef struct {
	MclList  tasks;
	MclSize threshold;
	MclSize poppedCount;
} TaskQueue;

MCL_PRIVATE void TaskQueue_Init(TaskQueue *queue, MclSize threshold) {
	MclList_Init(&queue->tasks, &MclListNodeAllocator_Default);
	queue->threshold = threshold;
	queue->poppedCount = 0;
}

MCL_PRIVATE void TaskQueue_Destroy(TaskQueue *queue) {
	MclList_Clear(&queue->tasks, (MclListDataDestroy)MclTask_Destroy);
}

MCL_PRIVATE bool MclTaskKeyPred_IsEqual(MclListData data, void *arg) {
	return ((MclTask*)data)->key == *(MclTaskKey*)arg;
}

MCL_PRIVATE void TaskQueue_Remove(TaskQueue *queue, MclTaskKey key) {
	MclList_RemoveAllByPred(&queue->tasks, MclTaskKeyPred_IsEqual, &key, (MclListDataDestroy)MclTask_Destroy);
}

MCL_PRIVATE bool TaskQueue_IsEmpty(const TaskQueue *queue) {
	return MclList_IsEmpty(&queue->tasks);
}

MCL_PRIVATE void TaskQueue_Push(TaskQueue *queue, MclTask *task) {
	MclList_PushBack(&queue->tasks, task);
}

MCL_PRIVATE bool TaskQueue_IsReachThreshold(const TaskQueue *queue) {
	if (queue->threshold == 0 || queue->poppedCount == 0) return false;
	return queue->poppedCount % queue->threshold == 0;
}

MCL_PRIVATE void TaskQueue_ResetPoppedCount(TaskQueue *queue) {
	queue->poppedCount = 0;
}

MCL_PRIVATE MclTask* TaskQueue_Pop(TaskQueue *queue) {
	if (MclList_IsEmpty(&queue->tasks)) return NULL;

	MclTask *task = (MclTask*)MclList_RemoveFirst(&queue->tasks);
	if (!task) return NULL;

	queue->poppedCount += 1;
	return task;
}

///////////////////////////////////////////////////////////
MCL_TYPE(MclTaskQueue) {
	MclMutex mutex;
	MclCond   cond;
	bool    stopped;
	MclSize queueCount;
	TaskQueue queues[];
};

MCL_PRIVATE bool MclTaskQueue_IsEmptyImpl(const MclTaskQueue *self) {
    for (MclSize i = 0; i < self->queueCount; i++) {
        if (!TaskQueue_IsEmpty(&self->queues[i])) return false;
    }
    return true;
}

MCL_PRIVATE void MclTaskQueue_WaitReady(MclTaskQueue *self) {
    if (!self->stopped && MclTaskQueue_IsEmptyImpl(self)) {
        MclCond_Wait(&self->cond, &self->mutex);
    }
}

MCL_PRIVATE MclTask* MclTaskQueue_PopTaskImpl(MclTaskQueue *self) {
    for (MclSize i = 0; i < self->queueCount; i++) {
        if (TaskQueue_IsEmpty(&self->queues[i])) continue;
        if (TaskQueue_IsReachThreshold(&self->queues[i])) {
            TaskQueue_ResetPoppedCount(&self->queues[i]);
            continue;
        }
        return TaskQueue_Pop(&self->queues[i]);
    }
    return NULL;
}

MCL_PRIVATE void MclTaskQueue_DestroyQueues(MclTaskQueue *self) {
    for (MclSize i = 0; i < self->queueCount; i++) {
        TaskQueue_Destroy(&self->queues[i]);
    }
    self->queueCount = 0;
}

MCL_PRIVATE void  MclTaskQueue_Destroy(MclTaskQueue *self) {
    self->stopped = true;
    MclTaskQueue_DestroyQueues(self);
    MCL_PEEK_SUCC_CALL(MclMutex_Destroy(&self->mutex));
    MCL_PEEK_SUCC_CALL(MclCond_Destroy(&self->cond));
}

MCL_PRIVATE void MclTaskQueue_InitQueues(MclTaskQueue *self, MclSize priorities, MclSize *thresholds) {
	self->queueCount = priorities;
	for (MclSize i = 0; i < priorities; i++) {
		MclSize threshold = ((thresholds == NULL) || (i + 1 >= priorities)) ? 0 : thresholds[i];
		TaskQueue_Init(&self->queues[i], threshold);
	}
}

MCL_PRIVATE MclStatus MclTaskQueue_Init(MclTaskQueue *self, MclSize priorities, MclSize *thresholds) {
	if (MCL_FAILED(MclMutex_Init(&self->mutex, NULL))) {
		MCL_LOG_ERR("Init mutex failed!");
		return MCL_FAILURE;
	}
	if (MCL_FAILED(MclCond_Init(&self->cond, NULL))) {
		MCL_LOG_ERR("Init cond failed!");
        (void)MclMutex_Destroy(&self->mutex);
		return MCL_FAILURE;
	}
    MclTaskQueue_InitQueues(self, priorities, thresholds);
	self->stopped = false;
    return MCL_SUCCESS;
}

MclTaskQueue* MclTaskQueue_Create(MclSize priorities, MclSize *thresholds) {
	MCL_ASSERT_TRUE_NIL(priorities > 0);

	MclTaskQueue *self = MCL_MALLOC(sizeof(MclTaskQueue) + sizeof(TaskQueue) * priorities);
	MCL_ASSERT_VALID_PTR_NIL(self);

	if (MCL_FAILED(MclTaskQueue_Init(self, priorities, thresholds))) {
		MCL_FREE(self);
		return NULL;
	}
	return self;
}

/* IMPORTANT: SHOULD INVOKE AFTER ALL CONSUMER THREADS STOPPED!!! */
void MclTaskQueue_Delete(MclTaskQueue *self) {
	MCL_ASSERT_VALID_PTR_VOID(self);
	MCL_ASSERT_TRUE_VOID(self->stopped);

    MclTaskQueue_Destroy(self);
	MCL_FREE(self);
}

void MclTaskQueue_Start(MclTaskQueue *self) {
    MCL_LOCK_AUTO(self->mutex);
    self->stopped = false;
}

void MclTaskQueue_Stop(MclTaskQueue *self) {
	MCL_ASSERT_VALID_PTR_VOID(self);

    MCL_LOCK_AUTO(self->mutex);
    self->stopped = true;
    MclCond_Broadcast(&self->cond);
}

bool MclTaskQueue_IsEmpty(const MclTaskQueue *self) {
    MCL_ASSERT_VALID_PTR_R(self, true);

    MCL_LOCK_AUTO(self->mutex);
    return MclTaskQueue_IsEmptyImpl(self);
}

MclStatus MclTaskQueue_AddTask(MclTaskQueue *self, MclTask *task, MclTaskPriority priority) {
	MCL_ASSERT_VALID_PTR(self);
	MCL_ASSERT_VALID_PTR(task);
	MCL_ASSERT_TRUE(priority < self->queueCount);

	MCL_LOCK_AUTO(self->mutex);

	TaskQueue_Push(&self->queues[priority], task);
	MclCond_Signal(&self->cond);
	return MCL_SUCCESS;
}

MclStatus MclTaskQueue_DelTask(MclTaskQueue *self, MclTaskKey key, MclTaskPriority priority) {
	MCL_ASSERT_VALID_PTR(self);
	MCL_ASSERT_TRUE(MclTaskKey_IsValid(key));
	MCL_ASSERT_TRUE(priority < self->queueCount);

	MCL_LOCK_AUTO(self->mutex);

	TaskQueue_Remove(&self->queues[priority], key);
	return MCL_SUCCESS;
}

MclTask* MclTaskQueue_PopTask(MclTaskQueue *self) {
    MCL_ASSERT_VALID_PTR_NIL(self);

    MCL_LOCK_AUTO(self->mutex);

    MclTaskQueue_WaitReady(self);
    return MclTaskQueue_PopTaskImpl(self);
}
