// Sharing Data - Mutexes and Locks
// Book reference: 18.3 Sharing Data
//
// When multiple threads access shared data, synchronization is essential
// to avoid data races (undefined behavior).

#include <chrono>
#include <format>
#include <iostream>
#include <mutex>
#include <shared_mutex>
#include <thread>
#include <vector>

using namespace std::chrono_literals;

// ============================================================================
// Demonstrating data race (DON'T DO THIS)
// ============================================================================

namespace unsafe {

int counter = 0;

void increment() {
    for (int i = 0; i < 10000; ++i) {
        ++counter;  // DATA RACE! Undefined behavior!
    }
}

void demo() {
    std::cout << "--- Unsafe (Data Race) ---\n";
    counter = 0;

    std::thread t1{increment};
    std::thread t2{increment};

    t1.join();
    t2.join();

    std::cout << std::format("Expected: 20000, Got: {} (likely wrong!)\n\n",
                             counter);
}

}  // namespace unsafe

// ============================================================================
// Using mutex
// ============================================================================

namespace with_mutex {

int counter = 0;
std::mutex mtx;

void increment() {
    for (int i = 0; i < 10000; ++i) {
        mtx.lock();
        ++counter;
        mtx.unlock();
    }
}

void demo() {
    std::cout << "--- With Mutex (Manual Lock/Unlock) ---\n";
    counter = 0;

    std::thread t1{increment};
    std::thread t2{increment};

    t1.join();
    t2.join();

    std::cout << std::format("Expected: 20000, Got: {} (correct!)\n\n", counter);
}

}  // namespace with_mutex

// ============================================================================
// Using lock_guard (RAII)
// ============================================================================

namespace with_lock_guard {

int counter = 0;
std::mutex mtx;

void increment() {
    for (int i = 0; i < 10000; ++i) {
        std::lock_guard<std::mutex> lock{mtx};  // RAII - unlocks automatically
        ++counter;
    }
}

void demo() {
    std::cout << "--- With lock_guard (RAII) ---\n";
    counter = 0;

    std::thread t1{increment};
    std::thread t2{increment};

    t1.join();
    t2.join();

    std::cout << std::format("Expected: 20000, Got: {} (correct!)\n\n", counter);
}

}  // namespace with_lock_guard

// ============================================================================
// Using unique_lock (more flexible)
// ============================================================================

namespace with_unique_lock {

int counter = 0;
std::mutex mtx;

void increment() {
    for (int i = 0; i < 10000; ++i) {
        std::unique_lock<std::mutex> lock{mtx};
        ++counter;
        // Can unlock early if needed
        lock.unlock();
        // Do other work...
        // Can relock if needed
        // lock.lock();
    }
}

void demo() {
    std::cout << "--- With unique_lock (Flexible) ---\n";
    counter = 0;

    std::thread t1{increment};
    std::thread t2{increment};

    t1.join();
    t2.join();

    std::cout << std::format("Expected: 20000, Got: {} (correct!)\n\n", counter);
}

}  // namespace with_unique_lock

// ============================================================================
// Multiple mutexes - avoiding deadlock with scoped_lock
// ============================================================================

namespace multiple_mutexes {

std::mutex m1, m2;
int resource1 = 0;
int resource2 = 0;

void transfer_bad_order1() {
    // DON'T DO THIS - can deadlock with transfer_bad_order2
    // std::lock_guard<std::mutex> lk1{m1};
    // std::lock_guard<std::mutex> lk2{m2};
}

void transfer_bad_order2() {
    // DON'T DO THIS - opposite order can deadlock
    // std::lock_guard<std::mutex> lk2{m2};
    // std::lock_guard<std::mutex> lk1{m1};
}

void transfer_safe(int amount) {
    // scoped_lock acquires both mutexes without deadlock
    std::scoped_lock lock{m1, m2};
    resource1 -= amount;
    resource2 += amount;
}

void demo() {
    std::cout << "--- Multiple Mutexes (scoped_lock) ---\n";
    resource1 = 1000;
    resource2 = 0;

    std::vector<std::thread> threads;
    for (int i = 0; i < 10; ++i) {
        threads.emplace_back(transfer_safe, 100);
    }

    for (auto& t : threads) {
        t.join();
    }

    std::cout << std::format("resource1: {}, resource2: {} (total should be 1000)\n\n",
                             resource1, resource2);
}

}  // namespace multiple_mutexes

// ============================================================================
// Shared mutex (multiple readers, single writer)
// ============================================================================

namespace shared_mutex_demo {

std::shared_mutex rw_mutex;
int shared_data = 0;

void reader(int id) {
    for (int i = 0; i < 5; ++i) {
        std::shared_lock<std::shared_mutex> lock{rw_mutex};
        std::cout << std::format("Reader {} sees: {}\n", id, shared_data);
        std::this_thread::sleep_for(10ms);
    }
}

void writer() {
    for (int i = 1; i <= 5; ++i) {
        std::unique_lock<std::shared_mutex> lock{rw_mutex};
        shared_data = i * 10;
        std::cout << std::format("Writer set: {}\n", shared_data);
        std::this_thread::sleep_for(20ms);
    }
}

void demo() {
    std::cout << "--- Shared Mutex (Readers/Writer) ---\n";
    shared_data = 0;

    std::thread w{writer};
    std::thread r1{reader, 1};
    std::thread r2{reader, 2};

    w.join();
    r1.join();
    r2.join();
    std::cout << '\n';
}

}  // namespace shared_mutex_demo

// ============================================================================
// Thread-safe class example
// ============================================================================

class ThreadSafeCounter {
public:
    void increment() {
        std::lock_guard<std::mutex> lock{mutex_};
        ++value_;
    }

    void decrement() {
        std::lock_guard<std::mutex> lock{mutex_};
        --value_;
    }

    int get() const {
        std::lock_guard<std::mutex> lock{mutex_};
        return value_;
    }

private:
    mutable std::mutex mutex_;  // mutable for const get()
    int value_ = 0;
};

void thread_safe_class_demo() {
    std::cout << "--- Thread-Safe Class ---\n";

    ThreadSafeCounter counter;

    auto increment_task = [&counter]() {
        for (int i = 0; i < 10000; ++i) {
            counter.increment();
        }
    };

    std::thread t1{increment_task};
    std::thread t2{increment_task};

    t1.join();
    t2.join();

    std::cout << std::format("Counter value: {} (expected: 20000)\n\n",
                             counter.get());
}

// ============================================================================
// Atomic operations (lock-free)
// ============================================================================

#include <atomic>

namespace atomic_demo {

std::atomic<int> counter{0};

void increment() {
    for (int i = 0; i < 10000; ++i) {
        ++counter;  // Atomic increment - no mutex needed!
    }
}

void demo() {
    std::cout << "--- Atomic Operations ---\n";
    counter = 0;

    std::thread t1{increment};
    std::thread t2{increment};

    t1.join();
    t2.join();

    std::cout << std::format("Atomic counter: {} (expected: 20000)\n\n",
                             counter.load());

    // Other atomic operations
    std::atomic<int> x{10};
    x.fetch_add(5);                              // Add and return old value
    x.fetch_sub(3);                              // Subtract and return old value
    x.exchange(100);                             // Set and return old value

    int expected = 100;
    x.compare_exchange_strong(expected, 200);    // CAS operation

    std::cout << std::format("After atomic ops: {}\n\n", x.load());
}

}  // namespace atomic_demo

int main() {
    std::cout << "=== Sharing Data Safely ===\n\n";

    unsafe::demo();
    with_mutex::demo();
    with_lock_guard::demo();
    with_unique_lock::demo();
    multiple_mutexes::demo();
    shared_mutex_demo::demo();
    thread_safe_class_demo();
    atomic_demo::demo();

    std::cout << "=== All demos completed ===\n";

    return 0;
}

// Key points:
// - Data races are undefined behavior
// - Use std::mutex to protect shared data
// - Prefer std::lock_guard for simple RAII locking
// - Use std::unique_lock when you need to unlock/relock
// - Use std::scoped_lock for multiple mutexes (avoids deadlock)
// - Use std::shared_mutex when readers >> writers
// - Use std::atomic for simple shared variables
