// Functions - The Building Blocks of Programs
// Book reference: 1.3 Functions
//
// Functions encapsulate computation. They take arguments, perform
// operations, and return results. C++ supports overloading, default
// arguments, and various parameter passing modes.

#include <iostream>

// ----------------------------------------------------------------------------
// Basic function declaration and definition
// ----------------------------------------------------------------------------

// Declaration (prototype) - tells compiler the function exists
double square(double x);

// Definition - provides the implementation
double square(double x) {
    return x * x;
}

// ----------------------------------------------------------------------------
// Function overloading - same name, different parameter types
// ----------------------------------------------------------------------------

// Overload for int
int multiply(int a, int b) {
    return a * b;
}

// Overload for double
double multiply(double a, double b) {
    return a * b;
}

// Overload with different number of parameters
int multiply(int a, int b, int c) {
    return a * b * c;
}

// ----------------------------------------------------------------------------
// Default arguments
// ----------------------------------------------------------------------------

// Default arguments must come last in parameter list
void greet(const std::string& name, const std::string& greeting = "Hello") {
    std::cout << greeting << ", " << name << "!\n";
}

// ----------------------------------------------------------------------------
// Pass by value vs pass by reference
// ----------------------------------------------------------------------------

// Pass by value - copies the argument
void increment_value(int x) {
    x++;  // Modifies local copy, original unchanged
    std::cout << "Inside increment_value: x = " << x << "\n";
}

// Pass by reference - modifies the original
void increment_ref(int& x) {
    x++;  // Modifies the original variable
    std::cout << "Inside increment_ref: x = " << x << "\n";
}

// Pass by const reference - read-only access, no copy
void print_value(const int& x) {
    std::cout << "Value: " << x << "\n";
    // x++;  // ERROR: cannot modify const reference
}

// ----------------------------------------------------------------------------
// Returning values
// ----------------------------------------------------------------------------

// Return by value (most common for small types)
int add(int a, int b) {
    return a + b;
}

// Never return a reference to a local variable! (undefined behavior)
// int& bad_function() {
//     int local = 42;
//     return local;  // DANGER: local is destroyed, reference is dangling!
// }

// ----------------------------------------------------------------------------
// constexpr functions - can be evaluated at compile time
// ----------------------------------------------------------------------------

constexpr int factorial(int n) {
    return (n <= 1) ? 1 : n * factorial(n - 1);
}

// ----------------------------------------------------------------------------
// Main function demonstrating all concepts
// ----------------------------------------------------------------------------

int main() {
    std::cout << "=== Basic Functions ===\n";
    std::cout << "square(5.0) = " << square(5.0) << "\n";

    std::cout << "\n=== Function Overloading ===\n";
    std::cout << "multiply(3, 4) = " << multiply(3, 4) << "\n";
    std::cout << "multiply(3.0, 4.0) = " << multiply(3.0, 4.0) << "\n";
    std::cout << "multiply(2, 3, 4) = " << multiply(2, 3, 4) << "\n";

    std::cout << "\n=== Default Arguments ===\n";
    greet("Alice");                 // Uses default greeting
    greet("Bob", "Good morning");   // Custom greeting

    std::cout << "\n=== Pass by Value vs Reference ===\n";
    int num = 10;
    std::cout << "Original: num = " << num << "\n";

    increment_value(num);
    std::cout << "After increment_value: num = " << num << " (unchanged)\n";

    increment_ref(num);
    std::cout << "After increment_ref: num = " << num << " (modified)\n";

    std::cout << "\n=== constexpr Functions ===\n";
    constexpr int f5 = factorial(5);  // Computed at compile time
    std::cout << "factorial(5) = " << f5 << "\n";

    // Can also be called at runtime
    int n = 6;
    std::cout << "factorial(6) = " << factorial(n) << "\n";

    return 0;
}
