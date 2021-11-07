#ifndef H44CC7FED_9A51_46BF_8D15_E77E7706EB51
#define H44CC7FED_9A51_46BF_8D15_E77E7706EB51

#include "mcl/task/key.h"
#include "task/priority_level.h"
#include <string>

struct ExecutedTask {
	ExecutedTask(PriorityLevel priority, MclTaskKey key = 0)
	: priority(priority), key(key) {
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

	PriorityLevel priority;
	MclTaskKey key;
};

#define __ET(P, ...) ExecutedTask(P, ##__VA_ARGS__)

#endif
