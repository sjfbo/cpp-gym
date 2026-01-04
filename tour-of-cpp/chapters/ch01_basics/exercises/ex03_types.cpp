// Exercise 1.3: Types and Initialization
// Book reference: 1.4 Types, Variables, and Arithmetic
// Difficulty: *
//
// Instructions:
// This exercise tests your understanding of types and initialization.
// Answer the questions by filling in the expected values.
//
// Part 1: Fix the bugs in the code below
// Part 2: Predict the output
//
// Expected output:
//   All assertions passed!

#include <cassert>
#include <iostream>

int main() {
    std::cout << "=== Part 1: Fix the bugs ===\n";

    // Bug 1: This causes undefined behavior. Fix it.
    // int uninitialized;
    // std::cout << uninitialized << "\n";  // UB!

    // TODO: Declare 'value' properly initialized to 42
    // int value;
    // assert(value == 42);

    // Bug 2: This loses precision. Fix using proper initialization.
    // double pi = 3.14159;
    // int truncated = pi;  // Silently truncates!

    // TODO: Declare 'truncated' using list initialization to catch the error
    // Then fix it by using the correct type or explicit cast

    // Bug 3: This might overflow. Use the correct type.
    // int big = 3000000000;  // Too big for int on most systems

    // TODO: Declare 'big' with the correct type to hold 3000000000

    std::cout << "=== Part 2: Predict the output ===\n";

    // What are the values after these operations?

    [[maybe_unused]] int a = 17 / 5;      // TODO: What is a?
    [[maybe_unused]] int b = 17 % 5;      // TODO: What is b?
    [[maybe_unused]] double c = 17.0 / 5; // TODO: What is c?

    // Uncomment and fill in your predictions:
    // assert(a == ???);  // Integer division
    // assert(b == ???);  // Remainder
    // assert(c > 3.3 && c < 3.5);  // Floating-point division

    // What happens with increment?
    [[maybe_unused]] int x = 5;
    [[maybe_unused]] int y = x++;  // Post-increment
    [[maybe_unused]] int z = ++x;  // Pre-increment

    // Uncomment and fill in your predictions:
    // assert(x == ???);
    // assert(y == ???);
    // assert(z == ???);

    // auto type deduction
    [[maybe_unused]] auto i = 42;        // What type is i?
    [[maybe_unused]] auto d = 42.0;      // What type is d?
    [[maybe_unused]] auto s = "hello";   // What type is s? (tricky!)

    // These assertions check the types:
    static_assert(std::is_same_v<decltype(i), int>);
    static_assert(std::is_same_v<decltype(d), double>);
    // static_assert(std::is_same_v<decltype(s), ???>);  // What is s?

    std::cout << "TODO: Complete the exercises above\n";
    std::cout << "When done correctly: All assertions passed!\n";

    return 0;
}
