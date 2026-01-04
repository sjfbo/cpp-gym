#include "thread_pool.h"
#include <iostream>
#include <chrono>
#include <numeric>
#include <cmath>
#include <iomanip>

using namespace concurrent;
using namespace std::chrono;
using namespace std::chrono_literals;

/**
 * Demonstrates the thread pool functionality.
 */

// Helper to get current thread ID as a string
std::string thread_id() {
    std::ostringstream ss;
    ss << std::this_thread::get_id();
    return ss.str();
}

int main() {
    std::cout << "=== Thread Pool Demo ===\n\n";
    std::cout << "Hardware concurrency: " << std::thread::hardware_concurrency() << " threads\n\n";

    // 1. Basic task submission
    std::cout << "1. Basic task submission:\n";
    {
        ThreadPool pool(4);
        std::cout << "   Created pool with " << pool.size() << " threads\n";

        auto future = pool.submit([] {
            return 42;
        });

        std::cout << "   Result: " << future.get() << "\n";
    }

    // 2. Task with arguments
    std::cout << "\n2. Task with arguments:\n";
    {
        ThreadPool pool(4);

        auto add = pool.submit([](int a, int b) {
            return a + b;
        }, 10, 20);

        auto multiply = pool.submit([](double x, double y) {
            return x * y;
        }, 3.14, 2.0);

        std::cout << "   10 + 20 = " << add.get() << "\n";
        std::cout << "   3.14 * 2.0 = " << multiply.get() << "\n";
    }

    // 3. Multiple concurrent tasks
    std::cout << "\n3. Multiple concurrent tasks:\n";
    {
        ThreadPool pool(4);
        std::vector<std::future<std::string>> futures;

        for (int i = 0; i < 8; ++i) {
            futures.push_back(pool.submit([i] {
                std::this_thread::sleep_for(100ms);
                return "Task " + std::to_string(i) + " completed on thread " + thread_id();
            }));
        }

        for (auto& f : futures) {
            std::cout << "   " << f.get() << "\n";
        }
    }

    // 4. Parallel computation
    std::cout << "\n4. Parallel sum computation:\n";
    {
        ThreadPool pool(4);

        // Create a large vector
        std::vector<int> data(1'000'000);
        std::iota(data.begin(), data.end(), 1);

        auto start = high_resolution_clock::now();

        // Split into chunks and sum in parallel
        const size_t chunk_size = data.size() / 4;
        std::vector<std::future<long long>> futures;

        for (size_t i = 0; i < 4; ++i) {
            size_t begin = i * chunk_size;
            size_t end = (i == 3) ? data.size() : (i + 1) * chunk_size;

            futures.push_back(pool.submit([&data, begin, end] {
                long long sum = 0;
                for (size_t j = begin; j < end; ++j) {
                    sum += data[j];
                }
                return sum;
            }));
        }

        long long total = 0;
        for (auto& f : futures) {
            total += f.get();
        }

        auto end_time = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(end_time - start);

        std::cout << "   Sum of 1 to 1,000,000 = " << total << "\n";
        std::cout << "   Time: " << duration.count() << " microseconds\n";
    }

    // 5. Exception handling
    std::cout << "\n5. Exception handling:\n";
    {
        ThreadPool pool(2);

        auto future = pool.submit([] {
            throw std::runtime_error("Something went wrong!");
            return 0;
        });

        try {
            future.get();
        } catch (const std::exception& e) {
            std::cout << "   Caught exception: " << e.what() << "\n";
        }
    }

    // 6. Void tasks
    std::cout << "\n6. Void tasks (side effects):\n";
    {
        ThreadPool pool(2);
        std::atomic<int> counter{0};

        std::vector<std::future<void>> futures;
        for (int i = 0; i < 10; ++i) {
            futures.push_back(pool.submit([&counter] {
                ++counter;
            }));
        }

        // Wait for all tasks
        for (auto& f : futures) {
            f.get();
        }

        std::cout << "   Counter after 10 increments: " << counter.load() << "\n";
    }

    // 7. Task dependencies (chaining)
    std::cout << "\n7. Task chaining:\n";
    {
        ThreadPool pool(4);

        // First task: compute base value
        auto future1 = pool.submit([] {
            std::this_thread::sleep_for(50ms);
            return 10;
        });

        // Get result and use it in next task
        int base = future1.get();

        auto future2 = pool.submit([base] {
            return base * 2;
        });

        auto future3 = pool.submit([base] {
            return base + 5;
        });

        std::cout << "   Base: " << base << "\n";
        std::cout << "   Base * 2: " << future2.get() << "\n";
        std::cout << "   Base + 5: " << future3.get() << "\n";
    }

    // 8. CPU-intensive work
    std::cout << "\n8. CPU-intensive parallel work:\n";
    {
        ThreadPool pool(4);

        auto is_prime = [](int n) {
            if (n < 2) return false;
            for (int i = 2; i <= std::sqrt(n); ++i) {
                if (n % i == 0) return false;
            }
            return true;
        };

        // Count primes in ranges
        auto count_primes = [&is_prime](int start, int end) {
            int count = 0;
            for (int i = start; i < end; ++i) {
                if (is_prime(i)) ++count;
            }
            return count;
        };

        auto start = high_resolution_clock::now();

        auto f1 = pool.submit(count_primes, 2, 25000);
        auto f2 = pool.submit(count_primes, 25000, 50000);
        auto f3 = pool.submit(count_primes, 50000, 75000);
        auto f4 = pool.submit(count_primes, 75000, 100000);

        int total_primes = f1.get() + f2.get() + f3.get() + f4.get();

        auto end_time = high_resolution_clock::now();
        auto duration = duration_cast<milliseconds>(end_time - start);

        std::cout << "   Primes from 2 to 100,000: " << total_primes << "\n";
        std::cout << "   Time: " << duration.count() << " ms\n";
    }

    // 9. Pool status
    std::cout << "\n9. Pool status:\n";
    {
        ThreadPool pool(4);

        std::cout << "   Pool size: " << pool.size() << "\n";
        std::cout << "   Pending tasks: " << pool.pending() << "\n";
        std::cout << "   Stopped: " << std::boolalpha << pool.stopped() << "\n";

        // Submit some tasks
        for (int i = 0; i < 10; ++i) {
            pool.submit([] {
                std::this_thread::sleep_for(200ms);
            });
        }

        std::this_thread::sleep_for(10ms);  // Let pool start processing
        std::cout << "   After submitting 10 tasks:\n";
        std::cout << "   Pending tasks: ~" << pool.pending() << " (approximate)\n";
    }

    // 10. Graceful shutdown
    std::cout << "\n10. Graceful shutdown:\n";
    {
        ThreadPool pool(2);
        std::atomic<int> completed{0};

        // Submit several long-running tasks
        for (int i = 0; i < 5; ++i) {
            pool.submit([&completed, i] {
                std::this_thread::sleep_for(100ms);
                ++completed;
                std::cout << "      Task " << i << " completed\n";
            });
        }

        std::cout << "   Shutting down pool...\n";
        pool.shutdown();  // Explicitly shutdown
        std::cout << "   Pool shutdown complete\n";
        std::cout << "   Tasks completed: " << completed.load() << "\n";
    }

    // 11. Submit after shutdown (should throw)
    std::cout << "\n11. Submit after shutdown:\n";
    {
        ThreadPool pool(2);
        pool.shutdown();

        try {
            pool.submit([] { return 42; });
        } catch (const std::runtime_error& e) {
            std::cout << "   Caught expected error: " << e.what() << "\n";
        }
    }

    std::cout << "\n=== Demo Complete ===\n";
    return 0;
}
