// Parallel Algorithms - Execution Policies (C++17/20)
// Book reference: 13.6 Parallel Algorithms
//
// C++17 introduced execution policies to enable parallel execution
// of standard algorithms. Simply add an execution policy as the
// first argument to supported algorithms.

#include <algorithm>
#include <chrono>
#include <cmath>
#include <execution>
#include <iostream>
#include <numeric>
#include <random>
#include <vector>

// Timer helper
class Timer {
    using Clock = std::chrono::high_resolution_clock;
    Clock::time_point start_;
    std::string name_;

public:
    explicit Timer(std::string name) : start_{Clock::now()}, name_{std::move(name)} {}
    ~Timer() {
        auto end = Clock::now();
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start_).count();
        std::cout << name_ << ": " << ms << " ms\n";
    }
};

// Expensive computation for demonstration
double expensive_computation(double x) {
    // Simulate work with some math operations
    double result = x;
    for (int i = 0; i < 100; ++i) {
        result = std::sin(result) * std::cos(result) + std::sqrt(std::abs(result) + 1.0);
    }
    return result;
}

int main() {
    std::cout << "=== Parallel Algorithms ===\n\n";

    // -------------------------------------------------------------------------
    // Execution policies overview
    // -------------------------------------------------------------------------
    std::cout << "--- Execution Policies ---\n";
    std::cout << "std::execution::seq       - Sequential (default behavior)\n";
    std::cout << "std::execution::par       - Parallel (may use multiple threads)\n";
    std::cout << "std::execution::par_unseq - Parallel and vectorized\n";
    std::cout << "std::execution::unseq     - Vectorized only (C++20)\n\n";

    // -------------------------------------------------------------------------
    // Generate test data
    // -------------------------------------------------------------------------
    const std::size_t N = 10'000'000;  // 10 million elements
    std::cout << "Working with " << N << " elements\n\n";

    std::vector<double> data(N);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dist(0.0, 100.0);
    std::generate(data.begin(), data.end(), [&]() { return dist(gen); });

    // -------------------------------------------------------------------------
    // std::sort comparison
    // -------------------------------------------------------------------------
    std::cout << "--- std::sort Comparison ---\n";

    {
        std::vector<double> v = data;
        Timer t("Sequential sort");
        std::sort(std::execution::seq, v.begin(), v.end());
    }

    {
        std::vector<double> v = data;
        Timer t("Parallel sort   ");
        std::sort(std::execution::par, v.begin(), v.end());
    }

    {
        std::vector<double> v = data;
        Timer t("Par + Vectorized");
        std::sort(std::execution::par_unseq, v.begin(), v.end());
    }

    // -------------------------------------------------------------------------
    // std::for_each comparison
    // -------------------------------------------------------------------------
    std::cout << "\n--- std::for_each Comparison ---\n";

    std::vector<double> results1(N);
    std::vector<double> results2(N);
    std::vector<double> results3(N);

    {
        Timer t("Sequential for_each");
        std::transform(std::execution::seq,
            data.begin(), data.end(), results1.begin(),
            [](double x) { return expensive_computation(x); });
    }

    {
        Timer t("Parallel for_each   ");
        std::transform(std::execution::par,
            data.begin(), data.end(), results2.begin(),
            [](double x) { return expensive_computation(x); });
    }

    {
        Timer t("Par_unseq for_each  ");
        std::transform(std::execution::par_unseq,
            data.begin(), data.end(), results3.begin(),
            [](double x) { return expensive_computation(x); });
    }

    // Verify results are the same
    bool same = std::equal(results1.begin(), results1.end(), results2.begin());
    std::cout << "Results match: " << (same ? "yes" : "no") << "\n";

    // -------------------------------------------------------------------------
    // std::reduce (parallel-safe accumulate)
    // -------------------------------------------------------------------------
    std::cout << "\n--- std::reduce (Parallel Accumulate) ---\n";

    {
        Timer t("Sequential reduce");
        auto sum = std::reduce(std::execution::seq,
            data.begin(), data.end(), 0.0);
        std::cout << "  Sum: " << sum << "\n";
    }

    {
        Timer t("Parallel reduce  ");
        auto sum = std::reduce(std::execution::par,
            data.begin(), data.end(), 0.0);
        std::cout << "  Sum: " << sum << "\n";
    }

    // Note: std::accumulate is NOT parallelizable because it's strictly left-to-right
    // std::reduce allows reordering, enabling parallelization

    // -------------------------------------------------------------------------
    // std::transform_reduce (parallel inner product)
    // -------------------------------------------------------------------------
    std::cout << "\n--- std::transform_reduce ---\n";

    std::vector<double> v1(N, 1.0);
    std::vector<double> v2(N, 2.0);

    {
        Timer t("Sequential transform_reduce");
        auto dot = std::transform_reduce(std::execution::seq,
            v1.begin(), v1.end(), v2.begin(), 0.0);
        std::cout << "  Dot product: " << dot << "\n";
    }

    {
        Timer t("Parallel transform_reduce  ");
        auto dot = std::transform_reduce(std::execution::par,
            v1.begin(), v1.end(), v2.begin(), 0.0);
        std::cout << "  Dot product: " << dot << "\n";
    }

    // -------------------------------------------------------------------------
    // std::find with parallel execution
    // -------------------------------------------------------------------------
    std::cout << "\n--- std::find Comparison ---\n";

    // Place a needle near the end
    std::vector<double> haystack(N, 1.0);
    haystack[N - 1000] = -1.0;  // Needle near end

    {
        Timer t("Sequential find");
        auto it = std::find(std::execution::seq,
            haystack.begin(), haystack.end(), -1.0);
        std::cout << "  Found at index: " << std::distance(haystack.begin(), it) << "\n";
    }

    {
        Timer t("Parallel find  ");
        auto it = std::find(std::execution::par,
            haystack.begin(), haystack.end(), -1.0);
        std::cout << "  Found at index: " << std::distance(haystack.begin(), it) << "\n";
    }

    // -------------------------------------------------------------------------
    // std::count_if comparison
    // -------------------------------------------------------------------------
    std::cout << "\n--- std::count_if Comparison ---\n";

    {
        Timer t("Sequential count_if");
        auto count = std::count_if(std::execution::seq,
            data.begin(), data.end(), [](double x) { return x > 50.0; });
        std::cout << "  Count > 50: " << count << "\n";
    }

    {
        Timer t("Parallel count_if  ");
        auto count = std::count_if(std::execution::par,
            data.begin(), data.end(), [](double x) { return x > 50.0; });
        std::cout << "  Count > 50: " << count << "\n";
    }

    // -------------------------------------------------------------------------
    // Guidelines for using parallel algorithms
    // -------------------------------------------------------------------------
    std::cout << "\n--- Guidelines ---\n";
    std::cout << "Use parallel execution when:\n";
    std::cout << "  - Working with large datasets (10k+ elements)\n";
    std::cout << "  - Operations are computationally expensive\n";
    std::cout << "  - Operations are independent (no data races)\n";
    std::cout << "  - Memory access patterns allow parallelism\n\n";

    std::cout << "Avoid parallel execution when:\n";
    std::cout << "  - Small datasets (overhead > benefit)\n";
    std::cout << "  - Operations have side effects or shared state\n";
    std::cout << "  - Sequential dependencies exist\n";
    std::cout << "  - Memory bandwidth is the bottleneck\n\n";

    // -------------------------------------------------------------------------
    // Thread safety considerations
    // -------------------------------------------------------------------------
    std::cout << "--- Thread Safety ---\n";
    std::cout << "With parallel execution:\n";
    std::cout << "  - Function objects must be thread-safe\n";
    std::cout << "  - Avoid modifying shared state\n";
    std::cout << "  - Use atomic operations if needed\n";
    std::cout << "  - Prefer stateless lambdas\n";

    // Example of safe vs unsafe parallel execution
    std::cout << "\nSafe: stateless operation\n";
    std::vector<int> safe_input{1, 2, 3, 4, 5};
    std::vector<int> safe_output(5);
    std::transform(std::execution::par,
        safe_input.begin(), safe_input.end(),
        safe_output.begin(),
        [](int x) { return x * 2; });  // No shared state - safe!

    std::cout << "Result: ";
    for (int x : safe_output) std::cout << x << " ";
    std::cout << "\n";

    // Unsafe example (don't do this!):
    // int shared_counter = 0;
    // std::for_each(std::execution::par, v.begin(), v.end(),
    //     [&shared_counter](int x) { ++shared_counter; });  // DATA RACE!

    return 0;
}

// Key takeaways:
// - Add execution policy as first argument: std::sort(std::execution::par, ...)
// - std::execution::seq - sequential (baseline)
// - std::execution::par - parallel threads
// - std::execution::par_unseq - parallel + vectorized
// - std::reduce replaces std::accumulate for parallel reduction
// - Ensure thread safety: avoid shared mutable state
// - Parallel overhead exists - only use for large datasets or expensive operations
// - Not all compilers/platforms support all execution policies
