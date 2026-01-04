#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>
#include "thread_pool.h"
#include <atomic>
#include <chrono>
#include <set>

using namespace concurrent;
using namespace std::chrono_literals;

TEST_CASE("ThreadPool construction", "[thread_pool][construction]") {
    SECTION("default construction uses hardware concurrency") {
        ThreadPool pool;
        REQUIRE(pool.size() >= 1);
    }

    SECTION("construction with specified thread count") {
        ThreadPool pool(4);
        REQUIRE(pool.size() == 4);
    }

    SECTION("construction with zero threads creates one thread") {
        ThreadPool pool(0);
        REQUIRE(pool.size() == 1);
    }
}

TEST_CASE("ThreadPool basic task execution", "[thread_pool][execution]") {
    ThreadPool pool(2);

    SECTION("execute simple task") {
        auto future = pool.submit([] {
            return 42;
        });
        REQUIRE(future.get() == 42);
    }

    SECTION("execute task with arguments") {
        auto future = pool.submit([](int a, int b) {
            return a + b;
        }, 10, 20);
        REQUIRE(future.get() == 30);
    }

    SECTION("execute multiple tasks") {
        auto f1 = pool.submit([] { return 1; });
        auto f2 = pool.submit([] { return 2; });
        auto f3 = pool.submit([] { return 3; });

        REQUIRE(f1.get() + f2.get() + f3.get() == 6);
    }
}

TEST_CASE("ThreadPool different return types", "[thread_pool][types]") {
    ThreadPool pool(2);

    SECTION("void return type") {
        std::atomic<int> value{0};
        auto future = pool.submit([&value] {
            value = 42;
        });
        future.get();
        REQUIRE(value.load() == 42);
    }

    SECTION("string return type") {
        auto future = pool.submit([] {
            return std::string("hello");
        });
        REQUIRE(future.get() == "hello");
    }

    SECTION("double return type") {
        auto future = pool.submit([] {
            return 3.14159;
        });
        REQUIRE(future.get() == Catch::Approx(3.14159));
    }
}

TEST_CASE("ThreadPool concurrent execution", "[thread_pool][concurrent]") {
    ThreadPool pool(4);

    SECTION("tasks execute concurrently") {
        std::atomic<int> counter{0};
        std::vector<std::future<void>> futures;

        auto start = std::chrono::steady_clock::now();

        // Submit 4 tasks that each sleep for 100ms
        for (int i = 0; i < 4; ++i) {
            futures.push_back(pool.submit([&counter] {
                std::this_thread::sleep_for(100ms);
                ++counter;
            }));
        }

        // Wait for all
        for (auto& f : futures) {
            f.get();
        }

        auto end = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

        REQUIRE(counter.load() == 4);
        // If parallel, should take ~100ms, not 400ms
        REQUIRE(duration.count() < 300);
    }

    SECTION("tasks run on multiple threads") {
        std::mutex mtx;
        std::set<std::thread::id> thread_ids;
        std::vector<std::future<void>> futures;

        for (int i = 0; i < 8; ++i) {
            futures.push_back(pool.submit([&mtx, &thread_ids] {
                std::this_thread::sleep_for(50ms);
                std::lock_guard<std::mutex> lock(mtx);
                thread_ids.insert(std::this_thread::get_id());
            }));
        }

        for (auto& f : futures) {
            f.get();
        }

        // Should use multiple threads (at least 2)
        REQUIRE(thread_ids.size() >= 2);
    }
}

TEST_CASE("ThreadPool exception handling", "[thread_pool][exception]") {
    ThreadPool pool(2);

    SECTION("exception propagates through future") {
        auto future = pool.submit([] {
            throw std::runtime_error("test error");
            return 0;
        });

        REQUIRE_THROWS_AS(future.get(), std::runtime_error);
    }

    SECTION("exception message is preserved") {
        auto future = pool.submit([] {
            throw std::runtime_error("specific error message");
            return 0;
        });

        try {
            future.get();
            FAIL("Expected exception");
        } catch (const std::runtime_error& e) {
            REQUIRE(std::string(e.what()) == "specific error message");
        }
    }

    SECTION("pool continues after task exception") {
        auto bad_future = pool.submit([] {
            throw std::runtime_error("error");
            return 0;
        });

        auto good_future = pool.submit([] {
            return 42;
        });

        REQUIRE_THROWS(bad_future.get());
        REQUIRE(good_future.get() == 42);
    }
}

TEST_CASE("ThreadPool shutdown", "[thread_pool][shutdown]") {
    SECTION("graceful shutdown completes pending tasks") {
        std::atomic<int> completed{0};
        {
            ThreadPool pool(2);

            for (int i = 0; i < 10; ++i) {
                (void)pool.submit([&completed] {
                    std::this_thread::sleep_for(10ms);
                    ++completed;
                });
            }

            // Pool destructor will wait for all tasks
        }

        REQUIRE(completed.load() == 10);
    }

    SECTION("explicit shutdown") {
        ThreadPool pool(2);
        auto future = pool.submit([] { return 42; });

        pool.shutdown();

        REQUIRE(pool.stopped());
        REQUIRE(future.get() == 42);
    }

    SECTION("submit after shutdown throws") {
        ThreadPool pool(2);
        pool.shutdown();

        REQUIRE_THROWS_AS(pool.submit([] {}), std::runtime_error);
    }

    SECTION("multiple shutdown calls are safe") {
        ThreadPool pool(2);
        pool.shutdown();
        pool.shutdown();  // Should not crash
        pool.shutdown();  // Should not crash
    }
}

TEST_CASE("ThreadPool status methods", "[thread_pool][status]") {
    ThreadPool pool(4);

    SECTION("size returns correct thread count") {
        REQUIRE(pool.size() == 4);
    }

    SECTION("pending returns queue size") {
        REQUIRE(pool.pending() == 0);

        // Submit a long-running task to block workers
        std::atomic<bool> block{true};
        std::vector<std::future<void>> blocking_futures;
        for (size_t i = 0; i < pool.size(); ++i) {
            blocking_futures.push_back(pool.submit([&block] {
                while (block.load()) {
                    std::this_thread::sleep_for(1ms);
                }
            }));
        }

        // Give workers time to pick up tasks
        std::this_thread::sleep_for(10ms);

        // Now submit more tasks that will queue up
        std::vector<std::future<void>> queued_futures;
        for (int i = 0; i < 5; ++i) {
            queued_futures.push_back(pool.submit([] {}));
        }

        REQUIRE(pool.pending() >= 1);

        block = false;  // Unblock workers

        // Wait for all tasks to complete before block goes out of scope
        for (auto& f : blocking_futures) f.get();
        for (auto& f : queued_futures) f.get();
    }

    SECTION("stopped returns false initially") {
        REQUIRE_FALSE(pool.stopped());
    }

    SECTION("stopped returns true after shutdown") {
        pool.shutdown();
        REQUIRE(pool.stopped());
    }
}

TEST_CASE("ThreadPool with callable objects", "[thread_pool][callable]") {
    ThreadPool pool(2);

    SECTION("function pointer") {
        auto add = [](int a, int b) { return a + b; };
        auto future = pool.submit(add, 5, 3);
        REQUIRE(future.get() == 8);
    }

    SECTION("std::function") {
        std::function<int(int)> square = [](int x) { return x * x; };
        auto future = pool.submit(square, 7);
        REQUIRE(future.get() == 49);
    }

    SECTION("functor") {
        struct Multiplier {
            int factor;
            int operator()(int x) const { return x * factor; }
        };

        auto future = pool.submit(Multiplier{3}, 10);
        REQUIRE(future.get() == 30);
    }
}

TEST_CASE("ThreadPool stress test", "[thread_pool][stress]") {
    ThreadPool pool(8);
    std::atomic<int> counter{0};
    std::vector<std::future<void>> futures;

    const int num_tasks = 1000;

    for (int i = 0; i < num_tasks; ++i) {
        futures.push_back(pool.submit([&counter] {
            ++counter;
        }));
    }

    for (auto& f : futures) {
        f.get();
    }

    REQUIRE(counter.load() == num_tasks);
}

TEST_CASE("ThreadPool with move-only types", "[thread_pool][move]") {
    ThreadPool pool(2);

    SECTION("return move-only type") {
        auto future = pool.submit([] {
            return std::make_unique<int>(42);
        });

        auto result = future.get();
        REQUIRE(*result == 42);
    }

    SECTION("pass move-only argument") {
        auto future = pool.submit([](std::unique_ptr<int> ptr) {
            return *ptr * 2;
        }, std::make_unique<int>(21));

        REQUIRE(future.get() == 42);
    }
}

TEST_CASE("ThreadPool result ordering", "[thread_pool][ordering]") {
    ThreadPool pool(4);

    // Results should match their inputs regardless of execution order
    std::vector<std::future<int>> futures;

    for (int i = 0; i < 20; ++i) {
        futures.push_back(pool.submit([i] {
            // Use i-based delay to vary timing (avoids thread-unsafe rand())
            std::this_thread::sleep_for(std::chrono::milliseconds((i * 7) % 10));
            return i * i;
        }));
    }

    for (int i = 0; i < 20; ++i) {
        REQUIRE(futures[i].get() == i * i);
    }
}
