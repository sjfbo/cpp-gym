// Concepts Introduction - Using Standard Library Concepts
// Book reference: 8.1 Introduction, 8.2 Concepts
//
// C++20 concepts provide a way to constrain templates at compile time,
// producing clearer error messages and self-documenting code. The
// standard library provides many useful concepts in <concepts>.

#include <concepts>
#include <iostream>
#include <string>
#include <type_traits>
#include <vector>

// ============================================================================
// Basic concept constraints using standard concepts
// ============================================================================

// Using std::integral - constrains T to be an integral type
template<std::integral T>
T square(T x) {
    return x * x;
}

// Using std::floating_point - constrains T to be a floating-point type
template<std::floating_point T>
T half(T x) {
    return x / T(2);  // Use T(2) to avoid implicit conversion
}

// ============================================================================
// Multiple constraint syntax options
// ============================================================================

// Option 1: Constraint in template parameter (most common)
template<std::signed_integral T>
T negate_v1(T x) {
    return -x;
}

// Option 2: Requires clause after template declaration
template<typename T>
    requires std::signed_integral<T>
T negate_v2(T x) {
    return -x;
}

// Option 3: Trailing requires clause
template<typename T>
T negate_v3(T x) requires std::signed_integral<T> {
    return -x;
}

// Option 4: Abbreviated function template (C++20)
// The 'auto' with concept creates an implicit template
auto negate_v4(std::signed_integral auto x) {
    return -x;
}

// ============================================================================
// Combining concepts with logical operators
// ============================================================================

// A type that is either integral or floating-point
template<typename T>
    requires std::integral<T> || std::floating_point<T>
T absolute(T x) {
    return x < 0 ? -x : x;
}

// Multiple constraints combined with &&
template<typename T>
    requires std::integral<T> && std::signed_integral<T>
T safe_negate(T x) {
    // This is redundant (signed_integral implies integral)
    // but demonstrates && syntax
    return -x;
}

// ============================================================================
// Standard library concepts overview
// ============================================================================

// Demonstrate various standard concepts
void demonstrate_standard_concepts() {
    std::cout << "=== Standard Library Concepts ===\n\n";

    // Core language concepts
    std::cout << "--- Core Language Concepts ---\n";
    std::cout << "std::same_as<int, int>: " << std::same_as<int, int> << "\n";
    std::cout << "std::same_as<int, long>: " << std::same_as<int, long> << "\n";
    std::cout << "std::derived_from<std::string, std::string>: "
              << std::derived_from<std::string, std::string> << "\n";
    std::cout << "std::convertible_to<int, double>: "
              << std::convertible_to<int, double> << "\n";
    std::cout << "std::convertible_to<double, int>: "
              << std::convertible_to<double, int> << " (narrowing!)\n";

    std::cout << "\n--- Arithmetic Concepts ---\n";
    std::cout << "std::integral<int>: " << std::integral<int> << "\n";
    std::cout << "std::integral<double>: " << std::integral<double> << "\n";
    std::cout << "std::floating_point<double>: " << std::floating_point<double> << "\n";
    std::cout << "std::signed_integral<int>: " << std::signed_integral<int> << "\n";
    std::cout << "std::unsigned_integral<unsigned>: "
              << std::unsigned_integral<unsigned> << "\n";

    std::cout << "\n--- Comparison Concepts ---\n";
    std::cout << "std::equality_comparable<int>: "
              << std::equality_comparable<int> << "\n";
    std::cout << "std::totally_ordered<int>: " << std::totally_ordered<int> << "\n";
    std::cout << "std::totally_ordered<std::string>: "
              << std::totally_ordered<std::string> << "\n";

    std::cout << "\n--- Object Concepts ---\n";
    std::cout << "std::copyable<int>: " << std::copyable<int> << "\n";
    std::cout << "std::movable<std::string>: " << std::movable<std::string> << "\n";
    std::cout << "std::default_initializable<int>: "
              << std::default_initializable<int> << "\n";
    std::cout << "std::regular<int>: " << std::regular<int> << "\n";
}

// ============================================================================
// Concept-based function selection
// ============================================================================

// Different implementations based on type category
void process(std::integral auto value) {
    std::cout << "Processing integral: " << value << "\n";
}

void process(std::floating_point auto value) {
    std::cout << "Processing floating-point: " << value << "\n";
}

// ============================================================================
// Demonstrating improved error messages
// ============================================================================

// This function only works with integral types
template<std::integral T>
T divide_int(T a, T b) {
    return a / b;  // Integer division
}

// Without concepts, calling divide_int(3.14, 2.0) would produce
// a long, confusing error. With concepts, you get:
// "error: constraints not satisfied"
// with clear indication that std::integral<double> is false

// ============================================================================
// Main demonstration
// ============================================================================

int main() {
    std::cout << "=== Concepts Introduction ===\n\n";

    // Basic usage
    std::cout << "--- Basic Concept Usage ---\n";
    std::cout << "square(5): " << square(5) << "\n";
    std::cout << "square(3L): " << square(3L) << "\n";
    std::cout << "half(10.0): " << half(10.0) << "\n";
    std::cout << "half(5.5f): " << half(5.5f) << "\n";

    // All four syntax options work the same
    std::cout << "\n--- Different Syntax Options ---\n";
    std::cout << "negate_v1(-42): " << negate_v1(-42) << "\n";
    std::cout << "negate_v2(-42): " << negate_v2(-42) << "\n";
    std::cout << "negate_v3(-42): " << negate_v3(-42) << "\n";
    std::cout << "negate_v4(-42): " << negate_v4(-42) << "\n";

    // Combined concepts
    std::cout << "\n--- Combined Concepts ---\n";
    std::cout << "absolute(-5): " << absolute(-5) << "\n";
    std::cout << "absolute(-3.14): " << absolute(-3.14) << "\n";
    std::cout << "absolute(42u): " << absolute(42u) << "\n";

    // Concept-based overloading
    std::cout << "\n--- Concept-Based Overloading ---\n";
    process(42);
    process(3.14);
    process(100L);
    process(2.5f);

    // Integer division
    std::cout << "\n--- Integer Division ---\n";
    std::cout << "divide_int(17, 5): " << divide_int(17, 5) << "\n";
    std::cout << "divide_int(20L, 3L): " << divide_int(20L, 3L) << "\n";

    // Standard concepts demonstration
    std::cout << "\n";
    demonstrate_standard_concepts();

    // Compile-time concept checks
    std::cout << "\n=== Compile-Time Checks ===\n";
    static_assert(std::integral<int>, "int should be integral");
    static_assert(std::floating_point<double>, "double should be floating_point");
    static_assert(!std::integral<double>, "double should not be integral");
    static_assert(std::signed_integral<int>, "int should be signed_integral");
    static_assert(!std::signed_integral<unsigned>, "unsigned should not be signed");
    std::cout << "All static_assert checks passed!\n";

    return 0;
}
