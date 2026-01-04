// Exercise 18.1: Thread Basics
// Book reference: 18.2 Tasks and Threads
// Difficulty: **
//
// Instructions:
// Practice creating and managing threads.
// Pay attention to joining threads and passing arguments correctly.

#include <chrono>
#include <format>
#include <functional>
#include <iostream>
#include <mutex>
#include <numeric>
#include <thread>
#include <vector>

using namespace std::chrono_literals;

// Global mutex for printing (so output doesn't get mixed up)
std::mutex print_mutex;

void safe_print(const std::string& msg) {
    std::lock_guard<std::mutex> lock{print_mutex};
    std::cout << msg << '\n';
}

// ============================================================================
// Part 1: Basic Thread Creation
// ============================================================================

// TODO 1: Create a function that prints a message with the thread ID.
// Then launch it in a thread and join.

void print_thread_id() {
    // TODO: Print this thread's ID
}

void exercise_1() {
    std::cout << "--- Exercise 1: Basic Thread ---\n";
    // TODO: Create a thread running print_thread_id and join it
    std::cout << '\n';
}

// ============================================================================
// Part 2: Passing Arguments
// ============================================================================

// TODO 2: Create a function that takes an int and a string,
// then launch it in a thread with arguments.

void greet(int count, const std::string& name) {
    // TODO: Print the greeting 'count' times
    (void)count;
    (void)name;
}

void exercise_2() {
    std::cout << "--- Exercise 2: Thread with Arguments ---\n";
    // TODO: Create a thread calling greet(3, "World") and join it
    std::cout << '\n';
}

// ============================================================================
// Part 3: Passing by Reference
// ============================================================================

// TODO 3: Create a function that modifies a passed-in reference.
// Make sure to use std::ref when passing to the thread.

void double_value(int& value) {
    // TODO: Double the value
    (void)value;
}

void exercise_3() {
    std::cout << "--- Exercise 3: Passing by Reference ---\n";
    int value = 21;
    // TODO: Create a thread that doubles 'value' using double_value
    // Remember to use std::ref!

    std::cout << std::format("Value after thread: {} (expected: 42)\n\n", value);
}

// ============================================================================
// Part 4: Lambda Threads
// ============================================================================

// TODO 4: Create a thread using a lambda that captures and modifies
// local variables.

void exercise_4() {
    std::cout << "--- Exercise 4: Lambda Thread ---\n";
    int sum = 0;

    // TODO: Create a thread with a lambda that adds 1+2+3+...+10 to sum
    // Use capture by reference

    std::cout << std::format("Sum: {} (expected: 55)\n\n", sum);
}

// ============================================================================
// Part 5: Multiple Threads
// ============================================================================

// TODO 5: Create multiple threads that each do some work,
// then join all of them.

void worker(int id, int& result) {
    // Simulate work
    std::this_thread::sleep_for(std::chrono::milliseconds(id * 10));
    result = id * id;
}

void exercise_5() {
    std::cout << "--- Exercise 5: Multiple Threads ---\n";

    const int num_threads = 5;
    std::vector<int> results(num_threads);

    // TODO: Create num_threads threads, each calling worker(i, results[i])
    // Then join all threads and print results

    std::cout << "Results: ";
    for (int r : results) {
        std::cout << r << ' ';
    }
    std::cout << "(expected: 0 1 4 9 16)\n\n";
}

// ============================================================================
// Part 6: Thread-Safe Counter (with mutex)
// ============================================================================

// TODO 6: Implement a thread-safe increment function using a mutex.

int counter = 0;
std::mutex counter_mutex;

void safe_increment(int times) {
    // TODO: Increment 'counter' 'times' times, with proper locking
    (void)times;
}

void exercise_6() {
    std::cout << "--- Exercise 6: Thread-Safe Counter ---\n";
    counter = 0;

    // TODO: Create two threads, each calling safe_increment(10000)
    // Then join and check the result

    std::cout << std::format("Counter: {} (expected: 20000)\n\n", counter);
}

// ============================================================================
// Part 7: Parallel Sum
// ============================================================================

// TODO 7: Implement parallel sum of a vector using multiple threads.

long long parallel_sum(const std::vector<int>& data, int num_threads) {
    // TODO: Split data into num_threads chunks
    // Create threads to sum each chunk
    // Collect and return total sum
    (void)data;
    (void)num_threads;
    return 0;
}

void exercise_7() {
    std::cout << "--- Exercise 7: Parallel Sum ---\n";

    std::vector<int> data(1000);
    std::iota(data.begin(), data.end(), 1);  // 1, 2, 3, ..., 1000

    long long expected = 1000LL * 1001 / 2;  // Sum formula
    long long result = parallel_sum(data, 4);

    std::cout << std::format("Sum: {} (expected: {})\n\n", result, expected);
}

// ============================================================================
// Part 8: Thread RAII Wrapper
// ============================================================================

// TODO 8: Implement a simple RAII wrapper for std::thread that
// automatically joins on destruction.

class JoiningThread {
public:
    // TODO: Implement constructor that takes a thread
    // TODO: Implement destructor that joins if joinable
    // TODO: Make it move-only (delete copy, implement move)

    template <typename F, typename... Args>
    explicit JoiningThread(F&& f, Args&&... args) {
        // TODO: Create thread with forwarded arguments
        (void)f;
        ((void)args, ...);
    }

    ~JoiningThread() {
        // TODO: Join if joinable
    }

    JoiningThread(const JoiningThread&) = delete;
    JoiningThread& operator=(const JoiningThread&) = delete;

    JoiningThread(JoiningThread&& other) noexcept {
        // TODO: Move the thread
        (void)other;
    }

    JoiningThread& operator=(JoiningThread&& other) noexcept {
        // TODO: Move-assign the thread
        (void)other;
        return *this;
    }

private:
    std::thread thread_;
};

void exercise_8() {
    std::cout << "--- Exercise 8: RAII Thread Wrapper ---\n";

    {
        JoiningThread t{[]() {
            safe_print("JoiningThread running!");
        }};
        // Thread should join automatically when t goes out of scope
    }

    std::cout << "Thread joined automatically\n\n";
}

// ============================================================================
// Main
// ============================================================================

int main() {
    std::cout << "=== Exercise 18.1: Thread Basics ===\n\n";

    exercise_1();
    exercise_2();
    exercise_3();
    exercise_4();
    exercise_5();
    exercise_6();
    exercise_7();
    exercise_8();

    std::cout << "=== Exercises complete ===\n";

    return 0;
}
