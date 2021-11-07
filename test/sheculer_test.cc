#include <cctest/cctest.h>
#include "mcl/task/scheduler.h"
#include "mcl/task/mutex.h"
#include "mcl/task/task.h"
#include "mcl/log/log.h"
#include <unistd.h>
#include <list>

namespace {
	enum PriorityLevel {
		URGENT, NORMAL, SLOW, MAX_PRIORITY
	};

	struct ExecutedTask {
		ExecutedTask(MclTaskKey key, PriorityLevel priority)
		: key(key), priority(priority) {
		}

		MclTaskKey key;
		PriorityLevel priority;
	};

	struct ExecutedTaskHistory {
		ExecutedTaskHistory() {
			MclMutex_InitRecursive(&mutex);
		}

		~ExecutedTaskHistory() {
			MclMutex_Destroy(&mutex);
		}

		void onTaskExecuted(const ExecutedTask& task) {
			MCL_LOCK_AUTO(mutex);
			executedTasks.push_back(task);
		}

		void clear() {
			MCL_LOCK_AUTO(mutex);
			executedTasks.clear();
		}

		size_t getSize() const {
			return executedTasks.size();
		}

		const std::list<ExecutedTask>& getHistory() const {
			return executedTasks;
		}

	private:
		std::list<ExecutedTask> executedTasks;
		MclMutex mutex;
	};

	struct DemoTask {
		MclTask task;
		uint32_t pauseTime;
		PriorityLevel priority;
		ExecutedTaskHistory *history;
	};

	MclStatus DemoTask_Execute(MclTask *task) {
		MCL_ASSERT_VALID_PTR(task);

		DemoTask *self = MCL_TYPE_OF(task, DemoTask, task);
		if (self->priority == SLOW) {
			sleep(self->pauseTime);
		}
		self->history->onTaskExecuted(ExecutedTask(self->task.key, self->priority));
		return MCL_SUCCESS;
	}

	void DemoTask_Init(DemoTask *self,
			           MclTaskKey key,
					   PriorityLevel priority,
					   uint32_t pauseTime,
					   ExecutedTaskHistory *history) {

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
}

FIXTURE(SchedulerTest)
{
	static constexpr uint32_t TASK_COUNT{10};

	DemoTask demoTasks[MAX_PRIORITY][TASK_COUNT];

	ExecutedTaskHistory history;

	uint32_t slowTaskPauseTime{1};

	SchedulerTest() {
		initTasks();
	}

	void initTasks() {
		for (int priority = 0; priority < MAX_PRIORITY; priority++) {
			for (uint32_t key = 0; key < TASK_COUNT; key++) {
				DemoTask_Init(&demoTasks[priority][key], key, (PriorityLevel)priority, slowTaskPauseTime, &history);
			}
		}
	}

	void scheduleTask(MclTaskScheduler *scheduler, PriorityLevel priority, int index = 0) {
		if (index >= TASK_COUNT) return;
		if (priority > MAX_PRIORITY) return;
		MclTaskScheduler_AddTask(scheduler, &demoTasks[priority][index].task, priority);
	}

	void waitScheduleDone(MclTaskScheduler *scheduler) {
		MclTaskScheduler_WaitDone(scheduler);
		sleep(slowTaskPauseTime);
	}

	BEFORE {
		history.clear();
	}

	TEST("should start and stop scheduler")
	{
		MclTaskScheduler *scheduler = MclTaskScheduler_Create(1, MAX_PRIORITY, NULL);
		ASSERT_TRUE(scheduler != NULL);

		MclTaskScheduler_Start(scheduler);
		ASSERT_TRUE(MclTaskScheduler_IsRunning(scheduler));

		MclTaskScheduler_Stop(scheduler);
		ASSERT_FALSE(MclTaskScheduler_IsRunning(scheduler));

		MclTaskScheduler_Delete(scheduler);
	}

	TEST("should execute task")
	{
		MclTaskScheduler *scheduler = MclTaskScheduler_Create(1, MAX_PRIORITY, NULL);

		MclTaskScheduler_Start(scheduler);

		scheduleTask(scheduler, NORMAL);

		waitScheduleDone(scheduler);

		ASSERT_EQ(1, history.getSize());

		MclTaskScheduler_Delete(scheduler);
	}

	TEST("should execute tasks in different priorities")
	{
		MclTaskScheduler *scheduler = MclTaskScheduler_Create(1, MAX_PRIORITY, NULL);

		MclTaskScheduler_Start(scheduler);

		scheduleTask(scheduler, SLOW);
		scheduleTask(scheduler, NORMAL);
		scheduleTask(scheduler, URGENT);

		waitScheduleDone(scheduler);

		MclTaskScheduler_Delete(scheduler);

		ASSERT_EQ(3, history.getSize());
	}

	TEST("should execute tasks limited by threshold")
	{
		uint32_t threshold[] = {3, 2};
		MclTaskScheduler *scheduler = MclTaskScheduler_Create(1, MAX_PRIORITY, threshold);

		MclTaskScheduler_Start(scheduler);

		for (int i = 0; i < 5; i++) {
			scheduleTask(scheduler, SLOW);
			scheduleTask(scheduler, URGENT);
			scheduleTask(scheduler, NORMAL);
		}

		waitScheduleDone(scheduler);
		MclTaskScheduler_Delete(scheduler);

		ASSERT_EQ(15, history.getSize());
	}

	TEST("should execute tasks by multiple threads")
	{
		uint32_t threshold[] = {5, 2};
		MclTaskScheduler *scheduler = MclTaskScheduler_Create(2, MAX_PRIORITY, threshold);

		MclTaskScheduler_Start(scheduler);

		for (int i = 0; i < 10; i++) {
			scheduleTask(scheduler, SLOW,   i);
			scheduleTask(scheduler, URGENT, i);
			scheduleTask(scheduler, NORMAL, i);
		}

		waitScheduleDone(scheduler);
		MclTaskScheduler_Delete(scheduler);

		ASSERT_EQ(30, history.getSize());
	}
};
