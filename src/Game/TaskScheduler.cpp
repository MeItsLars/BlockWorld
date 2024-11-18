#include "TaskScheduler.h"

#include "spdlog/spdlog.h"

void TaskScheduler::initialize(const char *name, const int numWorkers) {
    this->name = name;
    this->numWorkers = numWorkers;
    for (int i = 0; i < numWorkers; i++) {
        workers.push_back(std::thread(workerThread, this));
    }
}

void TaskScheduler::addTask(TaskPriority priority, const std::function<void()>& function) {
    // Lock the queue
    {
        std::lock_guard lock(queueMutex);
        taskQueue.push({priority, taskIndex.fetch_add(1), function});
    }
    // Notify a worker that a task is available
    taskAvailable.notify_one();
}

void TaskScheduler::workerThread() {
    spdlog::info("[TS-{}] Worker thread started. Waiting for tasks...", name);
    while (running.load()) {
        std::function<void()> task;

        {
            std::unique_lock lock(queueMutex);
            taskAvailable.wait(lock, [this] { return !taskQueue.empty() || !running.load(); });

            // If we were notified due to the scheduler stopping, return
            if (!running.load()) {
                return;
            }

            // Otherwise, get the next task
            task = taskQueue.top().function;
            taskQueue.pop();
        }

        // Execute the task (outside of the lock)
        task();
    }
}

void TaskScheduler::stop() {
    spdlog::info("[TS-{}] Stopping...", name);
    // Stop the workers
    running.store(false);
    taskAvailable.notify_all();
    // Wait for all workers to finish
    for (auto &worker : workers) {
        if (worker.joinable()) {
            worker.join();
        }
    }
    spdlog::info("[TS-{}] Stopped with {} tasks remaining", name, taskQueue.size());
}

