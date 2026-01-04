// Variadic Templates - Parameter Packs and Fold Expressions
// Book reference: 8.4 Variadic Templates
//
// This example demonstrates variadic templates, parameter packs,
// sizeof..., fold expressions, and perfect forwarding with variadic args.

#include <concepts>
#include <cstddef>
#include <functional>
#include <iostream>
#include <sstream>
#include <string>
#include <tuple>
#include <type_traits>
#include <utility>
#include <vector>

// ============================================================================
// Basic variadic templates
// ============================================================================

// Count the number of template arguments
template<typename... Ts>
constexpr std::size_t count_types() {
    return sizeof...(Ts);
}

// Count the number of function arguments
template<typename... Args>
constexpr std::size_t count_args(Args...) {
    return sizeof...(Args);
}

// ============================================================================
// Recursive variadic templates (pre-C++17 style)
// ============================================================================

// Base case: no arguments
void print_recursive() {
    std::cout << "\n";
}

// Recursive case: process first, then rest
template<typename T, typename... Rest>
void print_recursive(const T& first, const Rest&... rest) {
    std::cout << first;
    if constexpr (sizeof...(rest) > 0) {
        std::cout << ", ";
    }
    print_recursive(rest...);
}

// ============================================================================
// Fold expressions (C++17/20)
// ============================================================================

// Unary right fold: (args op ...)
// Expands to: arg1 op (arg2 op (arg3 op ...))
template<typename... Args>
auto sum_right(Args... args) {
    return (args + ...);
}

// Unary left fold: (... op args)
// Expands to: ((arg1 op arg2) op arg3) op ...
template<typename... Args>
auto sum_left(Args... args) {
    return (... + args);
}

// Binary right fold with init: (init op ... op args)
template<typename... Args>
constexpr auto sum_with_init(Args... args) {
    return (0 + ... + args);  // Works even with empty pack
}

// Binary left fold with init: (args op ... op init)
template<typename... Args>
auto product(Args... args) {
    return (args * ... * 1);  // Works even with empty pack
}

// ============================================================================
// Common fold expression patterns
// ============================================================================

// Print all arguments using comma fold
template<typename... Args>
void print_fold(Args&&... args) {
    ((std::cout << args << ' '), ...);
    std::cout << '\n';
}

// Print with separator using fold
template<typename... Args>
void print_with_sep(const std::string& sep, Args&&... args) {
    std::size_t n = 0;
    ((std::cout << args << (++n < sizeof...(args) ? sep : "")), ...);
    std::cout << '\n';
}

// Check if all arguments are true (logical AND)
template<typename... Args>
constexpr bool all_true(Args... args) {
    return (... && args);
}

// Check if any argument is true (logical OR)
template<typename... Args>
constexpr bool any_true(Args... args) {
    return (... || args);
}

// Check if all types satisfy a concept
template<typename... Ts>
constexpr bool all_integral() {
    return (std::integral<Ts> && ...);
}

// ============================================================================
// Variadic templates with concepts
// ============================================================================

// Constrain all arguments to be integral
template<std::integral... Args>
auto sum_integral(Args... args) {
    return (args + ... + 0);
}

// Constrain all arguments to be the same type
template<typename T, std::same_as<T>... Rest>
auto sum_same_type(T first, Rest... rest) {
    return (first + ... + rest);
}

// Concept for printable types
template<typename T>
concept Printable = requires(std::ostream& os, const T& val) {
    { os << val } -> std::same_as<std::ostream&>;
};

// Constrain all arguments to be printable
template<Printable... Args>
void print_all(Args&&... args) {
    ((std::cout << args << ' '), ...);
    std::cout << '\n';
}

// ============================================================================
// Perfect forwarding with variadic templates
// ============================================================================

// Factory function that forwards arguments to constructor
template<typename T, typename... Args>
T make_object(Args&&... args) {
    return T(std::forward<Args>(args)...);
}

// Wrapper that forwards to another function
template<typename Func, typename... Args>
    requires std::invocable<Func, Args...>
auto invoke_and_log(Func&& func, Args&&... args) {
    std::cout << "Invoking function with " << sizeof...(args) << " arguments\n";
    return std::invoke(std::forward<Func>(func), std::forward<Args>(args)...);
}

// ============================================================================
// Building strings with variadic templates
// ============================================================================

// Concatenate all arguments into a string
template<typename... Args>
std::string concat(Args&&... args) {
    std::ostringstream oss;
    ((oss << args), ...);
    return oss.str();
}

// Join with delimiter
template<typename... Args>
std::string join(const std::string& delim, Args&&... args) {
    std::ostringstream oss;
    std::size_t n = 0;
    ((oss << args << (++n < sizeof...(args) ? delim : "")), ...);
    return oss.str();
}

// ============================================================================
// Working with tuples and parameter packs
// ============================================================================

// Apply function to each element (using index sequence)
template<typename Func, typename Tuple, std::size_t... Is>
void for_each_impl(Func&& func, Tuple&& tuple, std::index_sequence<Is...>) {
    (func(std::get<Is>(std::forward<Tuple>(tuple))), ...);
}

template<typename Func, typename Tuple>
void for_each_in_tuple(Func&& func, Tuple&& tuple) {
    constexpr auto size = std::tuple_size_v<std::remove_reference_t<Tuple>>;
    for_each_impl(std::forward<Func>(func), std::forward<Tuple>(tuple),
                  std::make_index_sequence<size>{});
}

// Create tuple from variadic args
template<typename... Args>
auto make_my_tuple(Args&&... args) {
    return std::tuple{std::forward<Args>(args)...};
}

// ============================================================================
// Type list operations
// ============================================================================

// Check if a type is in a pack
template<typename T, typename... Ts>
constexpr bool is_one_of() {
    return (std::same_as<T, Ts> || ...);
}

// Count how many types in pack satisfy a concept
template<template<typename> class Concept, typename... Ts>
constexpr std::size_t count_satisfying() {
    return (static_cast<std::size_t>(Concept<Ts>::value) + ...);
}

// ============================================================================
// Practical examples
// ============================================================================

// Min of variadic arguments
template<std::totally_ordered T, std::same_as<T>... Rest>
T min_of(T first, Rest... rest) {
    T result = first;
    ((result = rest < result ? rest : result), ...);
    return result;
}

// Max of variadic arguments
template<std::totally_ordered T, std::same_as<T>... Rest>
T max_of(T first, Rest... rest) {
    T result = first;
    ((result = rest > result ? rest : result), ...);
    return result;
}

// Push multiple elements to a vector
template<typename T, typename... Args>
    requires (std::convertible_to<Args, T> && ...)
void push_all(std::vector<T>& vec, Args&&... args) {
    (vec.push_back(std::forward<Args>(args)), ...);
}

// ============================================================================
// Main demonstration
// ============================================================================

int main() {
    std::cout << "=== Variadic Templates ===\n\n";

    // sizeof...
    std::cout << "--- sizeof... ---\n";
    std::cout << "count_types<int, double, char>(): "
              << count_types<int, double, char>() << "\n";
    std::cout << "count_args(1, 2.0, 'a', \"hello\"): "
              << count_args(1, 2.0, 'a', "hello") << "\n";

    // Recursive print
    std::cout << "\n--- Recursive Print ---\n";
    std::cout << "print_recursive(1, 2.5, \"hello\", 'x'): ";
    print_recursive(1, 2.5, "hello", 'x');

    // Fold expressions
    std::cout << "\n--- Fold Expressions ---\n";
    std::cout << "sum_right(1, 2, 3, 4, 5): " << sum_right(1, 2, 3, 4, 5) << "\n";
    std::cout << "sum_left(1, 2, 3, 4, 5): " << sum_left(1, 2, 3, 4, 5) << "\n";
    std::cout << "sum_with_init(): " << sum_with_init() << " (empty pack)\n";
    std::cout << "sum_with_init(1, 2, 3): " << sum_with_init(1, 2, 3) << "\n";
    std::cout << "product(2, 3, 4): " << product(2, 3, 4) << "\n";
    std::cout << "product(): " << product() << " (empty pack = 1)\n";

    // Print variations
    std::cout << "\n--- Print Variations ---\n";
    std::cout << "print_fold: ";
    print_fold(1, 2.5, "hello", 'x');
    std::cout << "print_with_sep(\", \"): ";
    print_with_sep(", ", 1, 2, 3, 4, 5);

    // Logical folds
    std::cout << "\n--- Logical Folds ---\n";
    std::cout << "all_true(true, true, true): " << all_true(true, true, true) << "\n";
    std::cout << "all_true(true, false, true): " << all_true(true, false, true) << "\n";
    std::cout << "any_true(false, false, true): " << any_true(false, false, true) << "\n";
    std::cout << "any_true(false, false, false): " << any_true(false, false, false) << "\n";

    // Concept-constrained variadic
    std::cout << "\n--- Concept-Constrained Variadic ---\n";
    std::cout << "sum_integral(1, 2, 3, 4, 5): " << sum_integral(1, 2, 3, 4, 5) << "\n";
    std::cout << "sum_same_type(1.0, 2.0, 3.0): " << sum_same_type(1.0, 2.0, 3.0) << "\n";
    std::cout << "all_integral<int, long, short>(): "
              << all_integral<int, long, short>() << "\n";
    std::cout << "all_integral<int, double>(): "
              << all_integral<int, double>() << "\n";

    // Perfect forwarding
    std::cout << "\n--- Perfect Forwarding ---\n";
    auto str = make_object<std::string>(5, 'x');
    std::cout << "make_object<std::string>(5, 'x'): \"" << str << "\"\n";

    auto result = invoke_and_log([](int a, int b) { return a + b; }, 3, 4);
    std::cout << "Result: " << result << "\n";

    // String operations
    std::cout << "\n--- String Operations ---\n";
    std::cout << "concat(\"Hello\", ' ', \"World\", '!'): "
              << concat("Hello", ' ', "World", '!') << "\n";
    std::cout << "join(\"-\", 2024, 1, 15): " << join("-", 2024, 1, 15) << "\n";

    // Tuple operations
    std::cout << "\n--- Tuple Operations ---\n";
    auto my_tuple = make_my_tuple(1, "hello", 3.14);
    std::cout << "for_each_in_tuple: ";
    for_each_in_tuple([](const auto& x) { std::cout << x << ' '; }, my_tuple);
    std::cout << "\n";

    // Type checks
    std::cout << "\n--- Type Pack Operations ---\n";
    std::cout << "is_one_of<int, char, int, double>(): "
              << is_one_of<int, char, int, double>() << "\n";
    std::cout << "is_one_of<float, char, int, double>(): "
              << is_one_of<float, char, int, double>() << "\n";

    // Practical examples
    std::cout << "\n--- Practical Examples ---\n";
    std::cout << "min_of(5, 2, 8, 1, 9): " << min_of(5, 2, 8, 1, 9) << "\n";
    std::cout << "max_of(5, 2, 8, 1, 9): " << max_of(5, 2, 8, 1, 9) << "\n";

    std::vector<int> vec;
    push_all(vec, 1, 2, 3, 4, 5);
    std::cout << "push_all(vec, 1, 2, 3, 4, 5): [";
    for (std::size_t i = 0; i < vec.size(); ++i) {
        if (i > 0) std::cout << ", ";
        std::cout << vec[i];
    }
    std::cout << "]\n";

    // Compile-time checks
    std::cout << "\n=== Compile-Time Checks ===\n";
    static_assert(count_types<int, double, char>() == 3);
    static_assert(sum_with_init(1, 2, 3) == 6);
    static_assert(all_true(true, true, true));
    static_assert(!all_true(true, false));
    static_assert(any_true(false, true, false));
    static_assert(all_integral<int, long, short>());
    static_assert(!all_integral<int, double>());
    static_assert(is_one_of<int, char, int, double>());
    static_assert(!is_one_of<float, char, int, double>());
    std::cout << "All compile-time checks passed!\n";

    return 0;
}
