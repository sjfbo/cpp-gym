// Standard Library Tour - Overview of Major Components
// Book reference: 9.2 Standard-Library Components
//
// This example provides a tour of the C++ standard library,
// demonstrating key components from each major category.

#include <algorithm>
#include <chrono>
#include <format>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <numeric>
#include <optional>
#include <random>
#include <ranges>
#include <set>
#include <span>
#include <sstream>
#include <string>
#include <string_view>
#include <tuple>
#include <variant>
#include <vector>

// ============================================================================
// Containers - Store collections of objects
// ============================================================================
void demonstrate_containers() {
    std::cout << "=== Containers ===\n\n";

    // Sequence containers
    std::vector<int> vec = {5, 2, 8, 1, 9};
    std::cout << "vector: ";
    for (int x : vec) std::cout << x << " ";
    std::cout << "\n";

    // Associative containers
    std::map<std::string, int> ages = {
        {"Alice", 30},
        {"Bob", 25},
        {"Charlie", 35}
    };
    std::cout << "map:\n";
    for (const auto& [name, age] : ages) {
        std::cout << std::format("  {} is {} years old\n", name, age);
    }

    // Set - unique elements, automatically sorted
    std::set<int> unique_nums = {3, 1, 4, 1, 5, 9, 2, 6};
    std::cout << "set (duplicates removed, sorted): ";
    for (int x : unique_nums) std::cout << x << " ";
    std::cout << "\n\n";
}

// ============================================================================
// Algorithms - Generic operations on ranges
// ============================================================================
void demonstrate_algorithms() {
    std::cout << "=== Algorithms ===\n\n";

    std::vector<int> vec = {5, 2, 8, 1, 9, 3, 7, 4, 6};

    // Sorting
    std::ranges::sort(vec);
    std::cout << "sorted: ";
    for (int x : vec) std::cout << x << " ";
    std::cout << "\n";

    // Finding
    auto it = std::ranges::find(vec, 5);
    if (it != vec.end()) {
        std::cout << std::format("found 5 at index {}\n", it - vec.begin());
    }

    // Counting
    auto count = std::ranges::count_if(vec, [](int x) { return x > 5; });
    std::cout << std::format("numbers > 5: {}\n", count);

    // Transforming
    std::vector<int> squared;
    std::ranges::transform(vec, std::back_inserter(squared),
                           [](int x) { return x * x; });
    std::cout << "squared: ";
    for (int x : squared) std::cout << x << " ";
    std::cout << "\n";

    // Accumulating (numeric)
    int sum = std::accumulate(vec.begin(), vec.end(), 0);
    std::cout << std::format("sum: {}\n\n", sum);
}

// ============================================================================
// Strings - Text handling
// ============================================================================
void demonstrate_strings() {
    std::cout << "=== Strings ===\n\n";

    std::string str = "Hello, C++ World!";
    std::cout << std::format("string: '{}'\n", str);
    std::cout << std::format("length: {}\n", str.length());
    std::cout << std::format("substring: '{}'\n", str.substr(7, 3));

    // string_view - non-owning reference (no allocation)
    std::string_view sv = str;
    std::cout << std::format("string_view: '{}'\n", sv);

    // Find and replace
    std::string modified = str;
    if (auto pos = modified.find("World"); pos != std::string::npos) {
        modified.replace(pos, 5, "Standard Library");
    }
    std::cout << std::format("modified: '{}'\n\n", modified);
}

// ============================================================================
// I/O Streams - Input/output operations
// ============================================================================
void demonstrate_io() {
    std::cout << "=== I/O Streams ===\n\n";

    // String streams for in-memory I/O
    std::ostringstream oss;
    oss << "The answer is " << 42 << " and pi is " << 3.14159;
    std::string result = oss.str();
    std::cout << std::format("ostringstream: '{}'\n", result);

    // Parsing with istringstream
    std::istringstream iss("10 20 30 40 50");
    std::vector<int> numbers;
    int num;
    while (iss >> num) {
        numbers.push_back(num);
    }
    std::cout << "parsed from istringstream: ";
    for (int x : numbers) std::cout << x << " ";
    std::cout << "\n";

    // std::format (C++20)
    auto formatted = std::format("Name: {:>10}, Age: {:03}", "Alice", 7);
    std::cout << std::format("std::format: '{}'\n\n", formatted);
}

// ============================================================================
// Utilities - General-purpose tools
// ============================================================================
void demonstrate_utilities() {
    std::cout << "=== Utilities ===\n\n";

    // std::pair
    std::pair<std::string, int> p = {"Alice", 30};
    auto [name, age] = p;  // structured binding
    std::cout << std::format("pair: ({}, {})\n", name, age);

    // std::tuple
    std::tuple<std::string, int, double> t = {"Bob", 25, 1.75};
    auto [n, a, h] = t;
    std::cout << std::format("tuple: ({}, {}, {})\n", n, a, h);

    // std::optional - may or may not contain a value
    auto find_even = [](const std::vector<int>& v) -> std::optional<int> {
        for (int x : v) {
            if (x % 2 == 0) return x;
        }
        return std::nullopt;
    };
    std::vector<int> odds = {1, 3, 5};
    std::vector<int> mixed = {1, 4, 5};
    std::cout << std::format("find_even in odds: {}\n",
                             find_even(odds).value_or(-1));
    std::cout << std::format("find_even in mixed: {}\n",
                             find_even(mixed).value_or(-1));

    // std::variant - type-safe union
    std::variant<int, double, std::string> v;
    v = 42;
    std::cout << std::format("variant (int): {}\n", std::get<int>(v));
    v = "hello";
    std::cout << std::format("variant (string): {}\n", std::get<std::string>(v));
    std::cout << "\n";
}

// ============================================================================
// Smart Pointers - Automatic memory management
// ============================================================================
void demonstrate_smart_pointers() {
    std::cout << "=== Smart Pointers ===\n\n";

    // unique_ptr - exclusive ownership
    auto up = std::make_unique<std::string>("unique ownership");
    std::cout << std::format("unique_ptr: '{}'\n", *up);
    // Cannot copy, only move
    auto up2 = std::move(up);  // up is now null

    // shared_ptr - shared ownership
    auto sp1 = std::make_shared<std::string>("shared ownership");
    auto sp2 = sp1;  // reference count = 2
    std::cout << std::format("shared_ptr: '{}', use_count: {}\n",
                             *sp1, sp1.use_count());
    std::cout << "\n";
}

// ============================================================================
// Random Numbers - Proper random generation
// ============================================================================
void demonstrate_random() {
    std::cout << "=== Random Numbers ===\n\n";

    // Random engine (seeded from random_device)
    std::random_device rd;
    std::mt19937 gen(rd());

    // Distributions
    std::uniform_int_distribution<> dice(1, 6);
    std::cout << "dice rolls: ";
    for (int i = 0; i < 10; ++i) {
        std::cout << dice(gen) << " ";
    }
    std::cout << "\n";

    std::normal_distribution<> normal(100.0, 15.0);  // mean=100, stddev=15
    std::cout << "normal samples: ";
    for (int i = 0; i < 5; ++i) {
        std::cout << std::format("{:.1f} ", normal(gen));
    }
    std::cout << "\n\n";
}

// ============================================================================
// Time - Chrono library
// ============================================================================
void demonstrate_chrono() {
    std::cout << "=== Chrono (Time) ===\n\n";

    using namespace std::chrono_literals;

    // Durations
    auto one_sec = 1s;
    auto half_sec = 500ms;
    auto total = one_sec + half_sec;
    std::cout << std::format("1s + 500ms = {}ms\n",
                             std::chrono::duration_cast<std::chrono::milliseconds>(total).count());

    // Measuring time
    auto start = std::chrono::high_resolution_clock::now();

    // Do some work
    volatile int sum = 0;
    for (int i = 0; i < 1000000; ++i) {
        sum += i;
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << std::format("loop took {}us\n\n", duration.count());
}

// ============================================================================
// Span - Non-owning view of contiguous data
// ============================================================================
void print_span(std::span<const int> data) {
    std::cout << "span contents: ";
    for (int x : data) std::cout << x << " ";
    std::cout << "\n";
}

void demonstrate_span() {
    std::cout << "=== Span ===\n\n";

    // Works with vector
    std::vector<int> vec = {1, 2, 3, 4, 5};
    print_span(vec);

    // Works with array
    int arr[] = {10, 20, 30};
    print_span(arr);

    // Works with part of a container
    print_span({vec.data() + 1, 3});  // elements 2, 3, 4

    std::cout << "\n";
}

// ============================================================================
// Function Objects - Callable wrappers
// ============================================================================
void demonstrate_function_objects() {
    std::cout << "=== Function Objects ===\n\n";

    // std::function - type-erased callable wrapper
    std::function<int(int, int)> op;

    op = [](int a, int b) { return a + b; };
    std::cout << std::format("add: {}\n", op(3, 4));

    op = [](int a, int b) { return a * b; };
    std::cout << std::format("multiply: {}\n", op(3, 4));

    // Standard function objects
    std::vector<int> vec = {5, 2, 8, 1};
    std::ranges::sort(vec, std::greater<>{});  // sort descending
    std::cout << "sorted descending: ";
    for (int x : vec) std::cout << x << " ";
    std::cout << "\n\n";
}

int main() {
    std::cout << "========================================\n";
    std::cout << "    C++ Standard Library Tour\n";
    std::cout << "========================================\n\n";

    demonstrate_containers();
    demonstrate_algorithms();
    demonstrate_strings();
    demonstrate_io();
    demonstrate_utilities();
    demonstrate_smart_pointers();
    demonstrate_random();
    demonstrate_chrono();
    demonstrate_span();
    demonstrate_function_objects();

    std::cout << "========================================\n";
    std::cout << "  End of Standard Library Tour\n";
    std::cout << "========================================\n";

    return 0;
}

// Key points:
// - The standard library is organized into headers (e.g., <vector>, <algorithm>)
// - All names are in the std namespace
// - Containers store data, algorithms operate on ranges
// - Smart pointers manage memory automatically
// - Utilities like optional, variant, tuple handle common patterns
// - Chrono provides time handling, random provides proper random numbers
// - Prefer standard library over hand-rolled solutions
