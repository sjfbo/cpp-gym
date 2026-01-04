// Chapter 9 Tests: Library Overview
// These tests verify understanding of standard library components.

#include <algorithm>
#include <catch2/catch_test_macros.hpp>
#include <functional>
#include <map>
#include <memory>
#include <numeric>
#include <optional>
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
// Section 9.2: Standard-Library Components
// ============================================================================

// ----------------------------------------------------------------------------
// Containers
// ----------------------------------------------------------------------------

TEST_CASE("Containers: vector basics", "[ch09][containers]") {
    std::vector<int> vec = {1, 2, 3, 4, 5};

    REQUIRE(vec.size() == 5);
    REQUIRE(vec[0] == 1);
    REQUIRE(vec.back() == 5);

    vec.push_back(6);
    REQUIRE(vec.size() == 6);
    REQUIRE(vec.back() == 6);
}

TEST_CASE("Containers: map basics", "[ch09][containers]") {
    std::map<std::string, int> ages;
    ages["Alice"] = 30;
    ages["Bob"] = 25;
    ages.insert({"Charlie", 35});

    REQUIRE(ages.size() == 3);
    REQUIRE(ages["Alice"] == 30);
    REQUIRE(ages.contains("Bob"));
    REQUIRE_FALSE(ages.contains("Dave"));
}

TEST_CASE("Containers: set maintains uniqueness and order", "[ch09][containers]") {
    std::set<int> nums = {5, 2, 8, 2, 1, 8, 3};

    REQUIRE(nums.size() == 5);  // duplicates removed

    // Elements are sorted
    std::vector<int> sorted(nums.begin(), nums.end());
    REQUIRE(sorted == std::vector{1, 2, 3, 5, 8});
}

// ----------------------------------------------------------------------------
// Algorithms
// ----------------------------------------------------------------------------

TEST_CASE("Algorithms: sort", "[ch09][algorithms]") {
    std::vector<int> vec = {5, 2, 8, 1, 9};
    std::ranges::sort(vec);
    REQUIRE(vec == std::vector{1, 2, 5, 8, 9});

    // Sort descending
    std::ranges::sort(vec, std::greater<>{});
    REQUIRE(vec == std::vector{9, 8, 5, 2, 1});
}

TEST_CASE("Algorithms: find", "[ch09][algorithms]") {
    std::vector<int> vec = {10, 20, 30, 40, 50};

    auto it = std::ranges::find(vec, 30);
    REQUIRE(it != vec.end());
    REQUIRE(*it == 30);
    REQUIRE(it - vec.begin() == 2);

    auto not_found = std::ranges::find(vec, 99);
    REQUIRE(not_found == vec.end());
}

TEST_CASE("Algorithms: count_if", "[ch09][algorithms]") {
    std::vector<int> vec = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    auto even_count = std::ranges::count_if(vec, [](int x) { return x % 2 == 0; });
    REQUIRE(even_count == 5);

    auto over_five = std::ranges::count_if(vec, [](int x) { return x > 5; });
    REQUIRE(over_five == 5);
}

TEST_CASE("Algorithms: transform", "[ch09][algorithms]") {
    std::vector<int> input = {1, 2, 3, 4, 5};
    std::vector<int> output;

    std::ranges::transform(input, std::back_inserter(output),
                           [](int x) { return x * x; });

    REQUIRE(output == std::vector{1, 4, 9, 16, 25});
}

TEST_CASE("Algorithms: accumulate", "[ch09][algorithms]") {
    std::vector<int> vec = {1, 2, 3, 4, 5};

    int sum = std::accumulate(vec.begin(), vec.end(), 0);
    REQUIRE(sum == 15);

    int product = std::accumulate(vec.begin(), vec.end(), 1, std::multiplies<>{});
    REQUIRE(product == 120);
}

// ----------------------------------------------------------------------------
// Strings
// ----------------------------------------------------------------------------

TEST_CASE("Strings: basic operations", "[ch09][strings]") {
    std::string s = "Hello, World!";

    REQUIRE(s.length() == 13);
    REQUIRE(s.substr(0, 5) == "Hello");
    REQUIRE(s.find("World") == 7);
    REQUIRE(s.find("xyz") == std::string::npos);

    s.replace(7, 5, "C++");
    REQUIRE(s == "Hello, C++!");
}

TEST_CASE("Strings: string_view", "[ch09][strings]") {
    std::string str = "Hello, World!";
    std::string_view sv = str;

    REQUIRE(sv.length() == 13);
    REQUIRE(sv.substr(0, 5) == "Hello");

    // string_view doesn't own data - just a view
    std::string_view sub = sv.substr(7, 5);
    REQUIRE(sub == "World");
}

// ----------------------------------------------------------------------------
// I/O Streams
// ----------------------------------------------------------------------------

TEST_CASE("I/O: stringstream", "[ch09][io]") {
    // Output string stream
    std::ostringstream oss;
    oss << "Value: " << 42 << ", Pi: " << 3.14;
    REQUIRE(oss.str() == "Value: 42, Pi: 3.14");

    // Input string stream
    std::istringstream iss("10 20 30");
    int a, b, c;
    iss >> a >> b >> c;
    REQUIRE(a == 10);
    REQUIRE(b == 20);
    REQUIRE(c == 30);
}

// ----------------------------------------------------------------------------
// Utilities
// ----------------------------------------------------------------------------

TEST_CASE("Utilities: pair", "[ch09][utilities]") {
    std::pair<std::string, int> p = {"Alice", 30};

    REQUIRE(p.first == "Alice");
    REQUIRE(p.second == 30);

    // Structured binding
    auto [name, age] = p;
    REQUIRE(name == "Alice");
    REQUIRE(age == 30);
}

TEST_CASE("Utilities: tuple", "[ch09][utilities]") {
    std::tuple<std::string, int, double> t = {"Bob", 25, 1.75};

    REQUIRE(std::get<0>(t) == "Bob");
    REQUIRE(std::get<1>(t) == 25);
    REQUIRE(std::get<2>(t) == 1.75);

    // Structured binding
    auto [name, age, height] = t;
    REQUIRE(name == "Bob");
    REQUIRE(age == 25);
    REQUIRE(height == 1.75);
}

TEST_CASE("Utilities: optional", "[ch09][utilities]") {
    auto divide = [](int a, int b) -> std::optional<int> {
        if (b == 0) return std::nullopt;
        return a / b;
    };

    auto result1 = divide(10, 2);
    REQUIRE(result1.has_value());
    REQUIRE(result1.value() == 5);

    auto result2 = divide(10, 0);
    REQUIRE_FALSE(result2.has_value());
    REQUIRE(result2.value_or(-1) == -1);
}

TEST_CASE("Utilities: variant", "[ch09][utilities]") {
    std::variant<int, double, std::string> v;

    v = 42;
    REQUIRE(std::holds_alternative<int>(v));
    REQUIRE(std::get<int>(v) == 42);

    v = 3.14;
    REQUIRE(std::holds_alternative<double>(v));
    REQUIRE(std::get<double>(v) == 3.14);

    v = "hello";
    REQUIRE(std::holds_alternative<std::string>(v));
    REQUIRE(std::get<std::string>(v) == "hello");
}

// ----------------------------------------------------------------------------
// Smart Pointers
// ----------------------------------------------------------------------------

TEST_CASE("Smart pointers: unique_ptr", "[ch09][memory]") {
    auto ptr = std::make_unique<int>(42);
    REQUIRE(*ptr == 42);

    // Move ownership
    auto ptr2 = std::move(ptr);
    REQUIRE(ptr == nullptr);
    REQUIRE(*ptr2 == 42);
}

TEST_CASE("Smart pointers: shared_ptr", "[ch09][memory]") {
    auto ptr1 = std::make_shared<int>(42);
    REQUIRE(ptr1.use_count() == 1);

    {
        auto ptr2 = ptr1;  // Share ownership
        REQUIRE(ptr1.use_count() == 2);
        REQUIRE(*ptr2 == 42);
    }  // ptr2 destroyed

    REQUIRE(ptr1.use_count() == 1);
    REQUIRE(*ptr1 == 42);
}

// ----------------------------------------------------------------------------
// Span (C++20)
// ----------------------------------------------------------------------------

int sum_span(std::span<const int> data) {
    return std::accumulate(data.begin(), data.end(), 0);
}

TEST_CASE("Span: non-owning view", "[ch09][span]") {
    std::vector<int> vec = {1, 2, 3, 4, 5};
    REQUIRE(sum_span(vec) == 15);

    int arr[] = {10, 20, 30};
    REQUIRE(sum_span(arr) == 60);

    // Subspan
    REQUIRE(sum_span({vec.data() + 1, 3}) == 9);  // 2 + 3 + 4
}

// ----------------------------------------------------------------------------
// Function Objects
// ----------------------------------------------------------------------------

TEST_CASE("Function objects: std::function", "[ch09][functional]") {
    std::function<int(int, int)> op;

    op = [](int a, int b) { return a + b; };
    REQUIRE(op(3, 4) == 7);

    op = [](int a, int b) { return a * b; };
    REQUIRE(op(3, 4) == 12);

    op = std::minus<int>{};
    REQUIRE(op(10, 3) == 7);
}
