#include "mini_vector.h"
#include <iostream>
#include <string>

/**
 * Demonstrates the mini::Vector class template.
 */

// Helper to print vector contents
template <typename T>
void print_vector(const std::string& label, const mini::Vector<T>& vec) {
    std::cout << label << " [size=" << vec.size()
              << ", capacity=" << vec.capacity() << "]: ";
    for (const auto& elem : vec) {
        std::cout << elem << " ";
    }
    std::cout << "\n";
}

int main() {
    std::cout << "=== Mini Vector Demo ===\n\n";

    // 1. Default construction and push_back
    std::cout << "1. Basic operations:\n";
    mini::Vector<int> numbers;
    numbers.push_back(10);
    numbers.push_back(20);
    numbers.push_back(30);
    print_vector("   numbers", numbers);

    // 2. Initializer list construction
    std::cout << "\n2. Initializer list construction:\n";
    mini::Vector<double> doubles{1.1, 2.2, 3.3, 4.4, 5.5};
    print_vector("   doubles", doubles);

    // 3. Element access
    std::cout << "\n3. Element access:\n";
    std::cout << "   numbers[1] = " << numbers[1] << "\n";
    std::cout << "   numbers.at(2) = " << numbers.at(2) << "\n";
    std::cout << "   numbers.front() = " << numbers.front() << "\n";
    std::cout << "   numbers.back() = " << numbers.back() << "\n";

    // 4. Bounds checking with at()
    std::cout << "\n4. Bounds checking:\n";
    try {
        std::cout << "   Attempting numbers.at(100)...\n";
        numbers.at(100);
    } catch (const std::out_of_range& e) {
        std::cout << "   Caught exception: " << e.what() << "\n";
    }

    // 5. Copy semantics
    std::cout << "\n5. Copy semantics:\n";
    mini::Vector<int> numbers_copy = numbers;  // Copy constructor
    numbers_copy.push_back(40);
    print_vector("   original", numbers);
    print_vector("   copy (modified)", numbers_copy);

    // 6. Move semantics
    std::cout << "\n6. Move semantics:\n";
    mini::Vector<std::string> strings{"hello", "world"};
    print_vector("   before move", strings);
    mini::Vector<std::string> moved_strings = std::move(strings);
    print_vector("   after move (destination)", moved_strings);
    std::cout << "   source after move: size=" << strings.size() << "\n";

    // 7. Range-based for loop
    std::cout << "\n7. Range-based for loop:\n";
    mini::Vector<int> primes{2, 3, 5, 7, 11, 13};
    std::cout << "   Primes: ";
    for (int p : primes) {
        std::cout << p << " ";
    }
    std::cout << "\n";

    // 8. Capacity operations
    std::cout << "\n8. Capacity operations:\n";
    mini::Vector<int> growing;
    std::cout << "   Initial: size=" << growing.size()
              << ", capacity=" << growing.capacity() << "\n";
    for (int i = 0; i < 10; ++i) {
        growing.push_back(i);
        std::cout << "   After push " << i << ": size=" << growing.size()
                  << ", capacity=" << growing.capacity() << "\n";
    }

    // 9. Reserve
    std::cout << "\n9. Reserve:\n";
    mini::Vector<int> reserved;
    reserved.reserve(100);
    std::cout << "   After reserve(100): size=" << reserved.size()
              << ", capacity=" << reserved.capacity() << "\n";

    // 10. Pop back and clear
    std::cout << "\n10. Pop back and clear:\n";
    mini::Vector<int> data{1, 2, 3, 4, 5};
    print_vector("   initial", data);
    data.pop_back();
    print_vector("   after pop_back()", data);
    data.clear();
    std::cout << "   after clear(): empty=" << std::boolalpha << data.empty() << "\n";

    // 11. Using with custom types
    std::cout << "\n11. Using with custom types:\n";
    struct Point {
        int x = 0, y = 0;
    };
    mini::Vector<Point> points;
    points.push_back({1, 2});
    points.push_back({3, 4});
    points.emplace_back(5, 6);
    std::cout << "   Points: ";
    for (const auto& p : points) {
        std::cout << "(" << p.x << "," << p.y << ") ";
    }
    std::cout << "\n";

    std::cout << "\n=== Demo Complete ===\n";
    return 0;
}
