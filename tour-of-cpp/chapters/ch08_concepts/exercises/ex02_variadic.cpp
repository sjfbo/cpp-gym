// Exercise 8.2: Variadic Print Function
// Book reference: 8.4 Variadic Templates
// Difficulty: **
//
// Instructions:
// Implement variadic template functions using parameter packs and
// fold expressions. This exercise helps you understand how to work
// with variadic templates in modern C++.
//
// Tasks:
// 1. Implement print() - prints all arguments separated by spaces
// 2. Implement println() - like print() but adds a newline at the end
// 3. Implement print_lines() - prints each argument on its own line
// 4. Implement sum() - returns the sum of all arguments
// 5. Implement min() - returns the minimum of all arguments
// 6. Implement max() - returns the maximum of all arguments
// 7. Implement all_of() - returns true if all arguments are true
// 8. Implement any_of() - returns true if any argument is true
// 9. Implement count_args() - returns the number of arguments
// 10. Implement make_string() - concatenates all arguments into a string
//
// Expected behavior:
// - print(1, 2, 3) outputs: "1 2 3"
// - println("Hello", "World") outputs: "Hello World\n"
// - sum(1, 2, 3, 4) returns 10
// - min(5, 2, 8, 1) returns 1
// - all_of(true, true, false) returns false
//
// Hints:
// - Use sizeof...(args) to count arguments
// - Fold expressions: (args + ...), (... && args), ((std::cout << args), ...)
// - For min/max, use the comma operator in fold: ((result = std::min(result, args)), ...)
// - Use std::ostringstream for building strings

#include <concepts>
#include <iostream>
#include <sstream>
#include <string>

// ============================================================================
// TODO: Implement these variadic functions
// ============================================================================

// Task 1: print() - Print all arguments separated by spaces (no trailing newline)
// Example: print(1, 2.5, "hello") outputs "1 2.5 hello"
// template<typename... Args>
// void print(Args&&... args) {
//     ???
// }

// Task 2: println() - Like print() but adds a newline at the end
// Example: println(1, 2, 3) outputs "1 2 3\n"
// template<typename... Args>
// void println(Args&&... args) {
//     ???
// }

// Task 3: print_lines() - Print each argument on its own line
// Example: print_lines(1, 2, 3) outputs "1\n2\n3\n"
// template<typename... Args>
// void print_lines(Args&&... args) {
//     ???
// }

// Task 4: sum() - Return the sum of all arguments
// Example: sum(1, 2, 3, 4) returns 10
// Should work with empty argument list (return 0)
// template<typename... Args>
// auto sum(Args... args) {
//     ???
// }

// Task 5: min_value() - Return the minimum of all arguments
// Example: min_value(5, 2, 8, 1, 9) returns 1
// Hint: Initialize with first argument, use fold with comma operator
// template<typename T, typename... Rest>
// T min_value(T first, Rest... rest) {
//     ???
// }

// Task 6: max_value() - Return the maximum of all arguments
// Example: max_value(5, 2, 8, 1, 9) returns 9
// template<typename T, typename... Rest>
// T max_value(T first, Rest... rest) {
//     ???
// }

// Task 7: all_of() - Return true if all arguments are true
// Example: all_of(true, true, true) returns true
// Example: all_of(true, false, true) returns false
// Should return true for empty argument list
// template<typename... Args>
// constexpr bool all_of(Args... args) {
//     ???
// }

// Task 8: any_of() - Return true if any argument is true
// Example: any_of(false, true, false) returns true
// Example: any_of(false, false, false) returns false
// Should return false for empty argument list
// template<typename... Args>
// constexpr bool any_of(Args... args) {
//     ???
// }

// Task 9: count_args() - Return the number of arguments
// Example: count_args(1, 2, 3, 4, 5) returns 5
// Example: count_args() returns 0
// template<typename... Args>
// constexpr std::size_t count_args(Args...) {
//     ???
// }

// Task 10: make_string() - Concatenate all arguments into a string
// Example: make_string("Hello", ' ', "World", '!') returns "Hello World!"
// Example: make_string(2024, '-', 1, '-', 15) returns "2024-1-15"
// Hint: Use std::ostringstream
// template<typename... Args>
// std::string make_string(Args&&... args) {
//     ???
// }

// ============================================================================
// Test your implementation
// ============================================================================

void test_print_functions() {
    std::cout << "=== Testing Print Functions ===\n\n";

    // Uncomment as you implement each function

    // std::cout << "print(1, 2.5, \"hello\"): ";
    // print(1, 2.5, "hello");
    // std::cout << "\n";

    // std::cout << "println(\"Hello\", \"World\"): ";
    // println("Hello", "World");

    // std::cout << "print_lines(1, 2, 3):\n";
    // print_lines(1, 2, 3);

    std::cout << "\n";
}

void test_arithmetic_functions() {
    std::cout << "=== Testing Arithmetic Functions ===\n\n";

    // Uncomment as you implement each function

    // std::cout << "sum(1, 2, 3, 4, 5): " << sum(1, 2, 3, 4, 5) << " (expected: 15)\n";
    // std::cout << "sum(): " << sum() << " (expected: 0)\n";
    // std::cout << "sum(1.5, 2.5, 3.0): " << sum(1.5, 2.5, 3.0) << " (expected: 7)\n";

    // std::cout << "\nmin_value(5, 2, 8, 1, 9): " << min_value(5, 2, 8, 1, 9) << " (expected: 1)\n";
    // std::cout << "min_value(3.14, 2.71, 1.41): " << min_value(3.14, 2.71, 1.41) << " (expected: 1.41)\n";
    // std::cout << "min_value(42): " << min_value(42) << " (expected: 42)\n";

    // std::cout << "\nmax_value(5, 2, 8, 1, 9): " << max_value(5, 2, 8, 1, 9) << " (expected: 9)\n";
    // std::cout << "max_value(3.14, 2.71, 1.41): " << max_value(3.14, 2.71, 1.41) << " (expected: 3.14)\n";
    // std::cout << "max_value(42): " << max_value(42) << " (expected: 42)\n";

    std::cout << "\n";
}

void test_logical_functions() {
    std::cout << "=== Testing Logical Functions ===\n\n";

    // Uncomment as you implement each function

    // std::cout << "all_of(true, true, true): " << all_of(true, true, true) << " (expected: 1)\n";
    // std::cout << "all_of(true, false, true): " << all_of(true, false, true) << " (expected: 0)\n";
    // std::cout << "all_of(): " << all_of() << " (expected: 1)\n";

    // std::cout << "\nany_of(false, true, false): " << any_of(false, true, false) << " (expected: 1)\n";
    // std::cout << "any_of(false, false, false): " << any_of(false, false, false) << " (expected: 0)\n";
    // std::cout << "any_of(): " << any_of() << " (expected: 0)\n";

    std::cout << "\n";
}

void test_utility_functions() {
    std::cout << "=== Testing Utility Functions ===\n\n";

    // Uncomment as you implement each function

    // std::cout << "count_args(1, 2, 3, 4, 5): " << count_args(1, 2, 3, 4, 5) << " (expected: 5)\n";
    // std::cout << "count_args(): " << count_args() << " (expected: 0)\n";
    // std::cout << "count_args(\"a\", \"b\"): " << count_args("a", "b") << " (expected: 2)\n";

    // std::cout << "\nmake_string(\"Hello\", ' ', \"World\", '!'): \""
    //           << make_string("Hello", ' ', "World", '!') << "\" (expected: \"Hello World!\")\n";
    // std::cout << "make_string(2024, '-', 1, '-', 15): \""
    //           << make_string(2024, '-', 1, '-', 15) << "\" (expected: \"2024-1-15\")\n";
    // std::cout << "make_string(3.14): \"" << make_string(3.14) << "\" (expected: \"3.14\")\n";

    std::cout << "\n";
}

void verify_static_assertions() {
    std::cout << "=== Static Assertions ===\n";

    // Uncomment as you implement each function

    // sum static assertions
    // static_assert(sum(1, 2, 3) == 6);
    // static_assert(sum() == 0);

    // all_of static assertions
    // static_assert(all_of(true, true, true));
    // static_assert(!all_of(true, false, true));
    // static_assert(all_of());  // Empty should be true

    // any_of static assertions
    // static_assert(any_of(false, true, false));
    // static_assert(!any_of(false, false, false));
    // static_assert(!any_of());  // Empty should be false

    // count_args static assertions
    // static_assert(count_args(1, 2, 3, 4, 5) == 5);
    // static_assert(count_args() == 0);
    // static_assert(count_args("hello") == 1);

    std::cout << "All static assertions passed!\n\n";
}

int main() {
    std::cout << "Exercise 8.2: Variadic Print Function\n";
    std::cout << "======================================\n\n";

    test_print_functions();
    test_arithmetic_functions();
    test_logical_functions();
    test_utility_functions();
    verify_static_assertions();

    std::cout << "Implement the functions, then uncomment the tests!\n";

    return 0;
}
