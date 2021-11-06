#include "mcl/task/queue.h"
#include "mcl/link/link.h"
#include "mcl/task/atom.h"
#include "mcl/task/cond.h"
#include "mcl/task/task.h"
#include "mcl/assert.h"
#include "mcl/memory.h"

MCL_TYPE_DEF(TaskQueue) {
	MclLink tasks;
	uint32_t threshold;
};

MCL_PRIVATE void TaskQueue_Init(TaskQueue *queue, uint32_t threshold) {
	MclLink_Init(&queue->tasks);
	queue->threshold = threshold;
}

MCL_PRIVATE void TaskQueue_Destroy(TaskQueue *queue) {
	MclLink_Clear(&queue->tasks, (MclLinkDataDeleter)MclTask_Destroy);
	queue->threshold = 0;
}

MCL_PRIVATE bool TaskQueue_IsEmpty(const TaskQueue *queue) {
	return MclLink_IsEmpty(&queue->tasks);
}

MCL_PRIVATE void TaskQueue_Remove(TaskQueue *queue, MclTaskKey key) {
	MclLinkNode *taskNode = NULL;
	MclLinkNode *tmpNode = NULL;
	MCL_LINK_FOR_EACH_SAFE(&queue->tasks, taskNode, tmpNode) {
		MclTask *task = (MclTask*)MclLinkNode_GetData(taskNode);
		if (task && task->key == key) {
			MclLink_RemoveNode(&queue->tasks, taskNode, (MclLinkDataDeleter)MclTask_Destroy);
		}
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

	return task;
}

MCL_TYPE_DEF(MclTaskQueue) {
	MclAtom isReady;
	MclMutex mutex;
	MclCond cond;
	uint32_t queueCount;
	TaskQueue queues[];
};

MCL_PRIVATE bool MclTaskQueue_IsEmpty(const MclTaskQueue *self) {
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

		MclTask *task = TaskQueue_Pop(&self->queues[i]);
		if (!task) break;

		MCL_LOG_DBG("Pop task %u of level %u", task->key, i);
		return task;
	}
	MCL_LOG_DBG("Pop none task");
	return NULL;
}

MCL_PRIVATE void MclTaskQueue_InitQueues(MclTaskQueue *self, uint32_t levels, uint32_t *levelThresholds) {
	self->queueCount = levels;
	for (uint32_t i = 0; i < levels; i++) {
		TaskQueue_Init(&self->queues[i], levelThresholds ? levelThresholds[i] : 0);
	}
}

MCL_PRIVATE void MclTaskQueue_DestroyQueues(MclTaskQueue *self) {
	for (uint32_t i = 0; i < self->queueCount; i++) {
		TaskQueue_Destroy(&self->queues[i]);
	}
	self->queueCount = 0;
}

MCL_PRIVATE MclStatus MclTaskQueue_Init(MclTaskQueue *self, uint32_t levels, uint32_t *levelThresholds) {
	MclTaskQueue_InitQueues(self, levels, levelThresholds);
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

	if (MclAtom_IsTrue(&self->isReady)) return MCL_SUCCESS;

	MCL_LOCK_SCOPE(self->mutex) {
		MclAtom_Set(&self->isReady, 1);
		MclCond_Broadcast(&self->cond);
		MCL_LOG_DBG("Task queue start OK!");
	}
	return MCL_SUCCESS;
}

MclStatus MclTaskQueue_Stop(MclTaskQueue *self) {
	MCL_ASSERT_VALID_PTR(self);

	MCL_LOCK_SCOPE(self->mutex) {
		MclAtom_Set(&self->isReady, 0);
		MclCond_Broadcast(&self->cond);
		MCL_LOG_DBG("Task queue stop OK!");
	}
	return MCL_SUCCESS;
}

MclStatus MclTaskQueue_InsertTask(MclTaskQueue *self, MclTask *task, uint32_t priority) {
	MCL_ASSERT_VALID_PTR(self);
	MCL_ASSERT_VALID_PTR(task);
	MCL_ASSERT_TRUE(priority < self->queueCount);

	MCL_LOCK_SCOPE(self->mutex) {
		TaskQueue_Push(&self->queues[priority], task);
		MclCond_Signal(&self->cond);
		MCL_LOG_DBG("Task queue insert task %u of level %u OK!", task->key, priority);
	}
	return MCL_SUCCESS;
}

MclStatus MclTaskQueue_RemoveTask(MclTaskQueue *self, MclTaskKey key, uint32_t priority) {
	MCL_ASSERT_VALID_PTR(self);
	MCL_ASSERT_TRUE(priority < self->queueCount);

	MCL_LOCK_SCOPE(self->mutex) {
		TaskQueue_Remove(&self->queues[priority], key);
		MCL_LOG_DBG("Task queue remove task %u of level %u OK!", key, priority);
	}
	return MCL_SUCCESS;
}

bool MclTaskQueue_IsReady(const MclTaskQueue *self) {
	MCL_ASSERT_VALID_PTR_NIL(self);
	return MclAtom_IsTrue(&self->isReady);
}

void* MclTaskQueue_Execute(void *data) {
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
			if (!MclTaskQueue_IsReady(self)) {
				MCL_LOG_DBG("Task queue execute is stopped!");
				return NULL;
			}
			task = MclTaskQueue_PopTask(self);
		}
		if (task) {
			MclTask_Execute(task);
		}
	}
	MCL_LOG_DBG("Task queue execute exit!");
	return NULL;
}
