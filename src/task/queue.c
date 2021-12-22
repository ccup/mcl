#include "mcl/task/queue.h"
#include "mcl/task/atom.h"
#include "mcl/task/cond.h"
#include "mcl/task/task.h"
#include "mcl/link/link.h"
#include "mcl/link/link_node_allocator.h"
#include "mcl/mem/malloc.h"
#include "mcl/assert.h"

MCL_TYPE(TaskQueue) {
	MclLink  tasks;
	uint32_t threshold;
	uint32_t poppedCount;
};

MCL_PRIVATE void TaskQueue_Init(TaskQueue *queue, uint32_t threshold) {
	MclLink_Init(&queue->tasks, MclLinkNodeAllocator_GetDefault());
	queue->threshold = threshold;
	queue->poppedCount = 0;
}

MCL_PRIVATE void TaskQueue_DestroyTask(MclLinkDataDeleter *deleter, MclLinkData data) {
    MclTask *task = (MclTask*)data;
    MclTask_Destroy(task);
}

MCL_PRIVATE MclLinkDataDeleter taskDeleter = MCL_LINK_DATA_DELETER(TaskQueue_DestroyTask);

MCL_PRIVATE void TaskQueue_Destroy(TaskQueue *queue) {
	MclLink_Clear(&queue->tasks, &taskDeleter);
}

MCL_PRIVATE bool TaskQueue_IsEmpty(const TaskQueue *queue) {
	return MclLink_IsEmpty(&queue->tasks);
}

MCL_PRIVATE bool TaskQueue_IsReachThreshold(const TaskQueue *queue) {
	if (queue->threshold == 0 || queue->poppedCount == 0) return false;
	return queue->poppedCount % queue->threshold == 0;
}

MCL_PRIVATE void TaskQueue_ResetPoppedCount(TaskQueue *queue) {
	queue->poppedCount = 0;
}

MCL_PRIVATE void TaskQueue_Remove(TaskQueue *queue, MclTaskKey key) {
	MclLinkNode *taskNode = NULL;
	MclLinkNode *tmpNode = NULL;
	MCL_LINK_FOR_EACH_SAFE(&queue->tasks, taskNode, tmpNode) {
		MclTask *task = (MclTask*)MclLinkNode_GetData(taskNode);
		if (task && task->key != key) continue;
		return MclLink_RemoveNode(&queue->tasks, taskNode, &taskDeleter);
	}
}

MCL_PRIVATE void TaskQueue_Push(TaskQueue *queue, MclTask *task) {
	MclLink_PushBack(&queue->tasks, task);
}

MCL_PRIVATE MclTask* TaskQueue_Pop(TaskQueue *queue) {
	if (MclLink_IsEmpty(&queue->tasks)) return NULL;

	MclLinkNode *node = MclLink_GetFirst(&queue->tasks);
	MCL_ASSERT_VALID_PTR_NIL(node);

	MclTask *task = (MclTask*)MclLinkNode_GetData(node);
	MclLink_RemoveNode(&queue->tasks, node, NULL);
	queue->poppedCount++;

	return task;
}

MCL_TYPE(MclTaskQueue) {
	MclMutex mutex;
	MclCond  cond;
	MclAtom  isReady;
	uint32_t queueCount;
	TaskQueue queues[];
};

bool MclTaskQueue_IsReady(const MclTaskQueue *self) {
	MCL_ASSERT_VALID_PTR_BOOL(self);
	return MclAtom_IsTrue(&self->isReady);
}

bool MclTaskQueue_IsEmpty(const MclTaskQueue *self) {
	MCL_ASSERT_VALID_PTR_R(self, true);

	MCL_LOCK_AUTO(((MclTaskQueue*)self)->mutex);

	if (!MclTaskQueue_IsReady(self)) return true;

	for (uint32_t i = 0; i < self->queueCount; i++) {
		if (!TaskQueue_IsEmpty(&self->queues[i])) return false;
	}
	return true;
}

MCL_PRIVATE bool MclTaskQueue_NeedWaiting(const MclTaskQueue *self) {
	return MclAtom_IsTrue(&self->isReady) && MclTaskQueue_IsEmpty(self);
}

MCL_PRIVATE MclTask* MclTaskQueue_PopTask(MclTaskQueue *self) {
	for (uint32_t i = 0; i < self->queueCount; i++) {
		if (TaskQueue_IsEmpty(&self->queues[i])) continue;
		if (TaskQueue_IsReachThreshold(&self->queues[i])) {
			TaskQueue_ResetPoppedCount(&self->queues[i]);
			continue;
		}

		MclTask *task = TaskQueue_Pop(&self->queues[i]);
		if (!task) break;

		MCL_LOG_DBG("Pop task %u of level %u", task->key, i);
		return task;
	}
	MCL_LOG_DBG("Pop none task");
	return NULL;
}

MCL_PRIVATE void MclTaskQueue_InitQueues(MclTaskQueue *self, uint32_t priorities, uint32_t *thresholds) {
	self->queueCount = priorities;
	for (uint32_t i = 0; i < priorities; i++) {
		uint32_t threshold = ((thresholds == NULL) || (i + 1 >= priorities)) ? 0 : thresholds[i];
		TaskQueue_Init(&self->queues[i], threshold);
	}
}

MCL_PRIVATE void MclTaskQueue_DestroyQueues(MclTaskQueue *self) {
	for (uint32_t i = 0; i < self->queueCount; i++) {
		TaskQueue_Destroy(&self->queues[i]);
	}
	self->queueCount = 0;
}

MCL_PRIVATE MclStatus MclTaskQueue_Init(MclTaskQueue *self, uint32_t priorities, uint32_t *thresholds) {
	MclTaskQueue_InitQueues(self, priorities, thresholds);
	if (MCL_FAILED(MclMutex_InitRecursive(&self->mutex))) {
		MCL_LOG_ERR("MclCond_Init failed!");
		MclTaskQueue_DestroyQueues(self);
		return MCL_FAILURE;
	}
	if (MCL_FAILED(MclCond_Init(&self->cond, NULL))) {
		MCL_LOG_ERR("MclCond_Init failed!");
		MclMutex_Destroy(&self->mutex);
		MclTaskQueue_DestroyQueues(self);
		return MCL_FAILURE;
	}

	MclAtom_Set(&self->isReady, 0);
	return MCL_SUCCESS;
}

MclTaskQueue* MclTaskQueue_Create(uint32_t priorities, uint32_t *thresholds) {
	MCL_ASSERT_TRUE_NIL(priorities > 0);

	MclTaskQueue *self = MCL_MALLOC(sizeof(MclTaskQueue) + sizeof(TaskQueue) * priorities);
	MCL_ASSERT_VALID_PTR_NIL(self);

	if (MCL_FAILED(MclTaskQueue_Init(self, priorities, thresholds))) {
		MCL_LOG_ERR("Create task queue failed!");
		MCL_FREE(self);
		return NULL;
	}

	MCL_LOG_DBG("Create task queue OK!");
	return self;
}

/* IMPORTANT: SHOULD INVOKE AFTER ALL OTHER USER THREADS STOPPED!!! */
void MclTaskQueue_Delete(MclTaskQueue *self) {
	MCL_ASSERT_VALID_PTR_VOID(self);

	if (MclAtom_IsTrue(&self->isReady)) {
		MclTaskQueue_Stop(self);
	}

	MCL_LOCK_SCOPE(self->mutex) {
		MclTaskQueue_DestroyQueues(self);
	}

	MCL_ASSERT_SUCC_CALL_VOID(MclMutex_Destroy(&self->mutex));
	MCL_ASSERT_SUCC_CALL_VOID(MclCond_Destroy(&self->cond));
	MCL_FREE(self);

	MCL_LOG_DBG("Delete task queue OK!");
}

MclStatus MclTaskQueue_Start(MclTaskQueue *self) {
	MCL_ASSERT_VALID_PTR(self);

	MCL_LOCK_AUTO(self->mutex);

	if(MclTaskQueue_IsReady(self)) return MCL_SUCCESS;

	MclAtom_Set(&self->isReady, 1);
	MclCond_Broadcast(&self->cond);

	MCL_LOG_DBG("Task queue start OK!");
	return MCL_SUCCESS;
}

MclStatus MclTaskQueue_Stop(MclTaskQueue *self) {
	MCL_ASSERT_VALID_PTR(self);

	MCL_LOCK_AUTO(self->mutex);

	if(!MclTaskQueue_IsReady(self)) return MCL_SUCCESS;

	MclAtom_Set(&self->isReady, 0);
	MclCond_Broadcast(&self->cond);

	MCL_LOG_DBG("Task queue stop OK!");
	return MCL_SUCCESS;
}

MclStatus MclTaskQueue_SubmitTask(MclTaskQueue *self, MclTask *task, uint32_t priority) {
	MCL_ASSERT_VALID_PTR(self);
	MCL_ASSERT_VALID_PTR(task);
	MCL_ASSERT_TRUE(priority < self->queueCount);

	MCL_LOCK_AUTO(self->mutex);

	TaskQueue_Push(&self->queues[priority], task);
	MclCond_Signal(&self->cond);

	MCL_LOG_DBG("Task queue insert task %u of level %u OK!", task->key, priority);
	return MCL_SUCCESS;
}

MclStatus MclTaskQueue_RemoveTask(MclTaskQueue *self, MclTaskKey key, uint32_t priority) {
	MCL_ASSERT_VALID_PTR(self);
	MCL_ASSERT_TRUE(priority < self->queueCount);

	MCL_LOCK_AUTO(self->mutex);

	TaskQueue_Remove(&self->queues[priority], key);

	MCL_LOG_DBG("Task queue remove task %u of level %u OK!", key, priority);
	return MCL_SUCCESS;
}

void MclTaskQueue_ExecuteAll(MclTaskQueue *self) {
	MCL_ASSERT_VALID_PTR_VOID(self);

	MCL_LOCK_AUTO(self->mutex);

	while (!MclTaskQueue_IsEmpty(self)) {
		MclTask *task = MclTaskQueue_PopTask(self);
		if (task) {
			if (MCL_FAILED(MclTask_Execute(task))) {
				MCL_LOG_ERR("Execute task %u failed!", task->key);
			}
		}
	}
	MCL_LOG_DBG("Task queue executes finish!");
}

void* MclTaskQueue_ThreadExecute(void *data) {

	MclTaskQueue *self = (MclTaskQueue*)data;

	while (MclTaskQueue_IsReady(self)) {
		MclTask *task = NULL;
		MCL_LOCK_SCOPE(self->mutex) {
			MCL_LOG_DBG("Task queue get mutex.");
			while (MclTaskQueue_NeedWaiting(self)) {
				MCL_LOG_DBG("Task queue begin waiting...");
				MclCond_Wait(&self->cond, &self->mutex);
				MCL_LOG_DBG("Task queue end waiting.");
			}
			if (!MclAtom_IsTrue(&self->isReady)) {
				MCL_LOG_DBG("Task queue async execute is stopped!");
				return NULL;
			}
			task = MclTaskQueue_PopTask(self);
		}
		/* IMPORTANT: SHOULD EXECUTE TASK IN OUTSIDE OF LOCK SCOPE!!! */
		if (task) {
			(void)MclTask_Execute(task);
		}
	}
	MCL_LOG_DBG("Task queue async execute exit!");
	return NULL;
}

