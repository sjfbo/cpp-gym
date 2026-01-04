// Futures and Promises - Async Results
// Book reference: 18.5 Communicating Tasks
//
// std::future and std::promise provide a way to transfer a value
// between threads, and std::async provides easy task-based parallelism.

#include <chrono>
#include <format>
#include <functional>
#include <future>
#include <iostream>
#include <numeric>
#include <stdexcept>
#include <thread>
#include <vector>

using namespace std::chrono_literals;

// ============================================================================
// Basic future/promise
// ============================================================================

namespace basic_future {

void demo() {
    std::cout << "--- Basic Future/Promise ---\n";

    std::promise<int> promise;
    std::future<int> future = promise.get_future();

    // Producer thread sets the value
    std::thread producer{[&promise]() {
        std::this_thread::sleep_for(100ms);
        promise.set_value(42);
        std::cout << "Producer: value set\n";
    }};

    // Consumer gets the value (blocks until available)
    std::cout << "Consumer: waiting for value...\n";
    int result = future.get();
    std::cout << std::format("Consumer: got {}\n", result);

    producer.join();
    std::cout << '\n';
}

}  // namespace basic_future

// ============================================================================
// Exception through promise
// ============================================================================

namespace exception_demo {

void demo() {
    std::cout << "--- Exception via Promise ---\n";

    std::promise<int> promise;
    std::future<int> future = promise.get_future();

    std::thread producer{[&promise]() {
        try {
            // Simulate work that fails
            throw std::runtime_error("Something went wrong!");
        } catch (...) {
            promise.set_exception(std::current_exception());
        }
    }};

    try {
        int result = future.get();  // Will throw
        std::cout << std::format("Got: {}\n", result);
    } catch (const std::exception& e) {
        std::cout << std::format("Caught exception: {}\n", e.what());
    }

    producer.join();
    std::cout << '\n';
}

}  // namespace exception_demo

// ============================================================================
// std::async - simple async execution
// ============================================================================

namespace async_demo {

int compute(int x) {
    std::this_thread::sleep_for(100ms);
    return x * x;
}

void demo() {
    std::cout << "--- std::async ---\n";

    // Launch async task (may run in new thread or deferred)
    std::future<int> f1 = std::async(compute, 5);

    // Force new thread
    std::future<int> f2 = std::async(std::launch::async, compute, 10);

    // Deferred - runs when get() is called
    std::future<int> f3 = std::async(std::launch::deferred, compute, 15);

    std::cout << "Tasks launched, doing other work...\n";

    std::cout << std::format("f1.get() = {}\n", f1.get());
    std::cout << std::format("f2.get() = {}\n", f2.get());
    std::cout << "f3 is deferred, will run now when we call get()...\n";
    std::cout << std::format("f3.get() = {}\n", f3.get());
    std::cout << '\n';
}

}  // namespace async_demo

// ============================================================================
// Parallel computation with async
// ============================================================================

namespace parallel_sum {

long long sum_range(const std::vector<int>& v, size_t start, size_t end) {
    long long result = 0;
    for (size_t i = start; i < end; ++i) {
        result += v[i];
    }
    return result;
}

void demo() {
    std::cout << "--- Parallel Sum ---\n";

    std::vector<int> data(1000000);
    std::iota(data.begin(), data.end(), 1);  // 1, 2, 3, ..., 1000000

    auto start = std::chrono::high_resolution_clock::now();

    // Sequential sum
    long long seq_sum = std::accumulate(data.begin(), data.end(), 0LL);

    auto seq_time = std::chrono::high_resolution_clock::now() - start;
    std::cout << std::format("Sequential sum: {} in {}us\n",
        seq_sum,
        std::chrono::duration_cast<std::chrono::microseconds>(seq_time).count());

    // Parallel sum using async
    start = std::chrono::high_resolution_clock::now();

    size_t mid = data.size() / 2;
    std::future<long long> f1 = std::async(std::launch::async,
        sum_range, std::cref(data), 0, mid);
    std::future<long long> f2 = std::async(std::launch::async,
        sum_range, std::cref(data), mid, data.size());

    long long par_sum = f1.get() + f2.get();

    auto par_time = std::chrono::high_resolution_clock::now() - start;
    std::cout << std::format("Parallel sum: {} in {}us\n",
        par_sum,
        std::chrono::duration_cast<std::chrono::microseconds>(par_time).count());

    std::cout << '\n';
}

}  // namespace parallel_sum

// ============================================================================
// Future status checking
// ============================================================================

namespace status_demo {

void demo() {
    std::cout << "--- Future Status ---\n";

    std::future<int> future = std::async(std::launch::async, []() {
        std::this_thread::sleep_for(200ms);
        return 42;
    });

    // Check status without blocking
    while (true) {
        auto status = future.wait_for(50ms);
        if (status == std::future_status::ready) {
            std::cout << "Future is ready!\n";
            break;
        } else if (status == std::future_status::timeout) {
            std::cout << "Still waiting...\n";
        } else if (status == std::future_status::deferred) {
            std::cout << "Task is deferred\n";
            break;
        }
    }

    std::cout << std::format("Result: {}\n\n", future.get());
}

}  // namespace status_demo

// ============================================================================
// std::packaged_task
// ============================================================================

namespace packaged_task_demo {

void demo() {
    std::cout << "--- std::packaged_task ---\n";

    // Wrap a function in a packaged_task
    std::packaged_task<int(int, int)> task{[](int a, int b) {
        std::this_thread::sleep_for(50ms);
        return a + b;
    }};

    std::future<int> future = task.get_future();

    // Run the task in another thread
    std::thread t{std::move(task), 10, 20};

    std::cout << std::format("Result: {}\n", future.get());

    t.join();
    std::cout << '\n';
}

}  // namespace packaged_task_demo

// ============================================================================
// Multiple futures
// ============================================================================

namespace multiple_futures {

void demo() {
    std::cout << "--- Multiple Futures ---\n";

    std::vector<std::future<int>> futures;

    // Launch multiple async tasks
    for (int i = 0; i < 5; ++i) {
        futures.push_back(std::async(std::launch::async, [i]() {
            std::this_thread::sleep_for(std::chrono::milliseconds(i * 50));
            return i * i;
        }));
    }

    // Collect results
    std::cout << "Results: ";
    for (auto& f : futures) {
        std::cout << f.get() << ' ';
    }
    std::cout << "\n\n";
}

}  // namespace multiple_futures

// ============================================================================
// std::shared_future - multiple consumers
// ============================================================================

namespace shared_future_demo {

void demo() {
    std::cout << "--- std::shared_future ---\n";

    std::promise<int> promise;
    std::shared_future<int> shared_future = promise.get_future().share();

    // Multiple consumers can wait on the same future
    std::thread t1{[shared_future]() {
        std::cout << std::format("Thread 1 got: {}\n", shared_future.get());
    }};

    std::thread t2{[shared_future]() {
        std::cout << std::format("Thread 2 got: {}\n", shared_future.get());
    }};

    std::thread t3{[shared_future]() {
        std::cout << std::format("Thread 3 got: {}\n", shared_future.get());
    }};

    std::this_thread::sleep_for(50ms);
    promise.set_value(42);

    t1.join();
    t2.join();
    t3.join();
    std::cout << '\n';
}

}  // namespace shared_future_demo

// ============================================================================
// Real-world example: Async file processing
// ============================================================================

namespace file_processing {

std::string fetch_url(const std::string& url) {
    std::this_thread::sleep_for(100ms);  // Simulate network delay
    return "Content from " + url;
}

std::string process_content(const std::string& content) {
    std::this_thread::sleep_for(50ms);  // Simulate processing
    return "Processed: " + content;
}

void demo() {
    std::cout << "--- Async Pipeline Example ---\n";

    std::vector<std::string> urls{"url1", "url2", "url3"};

    // Fetch all URLs in parallel
    std::vector<std::future<std::string>> fetch_futures;
    for (const auto& url : urls) {
        fetch_futures.push_back(std::async(std::launch::async, fetch_url, url));
    }

    // Process results as they come in
    for (size_t i = 0; i < fetch_futures.size(); ++i) {
        std::string content = fetch_futures[i].get();
        std::cout << std::format("Fetched: {}\n", content);

        // Could chain another async here for processing
        auto processed = std::async(std::launch::async, process_content, content);
        std::cout << std::format("{}\n", processed.get());
    }
    std::cout << '\n';
}

}  // namespace file_processing

int main() {
    std::cout << "=== Futures and Promises ===\n\n";

    basic_future::demo();
    exception_demo::demo();
    async_demo::demo();
    parallel_sum::demo();
    status_demo::demo();
    packaged_task_demo::demo();
    multiple_futures::demo();
    shared_future_demo::demo();
    file_processing::demo();

    std::cout << "=== All demos completed ===\n";

    return 0;
}

// Key points:
// - std::promise sets a value, std::future retrieves it
// - std::async is the easiest way to run code asynchronously
// - future.get() blocks until the result is available
// - Exceptions can be passed through promise/future
// - shared_future allows multiple consumers
// - packaged_task wraps a callable for deferred execution
// - Use std::launch::async to force a new thread
