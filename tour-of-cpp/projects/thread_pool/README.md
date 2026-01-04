# Thread Pool

A simple thread pool implementation demonstrating concurrency, synchronization, and futures in modern C++.

## Learning Objectives

After completing this project, you will understand:

1. **Thread Management**
   - Creating and joining threads with `std::thread`
   - Thread lifecycle and proper cleanup
   - Determining hardware concurrency

2. **Synchronization Primitives**
   - Mutexes (`std::mutex`) for exclusive access
   - Lock guards (`std::unique_lock`, `std::lock_guard`)
   - Condition variables (`std::condition_variable`)

3. **Futures and Promises**
   - `std::future` for asynchronous results
   - `std::promise` for setting values
   - `std::packaged_task` for wrapping callables

4. **Task Queue Design**
   - Producer-consumer pattern
   - Thread-safe queue implementation
   - Graceful shutdown signaling

5. **Perfect Forwarding**
   - Using `std::forward` for argument forwarding
   - Universal references in templates
   - Preserving value categories

## Project Structure

```
thread_pool/
├── CMakeLists.txt          # Build configuration
├── README.md               # This file
├── thread_pool.h           # Thread pool implementation
├── main.cpp                # Demo program
└── tests/
    └── test_thread_pool.cpp # Catch2 unit tests
```

## Thread Pool Design

### Architecture

```
┌─────────────────────────────────────────────────────────┐
│                      Thread Pool                         │
├─────────────────────────────────────────────────────────┤
│                                                          │
│   ┌─────────┐  submit()   ┌──────────────┐              │
│   │  User   │ ──────────> │  Task Queue  │              │
│   │  Code   │             │  (mutex +    │              │
│   └─────────┘             │   condvar)   │              │
│       │                   └──────┬───────┘              │
│       │                          │                       │
│       │  returns                 │ tasks                 │
│       ▼                          ▼                       │
│   ┌─────────┐             ┌──────────────┐              │
│   │ future  │ <────────── │   Worker     │ x N threads  │
│   └─────────┘   result    │   Threads    │              │
│                           └──────────────┘              │
│                                                          │
└─────────────────────────────────────────────────────────┘
```

### Key Components

1. **Task Queue**: Thread-safe queue holding tasks (as `std::function<void()>`)
2. **Worker Threads**: Fixed number of threads waiting for and executing tasks
3. **Condition Variable**: For efficient waiting/notification
4. **Stop Flag**: Atomic flag for graceful shutdown

## Usage Example

```cpp
#include "thread_pool.h"

// Create pool with 4 threads
ThreadPool pool(4);

// Submit a task and get a future
auto future = pool.submit([] {
    return 42;
});

// Get the result (blocks until ready)
int result = future.get();

// Submit a task with arguments
auto future2 = pool.submit([](int a, int b) {
    return a + b;
}, 10, 20);
```

## Building

```bash
mkdir build && cd build
cmake ..
cmake --build .
```

## Running

```bash
# Run the demo
./thread_pool_demo

# Run tests
ctest --output-on-failure
```

## Key Concepts from "A Tour of C++"

This project applies concepts from:
- **Chapter 18**: Concurrency (threads, mutexes, condition variables)
- **Chapter 18**: Tasks and Futures (std::future, std::promise)
- **Chapter 8**: Function Objects (std::function, lambdas)

## Implementation Notes

### Thread Safety

The thread pool must be thread-safe:
- Multiple threads can call `submit()` concurrently
- Worker threads access the queue concurrently
- All shared state is protected by mutexes

### Graceful Shutdown

When the thread pool is destroyed:
1. Set stop flag to prevent new task acceptance
2. Wake all waiting workers
3. Wait for all workers to complete their current tasks
4. Join all worker threads

### Exception Handling

Exceptions thrown by tasks are captured in the future:
- Worker threads don't crash on task exceptions
- Exceptions propagate to the caller via `future.get()`

## Extension Ideas

- Add task priorities
- Add task cancellation support
- Add resize capability (add/remove workers)
- Add task stealing for load balancing
- Add timeout support for tasks
- Track task statistics (completed, pending, failed)
