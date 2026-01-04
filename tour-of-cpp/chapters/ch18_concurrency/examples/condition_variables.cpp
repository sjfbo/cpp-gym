// Condition Variables - Thread Synchronization
// Book reference: 18.4 Waiting for Events
//
// Condition variables allow threads to wait for a condition to become true,
// without busy-waiting.

#include <chrono>
#include <condition_variable>
#include <format>
#include <iostream>
#include <mutex>
#include <queue>
#include <string>
#include <thread>

using namespace std::chrono_literals;

// ============================================================================
// Basic condition variable usage
// ============================================================================

namespace basic_cv {

std::mutex mtx;
std::condition_variable cv;
bool ready = false;
std::string data;

void producer() {
    std::this_thread::sleep_for(100ms);  // Simulate work

    {
        std::lock_guard<std::mutex> lock{mtx};
        data = "Hello from producer!";
        ready = true;
        std::cout << "Producer: data ready\n";
    }

    cv.notify_one();  // Wake up one waiting thread
}

void consumer() {
    std::unique_lock<std::mutex> lock{mtx};

    // Wait until ready becomes true
    // Handles spurious wakeups automatically
    cv.wait(lock, []{ return ready; });

    std::cout << std::format("Consumer received: {}\n", data);
}

void demo() {
    std::cout << "--- Basic Condition Variable ---\n";
    ready = false;
    data.clear();

    std::thread t1{consumer};
    std::thread t2{producer};

    t1.join();
    t2.join();
    std::cout << '\n';
}

}  // namespace basic_cv

// ============================================================================
// Notify all
// ============================================================================

namespace notify_all_demo {

std::mutex mtx;
std::condition_variable cv;
bool ready = false;

void worker(int id) {
    std::unique_lock<std::mutex> lock{mtx};
    cv.wait(lock, []{ return ready; });
    std::cout << std::format("Worker {} started\n", id);
}

void starter() {
    std::this_thread::sleep_for(50ms);

    {
        std::lock_guard<std::mutex> lock{mtx};
        ready = true;
        std::cout << "Starter: signaling all workers\n";
    }

    cv.notify_all();  // Wake up ALL waiting threads
}

void demo() {
    std::cout << "--- Notify All ---\n";
    ready = false;

    std::vector<std::thread> workers;
    for (int i = 0; i < 5; ++i) {
        workers.emplace_back(worker, i);
    }

    std::thread t{starter};

    for (auto& w : workers) {
        w.join();
    }
    t.join();
    std::cout << '\n';
}

}  // namespace notify_all_demo

// ============================================================================
// wait_for with timeout
// ============================================================================

namespace wait_for_demo {

std::mutex mtx;
std::condition_variable cv;
bool ready = false;

void waiter() {
    std::unique_lock<std::mutex> lock{mtx};

    // Wait with timeout
    if (cv.wait_for(lock, 200ms, []{ return ready; })) {
        std::cout << "Condition became true!\n";
    } else {
        std::cout << "Timed out waiting\n";
    }
}

void demo() {
    std::cout << "--- Wait with Timeout ---\n";
    ready = false;

    std::thread t1{waiter};
    t1.join();

    // Now with signal
    ready = false;
    std::thread t2{waiter};
    std::this_thread::sleep_for(50ms);
    {
        std::lock_guard<std::mutex> lock{mtx};
        ready = true;
    }
    cv.notify_one();
    t2.join();
    std::cout << '\n';
}

}  // namespace wait_for_demo

// ============================================================================
// Producer-Consumer Queue
// ============================================================================

namespace producer_consumer {

template <typename T>
class ThreadSafeQueue {
public:
    void push(T value) {
        {
            std::lock_guard<std::mutex> lock{mutex_};
            queue_.push(std::move(value));
        }
        cv_.notify_one();
    }

    T pop() {
        std::unique_lock<std::mutex> lock{mutex_};
        cv_.wait(lock, [this]{ return !queue_.empty(); });
        T value = std::move(queue_.front());
        queue_.pop();
        return value;
    }

    bool try_pop(T& value, std::chrono::milliseconds timeout) {
        std::unique_lock<std::mutex> lock{mutex_};
        if (!cv_.wait_for(lock, timeout, [this]{ return !queue_.empty(); })) {
            return false;
        }
        value = std::move(queue_.front());
        queue_.pop();
        return true;
    }

    bool empty() const {
        std::lock_guard<std::mutex> lock{mutex_};
        return queue_.empty();
    }

private:
    mutable std::mutex mutex_;
    std::condition_variable cv_;
    std::queue<T> queue_;
};

void demo() {
    std::cout << "--- Producer-Consumer Queue ---\n";

    ThreadSafeQueue<int> queue;

    // Producer
    std::thread producer{[&queue]() {
        for (int i = 0; i < 5; ++i) {
            std::this_thread::sleep_for(20ms);
            queue.push(i);
            std::cout << std::format("Produced: {}\n", i);
        }
    }};

    // Consumer
    std::thread consumer{[&queue]() {
        for (int i = 0; i < 5; ++i) {
            int value = queue.pop();
            std::cout << std::format("Consumed: {}\n", value);
        }
    }};

    producer.join();
    consumer.join();
    std::cout << '\n';
}

}  // namespace producer_consumer

// ============================================================================
// Bounded buffer (blocking when full)
// ============================================================================

namespace bounded_buffer {

template <typename T, std::size_t Capacity>
class BoundedBuffer {
public:
    void push(T value) {
        std::unique_lock<std::mutex> lock{mutex_};
        not_full_.wait(lock, [this]{ return buffer_.size() < Capacity; });
        buffer_.push(std::move(value));
        not_empty_.notify_one();
    }

    T pop() {
        std::unique_lock<std::mutex> lock{mutex_};
        not_empty_.wait(lock, [this]{ return !buffer_.empty(); });
        T value = std::move(buffer_.front());
        buffer_.pop();
        not_full_.notify_one();
        return value;
    }

private:
    std::mutex mutex_;
    std::condition_variable not_empty_;
    std::condition_variable not_full_;
    std::queue<T> buffer_;
};

void demo() {
    std::cout << "--- Bounded Buffer ---\n";

    BoundedBuffer<int, 3> buffer;  // Capacity of 3

    // Fast producer, slow consumer
    std::thread producer{[&buffer]() {
        for (int i = 0; i < 10; ++i) {
            buffer.push(i);
            std::cout << std::format("Pushed: {} (will block if full)\n", i);
        }
    }};

    std::thread consumer{[&buffer]() {
        for (int i = 0; i < 10; ++i) {
            std::this_thread::sleep_for(50ms);
            int value = buffer.pop();
            std::cout << std::format("Popped: {}\n", value);
        }
    }};

    producer.join();
    consumer.join();
    std::cout << '\n';
}

}  // namespace bounded_buffer

// ============================================================================
// One-shot signal (latch-like behavior)
// ============================================================================

namespace one_shot {

class Signal {
public:
    void set() {
        {
            std::lock_guard<std::mutex> lock{mutex_};
            signaled_ = true;
        }
        cv_.notify_all();
    }

    void wait() {
        std::unique_lock<std::mutex> lock{mutex_};
        cv_.wait(lock, [this]{ return signaled_; });
    }

    bool wait_for(std::chrono::milliseconds timeout) {
        std::unique_lock<std::mutex> lock{mutex_};
        return cv_.wait_for(lock, timeout, [this]{ return signaled_; });
    }

private:
    std::mutex mutex_;
    std::condition_variable cv_;
    bool signaled_ = false;
};

void demo() {
    std::cout << "--- One-Shot Signal ---\n";

    Signal start_signal;

    auto worker = [&start_signal](int id) {
        std::cout << std::format("Worker {} waiting for start signal\n", id);
        start_signal.wait();
        std::cout << std::format("Worker {} started!\n", id);
    };

    std::vector<std::thread> workers;
    for (int i = 0; i < 3; ++i) {
        workers.emplace_back(worker, i);
    }

    std::this_thread::sleep_for(100ms);
    std::cout << "Sending start signal...\n";
    start_signal.set();

    for (auto& w : workers) {
        w.join();
    }
    std::cout << '\n';
}

}  // namespace one_shot

int main() {
    std::cout << "=== Condition Variables ===\n\n";

    basic_cv::demo();
    notify_all_demo::demo();
    wait_for_demo::demo();
    producer_consumer::demo();
    bounded_buffer::demo();
    one_shot::demo();

    std::cout << "=== All demos completed ===\n";

    return 0;
}

// Key points:
// - cv.wait(lock, predicate) is the safe way to wait
// - Always use a predicate to handle spurious wakeups
// - notify_one() wakes one waiter, notify_all() wakes all
// - Condition variables work with unique_lock, not lock_guard
// - Common pattern: producer-consumer queue
// - Use wait_for/wait_until for timeouts
