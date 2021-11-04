#include <cctest/cctest.h>
#include "mcl/array.h"
#include "mcl/lock.h"
#include "mcl/log.h"
#include <list>
#include <unistd.h>

struct Task {
    // uint32_t key;
    void *data;
    void* (*execute)(void*);
};

enum TaskLevel {
    URGENT, NORMAL, MAX_LEVEL
};

struct TaskPriorityQueue {
    std::list<Task> queues[MAX_LEVEL];
    bool isRunning{false};
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    size_t urgentThreshold{0};
    size_t normalThreshold{0};
};

void TaskPriorityQueue_Initial(TaskPriorityQueue* queue, size_t urgentThreshold, size_t normalThreshold) {
    queue->urgentThreshold = urgentThreshold;
    queue->normalThreshold = normalThreshold;

    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
    pthread_mutex_init(&queue->mutex, &attr);
    pthread_cond_init(&queue->cond, NULL);

    queue->isRunning = false;
    MCL_DBG("TaskPriorityQueue_Initial OK!");
}

void TaskPriorityQueue_Destroy(TaskPriorityQueue* queue) {
    pthread_mutex_destroy(&queue->mutex);
    pthread_cond_destroy(&queue->cond);

    MCL_DBG("TaskPriorityQueue_Destroy OK!");
}

void TaskPriorityQueue_Start(TaskPriorityQueue* queue) {
    MCL_SCOPE_LOCK(queue->mutex) {
        if (queue->isRunning) return;
        queue->isRunning = true;
        MCL_DBG("TaskPriorityQueue_Start OK!");
    }
    pthread_cond_broadcast(&queue->cond);
}

void TaskPriorityQueue_Stop(TaskPriorityQueue* queue) {
    MCL_SCOPE_LOCK(queue->mutex) {
        if (!queue->isRunning) return;
        queue->isRunning = false;
        MCL_DBG("TaskPriorityQueue_Stop OK!");
    }
    pthread_cond_broadcast(&queue->cond);
}

void TaskPriorityQueue_PushTask(TaskPriorityQueue* queue, TaskLevel level, const Task* task) {
    if (level >= MAX_LEVEL) return;

    MCL_SCOPE_LOCK(queue->mutex) {
        queue->queues[level].push_back(*task);
        MCL_DBG("TaskPriorityQueue_PushTask Level %d OK!", level);
    }
    pthread_cond_broadcast(&queue->cond);
}

Task TaskPriorityQueue_PopTask(TaskPriorityQueue* queue) {
    MCL_AUTO_LOCK(queue->mutex);

    for (int i = 0; i < MAX_LEVEL; i++) {
        if (!queue->queues[i].empty()) {
            Task task = queue->queues[i].front();
            queue->queues[i].pop_front();
            MCL_DBG("TaskPriorityQueue_PopTask OK!");
            return task;
        }
    }
    MCL_DBG("TaskPriorityQueue_PopTask NONE!");
    Task task{.execute = NULL};
    return task;
}

void TaskPriorityQueue_DelTask(TaskPriorityQueue* queue, TaskLevel level) {
    MCL_DBG("TaskPriorityQueue_DelTask OK!");
}

bool TaskPriorityQueue_IsEmpty(const TaskPriorityQueue* queue) {
    for (int i = 0; i < MAX_LEVEL; i++) {
        if (!queue->queues[i].empty()) return false;
    }
    return true;
}

bool TaskPriorityQueue_IsStopped(const TaskPriorityQueue* queue) {
    return !queue->isRunning;
}

bool TaskPriorityQueue_NeedWaiting(const TaskPriorityQueue* queue) {
    return !TaskPriorityQueue_IsStopped(queue) && TaskPriorityQueue_IsEmpty(queue);
}

void* TaskPriorityQueue_Execute(void* queue) {
    TaskPriorityQueue *taskQueue = (TaskPriorityQueue *)queue;

    while(true) {
        Task task;
        MCL_SCOPE_LOCK(taskQueue->mutex) {
            MCL_DBG("TaskPriorityQueue_Execute get mutex!");
            while (TaskPriorityQueue_NeedWaiting(taskQueue)) {

                MCL_DBG("TaskPriorityQueue_Execute begin waiting...!");
                pthread_cond_wait(&taskQueue->cond, &taskQueue->mutex);
                MCL_DBG("TaskPriorityQueue_Execute end wait!");
            }

            if (TaskPriorityQueue_IsStopped(taskQueue)) {
                MCL_DBG("TaskPriorityQueue_Execute exit!!!");
                return NULL;
            }
            task = TaskPriorityQueue_PopTask(taskQueue);
            MCL_DBG("TaskPriorityQueue_Execute get task!!!");
        }

        if (task.execute) {
            task.execute(task.data);
        }

    }
    return NULL;
}

struct TaskPool {
    TaskPool(size_t threadCount, size_t urgentThreshold, size_t normalThreshold) {
        TaskPriorityQueue_Initial(&taskQueue, urgentThreshold, normalThreshold);

        this->threadCount = threadCount;
        threads = new pthread_t[threadCount];
    }

    ~TaskPool() {
        stop(true);
        TaskPriorityQueue_Destroy(&taskQueue);
        delete[] threads;
    }

    void start() {
        TaskPriorityQueue_Start(&taskQueue);

        for (int i = 0; i < threadCount; i++) {
            pthread_create(&threads[i], NULL, TaskPriorityQueue_Execute, &taskQueue);
        }
    }

    void stop(bool isJoin) {
        TaskPriorityQueue_Stop(&taskQueue);

        if(!isJoin) return;
        for (int i = 0; i < threadCount; i++) {
            pthread_join(threads[i], NULL);
        }
    }

    void addTask(TaskLevel level, const Task& task) {
        TaskPriorityQueue_PushTask(&taskQueue, level, &task);
    }

private:
    pthread_t *threads{nullptr};
    size_t threadCount{0};

    TaskPriorityQueue taskQueue;
};

void* urgentTask(void* data) {
    long value = (long)data;
    printf("urgentTask %ld\n", value);
    sleep(1);
    return NULL;
}

void* normalTask(void* data) {
    long value = (long)data;
    printf("normalTask %ld\n", value);
    sleep(1);
    return NULL;
}

FIXTURE(TaskPoolTest)
{
	TEST("test task pool")
	{
        size_t threadCount = 4;
        size_t urgentCount = 1;
        size_t normalCount = 1;

        TaskPool tasks{threadCount, urgentCount, normalCount};

        for (int i = 0; i < 10; i++) {
            Task task;
            task.data = (void*)i;
            task.execute = normalTask;
            tasks.addTask(NORMAL, task);
            // sleep(1);
        }

        tasks.start();

        for (int i = 0; i < 5; i++) {
            Task task;
            task.data = (void*)i;
            task.execute = urgentTask;
            tasks.addTask(URGENT, task);
            // sleep(1);
        }

        sleep(5);

        for (int i = 0; i < 5; i++) {
            Task task;
            task.data = (void*)i;
            task.execute = urgentTask;
            tasks.addTask(URGENT, task);
            // sleep(1);
        }  

        sleep(5);      
	}
};
