// Using the Vector Class - Demonstrating Separate Compilation
// Book reference: 3.2 Separate Compilation
//
// This file demonstrates how to use a class defined in separate header
// and implementation files. The compiler only needs the header to compile
// this file; the implementation is linked later.
//
// Compilation process:
// 1. main.cpp + vector.h -> main.o (compilation)
// 2. vector.cpp + vector.h -> vector.o (compilation)
// 3. main.o + vector.o -> executable (linking)

#include "vector.h"  // We only need the header, not the implementation

#include <iostream>

// Using declaration - bring specific names into scope - See 3.3
using containers::Vector;

int main() {
    std::cout << "=== Separate Compilation Demo ===\n\n";

    // --------------------------------------------------------------------
    // Basic usage
    // --------------------------------------------------------------------
    std::cout << "--- Creating Vectors ---\n";

    Vector v1(5);  // Size constructor
    std::cout << "v1 (size 5, zero-initialized): ";
    for (std::size_t i = 0; i < v1.size(); ++i) {
        std::cout << v1[i] << " ";
    }
    std::cout << "\n";

    Vector v2{1.0, 2.0, 3.0, 4.0, 5.0};  // Initializer list
    std::cout << "v2 (initializer list): ";
    for (const auto& elem : v2) {
        std::cout << elem << " ";
    }
    std::cout << "\n";

    // --------------------------------------------------------------------
    // Element access
    // --------------------------------------------------------------------
    std::cout << "\n--- Element Access ---\n";

    v1[0] = 10;
    v1[1] = 20;
    v1[2] = 30;
    std::cout << "After assignment v1: ";
    for (const auto& elem : v1) {
        std::cout << elem << " ";
    }
    std::cout << "\n";

    // Bounds-checked access
    try {
        std::cout << "v1.at(10): ";
        v1.at(10);  // Will throw
    } catch (const std::out_of_range& e) {
        std::cout << "Exception caught: " << e.what() << "\n";
    }

    // --------------------------------------------------------------------
    // Copy semantics
    // --------------------------------------------------------------------
    std::cout << "\n--- Copy Semantics ---\n";

    Vector v3 = v2;  // Copy constructor
    std::cout << "v3 (copy of v2): ";
    for (const auto& elem : v3) {
        std::cout << elem << " ";
    }
    std::cout << "\n";

    v3[0] = 100;
    std::cout << "After v3[0] = 100:\n";
    std::cout << "  v2[0] = " << v2[0] << " (unchanged - deep copy)\n";
    std::cout << "  v3[0] = " << v3[0] << "\n";

    // --------------------------------------------------------------------
    // Move semantics
    // --------------------------------------------------------------------
    std::cout << "\n--- Move Semantics ---\n";

    Vector v4 = std::move(v3);  // Move constructor
    std::cout << "v4 (moved from v3): ";
    for (const auto& elem : v4) {
        std::cout << elem << " ";
    }
    std::cout << "\n";
    std::cout << "v3 size after move: " << v3.size() << " (empty)\n";

    // --------------------------------------------------------------------
    // Non-member functions
    // --------------------------------------------------------------------
    std::cout << "\n--- Non-member Functions ---\n";

    std::cout << "sum(v2) = " << containers::sum(v2) << "\n";

    Vector a{1, 2, 3};
    Vector b{4, 5, 6};
    std::cout << "dot_product({1,2,3}, {4,5,6}) = "
              << containers::dot_product(a, b) << "\n";

    // --------------------------------------------------------------------
    // Key points about separate compilation
    // --------------------------------------------------------------------
    std::cout << "\n=== Key Points ===\n";
    std::cout << "1. Header (vector.h): declarations only\n";
    std::cout << "2. Implementation (vector.cpp): definitions\n";
    std::cout << "3. Client (main.cpp): #include header, link implementation\n";
    std::cout << "4. Changes to implementation don't require recompiling clients\n";
    std::cout << "5. Headers provide the interface contract\n";

    return 0;
}
