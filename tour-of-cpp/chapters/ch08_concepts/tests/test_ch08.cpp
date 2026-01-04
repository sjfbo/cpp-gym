// Chapter 8 Tests: Concepts and Generic Programming
// These tests verify understanding of C++20 concepts and variadic templates.

#include <catch2/catch_test_macros.hpp>
#include <concepts>
#include <cstddef>
#include <functional>
#include <sstream>
#include <string>
#include <type_traits>
#include <vector>

// ============================================================================
// Section 8.2: Concepts
// ============================================================================

// Custom concepts for testing
template<typename T>
concept Numeric = std::integral<T> || std::floating_point<T>;

template<typename T>
concept Addable = requires(T a, T b) {
    a + b;
};

template<typename T>
concept Arithmetic = requires(T a, T b) {
    a + b;
    a - b;
    a * b;
    a / b;
};

template<typename T>
concept Hashable = requires(T a) {
    { std::hash<T>{}(a) } -> std::convertible_to<std::size_t>;
};

template<typename T>
concept Container = requires(T c) {
    typename T::value_type;
    typename T::iterator;
    { c.begin() } -> std::same_as<typename T::iterator>;
    { c.end() } -> std::same_as<typename T::iterator>;
    { c.size() } -> std::convertible_to<std::size_t>;
};

// Concept-constrained functions
template<Numeric T>
T square(T x) {
    return x * x;
}

template<std::integral T>
T integer_divide(T a, T b) {
    return a / b;
}

template<Addable T>
T add_values(T a, T b) {
    return a + b;
}

TEST_CASE("Concepts: standard library concepts", "[ch08][concepts][std]") {
    // Integral concept
    STATIC_REQUIRE(std::integral<int>);
    STATIC_REQUIRE(std::integral<long>);
    STATIC_REQUIRE(std::integral<char>);
    STATIC_REQUIRE(std::integral<bool>);
    STATIC_REQUIRE_FALSE(std::integral<double>);
    STATIC_REQUIRE_FALSE(std::integral<float>);

    // Floating point concept
    STATIC_REQUIRE(std::floating_point<double>);
    STATIC_REQUIRE(std::floating_point<float>);
    STATIC_REQUIRE_FALSE(std::floating_point<int>);

    // Signed/unsigned integral
    STATIC_REQUIRE(std::signed_integral<int>);
    STATIC_REQUIRE(std::unsigned_integral<unsigned int>);
    STATIC_REQUIRE_FALSE(std::signed_integral<unsigned int>);
}

TEST_CASE("Concepts: custom concept definitions", "[ch08][concepts][custom]") {
    // Numeric concept
    STATIC_REQUIRE(Numeric<int>);
    STATIC_REQUIRE(Numeric<double>);
    STATIC_REQUIRE(Numeric<float>);
    STATIC_REQUIRE_FALSE(Numeric<std::string>);
    STATIC_REQUIRE_FALSE(Numeric<void*>);

    // Addable concept
    STATIC_REQUIRE(Addable<int>);
    STATIC_REQUIRE(Addable<double>);
    STATIC_REQUIRE(Addable<std::string>);

    // Arithmetic concept
    STATIC_REQUIRE(Arithmetic<int>);
    STATIC_REQUIRE(Arithmetic<double>);
    STATIC_REQUIRE_FALSE(Arithmetic<std::string>);

    // Hashable concept
    STATIC_REQUIRE(Hashable<int>);
    STATIC_REQUIRE(Hashable<std::string>);

    // Container concept
    STATIC_REQUIRE(Container<std::vector<int>>);
    STATIC_REQUIRE(Container<std::string>);
    STATIC_REQUIRE_FALSE(Container<int>);
}

TEST_CASE("Concepts: constrained template functions", "[ch08][concepts][functions]") {
    // square works with Numeric types
    REQUIRE(square(5) == 25);
    REQUIRE(square(3.0) == 9.0);
    REQUIRE(square(-4) == 16);

    // integer_divide works with integral types
    REQUIRE(integer_divide(17, 5) == 3);
    REQUIRE(integer_divide(20L, 6L) == 3L);

    // add_values works with Addable types
    REQUIRE(add_values(3, 4) == 7);
    REQUIRE(add_values(1.5, 2.5) == 4.0);
    REQUIRE(add_values(std::string("Hello"), std::string(" World")) == "Hello World");
}

TEST_CASE("Concepts: concept-based overloading", "[ch08][concepts][overloading]") {
    auto process = [](std::integral auto x) { return std::string("integral"); };
    auto process_fp = [](std::floating_point auto x) { return std::string("floating"); };

    REQUIRE(process(42) == "integral");
    REQUIRE(process(42L) == "integral");
    REQUIRE(process_fp(3.14) == "floating");
    REQUIRE(process_fp(2.5f) == "floating");
}

TEST_CASE("Concepts: requires expressions", "[ch08][concepts][requires]") {
    // Simple requires
    constexpr bool int_addable = requires(int a, int b) { a + b; };
    STATIC_REQUIRE(int_addable);

    // Compound requirements
    constexpr bool string_has_size = requires(std::string s) {
        { s.size() } -> std::convertible_to<std::size_t>;
    };
    STATIC_REQUIRE(string_has_size);

    // Type requirements
    constexpr bool vector_has_value_type = requires {
        typename std::vector<int>::value_type;
    };
    STATIC_REQUIRE(vector_has_value_type);
}

// ============================================================================
// Section 8.3: Generic Programming
// ============================================================================

// Generic algorithm with concept constraints
template<Container C>
auto container_sum(const C& container) {
    typename C::value_type result{};
    for (const auto& elem : container) {
        result = result + elem;
    }
    return result;
}

template<Container C>
    requires std::totally_ordered<typename C::value_type>
auto container_max(const C& container) {
    auto it = container.begin();
    if (it == container.end()) {
        return typename C::value_type{};
    }
    auto max_val = *it;
    ++it;
    while (it != container.end()) {
        if (*it > max_val) {
            max_val = *it;
        }
        ++it;
    }
    return max_val;
}

TEST_CASE("Generic programming: constrained algorithms", "[ch08][generic][algorithms]") {
    std::vector<int> nums = {1, 2, 3, 4, 5};

    REQUIRE(container_sum(nums) == 15);
    REQUIRE(container_max(nums) == 5);

    std::vector<double> doubles = {1.5, 2.5, 3.5};
    REQUIRE(container_sum(doubles) == 7.5);
    REQUIRE(container_max(doubles) == 3.5);
}

TEST_CASE("Generic programming: subsumption", "[ch08][generic][subsumption]") {
    // More specific concepts are preferred
    auto describe = [](std::integral auto) -> std::string { return "integral"; };
    auto describe_signed = [](std::signed_integral auto) -> std::string { return "signed_integral"; };

    // Can call both, compiler picks based on argument type
    REQUIRE(describe(42u) == "integral");
    REQUIRE(describe_signed(42) == "signed_integral");
}

// ============================================================================
// Section 8.4: Variadic Templates
// ============================================================================

// sizeof...
template<typename... Ts>
constexpr std::size_t count_types() {
    return sizeof...(Ts);
}

template<typename... Args>
constexpr std::size_t count_args(Args...) {
    return sizeof...(Args);
}

// Fold expressions
template<typename... Args>
constexpr auto sum(Args... args) {
    return (args + ... + 0);
}

template<typename... Args>
constexpr auto product(Args... args) {
    return (args * ... * 1);
}

template<typename... Args>
constexpr bool all_true(Args... args) {
    return (... && args);
}

template<typename... Args>
constexpr bool any_true(Args... args) {
    return (... || args);
}

// Min/max with variadic
template<typename T, std::same_as<T>... Rest>
constexpr T min_of(T first, Rest... rest) {
    T result = first;
    ((result = rest < result ? rest : result), ...);
    return result;
}

template<typename T, std::same_as<T>... Rest>
constexpr T max_of(T first, Rest... rest) {
    T result = first;
    ((result = rest > result ? rest : result), ...);
    return result;
}

// String concatenation
template<typename... Args>
std::string concat(Args&&... args) {
    std::ostringstream oss;
    ((oss << args), ...);
    return oss.str();
}

TEST_CASE("Variadic templates: sizeof...", "[ch08][variadic][sizeof]") {
    STATIC_REQUIRE(count_types<>() == 0);
    STATIC_REQUIRE(count_types<int>() == 1);
    STATIC_REQUIRE(count_types<int, double, char>() == 3);

    REQUIRE(count_args() == 0);
    REQUIRE(count_args(1) == 1);
    REQUIRE(count_args(1, 2.0, "hello", 'x') == 4);
}

TEST_CASE("Variadic templates: fold expressions - sum", "[ch08][variadic][fold]") {
    STATIC_REQUIRE(sum() == 0);
    STATIC_REQUIRE(sum(1) == 1);
    STATIC_REQUIRE(sum(1, 2, 3) == 6);
    STATIC_REQUIRE(sum(1, 2, 3, 4, 5) == 15);

    REQUIRE(sum(1.5, 2.5, 3.0) == 7.0);
}

TEST_CASE("Variadic templates: fold expressions - product", "[ch08][variadic][fold]") {
    STATIC_REQUIRE(product() == 1);
    STATIC_REQUIRE(product(5) == 5);
    STATIC_REQUIRE(product(2, 3, 4) == 24);

    REQUIRE(product(1.5, 2.0, 3.0) == 9.0);
}

TEST_CASE("Variadic templates: fold expressions - logical", "[ch08][variadic][fold][logical]") {
    // all_true
    STATIC_REQUIRE(all_true());  // Empty is true
    STATIC_REQUIRE(all_true(true));
    STATIC_REQUIRE(all_true(true, true, true));
    STATIC_REQUIRE_FALSE(all_true(true, false, true));
    STATIC_REQUIRE_FALSE(all_true(false));

    // any_true
    STATIC_REQUIRE_FALSE(any_true());  // Empty is false
    STATIC_REQUIRE(any_true(true));
    STATIC_REQUIRE(any_true(false, true, false));
    STATIC_REQUIRE_FALSE(any_true(false, false, false));
}

TEST_CASE("Variadic templates: min and max", "[ch08][variadic][minmax]") {
    STATIC_REQUIRE(min_of(5) == 5);
    STATIC_REQUIRE(min_of(5, 3, 8, 1, 9) == 1);
    STATIC_REQUIRE(min_of(1, 1, 1) == 1);

    STATIC_REQUIRE(max_of(5) == 5);
    STATIC_REQUIRE(max_of(5, 3, 8, 1, 9) == 9);
    STATIC_REQUIRE(max_of(1, 1, 1) == 1);

    REQUIRE(min_of(3.14, 2.71, 1.41) == 1.41);
    REQUIRE(max_of(3.14, 2.71, 1.41) == 3.14);
}

TEST_CASE("Variadic templates: string concatenation", "[ch08][variadic][string]") {
    REQUIRE(concat() == "");
    REQUIRE(concat("Hello") == "Hello");
    REQUIRE(concat("Hello", " ", "World") == "Hello World");
    REQUIRE(concat(1, 2, 3) == "123");
    REQUIRE(concat("Value: ", 42, ", Pi: ", 3.14) == "Value: 42, Pi: 3.14");
}

// ============================================================================
// Combined concepts and variadic templates
// ============================================================================

// Constrained variadic
template<std::integral... Args>
constexpr auto sum_integral(Args... args) {
    return (args + ... + 0);
}

// Check if all types satisfy a concept
template<typename... Ts>
constexpr bool all_numeric() {
    return (Numeric<Ts> && ...);
}

TEST_CASE("Combined: concept-constrained variadic", "[ch08][combined]") {
    STATIC_REQUIRE(sum_integral(1, 2, 3) == 6);
    STATIC_REQUIRE(sum_integral(1L, 2L, 3L) == 6L);

    STATIC_REQUIRE(all_numeric<int, double, float>());
    STATIC_REQUIRE(all_numeric<int, long, short>());
    STATIC_REQUIRE_FALSE(all_numeric<int, std::string>());
    STATIC_REQUIRE_FALSE(all_numeric<std::string>());
    STATIC_REQUIRE(all_numeric<>());  // Empty pack is true with &&
}

// Push multiple elements to vector
template<typename T, typename... Args>
    requires (std::convertible_to<Args, T> && ...)
void push_all(std::vector<T>& vec, Args&&... args) {
    (vec.push_back(static_cast<T>(std::forward<Args>(args))), ...);
}

TEST_CASE("Combined: push_all to vector", "[ch08][combined][vector]") {
    std::vector<int> vec;
    push_all(vec, 1, 2, 3, 4, 5);

    REQUIRE(vec.size() == 5);
    REQUIRE(vec[0] == 1);
    REQUIRE(vec[4] == 5);

    std::vector<double> doubles;
    push_all(doubles, 1.5, 2.5, 3.5);

    REQUIRE(doubles.size() == 3);
    REQUIRE(doubles[0] == 1.5);
    REQUIRE(doubles[2] == 3.5);
}
