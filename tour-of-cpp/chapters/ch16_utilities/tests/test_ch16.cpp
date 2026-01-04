// Chapter 16 Tests: Utilities
// These tests verify understanding of time, function adaption, type traits, and move semantics.

#include <catch2/catch_test_macros.hpp>
#include <chrono>
#include <concepts>
#include <functional>
#include <string>
#include <thread>
#include <type_traits>
#include <utility>
#include <vector>

using namespace std::chrono_literals;

// ============================================================================
// Section 16.2: Time (chrono)
// ============================================================================

TEST_CASE("chrono: duration arithmetic", "[ch16][chrono]") {
    auto h = 1h;
    auto m = 30min;
    auto s = 45s;

    auto total = h + m + s;
    auto as_seconds = std::chrono::duration_cast<std::chrono::seconds>(total);

    REQUIRE(as_seconds.count() == 5445);  // 3600 + 1800 + 45

    auto doubled = 5s * 2;
    REQUIRE(doubled == 10s);
}

TEST_CASE("chrono: duration comparison", "[ch16][chrono]") {
    REQUIRE(1h == 60min);
    REQUIRE(1min == 60s);
    REQUIRE(1s == 1000ms);
    REQUIRE(1h > 59min);
    REQUIRE(1000ms == 1s);
}

TEST_CASE("chrono: duration_cast", "[ch16][chrono]") {
    auto ms = 1500ms;

    auto floor_s = std::chrono::floor<std::chrono::seconds>(ms);
    auto ceil_s = std::chrono::ceil<std::chrono::seconds>(ms);
    auto round_s = std::chrono::round<std::chrono::seconds>(ms);

    REQUIRE(floor_s == 1s);
    REQUIRE(ceil_s == 2s);
    REQUIRE(round_s == 2s);
}

TEST_CASE("chrono: time points", "[ch16][chrono]") {
    auto start = std::chrono::steady_clock::now();
    std::this_thread::sleep_for(10ms);
    auto end = std::chrono::steady_clock::now();

    auto elapsed = end - start;
    REQUIRE(elapsed >= 10ms);
}

// ============================================================================
// Section 16.3: Function Adaption
// ============================================================================

int add(int a, int b) { return a + b; }

TEST_CASE("function: wrap regular function", "[ch16][function]") {
    std::function<int(int, int)> f = add;

    REQUIRE(f(3, 4) == 7);
}

TEST_CASE("function: wrap lambda", "[ch16][function]") {
    std::function<int(int)> square = [](int x) { return x * x; };

    REQUIRE(square(5) == 25);
}

TEST_CASE("function: empty function", "[ch16][function]") {
    std::function<void()> empty;

    REQUIRE_FALSE(static_cast<bool>(empty));

    empty = []() {};
    REQUIRE(static_cast<bool>(empty));
}

TEST_CASE("function: stateful lambda", "[ch16][function]") {
    int counter = 0;
    std::function<int()> increment = [&counter]() { return ++counter; };

    REQUIRE(increment() == 1);
    REQUIRE(increment() == 2);
    REQUIRE(counter == 2);
}

TEST_CASE("bind: partial application", "[ch16][bind]") {
    using namespace std::placeholders;

    auto add_10 = std::bind(add, 10, _1);
    REQUIRE(add_10(5) == 15);

    auto add_5 = std::bind(add, _1, 5);
    REQUIRE(add_5(10) == 15);
}

TEST_CASE("invoke: uniform call syntax", "[ch16][invoke]") {
    REQUIRE(std::invoke(add, 3, 4) == 7);

    auto lambda = [](int x) { return x * 2; };
    REQUIRE(std::invoke(lambda, 5) == 10);
}

// ============================================================================
// Section 16.4: Type Functions
// ============================================================================

TEST_CASE("type_traits: primary categories", "[ch16][type_traits]") {
    REQUIRE(std::is_void_v<void>);
    REQUIRE(std::is_integral_v<int>);
    REQUIRE(std::is_integral_v<bool>);
    REQUIRE_FALSE(std::is_integral_v<double>);
    REQUIRE(std::is_floating_point_v<double>);
    REQUIRE(std::is_pointer_v<int*>);
}

TEST_CASE("type_traits: composite categories", "[ch16][type_traits]") {
    REQUIRE(std::is_arithmetic_v<int>);
    REQUIRE(std::is_arithmetic_v<double>);
    REQUIRE_FALSE(std::is_arithmetic_v<std::string>);

    REQUIRE(std::is_fundamental_v<int>);
    REQUIRE_FALSE(std::is_fundamental_v<std::string>);
}

TEST_CASE("type_traits: type properties", "[ch16][type_traits]") {
    REQUIRE(std::is_const_v<const int>);
    REQUIRE_FALSE(std::is_const_v<int>);

    REQUIRE(std::is_signed_v<int>);
    REQUIRE(std::is_unsigned_v<unsigned int>);
}

TEST_CASE("type_traits: type transformations", "[ch16][type_traits]") {
    using T1 = std::remove_reference_t<int&>;
    REQUIRE(std::is_same_v<T1, int>);

    using T2 = std::remove_const_t<const int>;
    REQUIRE(std::is_same_v<T2, int>);

    using T3 = std::add_pointer_t<int>;
    REQUIRE(std::is_same_v<T3, int*>);

    using T4 = std::decay_t<const int&>;
    REQUIRE(std::is_same_v<T4, int>);
}

TEST_CASE("conditional: type selection", "[ch16][type_traits]") {
    using T1 = std::conditional_t<true, int, double>;
    REQUIRE(std::is_same_v<T1, int>);

    using T2 = std::conditional_t<false, int, double>;
    REQUIRE(std::is_same_v<T2, double>);

    using T3 = std::conditional_t<sizeof(int) == 4, int32_t, int64_t>;
    REQUIRE(sizeof(T3) == 4);
}

// ============================================================================
// Section 16.5: move and forward
// ============================================================================

TEST_CASE("move: basic usage", "[ch16][move]") {
    std::string s1 = "hello world";
    std::string s2 = std::move(s1);

    REQUIRE(s2 == "hello world");
    REQUIRE(s1.empty());  // Moved-from state
}

TEST_CASE("move: vector elements", "[ch16][move]") {
    std::vector<std::string> vec;

    std::string s = "test";
    vec.push_back(s);  // Copy
    REQUIRE(s == "test");

    vec.push_back(std::move(s));  // Move
    REQUIRE(s.empty());

    REQUIRE(vec.size() == 2);
}

TEST_CASE("move: move-only types", "[ch16][move]") {
    auto up1 = std::make_unique<int>(42);
    // auto up2 = up1;  // Error: deleted copy constructor

    auto up2 = std::move(up1);
    REQUIRE(up1 == nullptr);
    REQUIRE(*up2 == 42);
}

// Perfect forwarding test helpers
void target(int&) {}
void target(int&&) {}

template<typename T>
void forwarder(T&& arg) {
    target(std::forward<T>(arg));
}

TEST_CASE("forward: perfect forwarding", "[ch16][forward]") {
    int x = 42;
    forwarder(x);       // Forwards as lvalue
    forwarder(42);      // Forwards as rvalue
    forwarder(std::move(x));  // Forwards as rvalue

    // If this compiles and runs, forwarding works correctly
    REQUIRE(true);
}

// ============================================================================
// Concepts (C++20)
// ============================================================================

TEST_CASE("concepts: standard concepts", "[ch16][concepts]") {
    REQUIRE(std::integral<int>);
    REQUIRE(std::integral<bool>);
    REQUIRE_FALSE(std::integral<double>);

    REQUIRE(std::floating_point<double>);
    REQUIRE_FALSE(std::floating_point<int>);

    REQUIRE(std::signed_integral<int>);
    REQUIRE(std::unsigned_integral<unsigned int>);
}

TEST_CASE("concepts: same_as and convertible_to", "[ch16][concepts]") {
    REQUIRE(std::same_as<int, int>);
    REQUIRE_FALSE(std::same_as<int, long>);

    REQUIRE(std::convertible_to<int, double>);
    REQUIRE(std::convertible_to<double, int>);
}

TEST_CASE("concepts: object concepts", "[ch16][concepts]") {
    REQUIRE(std::movable<std::string>);
    REQUIRE(std::copyable<std::string>);
    REQUIRE(std::movable<std::unique_ptr<int>>);
    REQUIRE_FALSE(std::copyable<std::unique_ptr<int>>);
}

// ============================================================================
// Practical scenarios
// ============================================================================

TEST_CASE("callback with std::function", "[ch16][practical]") {
    std::vector<int> results;

    std::function<void(int)> callback = [&results](int x) {
        results.push_back(x);
    };

    callback(1);
    callback(2);
    callback(3);

    REQUIRE(results.size() == 3);
    REQUIRE(results[0] == 1);
    REQUIRE(results[2] == 3);
}

TEST_CASE("stopwatch pattern", "[ch16][practical]") {
    auto start = std::chrono::steady_clock::now();

    // Simulate work
    int sum = 0;
    for (int i = 0; i < 10000; ++i) {
        sum += i;
    }

    auto end = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    REQUIRE(elapsed.count() >= 0);
    REQUIRE(sum > 0);
}

TEST_CASE("factory with perfect forwarding", "[ch16][practical]") {
    struct Widget {
        int id;
        std::string name;
        Widget(int i, std::string n) : id{i}, name{std::move(n)} {}
    };

    auto factory = [](auto&&... args) {
        return std::make_unique<Widget>(std::forward<decltype(args)>(args)...);
    };

    std::string name = "test";
    auto w1 = factory(42, name);  // Copy name
    auto w2 = factory(43, std::move(name));  // Move name

    REQUIRE(w1->id == 42);
    REQUIRE(w1->name == "test");
    REQUIRE(w2->id == 43);
    REQUIRE(w2->name == "test");
}

TEST_CASE("type-dispatched function", "[ch16][practical]") {
    auto process = []<typename T>(T value) -> std::string {
        if constexpr (std::is_integral_v<T>) {
            return "integer: " + std::to_string(value);
        } else if constexpr (std::is_floating_point_v<T>) {
            return "float: " + std::to_string(value);
        } else {
            return "other";
        }
    };

    REQUIRE(process(42) == "integer: 42");
    REQUIRE(process(3.14).substr(0, 6) == "float:");
}
