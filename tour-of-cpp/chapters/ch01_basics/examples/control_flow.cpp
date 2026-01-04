// Control Flow - Tests and Loops
// Book reference: 1.8 Tests
//
// C++ provides standard control flow statements:
// - if/else for conditional execution
// - switch for multi-way branching
// - while, do-while, for loops for iteration
// - range-based for for container iteration

#include <iostream>
#include <string>
#include <vector>

int main() {
    std::cout << "=== if / else ===\n";

    int x = 42;

    if (x > 0) {
        std::cout << x << " is positive\n";
    } else if (x < 0) {
        std::cout << x << " is negative\n";
    } else {
        std::cout << x << " is zero\n";
    }

    // if with initializer (C++17)
    if (int y = x * 2; y > 50) {
        std::cout << "y (" << y << ") is greater than 50\n";
    } else {
        std::cout << "y (" << y << ") is not greater than 50\n";
    }
    // y is not accessible here

    std::cout << "\n=== Conditional (Ternary) Operator ===\n";

    int a = 5;
    int b = 10;
    int max_val = (a > b) ? a : b;
    std::cout << "max(" << a << ", " << b << ") = " << max_val << "\n";

    std::string status = (x > 0) ? "positive" : "non-positive";
    std::cout << x << " is " << status << "\n";

    std::cout << "\n=== switch ===\n";

    int day = 3;
    std::cout << "Day " << day << " is ";

    switch (day) {
        case 1:
            std::cout << "Monday";
            break;
        case 2:
            std::cout << "Tuesday";
            break;
        case 3:
            std::cout << "Wednesday";
            break;
        case 4:
            std::cout << "Thursday";
            break;
        case 5:
            std::cout << "Friday";
            break;
        case 6:
        case 7:  // Fallthrough for weekends
            std::cout << "Weekend!";
            break;
        default:
            std::cout << "Invalid day";
            break;
    }
    std::cout << "\n";

    // switch with initializer (C++17)
    switch (int n = day % 2; n) {
        case 0:
            std::cout << "Day " << day << " is even\n";
            break;
        case 1:
            std::cout << "Day " << day << " is odd\n";
            break;
    }

    std::cout << "\n=== while loop ===\n";

    int count = 0;
    while (count < 5) {
        std::cout << "count = " << count << "\n";
        count++;
    }

    std::cout << "\n=== do-while loop ===\n";

    int num = 0;
    do {
        std::cout << "num = " << num << "\n";
        num++;
    } while (num < 3);
    // Executes at least once, even if condition is initially false

    std::cout << "\n=== for loop ===\n";

    // Traditional for loop
    for (int i = 0; i < 5; i++) {
        std::cout << "i = " << i << "\n";
    }

    // Multiple initialization and increment
    std::cout << "Two variables: ";
    for (int i = 0, j = 10; i < j; i++, j--) {
        std::cout << "(" << i << "," << j << ") ";
    }
    std::cout << "\n";

    std::cout << "\n=== Range-based for loop ===\n";

    std::vector<int> vec = {10, 20, 30, 40, 50};

    // Read-only access
    std::cout << "Read-only: ";
    for (const auto& elem : vec) {
        std::cout << elem << " ";
    }
    std::cout << "\n";

    // Modify elements
    for (auto& elem : vec) {
        elem *= 2;
    }
    std::cout << "After doubling: ";
    for (const auto& elem : vec) {
        std::cout << elem << " ";
    }
    std::cout << "\n";

    // Works with C arrays too
    int arr[] = {1, 2, 3, 4, 5};
    std::cout << "C array: ";
    for (int val : arr) {
        std::cout << val << " ";
    }
    std::cout << "\n";

    std::cout << "\n=== break and continue ===\n";

    // break exits the loop
    std::cout << "break example: ";
    for (int i = 0; i < 10; i++) {
        if (i == 5) break;  // Exit when i is 5
        std::cout << i << " ";
    }
    std::cout << "\n";

    // continue skips to next iteration
    std::cout << "continue example (skip even): ";
    for (int i = 0; i < 10; i++) {
        if (i % 2 == 0) continue;  // Skip even numbers
        std::cout << i << " ";
    }
    std::cout << "\n";

    std::cout << "\n=== Nested loops with labels ===\n";

    // Note: C++ doesn't have labeled break/continue like some languages
    // Use a flag or restructure code instead
    bool found = false;
    for (int i = 0; i < 3 && !found; i++) {
        for (int j = 0; j < 3 && !found; j++) {
            std::cout << "(" << i << "," << j << ") ";
            if (i == 1 && j == 1) {
                std::cout << "<- found!";
                found = true;
            }
        }
    }
    std::cout << "\n";

    std::cout << "\n=== Comparison Operators ===\n";

    int p = 5;
    int q = 10;

    std::cout << std::boolalpha;  // Print true/false instead of 1/0
    std::cout << p << " == " << q << " : " << (p == q) << "\n";
    std::cout << p << " != " << q << " : " << (p != q) << "\n";
    std::cout << p << " < " << q << " : " << (p < q) << "\n";
    std::cout << p << " <= " << q << " : " << (p <= q) << "\n";
    std::cout << p << " > " << q << " : " << (p > q) << "\n";
    std::cout << p << " >= " << q << " : " << (p >= q) << "\n";

    std::cout << "\n=== Logical Operators ===\n";

    bool t = true;
    bool f = false;

    std::cout << "true && false : " << (t && f) << "\n";  // AND
    std::cout << "true || false : " << (t || f) << "\n";  // OR
    std::cout << "!true : " << (!t) << "\n";              // NOT

    // Short-circuit evaluation
    std::cout << "\nShort-circuit: && stops at first false, || stops at first true\n";

    return 0;
}
