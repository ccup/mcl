#ifndef H34BD157B_99A2_4FEB_B9FE_9C85A9BB6703
#define H34BD157B_99A2_4FEB_B9FE_9C85A9BB6703

#include "mcl/task/mutex.h"
#include "task/task_utils/executed_task.h"
#include <initializer_list>
#include <iostream>
#include <list>

struct TaskHistory {
	TaskHistory() {
		MclMutex_InitRecursive(&mutex);
	}

	~TaskHistory() {
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

	bool isInOrderOf(std::initializer_list<ExecutedTask> expects) {
		if (expects.size() != executedTasks.size()) return false;

		auto expect = expects.begin();
		for (const auto task : executedTasks) {
			if (task != *expect) {
				dump();
				return false;
			}
			expect++;
		}
		return true;
	}

private:
	std::list<ExecutedTask> executedTasks;
	MclMutex mutex;
};

#endif
