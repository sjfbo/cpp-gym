// Threads - Basic Thread Operations
// Book reference: 18.2 Tasks and Threads
//
// std::thread provides a way to run functions concurrently.

#include <chrono>
#include <format>
#include <functional>
#include <iostream>
#include <string>
#include <thread>
#include <vector>

using namespace std::chrono_literals;

// ============================================================================
// Basic thread functions
// ============================================================================

void simple_task() {
    std::cout << "Hello from thread " << std::this_thread::get_id() << '\n';
}

void task_with_args(int id, const std::string& message) {
    std::cout << std::format("Thread {}: {}\n", id, message);
}

void sleeping_task(int ms) {
    std::cout << std::format("Sleeping for {}ms...\n", ms);
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
    std::cout << "Woke up!\n";
}

// ============================================================================
// Class with member function
// ============================================================================

class Worker {
public:
    explicit Worker(int id) : id_(id) {}

    void do_work() {
        std::cout << std::format("Worker {} doing work\n", id_);
    }

    void do_work_with_param(int iterations) {
        for (int i = 0; i < iterations; ++i) {
            std::cout << std::format("Worker {} iteration {}\n", id_, i);
            std::this_thread::sleep_for(10ms);
        }
    }

private:
    int id_;
};

// ============================================================================
// Return values via reference
// ============================================================================

void compute_result(int input, int& output) {
    std::this_thread::sleep_for(50ms);  // Simulate work
    output = input * input;
}

int main() {
    std::cout << "=== Thread Basics ===\n\n";

    std::cout << "Main thread ID: " << std::this_thread::get_id() << '\n';
    std::cout << "Hardware concurrency: " << std::thread::hardware_concurrency()
              << " threads\n\n";

    // =========================================================================
    // Creating and joining threads
    // =========================================================================
    std::cout << "--- Basic Thread Creation ---\n";

    std::thread t1{simple_task};
    t1.join();  // Wait for thread to complete
    std::cout << '\n';

    // =========================================================================
    // Thread with arguments
    // =========================================================================
    std::cout << "--- Thread with Arguments ---\n";

    std::thread t2{task_with_args, 1, "Hello from t2"};
    std::thread t3{task_with_args, 2, "Hello from t3"};

    t2.join();
    t3.join();
    std::cout << '\n';

    // =========================================================================
    // Lambda as thread function
    // =========================================================================
    std::cout << "--- Lambda Thread ---\n";

    int value = 42;
    std::thread t4{[value]() {
        std::cout << std::format("Lambda thread sees value: {}\n", value);
    }};
    t4.join();
    std::cout << '\n';

    // =========================================================================
    // Member function
    // =========================================================================
    std::cout << "--- Member Function Thread ---\n";

    Worker worker{1};
    std::thread t5{&Worker::do_work, &worker};
    t5.join();

    std::thread t6{&Worker::do_work_with_param, &worker, 3};
    t6.join();
    std::cout << '\n';

    // =========================================================================
    // Passing by reference (requires std::ref)
    // =========================================================================
    std::cout << "--- Passing by Reference ---\n";

    int result = 0;
    std::thread t7{compute_result, 7, std::ref(result)};
    t7.join();
    std::cout << std::format("Result: {} (expected 49)\n\n", result);

    // =========================================================================
    // Multiple threads
    // =========================================================================
    std::cout << "--- Multiple Threads ---\n";

    std::vector<std::thread> threads;
    for (int i = 0; i < 5; ++i) {
        threads.emplace_back([i]() {
            std::this_thread::sleep_for(std::chrono::milliseconds(i * 10));
            std::cout << std::format("Thread {} completed\n", i);
        });
    }

    for (auto& t : threads) {
        t.join();
    }
    std::cout << '\n';

    // =========================================================================
    // Checking joinable
    // =========================================================================
    std::cout << "--- Joinable Check ---\n";

    std::thread t8{simple_task};
    std::cout << std::format("Before join, joinable: {}\n", t8.joinable());
    t8.join();
    std::cout << std::format("After join, joinable: {}\n\n", t8.joinable());

    // =========================================================================
    // Detached threads (fire and forget)
    // =========================================================================
    std::cout << "--- Detached Thread ---\n";

    std::thread t9{[]() {
        std::this_thread::sleep_for(10ms);
        std::cout << "Detached thread finished\n";
    }};
    t9.detach();
    std::cout << "Main thread continues immediately\n";

    // Give detached thread time to complete for demo purposes
    std::this_thread::sleep_for(50ms);
    std::cout << '\n';

    // =========================================================================
    // Thread ID comparison
    // =========================================================================
    std::cout << "--- Thread IDs ---\n";

    auto main_id = std::this_thread::get_id();
    std::thread t10{[main_id]() {
        auto this_id = std::this_thread::get_id();
        std::cout << std::format("This thread ID: {}\n", this_id);
        std::cout << std::format("Same as main? {}\n", this_id == main_id);
    }};
    t10.join();
    std::cout << '\n';

    // =========================================================================
    // Moving threads
    // =========================================================================
    std::cout << "--- Moving Threads ---\n";

    std::thread t11{simple_task};
    std::thread t12 = std::move(t11);  // t11 is now not-a-thread

    std::cout << std::format("t11 joinable after move: {}\n", t11.joinable());
    std::cout << std::format("t12 joinable after move: {}\n", t12.joinable());

    t12.join();

    std::cout << "\n=== All threads completed ===\n";

    return 0;
}

// Key points:
// - std::thread runs a callable concurrently
// - Always join() or detach() before thread destructor
// - Use std::ref() to pass references to threads
// - Threads are move-only, not copyable
// - Use hardware_concurrency() to query available parallelism
