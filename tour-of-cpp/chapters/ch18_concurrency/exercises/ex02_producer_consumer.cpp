// Exercise 18.2: Producer-Consumer Pattern
// Book reference: 18.3-18.4
// Difficulty: ***
//
// Instructions:
// Implement a thread-safe queue and use it for producer-consumer coordination.
// Practice using mutex, lock_guard, unique_lock, and condition_variable.

#include <chrono>
#include <condition_variable>
#include <format>
#include <functional>
#include <iostream>
#include <mutex>
#include <optional>
#include <queue>
#include <thread>
#include <vector>

using namespace std::chrono_literals;

// ============================================================================
// Part 1: Thread-Safe Queue
// ============================================================================

// TODO 1: Implement a thread-safe queue with the following methods:
// - push(value): Add a value to the queue
// - pop(): Remove and return front value (blocks if empty)
// - try_pop(): Try to pop, return optional (doesn't block)
// - empty(): Check if empty

template <typename T>
class ThreadSafeQueue {
public:
    void push(T value) {
        // TODO: Lock, push, notify
        (void)value;
    }

    T pop() {
        // TODO: Lock, wait until not empty, pop and return
        return T{};
    }

    std::optional<T> try_pop() {
        // TODO: Lock, try to pop without waiting
        return std::nullopt;
    }

    bool empty() const {
        // TODO: Lock and check
        return true;
    }

    std::size_t size() const {
        // TODO: Lock and return size
        return 0;
    }

private:
    mutable std::mutex mutex_;
    std::condition_variable cv_;
    std::queue<T> queue_;
};

// ============================================================================
// Part 2: Simple Producer-Consumer
// ============================================================================

// TODO 2: Implement producer and consumer functions that use the queue.

void producer(ThreadSafeQueue<int>& queue, int id, int count) {
    // TODO: Produce 'count' items (value = id * 1000 + i)
    // Add a small delay between productions
    (void)queue;
    (void)id;
    (void)count;
}

void consumer(ThreadSafeQueue<int>& queue, int id, int count) {
    // TODO: Consume 'count' items, print what you consume
    (void)queue;
    (void)id;
    (void)count;
}

void exercise_2() {
    std::cout << "--- Exercise 2: Simple Producer-Consumer ---\n";

    ThreadSafeQueue<int> queue;

    // TODO: Create 2 producers (each producing 5 items)
    // and 2 consumers (each consuming 5 items)

    std::cout << '\n';
}

// ============================================================================
// Part 3: Bounded Queue (blocks when full)
// ============================================================================

// TODO 3: Implement a bounded queue that blocks push() when at capacity.

template <typename T>
class BoundedQueue {
public:
    explicit BoundedQueue(std::size_t capacity) : capacity_(capacity) {
        // TODO: Initialize
    }

    void push(T value) {
        // TODO: Wait if at capacity, then push
        (void)value;
    }

    T pop() {
        // TODO: Wait if empty, then pop
        return T{};
    }

    std::size_t size() const {
        // TODO: Return current size
        return 0;
    }

private:
    std::size_t capacity_;
    mutable std::mutex mutex_;
    std::condition_variable not_full_;
    std::condition_variable not_empty_;
    std::queue<T> queue_;
};

void exercise_3() {
    std::cout << "--- Exercise 3: Bounded Queue ---\n";

    BoundedQueue<int> queue{3};  // Capacity of 3

    // TODO: Create a fast producer and slow consumer
    // Observe that producer blocks when queue is full

    std::cout << '\n';
}

// ============================================================================
// Part 4: Work Queue with Shutdown
// ============================================================================

// TODO 4: Implement a work queue that can be shut down.
// Workers should exit cleanly when shutdown is signaled.

template <typename T>
class WorkQueue {
public:
    void push(T value) {
        // TODO: Add work item (ignore if shutdown)
        (void)value;
    }

    std::optional<T> pop() {
        // TODO: Return nullopt if shutdown and empty
        return std::nullopt;
    }

    void shutdown() {
        // TODO: Signal shutdown, notify all waiters
    }

    bool is_shutdown() const {
        // TODO: Check shutdown status
        return false;
    }

private:
    mutable std::mutex mutex_;
    std::condition_variable cv_;
    std::queue<T> queue_;
    bool shutdown_ = false;
};

void worker_thread(WorkQueue<int>& queue, int id) {
    while (true) {
        auto item = queue.pop();
        if (!item) {
            std::cout << std::format("Worker {} exiting\n", id);
            break;
        }
        std::cout << std::format("Worker {} processed {}\n", id, *item);
        std::this_thread::sleep_for(10ms);
    }
}

void exercise_4() {
    std::cout << "--- Exercise 4: Work Queue with Shutdown ---\n";

    WorkQueue<int> queue;

    // TODO: Create 3 worker threads
    // Add 10 work items
    // Shutdown the queue
    // Join all workers

    std::cout << '\n';
}

// ============================================================================
// Part 5: Thread Pool (Simplified)
// ============================================================================

// TODO 5: Implement a simple thread pool.

class ThreadPool {
public:
    explicit ThreadPool(std::size_t num_threads) {
        // TODO: Create worker threads
        (void)num_threads;
    }

    ~ThreadPool() {
        // TODO: Shutdown and join all threads
    }

    template <typename F>
    void submit(F&& task) {
        // TODO: Add task to queue
        (void)task;
    }

private:
    void worker_loop() {
        // TODO: Pop and execute tasks until shutdown
    }

    std::vector<std::thread> workers_;
    WorkQueue<std::function<void()>> tasks_;
};

void exercise_5() {
    std::cout << "--- Exercise 5: Thread Pool ---\n";

    ThreadPool pool{4};

    // TODO: Submit 10 tasks that print their task number
    // Give time for tasks to complete

    std::cout << '\n';
}

// ============================================================================
// Part 6: Parallel Map
// ============================================================================

// TODO 6: Implement parallel_map that applies a function to each element
// using multiple threads.

template <typename T, typename F>
std::vector<T> parallel_map(const std::vector<T>& input, F func, int num_threads) {
    // TODO: Split work among threads, apply func to each element
    (void)input;
    (void)func;
    (void)num_threads;
    return {};
}

void exercise_6() {
    std::cout << "--- Exercise 6: Parallel Map ---\n";

    std::vector<int> input{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    auto square = [](int x) { return x * x; };

    auto result = parallel_map(input, square, 4);

    std::cout << "Squares: ";
    for (int x : result) {
        std::cout << x << ' ';
    }
    std::cout << "(expected: 1 4 9 16 25 36 49 64 81 100)\n\n";
}

// ============================================================================
// Main
// ============================================================================

int main() {
    std::cout << "=== Exercise 18.2: Producer-Consumer Pattern ===\n\n";

    std::cout << "--- Exercise 1: ThreadSafeQueue ---\n";
    {
        ThreadSafeQueue<int> q;
        q.push(1);
        q.push(2);
        std::cout << "Size: " << q.size() << " (expected: 2)\n";
        std::cout << "Pop: " << q.pop() << " (expected: 1)\n";
        std::cout << "Pop: " << q.pop() << " (expected: 2)\n";
        std::cout << "Empty: " << q.empty() << " (expected: 1)\n\n";
    }

    exercise_2();
    exercise_3();
    exercise_4();
    exercise_5();
    exercise_6();

    std::cout << "=== Exercises complete ===\n";

    return 0;
}
