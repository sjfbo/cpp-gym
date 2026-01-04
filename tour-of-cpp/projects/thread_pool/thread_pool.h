#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <functional>
#include <type_traits>
#include <stdexcept>

namespace concurrent {

/**
 * A simple fixed-size thread pool for executing tasks asynchronously.
 *
 * Features:
 * - Fixed number of worker threads
 * - Submit callable objects and get futures for results
 * - Graceful shutdown on destruction
 * - Exception propagation through futures
 */
class ThreadPool {
public:
    /**
     * Construct a thread pool with the specified number of worker threads.
     * @param num_threads Number of worker threads (default: hardware concurrency)
     */
    explicit ThreadPool(size_t num_threads = std::thread::hardware_concurrency())
        : stop_(false)
    {
        if (num_threads == 0) {
            num_threads = 1;
        }

        workers_.reserve(num_threads);
        for (size_t i = 0; i < num_threads; ++i) {
            workers_.emplace_back([this] {
                worker_loop();
            });
        }
    }

    /**
     * Destructor - waits for all tasks to complete.
     */
    ~ThreadPool() {
        shutdown();
    }

    // Non-copyable, non-movable
    ThreadPool(const ThreadPool&) = delete;
    ThreadPool& operator=(const ThreadPool&) = delete;
    ThreadPool(ThreadPool&&) = delete;
    ThreadPool& operator=(ThreadPool&&) = delete;

    /**
     * Submit a task to be executed by the thread pool.
     *
     * @param f Callable object to execute
     * @param args Arguments to pass to the callable
     * @return std::future for the result of the task
     * @throws std::runtime_error if the pool is stopped
     *
     * Example:
     *   auto future = pool.submit([](int x) { return x * 2; }, 21);
     *   int result = future.get(); // result = 42
     */
    template <typename F, typename... Args>
    [[nodiscard]] auto submit(F&& f, Args&&... args)
        -> std::future<std::invoke_result_t<F, Args...>>
    {
        using return_type = std::invoke_result_t<F, Args...>;

        // Create a packaged task wrapping the callable with its arguments
        auto task = std::make_shared<std::packaged_task<return_type()>>(
            [func = std::forward<F>(f),
             ...captured_args = std::forward<Args>(args)]() mutable {
                return func(std::forward<Args>(captured_args)...);
            }
        );

        std::future<return_type> future = task->get_future();

        {
            std::unique_lock<std::mutex> lock(mutex_);

            if (stop_) {
                throw std::runtime_error("Cannot submit to stopped thread pool");
            }

            // Add task to queue (wrap in void-returning function)
            tasks_.emplace([task = std::move(task)]() {
                (*task)();
            });
        }

        // Notify one waiting worker
        condition_.notify_one();

        return future;
    }

    /**
     * Get the number of worker threads.
     */
    [[nodiscard]] size_t size() const noexcept {
        return workers_.size();
    }

    /**
     * Get the number of pending tasks.
     */
    [[nodiscard]] size_t pending() const {
        std::unique_lock<std::mutex> lock(mutex_);
        return tasks_.size();
    }

    /**
     * Check if the pool is stopped.
     */
    [[nodiscard]] bool stopped() const {
        std::unique_lock<std::mutex> lock(mutex_);
        return stop_;
    }

    /**
     * Shutdown the thread pool.
     * Waits for all currently executing tasks to complete.
     * Pending tasks in the queue will still be executed.
     */
    void shutdown() {
        {
            std::unique_lock<std::mutex> lock(mutex_);
            if (stop_) {
                return; // Already stopped
            }
            stop_ = true;
        }

        // Wake up all workers so they can check the stop flag
        condition_.notify_all();

        // Wait for all workers to finish
        for (auto& worker : workers_) {
            if (worker.joinable()) {
                worker.join();
            }
        }
    }

private:
    /**
     * Worker thread loop - continuously takes and executes tasks.
     */
    void worker_loop() {
        while (true) {
            std::function<void()> task;

            {
                std::unique_lock<std::mutex> lock(mutex_);

                // Wait until there's a task or we're stopping
                condition_.wait(lock, [this] {
                    return stop_ || !tasks_.empty();
                });

                // If stopping and no more tasks, exit
                if (stop_ && tasks_.empty()) {
                    return;
                }

                // Get the next task
                task = std::move(tasks_.front());
                tasks_.pop();
            }

            // Execute the task (outside the lock)
            task();
        }
    }

    // Worker threads
    std::vector<std::thread> workers_;

    // Task queue
    std::queue<std::function<void()>> tasks_;

    // Synchronization
    mutable std::mutex mutex_;
    std::condition_variable condition_;

    // Stop flag
    bool stop_;
};

/**
 * Helper function to create a thread pool with default thread count.
 */
[[nodiscard]] inline ThreadPool make_thread_pool() {
    return ThreadPool();
}

/**
 * Helper function to create a thread pool with specified thread count.
 */
[[nodiscard]] inline ThreadPool make_thread_pool(size_t num_threads) {
    return ThreadPool(num_threads);
}

} // namespace concurrent

#endif // THREAD_POOL_H
