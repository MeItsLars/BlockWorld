#pragma once
#include <atomic>
#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <string>
#include <thread>

enum class TaskPriority {
    LOW,
    NORMAL,
    HIGH
};

struct Task {
    // Priority and index are used to sort tasks in the queue. Same-priority tasks are executed based on their index
    TaskPriority priority;
    int index;
    // The function to execute
    std::function<void()> function;

    bool operator<(const Task &other) const {
        return priority == other.priority ? index > other.index : priority < other.priority;
    }
};

class TaskScheduler {
public:
    TaskScheduler() = default;

    ~TaskScheduler() = default;

    void initialize(const char *name, int numWorkers);

    void addTask(TaskPriority priority, const std::function<void()> &function);

    void workerThread();

    void stop();

private:
    std::string name;
    std::vector<std::thread> workers;
    int numWorkers = 0;
    std::priority_queue<Task> taskQueue;
    std::atomic_int taskIndex{0};
    std::mutex queueMutex;
    std::condition_variable taskAvailable;
    std::atomic_bool running{true};
};

namespace TaskSchedulers {
    inline TaskScheduler WORLD_GEN;
    inline TaskScheduler MESH_GEN;
}
