// Exercise 5.2: Container Interface
// Book reference: 5.3 Abstract Types
// Difficulty: **
//
// Instructions:
// Implement a Container interface and two concrete implementations.
//
// 1. Complete the Container abstract base class:
//    - Virtual destructor
//    - Pure virtual: size(), operator[], empty()
//    - Pure virtual: push_back(), clear()
//
// 2. Implement DynamicArray:
//    - Uses std::vector<double> internally
//    - Implements all Container functions
//    - Grows dynamically
//
// 3. Implement FixedArray:
//    - Uses a fixed-size array (capacity set at construction)
//    - Throws std::out_of_range when full
//    - Implements all Container functions
//
// 4. Implement sum() function that works with any Container
//
// Expected output:
//   DynamicArray: [1, 2, 3, 4, 5] sum=15
//   FixedArray: [10, 20, 30] sum=60

#include <iostream>
#include <stdexcept>
#include <vector>

// ----------------------------------------------------------------------------
// TODO: Complete the Container interface
// ----------------------------------------------------------------------------

class Container {
public:
    // TODO: Virtual destructor

    // TODO: Pure virtual size() - returns number of elements

    // TODO: Pure virtual empty() - returns true if container is empty

    // TODO: Pure virtual operator[](int) - element access (non-const)

    // TODO: Pure virtual operator[](int) const - element access (const)

    // TODO: Pure virtual push_back(double) - add element to end

    // TODO: Pure virtual clear() - remove all elements
};

// ----------------------------------------------------------------------------
// TODO: Implement DynamicArray (uses std::vector internally)
// ----------------------------------------------------------------------------

class DynamicArray /* : public Container */ {
    // TODO: std::vector<double> member

public:
    // TODO: Default constructor

    // TODO: Constructor taking initializer_list<double>

    // TODO: Implement size()

    // TODO: Implement empty()

    // TODO: Implement operator[] (both const and non-const)

    // TODO: Implement push_back()

    // TODO: Implement clear()
};

// ----------------------------------------------------------------------------
// TODO: Implement FixedArray (fixed capacity, set at construction)
// ----------------------------------------------------------------------------

class FixedArray /* : public Container */ {
    // TODO: Fixed-size array (use unique_ptr<double[]> or raw array)
    // TODO: capacity member
    // TODO: count member (current number of elements)

public:
    // TODO: Constructor taking capacity

    // TODO: Implement size() - returns count, not capacity

    // TODO: Implement empty()

    // TODO: Implement operator[] with bounds checking

    // TODO: Implement push_back() - throw if full

    // TODO: Implement clear()

    // Optional: capacity() accessor
};

// ----------------------------------------------------------------------------
// TODO: Implement utility functions that work with any Container
// ----------------------------------------------------------------------------

// TODO: Calculate sum of all elements
// double sum(const Container& c) {
//     double total = 0;
//     for (int i = 0; i < c.size(); ++i) {
//         total += c[i];
//     }
//     return total;
// }

// TODO: Print container contents
// void print_container(const Container& c) {
//     std::cout << "[";
//     for (int i = 0; i < c.size(); ++i) {
//         if (i > 0) std::cout << ", ";
//         std::cout << c[i];
//     }
//     std::cout << "]";
// }

// ----------------------------------------------------------------------------
// Main - uncomment after implementing
// ----------------------------------------------------------------------------

int main() {
    std::cout << "Exercise 5.2: Container Interface\n";
    std::cout << "==================================\n\n";

    // Uncomment after implementing:

    // DynamicArray da;
    // da.push_back(1);
    // da.push_back(2);
    // da.push_back(3);
    // da.push_back(4);
    // da.push_back(5);
    //
    // std::cout << "DynamicArray: ";
    // print_container(da);
    // std::cout << " sum=" << sum(da) << "\n";
    //
    // FixedArray fa{5};  // Capacity of 5
    // fa.push_back(10);
    // fa.push_back(20);
    // fa.push_back(30);
    //
    // std::cout << "FixedArray: ";
    // print_container(fa);
    // std::cout << " sum=" << sum(fa) << "\n";
    //
    // // Test polymorphism
    // Container* containers[] = {&da, &fa};
    // std::cout << "\nPolymorphic access:\n";
    // for (Container* c : containers) {
    //     std::cout << "  size=" << c->size() << ", sum=" << sum(*c) << "\n";
    // }
    //
    // // Test bounds/capacity
    // try {
    //     FixedArray small{2};
    //     small.push_back(1);
    //     small.push_back(2);
    //     small.push_back(3);  // Should throw!
    // } catch (const std::out_of_range& e) {
    //     std::cout << "\nCaught overflow: " << e.what() << "\n";
    // }

    std::cout << "TODO: Implement Container, DynamicArray, and FixedArray\n";

    return 0;
}

// ----------------------------------------------------------------------------
// Bonus challenges:
// ----------------------------------------------------------------------------
//
// 1. Add a find(double value) method that returns index or -1
//
// 2. Add an insert(int index, double value) method
//
// 3. Add a remove(int index) method
//
// 4. Implement a RingBuffer that overwrites oldest elements when full
//
// 5. Add iterator support (begin(), end()) to enable range-for loops
