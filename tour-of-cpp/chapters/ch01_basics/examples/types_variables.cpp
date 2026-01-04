// Types and Variables - The Foundation of C++
// Book reference: 1.4 Types, Variables, and Arithmetic
//
// C++ is statically typed: every expression has a type known at compile time.
// The type determines what operations are valid and how values are stored.

#include <cstdint>
#include <iostream>
#include <limits>

int main() {
    std::cout << "=== Fundamental Types ===\n";

    // Boolean
    bool is_valid = true;
    bool is_empty = false;
    std::cout << "bool: " << is_valid << ", " << is_empty << "\n";

    // Character types
    char c = 'A';              // At least 8 bits
    [[maybe_unused]] char16_t c16 = u'A';       // UTF-16 character
    [[maybe_unused]] char32_t c32 = U'A';       // UTF-32 character
    [[maybe_unused]] wchar_t wc = L'A';         // Wide character
    std::cout << "char: " << c << " (ASCII: " << static_cast<int>(c) << ")\n";

    // Integer types
    short s = 32000;           // At least 16 bits
    int i = 42;                // At least 16 bits (usually 32)
    long l = 100000L;          // At least 32 bits
    long long ll = 9000000000LL;  // At least 64 bits

    std::cout << "short: " << s << "\n";
    std::cout << "int: " << i << "\n";
    std::cout << "long: " << l << "\n";
    std::cout << "long long: " << ll << "\n";

    // Unsigned variants (no negative values, larger positive range)
    unsigned int ui = 42u;
    [[maybe_unused]] unsigned long ul = 100000UL;
    std::cout << "unsigned int: " << ui << "\n";

    // Fixed-width integers (C++11, from <cstdint>)
    std::int32_t i32 = 42;     // Exactly 32 bits
    [[maybe_unused]] std::int64_t i64 = 42;     // Exactly 64 bits
    std::uint8_t u8 = 255;     // Exactly 8 bits, unsigned
    std::cout << "int32_t: " << i32 << ", uint8_t: " << static_cast<int>(u8) << "\n";

    // Floating-point types
    float f = 3.14f;           // Usually 32 bits (IEEE 754)
    double d = 3.141592653589; // Usually 64 bits
    [[maybe_unused]] long double ld = 3.14159265358979323846L;  // Extended precision
    std::cout << "float: " << f << "\n";
    std::cout << "double: " << d << "\n";

    std::cout << "\n=== Initialization Styles ===\n";

    // Copy initialization (traditional)
    int a1 = 42;

    // Direct initialization
    int a2(42);

    // List initialization (C++11, preferred)
    int a3{42};

    // List initialization with = (also valid)
    int a4 = {42};

    // Default initialization (zero for primitives with {})
    int a5{};  // a5 is 0

    std::cout << "a1=" << a1 << ", a2=" << a2 << ", a3=" << a3;
    std::cout << ", a4=" << a4 << ", a5=" << a5 << "\n";

    // List initialization prevents narrowing conversions
    // int narrow{3.14};  // ERROR: narrowing from double to int
    int truncated = static_cast<int>(3.14);  // Explicit truncation to 3
    std::cout << "truncated = " << truncated << " (was 3.14)\n";

    std::cout << "\n=== Type Deduction with auto ===\n";

    auto x = 42;           // int
    auto y = 3.14;         // double
    auto z = 'A';          // char
    [[maybe_unused]] auto s1 = "hello";     // const char*
    [[maybe_unused]] auto b = true;         // bool

    std::cout << "auto x = 42 -> int: " << x << "\n";
    std::cout << "auto y = 3.14 -> double: " << y << "\n";
    std::cout << "auto z = 'A' -> char: " << z << "\n";

    // auto with initializer list
    auto list = {1, 2, 3};  // std::initializer_list<int>
    std::cout << "auto list = {1,2,3} size: " << list.size() << "\n";

    std::cout << "\n=== Arithmetic Operations ===\n";

    int num1 = 17;
    int num2 = 5;

    std::cout << num1 << " + " << num2 << " = " << (num1 + num2) << "\n";
    std::cout << num1 << " - " << num2 << " = " << (num1 - num2) << "\n";
    std::cout << num1 << " * " << num2 << " = " << (num1 * num2) << "\n";
    std::cout << num1 << " / " << num2 << " = " << (num1 / num2) << " (integer division)\n";
    std::cout << num1 << " % " << num2 << " = " << (num1 % num2) << " (remainder)\n";

    // Increment and decrement
    int counter = 0;
    std::cout << "\n++counter: " << ++counter << " (pre-increment)\n";
    std::cout << "counter++: " << counter++ << " (post-increment, returns old value)\n";
    std::cout << "counter is now: " << counter << "\n";

    std::cout << "\n=== Type Sizes ===\n";

    std::cout << "sizeof(char) = " << sizeof(char) << " bytes\n";
    std::cout << "sizeof(int) = " << sizeof(int) << " bytes\n";
    std::cout << "sizeof(long) = " << sizeof(long) << " bytes\n";
    std::cout << "sizeof(long long) = " << sizeof(long long) << " bytes\n";
    std::cout << "sizeof(float) = " << sizeof(float) << " bytes\n";
    std::cout << "sizeof(double) = " << sizeof(double) << " bytes\n";
    std::cout << "sizeof(void*) = " << sizeof(void*) << " bytes\n";

    std::cout << "\n=== Numeric Limits ===\n";

    std::cout << "int range: " << std::numeric_limits<int>::min()
              << " to " << std::numeric_limits<int>::max() << "\n";
    std::cout << "double epsilon: " << std::numeric_limits<double>::epsilon() << "\n";
    std::cout << "double max: " << std::numeric_limits<double>::max() << "\n";

    return 0;
}
