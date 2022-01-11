#include <cctest/cctest.h>
#include "mcl/task/task_scheduler.h"
#include "task/task_utils/demo_task.h"

FIXTURE(TaskSchedulerTest)
{
	static constexpr uint32_t TASK_COUNT{10};

	DemoTask demoTasks[MAX_PRIORITY][TASK_COUNT];

	TaskHistory history;

	uint32_t slowTaskPauseTime{1};

	TaskSchedulerTest() {
		initTasks();
	}

	void initTasks() {
		for (int priority = 0; priority < MAX_PRIORITY; priority++) {
			for (uint32_t key = 0; key < TASK_COUNT; key++) {
				DemoTask_Init(&demoTasks[priority][key], key, (PriorityLevel)priority, slowTaskPauseTime, &history);
			}
		}
	}

	void submitTask(MclTaskScheduler *scheduler, PriorityLevel priority, int index = 0) {
		if (index >= TASK_COUNT) return;
		if (priority > MAX_PRIORITY) return;
        MclTaskScheduler_SubmitTask(scheduler, &demoTasks[priority][index].task, priority);
	}

	void waitScheduleDone(MclTaskScheduler *scheduler) {
        MclTaskScheduler_WaitDone(scheduler);
		sleep(slowTaskPauseTime + 1);
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

		submitTask(scheduler, NORMAL);

		waitScheduleDone(scheduler);

		ASSERT_EQ(1, history.getSize());

		MclTaskScheduler_Delete(scheduler);
	}

	TEST("should execute tasks by priority")
	{
		MclTaskScheduler *scheduler = MclTaskScheduler_Create(1, MAX_PRIORITY, NULL);

		submitTask(scheduler, SLOW);
		submitTask(scheduler, NORMAL);
		submitTask(scheduler, URGENT);

		MclTaskScheduler_Start(scheduler);

		waitScheduleDone(scheduler);

		ASSERT_EQ(3, history.getSize());
		ASSERT_TRUE(history.isInOrderOf({__ET(URGENT), __ET(NORMAL), __ET(SLOW)}));

		MclTaskScheduler_Delete(scheduler);
	}

	TEST("should execute tasks by multiple threads")
	{
		MclTaskScheduler *scheduler = MclTaskScheduler_Create(2, MAX_PRIORITY, NULL);

		for (int i = 0; i < 10; i++) {
			submitTask(scheduler, SLOW,   i);
			submitTask(scheduler, URGENT, i);
			submitTask(scheduler, NORMAL, i);
		}

		MclTaskScheduler_Start(scheduler);

		waitScheduleDone(scheduler);
		MclTaskScheduler_Delete(scheduler);

		ASSERT_EQ(30, history.getSize());
	}

	TEST("should start again after stop") {

		MclTaskScheduler *scheduler = MclTaskScheduler_Create(1, MAX_PRIORITY, NULL);

		MclTaskScheduler_Start(scheduler);

		for (int i = 0; i < 10; i++) {
			submitTask(scheduler, SLOW,   i);
			submitTask(scheduler, URGENT, i);
		}

		sleep(1);
		MclTaskScheduler_Stop(scheduler);

		ASSERT_TRUE(history.getSize() < 20);

		MclTaskScheduler_Start(scheduler);

		waitScheduleDone(scheduler);
		MclTaskScheduler_Delete(scheduler);

		ASSERT_EQ(20, history.getSize());
	}

	TEST("should execute by specified task thresholds") {
		uint32_t thresholds[] = {3, 2};

		MclTaskScheduler *scheduler = MclTaskScheduler_Create(1, MAX_PRIORITY, thresholds);

		for (int key = 0; key < 5; key++) {
			submitTask(scheduler, SLOW,   key);
			submitTask(scheduler, URGENT, key);
			submitTask(scheduler, NORMAL, key);
		}

		MclTaskScheduler_Start(scheduler);

		waitScheduleDone(scheduler);
		MclTaskScheduler_Delete(scheduler);

		ASSERT_TRUE(history.isInOrderOf({__ET(URGENT, 0), __ET(URGENT, 1), __ET(URGENT, 2),
			                             __ET(NORMAL, 0),
			                             __ET(URGENT, 3), __ET(URGENT, 4),
										 __ET(NORMAL, 1),
			                             __ET(SLOW, 0),
										 __ET(NORMAL, 2), __ET(NORMAL, 3),
										 __ET(SLOW, 1),
										 __ET(NORMAL, 4),
										 __ET(SLOW, 2), __ET(SLOW, 3), __ET(SLOW, 4) }));
	}

	TEST("should sync execute when 0 thread configured") {
		MclTaskScheduler *scheduler = MclTaskScheduler_Create(0, MAX_PRIORITY, NULL);

		MclTaskScheduler_Start(scheduler);

		submitTask(scheduler, SLOW);
		submitTask(scheduler, URGENT);
		submitTask(scheduler, NORMAL);

		sleep(1);

		ASSERT_EQ(0, history.getSize());

        MclTaskScheduler_LocalExecute(scheduler);
		MclTaskScheduler_Delete(scheduler);

		ASSERT_TRUE(history.isInOrderOf({__ET(URGENT), __ET(NORMAL), __ET(SLOW)}));
	}
};
