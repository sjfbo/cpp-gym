// Exercise 1.5: Scope and Lifetime
// Book reference: 1.5 Scope and Lifetime
// Difficulty: **
//
// Instructions:
// This exercise tests your understanding of variable scope and lifetime.
// Analyze the code and predict the output, then verify by running.
//
// Part 1: Predict the output (without running)
// Part 2: Fix the scope-related bugs
//
// Expected output after fixing:
//   outer x = 10
//   inner x = 20
//   after inner: x = 10
//   function result: 15
//   Final message: Hello from outer scope!

#include <iostream>
#include <string>

// A function that demonstrates scope
int scope_demo(int input) {
    int result = input;

    if (input > 0) {
        int bonus = 5;  // Only visible in this if block
        result = result + bonus;
    }

    // TODO: This line has a bug. Fix it.
    // return result + bonus;  // ERROR: bonus is out of scope!

    return result;  // Temporary fix - update when you understand the issue
}

int main() {
    std::cout << "=== Part 1: Predict the output ===\n";

    int x = 10;
    std::cout << "outer x = " << x << "\n";

    {
        // Inner scope - this x shadows the outer x
        int x = 20;
        std::cout << "inner x = " << x << "\n";
    }

    // What is x now?
    std::cout << "after inner: x = " << x << "\n";

    // Function scope
    int result = scope_demo(10);
    std::cout << "function result: " << result << "\n";

    std::cout << "\n=== Part 2: Fix the bugs ===\n";

    // Bug 1: Variable used outside its scope
    // Uncomment and fix:
    /*
    if (true) {
        std::string message = "Hello from inner scope!";
    }
    std::cout << message << "\n";  // ERROR: message out of scope
    */

    // TODO: Fix by declaring message in the correct scope
    std::string message = "Hello from outer scope!";
    if (true) {
        message = "Hello from inner scope!";  // Modifies outer message
    }
    std::cout << "Final message: " << message << "\n";

    // Bug 2: Returning reference to local variable
    // This function would be dangerous - understand why:
    /*
    int& dangerous() {
        int local = 42;
        return local;  // DANGER: local is destroyed after return!
    }
    */

    // Bug 3: Shadowing warnings
    // While legal, shadowing can be confusing:
    int value = 100;
    for (int i = 0; i < 3; i++) {
        // int value = i;  // Shadows outer 'value' - usually a bad idea
        int inner_value = i;  // Better: use a different name
        std::cout << "Loop " << i << ": inner_value = " << inner_value << "\n";
    }
    std::cout << "outer value still = " << value << "\n";

    std::cout << "\n=== Scope Quiz ===\n";

    // For each variable below, state:
    // 1. Where is it visible (scope)?
    // 2. When is it destroyed (lifetime)?

    static int static_var = 1;   // Q1: Scope? Lifetime?
    int local_var = 2;           // Q2: Scope? Lifetime?
    const int const_var = 3;     // Q3: Scope? Lifetime?

    {
        int block_var = 4;       // Q4: Scope? Lifetime?
        std::cout << "block_var = " << block_var << "\n";
    }

    for (int loop_var = 0; loop_var < 1; loop_var++) {
        // Q5: What is loop_var's scope and lifetime?
        std::cout << "loop_var = " << loop_var << "\n";
    }

    std::cout << "\nAnswers:\n";
    std::cout << "Q1: static_var - function scope, program lifetime\n";
    std::cout << "Q2: local_var - function scope, until function returns\n";
    std::cout << "Q3: const_var - function scope, until function returns\n";
    std::cout << "Q4: block_var - block scope, until } of block\n";
    std::cout << "Q5: loop_var - loop scope, each iteration creates new\n";

    return 0;
}
