/**
 * Chapter 4: Error Handling - Assertions
 *
 * Demonstrates:
 * - Runtime assertions with assert()
 * - Compile-time assertions with static_assert()
 * - When to use assertions vs exceptions
 * - Custom assertion macros
 * - C++20 source_location for better diagnostics
 */

#include <iostream>
#include <cassert>
#include <type_traits>
#include <string>
#include <vector>
#include <cmath>
#include <source_location>

// =============================================================================
// Basic assert() Usage
// =============================================================================

/**
 * Demonstrates basic runtime assertions.
 *
 * assert() is for catching programming errors during development.
 * - Disabled in release builds (when NDEBUG is defined)
 * - Should check conditions that should NEVER be false in correct code
 * - NOT for user input validation (use exceptions for that)
 */
void demonstrate_basic_assert() {
    std::cout << "=== Basic assert() ===\n\n";

    int x = 42;
    assert(x > 0);  // This passes
    std::cout << "Assertion x > 0 passed (x = " << x << ")\n";

    // Assertion with message (message appears in failure output)
    int* ptr = &x;
    assert(ptr != nullptr && "Pointer must not be null");
    std::cout << "Assertion ptr != nullptr passed\n";

    // Assertions in a function
    auto safe_divide = [](int a, int b) -> int {
        assert(b != 0 && "Division by zero is undefined");
        return a / b;
    };

    int result = safe_divide(10, 2);
    std::cout << "safe_divide(10, 2) = " << result << '\n';

    // Note: Uncommenting the following would abort in debug builds:
    // safe_divide(10, 0);  // Would trigger assertion failure

    std::cout << '\n';
}

// =============================================================================
// static_assert() - Compile-Time Checks
// =============================================================================

/**
 * static_assert performs compile-time checks.
 *
 * Use for:
 * - Type requirements in templates
 * - Platform/configuration assumptions
 * - Constant expression validation
 */

// Check platform assumptions
static_assert(sizeof(int) >= 4, "int must be at least 32 bits");
static_assert(sizeof(void*) >= 4, "Pointer size must be at least 32 bits");
static_assert(sizeof(long long) >= 8, "long long must be at least 64 bits");

// Check numeric limits
static_assert(std::numeric_limits<int>::max() >= 2'147'483'647,
              "int must support values up to 2^31-1");

// Check type properties
static_assert(std::is_same_v<std::int32_t, int> ||
              std::is_same_v<std::int32_t, long>,
              "int32_t must be either int or long");

/**
 * Template class with compile-time type requirements.
 */
template<typename T>
class NumericContainer {
    // Enforce type requirements at compile time
    static_assert(std::is_arithmetic_v<T>,
                  "NumericContainer requires an arithmetic type");

    static_assert(!std::is_same_v<T, bool>,
                  "NumericContainer does not support bool");

public:
    void add(T value) { data_.push_back(value); }

    T sum() const {
        T total = 0;
        for (const auto& val : data_) {
            total += val;
        }
        return total;
    }

private:
    std::vector<T> data_;
};

/**
 * Function template with constexpr requirements.
 */
template<size_t N>
constexpr auto create_array() {
    static_assert(N > 0, "Array size must be positive");
    static_assert(N <= 1000, "Array size must not exceed 1000");
    return std::array<int, N>{};
}

void demonstrate_static_assert() {
    std::cout << "=== static_assert() ===\n\n";

    std::cout << "All compile-time assertions passed!\n";
    std::cout << "sizeof(int) = " << sizeof(int) << '\n';
    std::cout << "sizeof(void*) = " << sizeof(void*) << '\n';
    std::cout << "sizeof(long long) = " << sizeof(long long) << '\n';

    // Use the template class
    NumericContainer<double> container;
    container.add(1.5);
    container.add(2.5);
    container.add(3.0);
    std::cout << "\nNumericContainer<double> sum: " << container.sum() << '\n';

    // These would cause compile-time errors:
    // NumericContainer<std::string> invalid1;  // Not arithmetic
    // NumericContainer<bool> invalid2;         // bool not supported
    // auto arr = create_array<0>();            // Size must be positive
    // auto arr = create_array<2000>();         // Size too large

    auto arr = create_array<5>();
    std::cout << "Created array of size " << arr.size() << '\n';

    std::cout << '\n';
}

// =============================================================================
// When to Use assert() vs Exceptions
// =============================================================================

/**
 * Demonstrates the distinction between assertions and exceptions.
 *
 * Use ASSERTIONS for:
 * - Internal logic errors (bugs in your code)
 * - Invariants that should never be violated
 * - Conditions that indicate programmer error
 *
 * Use EXCEPTIONS for:
 * - Recoverable errors (file not found, network failure)
 * - User input validation
 * - Conditions that can legitimately occur at runtime
 */

class Matrix {
public:
    Matrix(size_t rows, size_t cols)
        : rows_(rows), cols_(cols), data_(rows * cols, 0.0) {
        // Precondition: dimensions must be positive
        // This is a programming error if violated, so use assert
        assert(rows > 0 && "Matrix must have at least one row");
        assert(cols > 0 && "Matrix must have at least one column");
    }

    // Use assertion for index bounds - programming error to access out of bounds
    double& at_unchecked(size_t row, size_t col) {
        assert(row < rows_ && "Row index out of bounds");
        assert(col < cols_ && "Column index out of bounds");
        return data_[row * cols_ + col];
    }

    // Use exception for index bounds - user might provide bad indices
    double& at(size_t row, size_t col) {
        if (row >= rows_ || col >= cols_) {
            throw std::out_of_range(
                "Matrix index (" + std::to_string(row) + ", " +
                std::to_string(col) + ") out of bounds for " +
                std::to_string(rows_) + "x" + std::to_string(cols_) + " matrix"
            );
        }
        return data_[row * cols_ + col];
    }

    size_t rows() const { return rows_; }
    size_t cols() const { return cols_; }

private:
    size_t rows_;
    size_t cols_;
    std::vector<double> data_;
};

void demonstrate_assert_vs_exception() {
    std::cout << "=== assert() vs Exceptions ===\n\n";

    Matrix m(3, 4);
    std::cout << "Created 3x4 matrix\n";

    // Using unchecked access (trusts the programmer)
    m.at_unchecked(0, 0) = 1.0;
    m.at_unchecked(2, 3) = 2.0;
    std::cout << "Set m[0,0] = 1.0 and m[2,3] = 2.0 (unchecked)\n";

    // Using checked access (validates indices)
    try {
        m.at(0, 0) = 3.0;
        std::cout << "Set m[0,0] = 3.0 (checked)\n";

        m.at(10, 10) = 4.0;  // This will throw
    }
    catch (const std::out_of_range& e) {
        std::cerr << "Exception caught: " << e.what() << '\n';
    }

    std::cout << "\nGuidelines:\n";
    std::cout << "- Use assert() for internal invariants (bugs)\n";
    std::cout << "- Use exceptions for external errors (user input, I/O)\n";
    std::cout << "- at_unchecked() uses assert - crashes in debug if misused\n";
    std::cout << "- at() uses exceptions - always safe, slightly slower\n";

    std::cout << '\n';
}

// =============================================================================
// Custom Assertion Macros
// =============================================================================

/**
 * Custom assertion macro that provides better diagnostics.
 * Uses C++20 source_location.
 */
inline void assertion_failed(
    const char* expression,
    const char* message,
    const std::source_location& loc = std::source_location::current())
{
    std::cerr << "\n*** Assertion Failed ***\n"
              << "Expression: " << expression << '\n'
              << "Message: " << message << '\n'
              << "File: " << loc.file_name() << '\n'
              << "Line: " << loc.line() << '\n'
              << "Function: " << loc.function_name() << '\n';
    std::abort();
}

// Custom assertion that's always enabled (not disabled by NDEBUG)
#define ALWAYS_ASSERT(expr, msg) \
    ((expr) ? (void)0 : assertion_failed(#expr, msg))

// Debug-only assertion with source location
#ifdef NDEBUG
    #define DEBUG_ASSERT(expr, msg) ((void)0)
#else
    #define DEBUG_ASSERT(expr, msg) \
        ((expr) ? (void)0 : assertion_failed(#expr, msg))
#endif

// Assertion that logs but continues (for non-fatal issues)
#define SOFT_ASSERT(expr, msg) \
    if (!(expr)) { \
        std::cerr << "[Warning] Soft assertion failed: " << #expr \
                  << " - " << msg << '\n'; \
    }

/**
 * Function using custom assertions.
 */
double compute_log(double value) {
    // Use ALWAYS_ASSERT for critical preconditions
    // that should be checked even in release builds
    ALWAYS_ASSERT(value > 0, "Logarithm requires positive input");

    return std::log(value);
}

void demonstrate_custom_assertions() {
    std::cout << "=== Custom Assertion Macros ===\n\n";

    // Test soft assertion
    int* ptr = nullptr;
    SOFT_ASSERT(ptr != nullptr, "Expected non-null pointer");
    std::cout << "Soft assertion logged warning but continued\n";

    // Test compute_log with valid input
    double result = compute_log(10.0);
    std::cout << "log(10.0) = " << result << '\n';

    // Uncommenting the following would trigger ALWAYS_ASSERT:
    // compute_log(-5.0);  // Would abort with detailed message

    std::cout << '\n';
}

// =============================================================================
// C++20 source_location for Better Diagnostics
// =============================================================================

/**
 * Demonstrates C++20 source_location for automatic location capture.
 */
void log_with_location(
    const std::string& message,
    const std::source_location& location = std::source_location::current())
{
    std::cout << location.file_name() << ":"
              << location.line() << " ["
              << location.function_name() << "] "
              << message << '\n';
}

void check_condition(
    bool condition,
    const std::string& description,
    const std::source_location& location = std::source_location::current())
{
    if (!condition) {
        std::cerr << "Check failed at "
                  << location.file_name() << ":"
                  << location.line() << " in "
                  << location.function_name() << ": "
                  << description << '\n';
    }
}

void demonstrate_source_location() {
    std::cout << "=== C++20 source_location ===\n\n";

    log_with_location("Starting demonstration");

    int x = 42;
    check_condition(x > 0, "x must be positive");
    check_condition(x < 100, "x must be less than 100");
    check_condition(x == 0, "x must be zero");  // This will report failure

    log_with_location("Demonstration complete");

    std::cout << '\n';
}

// =============================================================================
// Assertions in constexpr Functions
// =============================================================================

/**
 * In C++20+, assertions can be used in constexpr functions.
 * They are evaluated at compile-time when the function is
 * evaluated at compile-time.
 */
constexpr int factorial(int n) {
    assert(n >= 0 && "Factorial requires non-negative input");
    if (n <= 1) return 1;
    return n * factorial(n - 1);
}

// static_assert with constexpr function
static_assert(factorial(5) == 120, "factorial(5) should be 120");
static_assert(factorial(0) == 1, "factorial(0) should be 1");

void demonstrate_constexpr_assertions() {
    std::cout << "=== Assertions in constexpr Functions ===\n\n";

    // Compile-time evaluation
    constexpr int f5 = factorial(5);
    std::cout << "factorial(5) = " << f5 << " (computed at compile-time)\n";

    // Runtime evaluation
    int n = 7;
    int f7 = factorial(n);
    std::cout << "factorial(7) = " << f7 << " (computed at runtime)\n";

    // The following would cause compile-time error (assertion in constexpr):
    // constexpr int f_neg = factorial(-1);

    // The following would abort at runtime:
    // factorial(-1);

    std::cout << '\n';
}

// =============================================================================
// Main
// =============================================================================

int main() {
    std::cout << "Chapter 4: Assertion Examples\n";
    std::cout << "=============================\n\n";

    demonstrate_basic_assert();
    demonstrate_static_assert();
    demonstrate_assert_vs_exception();
    demonstrate_custom_assertions();
    demonstrate_source_location();
    demonstrate_constexpr_assertions();

    std::cout << "All assertion demonstrations completed.\n";
    return 0;
}
