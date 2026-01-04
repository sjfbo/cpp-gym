// Exercise 8.1: Custom Concepts for Numeric Types
// Book reference: 8.2.4 Definition of Concepts
// Difficulty: **
//
// Instructions:
// Define custom concepts for numeric types and use them to constrain
// template functions. This exercise helps you understand how to create
// concepts using requires expressions and compound requirements.
//
// Tasks:
// 1. Define a concept 'Numeric' that accepts integral or floating-point types
// 2. Define a concept 'Addable' that checks if a type supports the + operator
// 3. Define a concept 'Comparable' that checks for ==, !=, <, >, <=, >= operators
// 4. Define a concept 'Arithmetic' that combines addable with -, *, / operators
// 5. Define a concept 'NumberLike' with compound requirements checking return types
// 6. Implement constrained template functions using your concepts
//
// Expected behavior:
// - Numeric<int> == true, Numeric<double> == true, Numeric<std::string> == false
// - Addable<std::string> == true (strings can be added)
// - Comparable<int> == true
// - All test assertions should pass
//
// Hints:
// - Use std::integral and std::floating_point from <concepts>
// - Requires expressions: requires(T a, T b) { a + b; }
// - Compound requirements: { expr } -> std::convertible_to<Type>
// - Combine concepts with && and ||

#include <concepts>
#include <iostream>
#include <string>
#include <type_traits>

// ============================================================================
// TODO: Define your concepts here
// ============================================================================

// Task 1: Numeric concept
// A type is Numeric if it is either integral or floating-point
// template<typename T>
// concept Numeric = ???;

// Task 2: Addable concept
// A type is Addable if you can add two values of that type
// template<typename T>
// concept Addable = ???;

// Task 3: Comparable concept
// A type is Comparable if it supports all comparison operators
// template<typename T>
// concept Comparable = ???;

// Task 4: Arithmetic concept
// A type is Arithmetic if it supports +, -, *, / operators
// template<typename T>
// concept Arithmetic = ???;

// Task 5: NumberLike concept
// A type is NumberLike if:
// - It is default constructible
// - It supports +, -, *, / and the results are convertible back to T
// - It is totally ordered
// template<typename T>
// concept NumberLike = ???;

// ============================================================================
// TODO: Implement these concept-constrained functions
// ============================================================================

// Task 6a: Implement a function that returns the absolute value
// Constrain it to work only with Numeric types
// template<???>
// ??? abs_value(T x) {
//     ???
// }

// Task 6b: Implement a function that clamps a value between min and max
// Constrain it to work only with Comparable types
// template<???>
// T clamp(T value, T min_val, T max_val) {
//     ???
// }

// Task 6c: Implement a function that computes the average of two values
// Constrain it to work only with Arithmetic types
// template<???>
// T average(T a, T b) {
//     ???
// }

// ============================================================================
// Test your implementation
// ============================================================================

void test_concepts() {
    std::cout << "=== Testing Concepts ===\n\n";

    // Uncomment these tests as you implement each concept

    // Test Numeric concept
    // std::cout << "--- Numeric Concept ---\n";
    // std::cout << "Numeric<int>: " << Numeric<int> << " (expected: 1)\n";
    // std::cout << "Numeric<double>: " << Numeric<double> << " (expected: 1)\n";
    // std::cout << "Numeric<float>: " << Numeric<float> << " (expected: 1)\n";
    // std::cout << "Numeric<long>: " << Numeric<long> << " (expected: 1)\n";
    // std::cout << "Numeric<std::string>: " << Numeric<std::string> << " (expected: 0)\n";
    // std::cout << "Numeric<char*>: " << Numeric<char*> << " (expected: 0)\n";

    // Test Addable concept
    // std::cout << "\n--- Addable Concept ---\n";
    // std::cout << "Addable<int>: " << Addable<int> << " (expected: 1)\n";
    // std::cout << "Addable<std::string>: " << Addable<std::string> << " (expected: 1)\n";
    // std::cout << "Addable<double>: " << Addable<double> << " (expected: 1)\n";

    // Test Comparable concept
    // std::cout << "\n--- Comparable Concept ---\n";
    // std::cout << "Comparable<int>: " << Comparable<int> << " (expected: 1)\n";
    // std::cout << "Comparable<double>: " << Comparable<double> << " (expected: 1)\n";
    // std::cout << "Comparable<std::string>: " << Comparable<std::string> << " (expected: 1)\n";

    // Test Arithmetic concept
    // std::cout << "\n--- Arithmetic Concept ---\n";
    // std::cout << "Arithmetic<int>: " << Arithmetic<int> << " (expected: 1)\n";
    // std::cout << "Arithmetic<double>: " << Arithmetic<double> << " (expected: 1)\n";
    // std::cout << "Arithmetic<std::string>: " << Arithmetic<std::string> << " (expected: 0)\n";

    // Test NumberLike concept
    // std::cout << "\n--- NumberLike Concept ---\n";
    // std::cout << "NumberLike<int>: " << NumberLike<int> << " (expected: 1)\n";
    // std::cout << "NumberLike<double>: " << NumberLike<double> << " (expected: 1)\n";
    // std::cout << "NumberLike<std::string>: " << NumberLike<std::string> << " (expected: 0)\n";

    std::cout << "\n";
}

void test_functions() {
    std::cout << "=== Testing Functions ===\n\n";

    // Uncomment these tests as you implement each function

    // Test abs_value
    // std::cout << "--- abs_value ---\n";
    // std::cout << "abs_value(-5): " << abs_value(-5) << " (expected: 5)\n";
    // std::cout << "abs_value(3.14): " << abs_value(3.14) << " (expected: 3.14)\n";
    // std::cout << "abs_value(-2.5f): " << abs_value(-2.5f) << " (expected: 2.5)\n";

    // Test clamp
    // std::cout << "\n--- clamp ---\n";
    // std::cout << "clamp(5, 0, 10): " << clamp(5, 0, 10) << " (expected: 5)\n";
    // std::cout << "clamp(-5, 0, 10): " << clamp(-5, 0, 10) << " (expected: 0)\n";
    // std::cout << "clamp(15, 0, 10): " << clamp(15, 0, 10) << " (expected: 10)\n";
    // std::cout << "clamp(3.5, 1.0, 5.0): " << clamp(3.5, 1.0, 5.0) << " (expected: 3.5)\n";

    // Test average
    // std::cout << "\n--- average ---\n";
    // std::cout << "average(4, 6): " << average(4, 6) << " (expected: 5)\n";
    // std::cout << "average(3.0, 5.0): " << average(3.0, 5.0) << " (expected: 4)\n";

    std::cout << "\n";
}

void verify_static_assertions() {
    std::cout << "=== Static Assertions ===\n";

    // Uncomment these as you implement each concept

    // Numeric concept
    // static_assert(Numeric<int>);
    // static_assert(Numeric<double>);
    // static_assert(Numeric<float>);
    // static_assert(Numeric<long long>);
    // static_assert(Numeric<unsigned>);
    // static_assert(!Numeric<std::string>);
    // static_assert(!Numeric<void*>);

    // Addable concept
    // static_assert(Addable<int>);
    // static_assert(Addable<double>);
    // static_assert(Addable<std::string>);

    // Comparable concept
    // static_assert(Comparable<int>);
    // static_assert(Comparable<double>);
    // static_assert(Comparable<std::string>);

    // Arithmetic concept
    // static_assert(Arithmetic<int>);
    // static_assert(Arithmetic<double>);
    // static_assert(!Arithmetic<std::string>);

    // NumberLike concept
    // static_assert(NumberLike<int>);
    // static_assert(NumberLike<double>);
    // static_assert(!NumberLike<std::string>);

    std::cout << "All static assertions passed!\n\n";
}

int main() {
    std::cout << "Exercise 8.1: Custom Concepts for Numeric Types\n";
    std::cout << "================================================\n\n";

    test_concepts();
    test_functions();
    verify_static_assertions();

    std::cout << "Implement the concepts and functions, then uncomment the tests!\n";

    return 0;
}
