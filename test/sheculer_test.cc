#include <cctest/cctest.h>
#include "mcl/task/scheduler.h"
#include "mcl/task/mutex.h"
#include "mcl/task/task.h"
#include "mcl/log/log.h"
#include <unistd.h>
#include <list>
#include <string>
#include <iostream>
#include <initializer_list>

namespace {
	enum PriorityLevel {
		URGENT, NORMAL, SLOW, MAX_PRIORITY
	};

	std::string PriorityLevel_GetStr(PriorityLevel priority) {
		switch (priority) {
		case URGENT : return "URGENT";
		case NORMAL : return "NORMAL";
		case SLOW : return "SLOW";
		default : break;
		}
		return "UNKNOWN";
	}

	struct ExecutedTask {
		ExecutedTask(MclTaskKey key, PriorityLevel priority)
		: key(key), priority(priority) {
		}

		bool operator== (const ExecutedTask& other) const {
			return (key == other.key) && (priority == other.priority);
		}

		bool operator!=(const ExecutedTask& other) const {
			return not(*this == other);
		}

		std::string toString() const {
			return std::string("Task {key : ") + std::to_string(key)
					+ ", priority : " + PriorityLevel_GetStr(priority) + "}";
		}

		MclTaskKey key;
		PriorityLevel priority;
	};

	#define __ET(K, P) ExecutedTask(K, P)

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

		void dump() const {
			for (const auto t : executedTasks) {
				std::cout << t.toString() << std::endl;
			}
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

	bool isScheduledByOrder(std::initializer_list<ExecutedTask> expects) {
		if (expects.size() != history.getSize()) return false;

		auto expect = expects.begin();
		for (const auto task : history.getHistory()) {
			if (task != *expect) {
				history.dump();
				return false;
			}
			expect++;
		}
		return true;
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

	TEST("should execute tasks by multiple threads")
	{
		MclTaskScheduler *scheduler = MclTaskScheduler_Create(2, MAX_PRIORITY, NULL);

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

	TEST("should execute by specified task thresholds") {
		uint32_t thresholds[] = {3, 2};

		MclTaskScheduler *scheduler = MclTaskScheduler_Create(1, MAX_PRIORITY, thresholds);

		for (int key = 0; key < 5; key++) {
			scheduleTask(scheduler, SLOW,   key);
			scheduleTask(scheduler, URGENT, key);
			scheduleTask(scheduler, NORMAL, key);
		}

		MclTaskScheduler_Start(scheduler);
		waitScheduleDone(scheduler);
		MclTaskScheduler_Delete(scheduler);

		ASSERT_TRUE(isScheduledByOrder({__ET(0, URGENT), __ET(1, URGENT), __ET(2, URGENT),
			                            __ET(0, NORMAL),
			                            __ET(3, URGENT), __ET(4, URGENT),
										__ET(1, NORMAL),
			                            __ET(0, SLOW),
										__ET(2, NORMAL), __ET(3, NORMAL),
										__ET(1, SLOW),
										__ET(4, NORMAL),
										__ET(2, SLOW), __ET(3, SLOW), __ET(4, SLOW) }));
	}
};
