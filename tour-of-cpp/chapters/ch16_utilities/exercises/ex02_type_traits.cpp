// Exercise 16.2: Type Traits and Concepts Practice
// Book reference: 16.4 Type Functions
// Difficulty: ***
//
// Instructions:
// Implement the TODOs below to practice type traits and concepts.
//
// Part 1: Create type traits for custom type queries
// Part 2: Use enable_if for SFINAE-based overloading
// Part 3: Create and use C++20 concepts
//
// Compile and run to verify your solutions.

#include <concepts>
#include <iostream>
#include <string>
#include <type_traits>
#include <vector>
#include <cassert>

// ============================================================================
// Part 1: Custom Type Traits
// ============================================================================

// TODO 1: Implement is_string_like
// A type is "string-like" if it's std::string, const char*, or char*
// Hint: Use std::is_same_v and std::disjunction
template<typename T>
struct is_string_like : std::false_type {};

// Add specializations or implement the primary template here

template<typename T>
inline constexpr bool is_string_like_v = is_string_like<T>::value;

// TODO 2: Implement is_container
// A type is a "container" if it has begin(), end(), and size() member functions
// Hint: Use std::void_t and SFINAE detection idiom
template<typename T, typename = void>
struct is_container : std::false_type {};

// Add specialization here

template<typename T>
inline constexpr bool is_container_v = is_container<T>::value;

// TODO 3: Implement remove_all_pointers
// Remove all pointer levels: int*** -> int
template<typename T>
struct remove_all_pointers {
    using type = T;
};

// Add specialization here

template<typename T>
using remove_all_pointers_t = typename remove_all_pointers<T>::type;

void test_part1() {
    std::cout << "=== Part 1: Custom Type Traits ===\n";

    // TODO: Uncomment these static_asserts after implementing the type traits

    // Test is_string_like
    // static_assert(is_string_like_v<std::string>);
    // static_assert(is_string_like_v<const char*>);
    // static_assert(is_string_like_v<char*>);
    // static_assert(!is_string_like_v<int>);
    // static_assert(!is_string_like_v<std::vector<char>>);
    std::cout << "is_string_like: implement and uncomment tests above\n";

    // Test is_container
    // static_assert(is_container_v<std::vector<int>>);
    // static_assert(is_container_v<std::string>);
    // static_assert(!is_container_v<int>);
    // static_assert(!is_container_v<int*>);
    std::cout << "is_container: implement and uncomment tests above\n";

    // Test remove_all_pointers
    // static_assert(std::is_same_v<remove_all_pointers_t<int***>, int>);
    // static_assert(std::is_same_v<remove_all_pointers_t<const double**>, const double>);
    // static_assert(std::is_same_v<remove_all_pointers_t<int>, int>);
    std::cout << "remove_all_pointers: implement and uncomment tests above\n";

    std::cout << "Part 1: Implementation required\n\n";
}

// ============================================================================
// Part 2: SFINAE with enable_if
// ============================================================================

// TODO 4: Implement print_value for numeric types
// Should print "Number: <value>"
// Enable only for arithmetic types
// Uncomment and fix the enable_if condition:
/*
template<typename T>
std::enable_if_t<std::is_arithmetic_v<T>, void>
print_value(T value) {
    std::cout << "Number: " << value << "\n";
}
*/

// TODO 5: Implement print_value for string-like types
// Should print "String: <value>"
// Enable only for string-like types (use your is_string_like trait)
// Uncomment and fix the enable_if condition:
/*
template<typename T>
std::enable_if_t<is_string_like_v<T>, void>
print_value(T value) {
    std::cout << "String: " << value << "\n";
}
*/

// TODO 6: Implement print_value for containers
// Should print "Container with <size> elements"
// Enable only for container types (use your is_container trait)
// Uncomment and fix the enable_if condition:
/*
template<typename T>
std::enable_if_t<is_container_v<T>, void>
print_value(const T& value) {
    std::cout << "Container with " << value.size() << " elements\n";
}
*/

void test_part2() {
    std::cout << "=== Part 2: SFINAE Overloading ===\n";

    // These should compile and produce correct output:
    // print_value(42);           // Number: 42
    // print_value(3.14);         // Number: 3.14
    // print_value("hello");      // String: hello
    // print_value(std::string{"world"});  // String: world
    // print_value(std::vector<int>{1, 2, 3});  // Container with 3 elements

    std::cout << "Part 2: Uncomment the print_value calls once implemented\n";
    std::cout << "Part 2 skipped (implementation required)\n\n";
}

// ============================================================================
// Part 3: C++20 Concepts
// ============================================================================

// TODO 7: Define the Printable concept
// A type is Printable if it can be output to std::ostream using <<
template<typename T>
concept Printable = false;  // Replace this with the correct requirement

// TODO 8: Define the Hashable concept
// A type is Hashable if std::hash<T> is defined for it
template<typename T>
concept Hashable = false;  // Replace this with the correct requirement

// TODO 9: Define the Comparable concept
// A type is Comparable if it supports ==, !=, <, >, <=, >=
template<typename T>
concept Comparable = false;  // Replace this with the correct requirement

// TODO 10: Implement a function that uses concepts
// Create a function that only accepts types that are both Printable and Comparable
// The function should find and return the maximum element
template<typename Container>
auto find_max(const Container& c) {
    // Your implementation here
    // Should require that Container is a range and its elements are Comparable
    return *c.begin();  // Replace with actual implementation
}

void test_part3() {
    std::cout << "=== Part 3: C++20 Concepts ===\n";

    // TODO: Uncomment these static_asserts after implementing the concepts

    // Test Printable
    // static_assert(Printable<int>);
    // static_assert(Printable<std::string>);
    // static_assert(Printable<double>);
    std::cout << "Printable: implement and uncomment tests above\n";

    // Test Hashable
    // static_assert(Hashable<int>);
    // static_assert(Hashable<std::string>);
    // static_assert(!Hashable<std::vector<int>>);  // vector is not hashable by default
    std::cout << "Hashable: implement and uncomment tests above\n";

    // Test Comparable
    // static_assert(Comparable<int>);
    // static_assert(Comparable<std::string>);
    // static_assert(Comparable<double>);
    std::cout << "Comparable: implement and uncomment tests above\n";

    // Test find_max
    [[maybe_unused]] std::vector<int> nums{3, 1, 4, 1, 5, 9, 2, 6};
    // auto max_num = find_max(nums);
    // assert(max_num == 9);
    // std::cout << "find_max result: " << max_num << "\n";

    std::cout << "Part 3: Implementation required\n\n";
}

// ============================================================================
// Bonus: Type-Safe Unit System
// ============================================================================

// TODO (Bonus): Implement a simple unit system using type traits
// Create Distance and Time types that prevent mixing units

template<typename T, typename Unit>
class Quantity {
public:
    explicit Quantity(T value) : value_{value} {}

    T value() const { return value_; }

    // TODO: Implement operator+ that only allows same units
    // TODO: Implement operator* for scaling
    // TODO: Consider how to implement Distance / Time = Speed

private:
    T value_;
};

struct MeterTag {};
struct SecondTag {};

using Distance = Quantity<double, MeterTag>;
using Time = Quantity<double, SecondTag>;

void test_bonus() {
    std::cout << "=== Bonus: Unit System ===\n";

    Distance d1{100.0};
    Distance d2{50.0};
    Time t1{10.0};

    // These should work:
    // auto d3 = d1 + d2;  // Distance + Distance = Distance

    // These should NOT compile:
    // auto bad = d1 + t1;  // Distance + Time = Error!

    std::cout << "Bonus: Implement unit system for extra practice\n";
    std::cout << "Bonus skipped (optional)\n\n";
}

int main() {
    test_part1();
    test_part2();
    test_part3();
    test_bonus();

    std::cout << "=== Exercises completed! ===\n";
    std::cout << "(Some parts require implementation to fully pass)\n";
    return 0;
}
