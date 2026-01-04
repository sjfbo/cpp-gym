// Chapter 18 Tests: Concurrency
// These tests verify understanding of C++ concurrency primitives.

#include <atomic>
#include <catch2/catch_test_macros.hpp>
#include <chrono>
#include <condition_variable>
#include <future>
#include <mutex>
#include <numeric>
#include <queue>
#include <thread>
#include <vector>

using namespace std::chrono_literals;

// ============================================================================
// Section 18.2: Threads
// ============================================================================

TEST_CASE("threads: basic creation and join", "[ch18][thread]") {
    bool executed = false;

    std::thread t{[&executed]() {
        executed = true;
    }};
    t.join();

    REQUIRE(executed);
}

TEST_CASE("threads: passing arguments", "[ch18][thread]") {
    int result = 0;

    std::thread t{[](int& r, int a, int b) {
        r = a + b;
    }, std::ref(result), 10, 20};
    t.join();

    REQUIRE(result == 30);
}

TEST_CASE("threads: hardware_concurrency", "[ch18][thread]") {
    unsigned int cores = std::thread::hardware_concurrency();
    // Should be at least 1 on any system
    REQUIRE(cores >= 1);
}

TEST_CASE("threads: multiple threads", "[ch18][thread]") {
    std::vector<int> results(4, 0);
    std::vector<std::thread> threads;

    for (int i = 0; i < 4; ++i) {
        threads.emplace_back([i, &results]() {
            results[i] = i * i;
        });
    }

    for (auto& t : threads) {
        t.join();
    }

    REQUIRE(results == std::vector<int>{0, 1, 4, 9});
}

TEST_CASE("threads: move semantics", "[ch18][thread]") {
    std::thread t1{[]() {}};
    REQUIRE(t1.joinable());

    std::thread t2 = std::move(t1);
    REQUIRE_FALSE(t1.joinable());
    REQUIRE(t2.joinable());

    t2.join();
}

// ============================================================================
// Section 18.3: Mutex
// ============================================================================

TEST_CASE("mutex: basic locking", "[ch18][mutex]") {
    std::mutex m;
    int counter = 0;

    auto increment = [&]() {
        for (int i = 0; i < 1000; ++i) {
            std::lock_guard<std::mutex> lock{m};
            ++counter;
        }
    };

    std::thread t1{increment};
    std::thread t2{increment};

    t1.join();
    t2.join();

    REQUIRE(counter == 2000);
}

TEST_CASE("mutex: lock_guard RAII", "[ch18][mutex]") {
    std::mutex m;

    {
        std::lock_guard<std::mutex> lock{m};
        // Mutex is locked here
    }
    // Mutex is automatically unlocked

    // Should be able to lock again
    REQUIRE(m.try_lock());
    m.unlock();
}

TEST_CASE("mutex: unique_lock flexibility", "[ch18][mutex]") {
    std::mutex m;

    std::unique_lock<std::mutex> lock{m};
    REQUIRE(lock.owns_lock());

    lock.unlock();
    REQUIRE_FALSE(lock.owns_lock());

    lock.lock();
    REQUIRE(lock.owns_lock());
}

TEST_CASE("mutex: scoped_lock for multiple mutexes", "[ch18][mutex]") {
    std::mutex m1, m2;
    int value1 = 100, value2 = 0;

    auto transfer = [&]() {
        for (int i = 0; i < 100; ++i) {
            std::scoped_lock lock{m1, m2};
            --value1;
            ++value2;
        }
    };

    std::thread t1{transfer};
    std::thread t2{[&]() {
        for (int i = 0; i < 100; ++i) {
            std::scoped_lock lock{m2, m1};  // Different order - still safe!
            // Just reading
        }
    }};

    t1.join();
    t2.join();

    REQUIRE(value1 + value2 == 100);
}

// ============================================================================
// Section 18.3: Atomic
// ============================================================================

TEST_CASE("atomic: basic operations", "[ch18][atomic]") {
    std::atomic<int> counter{0};

    auto increment = [&]() {
        for (int i = 0; i < 1000; ++i) {
            ++counter;
        }
    };

    std::thread t1{increment};
    std::thread t2{increment};

    t1.join();
    t2.join();

    REQUIRE(counter == 2000);
}

TEST_CASE("atomic: fetch operations", "[ch18][atomic]") {
    std::atomic<int> x{10};

    int old = x.fetch_add(5);
    REQUIRE(old == 10);
    REQUIRE(x == 15);

    old = x.fetch_sub(3);
    REQUIRE(old == 15);
    REQUIRE(x == 12);

    old = x.exchange(100);
    REQUIRE(old == 12);
    REQUIRE(x == 100);
}

TEST_CASE("atomic: compare_exchange", "[ch18][atomic]") {
    std::atomic<int> x{10};

    int expected = 10;
    bool success = x.compare_exchange_strong(expected, 20);
    REQUIRE(success);
    REQUIRE(x == 20);

    expected = 10;  // Wrong expected value
    success = x.compare_exchange_strong(expected, 30);
    REQUIRE_FALSE(success);
    REQUIRE(expected == 20);  // Updated to actual value
    REQUIRE(x == 20);         // Unchanged
}

// ============================================================================
// Section 18.4: Condition Variable
// ============================================================================

TEST_CASE("condition_variable: basic wait/notify", "[ch18][cv]") {
    std::mutex m;
    std::condition_variable cv;
    bool ready = false;
    int value = 0;

    std::thread producer{[&]() {
        std::this_thread::sleep_for(10ms);
        {
            std::lock_guard<std::mutex> lock{m};
            value = 42;
            ready = true;
        }
        cv.notify_one();
    }};

    std::thread consumer{[&]() {
        std::unique_lock<std::mutex> lock{m};
        cv.wait(lock, [&]{ return ready; });
        REQUIRE(value == 42);
    }};

    producer.join();
    consumer.join();
}

TEST_CASE("condition_variable: notify_all", "[ch18][cv]") {
    std::mutex m;
    std::condition_variable cv;
    bool ready = false;
    std::atomic<int> count{0};

    auto waiter = [&]() {
        std::unique_lock<std::mutex> lock{m};
        cv.wait(lock, [&]{ return ready; });
        ++count;
    };

    std::thread t1{waiter};
    std::thread t2{waiter};
    std::thread t3{waiter};

    std::this_thread::sleep_for(10ms);

    {
        std::lock_guard<std::mutex> lock{m};
        ready = true;
    }
    cv.notify_all();

    t1.join();
    t2.join();
    t3.join();

    REQUIRE(count == 3);
}

TEST_CASE("condition_variable: wait_for timeout", "[ch18][cv]") {
    std::mutex m;
    std::condition_variable cv;
    bool ready = false;

    std::unique_lock<std::mutex> lock{m};
    bool result = cv.wait_for(lock, 10ms, [&]{ return ready; });

    REQUIRE_FALSE(result);  // Timed out
}

// ============================================================================
// Section 18.5: Future and Promise
// ============================================================================

TEST_CASE("future: basic promise/future", "[ch18][future]") {
    std::promise<int> p;
    std::future<int> f = p.get_future();

    std::thread t{[&p]() {
        std::this_thread::sleep_for(10ms);
        p.set_value(42);
    }};

    int result = f.get();
    REQUIRE(result == 42);

    t.join();
}

TEST_CASE("future: exception through promise", "[ch18][future]") {
    std::promise<int> p;
    std::future<int> f = p.get_future();

    std::thread t{[&p]() {
        try {
            throw std::runtime_error("error");
        } catch (...) {
            p.set_exception(std::current_exception());
        }
    }};

    REQUIRE_THROWS_AS(f.get(), std::runtime_error);

    t.join();
}

TEST_CASE("future: std::async", "[ch18][future]") {
    auto f = std::async(std::launch::async, []() {
        std::this_thread::sleep_for(10ms);
        return 42;
    });

    REQUIRE(f.get() == 42);
}

TEST_CASE("future: async with arguments", "[ch18][future]") {
    auto f = std::async(std::launch::async, [](int a, int b) {
        return a * b;
    }, 6, 7);

    REQUIRE(f.get() == 42);
}

TEST_CASE("future: deferred execution", "[ch18][future]") {
    bool executed = false;
    auto f = std::async(std::launch::deferred, [&]() {
        executed = true;
        return 42;
    });

    // Not executed yet
    REQUIRE_FALSE(executed);

    // Executes when we call get()
    int result = f.get();
    REQUIRE(executed);
    REQUIRE(result == 42);
}

TEST_CASE("future: shared_future", "[ch18][future]") {
    std::promise<int> p;
    std::shared_future<int> sf = p.get_future().share();

    std::atomic<int> sum{0};

    std::thread t1{[sf, &sum]() { sum += sf.get(); }};
    std::thread t2{[sf, &sum]() { sum += sf.get(); }};
    std::thread t3{[sf, &sum]() { sum += sf.get(); }};

    p.set_value(10);

    t1.join();
    t2.join();
    t3.join();

    REQUIRE(sum == 30);
}

TEST_CASE("future: future status", "[ch18][future]") {
    auto f = std::async(std::launch::async, []() {
        std::this_thread::sleep_for(50ms);
        return 42;
    });

    auto status = f.wait_for(1ms);
    REQUIRE(status == std::future_status::timeout);

    f.wait();
    status = f.wait_for(0ms);
    REQUIRE(status == std::future_status::ready);
}

// ============================================================================
// Practical scenarios
// ============================================================================

TEST_CASE("practical: parallel accumulate", "[ch18][practical]") {
    std::vector<int> data(1000);
    std::iota(data.begin(), data.end(), 1);

    long long expected = 1000LL * 1001 / 2;

    auto partial_sum = [&data](size_t start, size_t end) {
        long long sum = 0;
        for (size_t i = start; i < end; ++i) {
            sum += data[i];
        }
        return sum;
    };

    size_t mid = data.size() / 2;
    auto f1 = std::async(std::launch::async, partial_sum, 0, mid);
    auto f2 = std::async(std::launch::async, partial_sum, mid, data.size());

    long long result = f1.get() + f2.get();
    REQUIRE(result == expected);
}

TEST_CASE("practical: producer-consumer", "[ch18][practical]") {
    std::mutex m;
    std::condition_variable cv;
    std::queue<int> q;
    bool done = false;

    std::atomic<int> sum{0};

    std::thread producer{[&]() {
        for (int i = 1; i <= 10; ++i) {
            {
                std::lock_guard<std::mutex> lock{m};
                q.push(i);
            }
            cv.notify_one();
        }
        {
            std::lock_guard<std::mutex> lock{m};
            done = true;
        }
        cv.notify_one();
    }};

    std::thread consumer{[&]() {
        while (true) {
            std::unique_lock<std::mutex> lock{m};
            cv.wait(lock, [&]{ return !q.empty() || done; });

            while (!q.empty()) {
                sum += q.front();
                q.pop();
            }

            if (done && q.empty()) break;
        }
    }};

    producer.join();
    consumer.join();

    REQUIRE(sum == 55);  // 1 + 2 + ... + 10
}
