#include <cctest/cctest.h>
#include "mcl/task/scheduler.h"
#include "mcl/task/task.h"
#include "mcl/log/log.h"
#include <unistd.h>

namespace {
	enum PriorityLevel {
		URGENT, NORMAL, SLOW, MAX_PRIORITY
	};

	int g_value = 0;

	MclStatus increase(MclTask *) {
		g_value++;
		return MCL_SUCCESS;
	}

	MclStatus decrease(MclTask *) {
		g_value--;
		return MCL_SUCCESS;
	}

	MclStatus slow_increase(MclTask *) {
		g_value++;
		sleep(1);
		return MCL_SUCCESS;
	}
}

FIXTURE(SchedulerTest)
{
	static constexpr uint32_t TASK_COUNT{10};
	MclTask tasks[MAX_PRIORITY][TASK_COUNT];

	SchedulerTest() {
		InitTasks(tasks[URGENT], increase, TASK_COUNT);
		InitTasks(tasks[NORMAL], decrease, TASK_COUNT);
		InitTasks(tasks[SLOW], slow_increase, TASK_COUNT);
	}

	void InitTasks(MclTask* tasks, MclStatus (*execute)(MclTask*), uint32_t count) {
		for (int i = 0; i < count; i++) {
			tasks[i].key = i;
			tasks[i].execute = execute;
			tasks[i].destroy = NULL;
		}
	}

	void scheduleTask(MclTaskScheduler *scheduler, PriorityLevel priority, int index = 0) {
		if (index >= TASK_COUNT) return;
		if (priority > MAX_PRIORITY) return;
		MclTaskScheduler_AddTask(scheduler, &tasks[priority][index], priority);
	}

	BEFORE {
		g_value = 0;
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

		MclTaskScheduler_WaitDone(scheduler);

		ASSERT_EQ(-1, g_value);

		MclTaskScheduler_Delete(scheduler);
	}

	TEST("should execute tasks in different priorities")
	{
		MclTaskScheduler *scheduler = MclTaskScheduler_Create(1, MAX_PRIORITY, NULL);

		MclTaskScheduler_Start(scheduler);

		scheduleTask(scheduler, SLOW);
		scheduleTask(scheduler, NORMAL);
		scheduleTask(scheduler, URGENT);

		MclTaskScheduler_WaitDone(scheduler);

		ASSERT_EQ(1, g_value);

		MclTaskScheduler_Delete(scheduler);
	}

	TEST("should execute tasks limited by threshold")
	{
		uint32_t threshold[] = {3, 2, 1};
		MclTaskScheduler *scheduler = MclTaskScheduler_Create(1, MAX_PRIORITY, threshold);

		MclTaskScheduler_Start(scheduler);

		for (int i = 0; i < 5; i++) {
			scheduleTask(scheduler, SLOW);
			scheduleTask(scheduler, URGENT);
			scheduleTask(scheduler, NORMAL);
		}

		MclTaskScheduler_WaitDone(scheduler);

		ASSERT_EQ(5, g_value);

		MclTaskScheduler_Delete(scheduler);
	}
};
