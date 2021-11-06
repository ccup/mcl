#include <cctest/cctest.h>
#include "mcl/task/scheduler.h"
#include "mcl/task/task.h"
#include "mcl/log/log.h"
#include <unistd.h>

namespace {
	enum PriorityLevel {
		URGENT, NORMAL, MAX_PRIORITY
	};

	int g_value = 0;

	MclStatus increase(MclTask *) {
		g_value++;
		return MCL_SUCCESS;
	}
}

FIXTURE(SchedulerTest)
{
	static constexpr uint32_t TASK_COUNT{10};
	MclTask tasks[TASK_COUNT];

	SchedulerTest() {
		for (int i = 0; i < TASK_COUNT; i++) {
			tasks[i].key = i;
			tasks[i].execute = increase;
			tasks[i].destroy = NULL;
		}
	}

	BEFORE {
		g_value = 0;
	}

	TEST("should schedule tasks")
	{
		uint32_t threadCount = 2;
		MclTaskScheduler *scheduler = MclTaskScheduler_Create(threadCount, MAX_PRIORITY, NULL);
		ASSERT_TRUE(scheduler != NULL);

		MclTaskScheduler_Start(scheduler);
		ASSERT_TRUE(MclTaskScheduler_IsRunning(scheduler));

		for (int i = 0; i < TASK_COUNT; i++) {
			MclTaskScheduler_AddTask(scheduler, &tasks[i], URGENT);
			sleep(1);
		}

		for (int i = 0; i < TASK_COUNT; i++) {
			MclTaskScheduler_AddTask(scheduler, &tasks[i], NORMAL);
			sleep(1);
		}

		sleep(1);

		ASSERT_EQ(20, g_value);

		MclTaskScheduler_Delete(scheduler);
	}
};
