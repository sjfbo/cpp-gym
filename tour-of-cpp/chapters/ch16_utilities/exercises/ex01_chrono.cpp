// Exercise 16.1: Chrono Practice
// Book reference: 16.2 Time
// Difficulty: **
//
// Instructions:
// Implement the TODOs below to practice chrono usage.
//
// Part 1: Implement a Timer class for measuring elapsed time
// Part 2: Implement rate limiting with time-based throttling
// Part 3: Implement a deadline-based timeout pattern
//
// Compile and run to verify your solutions.

#include <chrono>
#include <iostream>
#include <thread>
#include <cassert>
#include <functional>

using namespace std::chrono_literals;

// ============================================================================
// Part 1: Timer Class
// ============================================================================

class Timer {
public:
    // TODO 1: Implement start()
    // Record the current time using steady_clock
    void start() {
        // Your implementation here
    }

    // TODO 2: Implement stop()
    // Record the end time
    void stop() {
        // Your implementation here
    }

    // TODO 3: Implement elapsed()
    // Return the elapsed time as the specified duration type
    // If timer is still running, measure from start to now
    // If timer is stopped, measure from start to stop
    template<typename Duration = std::chrono::milliseconds>
    Duration elapsed() const {
        // Your implementation here
        return Duration{0};  // Replace this
    }

    // TODO 4: Implement reset()
    // Reset the timer to initial state
    void reset() {
        // Your implementation here
    }

private:
    // Add your member variables here
    // Hint: You'll need start time, end time, and a running flag
};

void test_part1() {
    std::cout << "=== Part 1: Timer Class ===\n";

    Timer timer;

    timer.start();
    std::this_thread::sleep_for(50ms);
    auto running_elapsed = timer.elapsed();
    assert(running_elapsed >= 50ms);
    std::cout << "Running elapsed: " << running_elapsed.count() << "ms\n";

    std::this_thread::sleep_for(50ms);
    timer.stop();
    auto final_elapsed = timer.elapsed();
    assert(final_elapsed >= 100ms);
    std::cout << "Final elapsed: " << final_elapsed.count() << "ms\n";

    // Elapsed should stay same after stop
    std::this_thread::sleep_for(50ms);
    assert(timer.elapsed() == final_elapsed);
    std::cout << "After additional wait: " << timer.elapsed().count() << "ms (unchanged)\n";

    // Reset should clear the timer
    timer.reset();
    timer.start();
    std::this_thread::sleep_for(20ms);
    assert(timer.elapsed() < 50ms);
    std::cout << "After reset: " << timer.elapsed().count() << "ms\n";

    std::cout << "Part 1 passed!\n\n";
}

// ============================================================================
// Part 2: Rate Limiter
// ============================================================================

class RateLimiter {
public:
    // TODO 5: Implement constructor
    // Initialize with the minimum interval between allowed actions
    explicit RateLimiter(std::chrono::milliseconds min_interval) {
        // Your implementation here
    }

    // TODO 6: Implement try_acquire()
    // Returns true if enough time has passed since last acquisition
    // If true, updates the last acquisition time
    // If false, the caller should wait
    bool try_acquire() {
        // Your implementation here
        return false;  // Replace this
    }

    // TODO 7: Implement time_until_available()
    // Returns how long until the next acquisition will be allowed
    // Returns 0ms if available now
    std::chrono::milliseconds time_until_available() const {
        // Your implementation here
        return 0ms;  // Replace this
    }

private:
    // Add your member variables here
};

void test_part2() {
    std::cout << "=== Part 2: Rate Limiter ===\n";

    RateLimiter limiter{100ms};

    // First acquisition should succeed
    assert(limiter.try_acquire());
    std::cout << "First acquisition: success\n";

    // Immediate second should fail
    assert(!limiter.try_acquire());
    std::cout << "Immediate second: blocked\n";

    auto wait_time = limiter.time_until_available();
    std::cout << "Time until available: " << wait_time.count() << "ms\n";
    assert(wait_time > 0ms && wait_time <= 100ms);

    // Wait for it
    std::this_thread::sleep_for(wait_time + 10ms);
    assert(limiter.try_acquire());
    std::cout << "After waiting: success\n";

    std::cout << "Part 2 passed!\n\n";
}

// ============================================================================
// Part 3: Deadline-Based Operations
// ============================================================================

// TODO 8: Implement run_with_deadline
// Execute the given function repeatedly until the deadline
// Return the number of times the function was executed
// The function returns true to continue, false to stop early
int run_with_deadline(
    std::chrono::steady_clock::time_point deadline,
    std::function<bool()> func) {
    // Your implementation here
    return 0;  // Replace this
}

// TODO 9: Implement wait_for_condition_with_timeout
// Wait for the condition to become true, checking every check_interval
// Returns true if condition was met, false if timeout occurred
bool wait_for_condition_with_timeout(
    std::function<bool()> condition,
    std::chrono::milliseconds timeout,
    std::chrono::milliseconds check_interval = 10ms) {
    // Your implementation here
    return false;  // Replace this
}

void test_part3() {
    std::cout << "=== Part 3: Deadline-Based Operations ===\n";

    // Test run_with_deadline
    auto deadline = std::chrono::steady_clock::now() + 100ms;
    int counter = 0;

    int iterations = run_with_deadline(deadline, [&counter]() {
        ++counter;
        std::this_thread::sleep_for(10ms);
        return true;  // Continue
    });

    std::cout << "Executed " << iterations << " times before deadline\n";
    assert(iterations >= 5 && iterations <= 15);
    assert(iterations == counter);

    // Test early termination
    counter = 0;
    deadline = std::chrono::steady_clock::now() + 1s;

    iterations = run_with_deadline(deadline, [&counter]() {
        ++counter;
        return counter < 5;  // Stop after 5
    });

    std::cout << "Executed " << iterations << " times before early stop\n";
    assert(iterations == 5);

    // Test wait_for_condition_with_timeout - success case
    std::atomic<bool> ready{false};

    // Start a thread that sets ready after 50ms
    std::thread([&ready]() {
        std::this_thread::sleep_for(50ms);
        ready = true;
    }).detach();

    bool result = wait_for_condition_with_timeout(
        [&ready]() { return ready.load(); },
        200ms);

    assert(result);
    std::cout << "Condition met within timeout: " << std::boolalpha << result << "\n";

    // Test wait_for_condition_with_timeout - timeout case
    result = wait_for_condition_with_timeout(
        []() { return false; },  // Never true
        50ms);

    assert(!result);
    std::cout << "Condition timeout: " << !result << "\n";

    std::cout << "Part 3 passed!\n\n";
}

#include <atomic>

int main() {
    test_part1();
    test_part2();
    test_part3();

    std::cout << "=== All exercises completed! ===\n";
    return 0;
}
