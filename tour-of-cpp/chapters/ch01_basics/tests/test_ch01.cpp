// Chapter 1 Tests: The Basics
// These tests verify understanding of fundamental C++ concepts.

#include <array>
#include <catch2/catch_test_macros.hpp>
#include <limits>
#include <string>
#include <type_traits>

// ============================================================================
// Section 1.3: Functions
// ============================================================================

// Helper functions for testing
constexpr int square(int x) {
    return x * x;
}

constexpr int factorial(int n) {
    return (n <= 1) ? 1 : n * factorial(n - 1);
}

int add(int a, int b) {
    return a + b;
}

int add(int a, int b, int c) {
    return a + b + c;
}

double add(double a, double b) {
    return a + b;
}

TEST_CASE("Functions: basic function calls", "[ch01][functions]") {
    REQUIRE(square(5) == 25);
    REQUIRE(square(0) == 0);
    REQUIRE(square(-3) == 9);
}

TEST_CASE("Functions: constexpr evaluation", "[ch01][functions][constexpr]") {
    // Verify compile-time evaluation
    constexpr int sq = square(10);
    static_assert(sq == 100, "square should work at compile time");

    constexpr int fact5 = factorial(5);
    static_assert(fact5 == 120, "factorial should work at compile time");

    REQUIRE(factorial(0) == 1);
    REQUIRE(factorial(1) == 1);
    REQUIRE(factorial(6) == 720);
}

TEST_CASE("Functions: overloading", "[ch01][functions][overloading]") {
    // Same name, different parameters
    REQUIRE(add(1, 2) == 3);        // int, int
    REQUIRE(add(1, 2, 3) == 6);     // int, int, int
    REQUIRE(add(1.5, 2.5) == 4.0);  // double, double
}

// ============================================================================
// Section 1.4: Types, Variables, and Arithmetic
// ============================================================================

TEST_CASE("Types: sizeof fundamental types", "[ch01][types]") {
    // These are minimum requirements
    REQUIRE(sizeof(char) >= 1);
    REQUIRE(sizeof(short) >= 2);
    REQUIRE(sizeof(int) >= 2);
    REQUIRE(sizeof(long) >= 4);
    REQUIRE(sizeof(long long) >= 8);
    REQUIRE(sizeof(float) >= 4);
    REQUIRE(sizeof(double) >= 8);

    // Pointer size depends on architecture
    REQUIRE((sizeof(void*) == 4 || sizeof(void*) == 8));
}

TEST_CASE("Types: initialization styles", "[ch01][types][initialization]") {
    // Copy initialization
    int a = 42;
    REQUIRE(a == 42);

    // Direct initialization
    int b(42);
    REQUIRE(b == 42);

    // List initialization (prevents narrowing)
    int c{42};
    REQUIRE(c == 42);

    // Default initialization with {}
    int d{};
    REQUIRE(d == 0);
}

TEST_CASE("Types: auto type deduction", "[ch01][types][auto]") {
    auto i = 42;
    static_assert(std::is_same_v<decltype(i), int>);

    auto d = 3.14;
    static_assert(std::is_same_v<decltype(d), double>);

    auto c = 'A';
    static_assert(std::is_same_v<decltype(c), char>);

    auto b = true;
    static_assert(std::is_same_v<decltype(b), bool>);

    auto s = "hello";
    static_assert(std::is_same_v<decltype(s), const char*>);
}

TEST_CASE("Types: arithmetic operations", "[ch01][types][arithmetic]") {
    REQUIRE(17 / 5 == 3);    // Integer division
    REQUIRE(17 % 5 == 2);    // Remainder
    REQUIRE(17.0 / 5 == 3.4);  // Floating-point division

    int x = 5;
    REQUIRE(x++ == 5);  // Post-increment returns old value
    REQUIRE(x == 6);
    REQUIRE(++x == 7);  // Pre-increment returns new value
}

// ============================================================================
// Section 1.6: Constants
// ============================================================================

TEST_CASE("Constants: const vs constexpr", "[ch01][constants]") {
    const int runtime_const = 42;     // Can be runtime value
    constexpr int compile_const = 42; // Must be compile-time

    REQUIRE(runtime_const == compile_const);

    // constexpr can be used in compile-time contexts
    std::array<int, compile_const> arr{};
    REQUIRE(arr.size() == 42);
}

TEST_CASE("Constants: constexpr functions", "[ch01][constants][constexpr]") {
    // Can be evaluated at compile time
    constexpr int sq = square(5);
    static_assert(sq == 25);

    // Can also be called at runtime
    int n = 6;
    int result = square(n);
    REQUIRE(result == 36);
}

// ============================================================================
// Section 1.7: Pointers, Arrays, and References
// ============================================================================

TEST_CASE("Pointers: basic operations", "[ch01][pointers]") {
    int x = 42;
    int* p = &x;

    REQUIRE(*p == 42);  // Dereference
    REQUIRE(p == &x);   // Address

    *p = 100;
    REQUIRE(x == 100);  // Modified through pointer
}

TEST_CASE("Pointers: nullptr", "[ch01][pointers][nullptr]") {
    int* p = nullptr;
    REQUIRE(p == nullptr);

    int x = 42;
    p = &x;
    REQUIRE(p != nullptr);
    REQUIRE(*p == 42);
}

TEST_CASE("References: basic operations", "[ch01][references]") {
    int x = 42;
    int& ref = x;

    REQUIRE(ref == 42);
    REQUIRE(&ref == &x);  // Same address

    ref = 100;
    REQUIRE(x == 100);  // Modified through reference
}

TEST_CASE("References: const references", "[ch01][references][const]") {
    int x = 42;
    const int& cref = x;

    REQUIRE(cref == 42);

    x = 100;
    REQUIRE(cref == 100);  // Reflects change in x

    // cref = 200;  // Would not compile: cannot modify through const ref
}

TEST_CASE("Arrays: C-style arrays", "[ch01][arrays]") {
    int arr[] = {1, 2, 3, 4, 5};

    REQUIRE(arr[0] == 1);
    REQUIRE(arr[4] == 5);
    REQUIRE(sizeof(arr) / sizeof(arr[0]) == 5);

    // Array decays to pointer
    int* p = arr;
    REQUIRE(*p == 1);
    REQUIRE(*(p + 2) == 3);
}

TEST_CASE("Arrays: std::array", "[ch01][arrays][std]") {
    std::array<int, 5> arr = {10, 20, 30, 40, 50};

    REQUIRE(arr[0] == 10);
    REQUIRE(arr.at(4) == 50);
    REQUIRE(arr.size() == 5);
    REQUIRE(arr.front() == 10);
    REQUIRE(arr.back() == 50);

    // Bounds checking with at()
    REQUIRE_THROWS_AS(arr.at(10), std::out_of_range);
}

TEST_CASE("Arrays: pointer arithmetic", "[ch01][arrays][pointers]") {
    int arr[] = {100, 200, 300, 400};
    int* p = arr;

    REQUIRE(*p == 100);
    REQUIRE(*(p + 1) == 200);
    REQUIRE(*(p + 3) == 400);

    p++;
    REQUIRE(*p == 200);

    int* end = arr + 4;
    REQUIRE(end - arr == 4);
}

// ============================================================================
// Section 1.5: Scope and Lifetime
// ============================================================================

TEST_CASE("Scope: block scope", "[ch01][scope]") {
    int outer = 10;

    {
        int inner = 20;  // Different variable in inner scope
        REQUIRE(inner == 20);
        REQUIRE(outer == 10);  // Outer still accessible
    }

    REQUIRE(outer == 10);  // Outer unchanged after block ends
}

TEST_CASE("Scope: static variables", "[ch01][scope][static]") {
    auto counter = []() -> int {
        static int count = 0;
        return ++count;
    };

    REQUIRE(counter() == 1);
    REQUIRE(counter() == 2);
    REQUIRE(counter() == 3);
}

// ============================================================================
// Section 1.8: Tests (Control Flow)
// ============================================================================

TEST_CASE("Control flow: comparison operators", "[ch01][control]") {
    REQUIRE((5 == 5) == true);
    REQUIRE((5 != 3) == true);
    REQUIRE((5 < 10) == true);
    REQUIRE((5 <= 5) == true);
    REQUIRE((5 > 3) == true);
    REQUIRE((5 >= 5) == true);
}

TEST_CASE("Control flow: logical operators", "[ch01][control]") {
    REQUIRE((true && true) == true);
    REQUIRE((true && false) == false);
    REQUIRE((true || false) == true);
    REQUIRE((false || false) == false);
    REQUIRE((!true) == false);
    REQUIRE((!false) == true);
}

TEST_CASE("Control flow: ternary operator", "[ch01][control]") {
    int a = 5;
    int b = 10;

    int max_val = (a > b) ? a : b;
    REQUIRE(max_val == 10);

    int min_val = (a < b) ? a : b;
    REQUIRE(min_val == 5);
}
