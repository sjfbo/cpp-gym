# Chapter 18: Concurrency

## TL;DR

C++ provides robust concurrency primitives: `std::thread` for parallel execution, `std::mutex` for mutual exclusion, `std::condition_variable` for synchronization, and `std::future`/`std::promise` for async results. Always protect shared data with mutexes, prefer RAII locks (`lock_guard`, `unique_lock`), and use `std::async` for simple parallelism. Data races are undefined behavior - avoid them at all costs.

## Key Concepts

| Concept | One-liner | Example |
|---------|-----------|---------|
| `std::thread` | Represents a thread of execution | `std::thread t{func};` |
| `std::mutex` | Mutual exclusion lock | `std::mutex m; m.lock();` |
| `std::lock_guard` | RAII mutex wrapper | `std::lock_guard<std::mutex> lk{m};` |
| `std::unique_lock` | Flexible mutex wrapper | `std::unique_lock<std::mutex> lk{m};` |
| `std::condition_variable` | Thread synchronization | `cv.wait(lk, pred);` |
| `std::future` | Holds result from async operation | `auto f = std::async(func);` |
| `std::promise` | Sets value for a future | `p.set_value(42);` |
| `std::atomic` | Lock-free atomic operations | `std::atomic<int> counter{0};` |
| Data race | UB from unsynchronized access | Two threads writing same variable |

## Things to Remember

- **Always join or detach**: A `std::thread` must be joined or detached before destruction.
- **RAII for locks**: Use `lock_guard` or `unique_lock`, never raw `lock()`/`unlock()`.
- **Protect shared data**: Any data accessed by multiple threads needs synchronization.
- **Condition variables need predicates**: Always use `wait(lock, predicate)` to avoid spurious wakeups.
- **`std::async` is simple**: Use it for fire-and-forget parallelism.
- **Data races are UB**: Even "benign" races are undefined behavior in C++.
- **Prefer higher-level abstractions**: Use `std::async`, thread pools over raw threads when possible.

## Modern C++ Highlights

| Feature | Old Style | Modern C++ |
|---------|-----------|------------|
| Thread creation | OS-specific APIs | `std::thread` |
| Mutex locking | Manual lock/unlock | `std::lock_guard`, `std::scoped_lock` |
| Async tasks | Roll your own | `std::async`, `std::future` |
| Atomic flag | Volatile (wrong!) | `std::atomic<bool>` |
| Multiple locks | Manual deadlock avoidance | `std::scoped_lock(m1, m2)` |

## Common Pitfalls

- **Forgetting to join/detach**:
  ```cpp
  void bad() {
      std::thread t{work};
      // t destructor called without join/detach -> std::terminate!
  }

  void good() {
      std::thread t{work};
      t.join();  // Or t.detach()
  }
  ```

- **Data races**:
  ```cpp
  int counter = 0;
  void increment() { ++counter; }  // Data race if called from multiple threads!

  std::atomic<int> safe_counter{0};
  void safe_increment() { ++safe_counter; }  // Safe
  ```

- **Deadlock from lock ordering**:
  ```cpp
  // Thread 1: lock(m1); lock(m2);
  // Thread 2: lock(m2); lock(m1);  // Deadlock possible!

  // Solution: use std::scoped_lock
  std::scoped_lock lk{m1, m2};  // Locks both safely
  ```

- **Spurious wakeups**:
  ```cpp
  // BAD: May wake up when condition isn't true
  cv.wait(lk);

  // GOOD: Loops until predicate is true
  cv.wait(lk, []{ return ready; });
  ```

- **Passing references to threads**:
  ```cpp
  void process(int& x) { x = 42; }
  int value = 0;

  // BAD: Copies value, doesn't modify original
  std::thread t{process, value};

  // GOOD: Use std::ref for references
  std::thread t{process, std::ref(value)};
  ```

## Quick Reference

```cpp
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <atomic>

// --- Threads ---
std::thread t{function};           // Create thread
std::thread t{function, arg1, arg2};  // With arguments
t.join();                          // Wait for completion
t.detach();                        // Let run independently
t.joinable();                      // Check if can join
std::this_thread::sleep_for(100ms);  // Sleep
std::this_thread::yield();         // Hint to reschedule
auto id = std::this_thread::get_id();  // Thread ID
auto n = std::thread::hardware_concurrency();  // CPU count

// --- Mutex ---
std::mutex m;
{
    std::lock_guard<std::mutex> lk{m};  // RAII lock
    // Critical section
}  // Automatically unlocked

std::unique_lock<std::mutex> lk{m};  // More flexible
lk.unlock();                       // Can unlock early
lk.lock();                         // Can relock

std::scoped_lock lk{m1, m2, m3};   // Lock multiple (C++17)

// --- Condition Variable ---
std::condition_variable cv;
std::mutex m;
bool ready = false;

// Waiting thread:
std::unique_lock<std::mutex> lk{m};
cv.wait(lk, []{ return ready; });  // Wait with predicate
// or: cv.wait_for(lk, 100ms, pred);
// or: cv.wait_until(lk, time_point, pred);

// Signaling thread:
{
    std::lock_guard<std::mutex> lk{m};
    ready = true;
}
cv.notify_one();                   // Wake one waiter
cv.notify_all();                   // Wake all waiters

// --- Future/Promise ---
std::promise<int> p;
std::future<int> f = p.get_future();

// In producer thread:
p.set_value(42);

// In consumer thread:
int result = f.get();              // Blocks until ready

// --- Async ---
auto f = std::async(function, args...);         // May run in new thread
auto f = std::async(std::launch::async, func);  // Must run in new thread
auto f = std::async(std::launch::deferred, func);  // Run when get() called
int result = f.get();              // Get result

// --- Atomics ---
std::atomic<int> counter{0};
counter++;                         // Atomic increment
counter.fetch_add(5);              // Atomic add, return old value
counter.store(10);                 // Atomic store
int val = counter.load();          // Atomic load
bool was_zero = counter.compare_exchange_strong(expected, desired);

std::atomic_flag flag = ATOMIC_FLAG_INIT;
flag.test_and_set();               // Set and return old value
flag.clear();                      // Clear
```

## Thread Safety Guidelines

```
1. Identify shared data
   - What data can be accessed by multiple threads?

2. Choose protection mechanism
   - std::atomic for simple counters/flags
   - std::mutex for complex data structures
   - std::shared_mutex for read-heavy workloads

3. Minimize critical sections
   - Lock for shortest time possible
   - Do computation outside the lock

4. Avoid deadlocks
   - Always lock in consistent order
   - Use std::scoped_lock for multiple mutexes
   - Don't call unknown code while holding locks

5. Consider lock-free alternatives
   - Immutable data needs no synchronization
   - Thread-local storage (thread_local)
   - Message passing instead of shared state
```

## Book Sections Covered

- **18.1** Introduction
- **18.2** Tasks and Threads
- **18.3** Sharing Data
- **18.4** Waiting for Events
- **18.5** Communicating Tasks
- **18.6** Coroutines (brief mention)

## Exercises in This Chapter

| Exercise | Difficulty | Concepts Practiced |
|----------|------------|-------------------|
| ex01_threads | ** | Thread creation, joining, passing arguments |
| ex02_producer_consumer | *** | Mutex, condition variable, thread coordination |

## Running the Examples

```bash
# Build all chapter 18 examples
cmake --build build --target ch18_examples

# Run individual examples
./build/bin/ch18_threads
./build/bin/ch18_sharing_data
./build/bin/ch18_condition_variables
./build/bin/ch18_futures_promises

# Run chapter 18 tests
ctest --test-dir build -R ch18
```
