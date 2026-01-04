// Constants - const and constexpr
// Book reference: 1.6 Constants
//
// C++ provides two ways to define constants:
// - const: runtime constant (value computed at runtime but cannot change)
// - constexpr: compile-time constant (value must be computable at compile time)
//
// Modern C++ encourages using constexpr whenever possible.

#include <array>
#include <cmath>
#include <iostream>

// ----------------------------------------------------------------------------
// constexpr variables - evaluated at compile time
// ----------------------------------------------------------------------------

constexpr double PI = 3.14159265358979323846;
constexpr int MAX_SIZE = 100;
constexpr auto GREETING = "Hello, World!";  // const char*

// ----------------------------------------------------------------------------
// constexpr functions - can be evaluated at compile time
// ----------------------------------------------------------------------------

constexpr int square(int x) {
    return x * x;
}

constexpr int factorial(int n) {
    return (n <= 1) ? 1 : n * factorial(n - 1);
}

// More complex constexpr function (C++14 allows loops)
constexpr int fibonacci(int n) {
    if (n <= 1) return n;

    int a = 0;
    int b = 1;
    for (int i = 2; i <= n; ++i) {
        int temp = a + b;
        a = b;
        b = temp;
    }
    return b;
}

// constexpr with arrays
constexpr std::array<int, 5> make_squares() {
    std::array<int, 5> result{};
    for (int i = 0; i < 5; ++i) {
        result[static_cast<size_t>(i)] = i * i;
    }
    return result;
}

// ----------------------------------------------------------------------------
// consteval (C++20) - MUST be evaluated at compile time
// ----------------------------------------------------------------------------

consteval int must_be_compile_time(int x) {
    return x * x;
}

// ----------------------------------------------------------------------------
// constinit (C++20) - ensures compile-time initialization of static/global
// ----------------------------------------------------------------------------

// constinit int global_val = 42;  // Guaranteed compile-time init

// ----------------------------------------------------------------------------
// Main function
// ----------------------------------------------------------------------------

int main() {
    std::cout << "=== constexpr Variables ===\n";

    std::cout << "PI = " << PI << "\n";
    std::cout << "MAX_SIZE = " << MAX_SIZE << "\n";
    std::cout << "GREETING = " << GREETING << "\n";

    // constexpr values can be used in compile-time contexts
    std::array<int, MAX_SIZE> large_array{};  // Size known at compile time
    std::cout << "Array size (compile-time): " << large_array.size() << "\n";

    std::cout << "\n=== constexpr Functions ===\n";

    // Evaluated at compile time (used in constexpr context)
    constexpr int sq5 = square(5);
    std::cout << "square(5) [compile-time] = " << sq5 << "\n";

    // Can also be called at runtime
    int n = 7;
    int sq7 = square(n);  // Runtime call
    std::cout << "square(7) [runtime] = " << sq7 << "\n";

    // Factorial at compile time
    constexpr int fact5 = factorial(5);
    std::cout << "factorial(5) [compile-time] = " << fact5 << "\n";

    // Fibonacci at compile time
    constexpr int fib10 = fibonacci(10);
    std::cout << "fibonacci(10) [compile-time] = " << fib10 << "\n";

    // Array of squares computed at compile time
    constexpr auto squares = make_squares();
    std::cout << "Compile-time squares: ";
    for (int val : squares) {
        std::cout << val << " ";
    }
    std::cout << "\n";

    std::cout << "\n=== const Variables ===\n";

    // const: cannot be modified after initialization
    [[maybe_unused]] const int x = 42;
    // x = 50;  // ERROR: cannot modify const

    // const with runtime value
    int input = 10;  // Could come from user input
    const int doubled = input * 2;  // const but not constexpr
    std::cout << "doubled (const, runtime) = " << doubled << "\n";

    // const references prevent modification
    int value = 100;
    const int& cref = value;
    // cref = 200;  // ERROR: cannot modify through const reference
    std::cout << "const ref: " << cref << "\n";

    std::cout << "\n=== const vs constexpr ===\n";

    // const: value fixed after initialization, but may be computed at runtime
    const double runtime_const = std::sqrt(2.0);  // Computed at runtime
    std::cout << "runtime_const (const) = " << runtime_const << "\n";

    // constexpr: value MUST be computable at compile time
    constexpr double compile_const = 1.41421356237;  // Must be literal
    std::cout << "compile_const (constexpr) = " << compile_const << "\n";

    // constexpr double bad = std::sqrt(2.0);  // ERROR: sqrt not constexpr

    std::cout << "\n=== consteval (C++20) ===\n";

    // consteval function MUST be evaluated at compile time
    constexpr int ce = must_be_compile_time(5);
    std::cout << "must_be_compile_time(5) = " << ce << "\n";

    // int runtime_n = 5;
    // int result = must_be_compile_time(runtime_n);  // ERROR: requires compile-time arg

    std::cout << "\n=== Practical Uses ===\n";

    // 1. Array sizes
    constexpr size_t BUFFER_SIZE = 1024;
    std::array<char, BUFFER_SIZE> buffer{};
    std::cout << "Buffer size: " << buffer.size() << "\n";

    // 2. Template arguments
    std::array<int, square(3)> nine_elements{};  // 9 elements
    std::cout << "Array with square(3) elements: " << nine_elements.size() << "\n";

    // 3. switch case labels (must be compile-time)
    constexpr int OPTION_A = 1;
    constexpr int OPTION_B = 2;

    int choice = 1;
    switch (choice) {
        case OPTION_A:
            std::cout << "Selected option A\n";
            break;
        case OPTION_B:
            std::cout << "Selected option B\n";
            break;
    }

    std::cout << "\n=== const Correctness Summary ===\n";
    std::cout << "Use 'const' for:\n";
    std::cout << "  - Values that shouldn't change after initialization\n";
    std::cout << "  - Reference parameters you won't modify\n";
    std::cout << "  - Member functions that don't modify object state\n";
    std::cout << "\nUse 'constexpr' for:\n";
    std::cout << "  - Values computable at compile time\n";
    std::cout << "  - Functions that can be evaluated at compile time\n";
    std::cout << "  - Template parameters, array sizes, case labels\n";
    std::cout << "\nUse 'consteval' for:\n";
    std::cout << "  - Functions that MUST be evaluated at compile time\n";

    return 0;
}
