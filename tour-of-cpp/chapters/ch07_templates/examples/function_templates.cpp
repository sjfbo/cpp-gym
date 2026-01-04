// Function Templates - Generic Algorithms
// Book reference: 7.3 Function Templates
//
// Function templates allow you to write generic algorithms that work with
// any type. The compiler deduces template arguments from function arguments,
// making the syntax clean and intuitive.

#include <array>
#include <iostream>
#include <string>
#include <type_traits>
#include <vector>

// ----------------------------------------------------------------------------
// Basic Function Template
// ----------------------------------------------------------------------------

// Generic max function - works with any comparable type
template<typename T>
T max_of(T a, T b) {
    return (a > b) ? a : b;
}

// Generic min function
template<typename T>
T min_of(T a, T b) {
    return (a < b) ? a : b;
}

// ----------------------------------------------------------------------------
// Template Argument Deduction
// ----------------------------------------------------------------------------

// The compiler deduces T from the argument types
template<typename T>
void print_type_info(const T& value) {
    std::cout << "Value: " << value << "\n";
    std::cout << "  Type name hint: ";

    // Using type_traits to show type information
    if constexpr (std::is_integral_v<T>) {
        std::cout << "integral";
    } else if constexpr (std::is_floating_point_v<T>) {
        std::cout << "floating-point";
    } else if constexpr (std::is_same_v<T, std::string>) {
        std::cout << "std::string";
    } else {
        std::cout << "other";
    }
    std::cout << ", size: " << sizeof(T) << " bytes\n";
}

// ----------------------------------------------------------------------------
// Multiple Template Parameters
// ----------------------------------------------------------------------------

// Function with two type parameters
template<typename T, typename U>
auto add(T a, U b) -> decltype(a + b) {
    return a + b;
}

// C++14 and later: auto return type deduction
template<typename T, typename U>
auto multiply(T a, U b) {
    return a * b;
}

// ----------------------------------------------------------------------------
// Abbreviated Function Templates (C++20)
// ----------------------------------------------------------------------------

// Using 'auto' in parameters creates a function template (C++20)
auto divide(auto a, auto b) {
    return a / b;
}

// Equivalent to:
// template<typename T, typename U>
// auto divide(T a, U b) { return a / b; }

// Constrained auto with concepts (preview - covered more in ch08)
auto absolute(auto value) {
    if (value < 0) {
        return -value;
    }
    return value;
}

// ----------------------------------------------------------------------------
// Function Templates with Containers
// ----------------------------------------------------------------------------

// Print any container with begin/end iterators
template<typename Container>
void print_container(const Container& c, const std::string& name) {
    std::cout << name << ": [";
    bool first = true;
    for (const auto& elem : c) {
        if (!first) std::cout << ", ";
        std::cout << elem;
        first = false;
    }
    std::cout << "]\n";
}

// Sum all elements in a container
template<typename Container>
auto sum(const Container& c) {
    using ValueType = typename Container::value_type;
    ValueType result{};
    for (const auto& elem : c) {
        result += elem;
    }
    return result;
}

// Find element in container (iterator-based)
// Note: Named my_find to avoid ambiguity with std::find
template<typename Iterator, typename T>
Iterator my_find(Iterator begin, Iterator end, const T& value) {
    while (begin != end) {
        if (*begin == value) {
            return begin;
        }
        ++begin;
    }
    return end;  // Not found
}

// Count occurrences
// Note: Named my_count to avoid ambiguity with std::count
template<typename Iterator, typename T>
size_t my_count(Iterator begin, Iterator end, const T& value) {
    size_t result = 0;
    while (begin != end) {
        if (*begin == value) {
            ++result;
        }
        ++begin;
    }
    return result;
}

// ----------------------------------------------------------------------------
// Function Templates with Predicates
// ----------------------------------------------------------------------------

// Find element matching a predicate
// Note: Named my_find_if to avoid ambiguity with std::find_if
template<typename Iterator, typename Predicate>
Iterator my_find_if(Iterator begin, Iterator end, Predicate pred) {
    while (begin != end) {
        if (pred(*begin)) {
            return begin;
        }
        ++begin;
    }
    return end;
}

// Count elements matching a predicate
// Note: Named my_count_if to avoid ambiguity with std::count_if
template<typename Iterator, typename Predicate>
size_t my_count_if(Iterator begin, Iterator end, Predicate pred) {
    size_t result = 0;
    while (begin != end) {
        if (pred(*begin)) {
            ++result;
        }
        ++begin;
    }
    return result;
}

// Transform elements using a function
// Note: Named my_transform to avoid ambiguity with std::transform
template<typename InputIt, typename OutputIt, typename UnaryOp>
OutputIt my_transform(InputIt first, InputIt last, OutputIt result, UnaryOp op) {
    while (first != last) {
        *result = op(*first);
        ++first;
        ++result;
    }
    return result;
}

// ----------------------------------------------------------------------------
// Explicit Template Arguments
// ----------------------------------------------------------------------------

// Sometimes you need to specify template arguments explicitly
template<typename Target, typename Source>
Target convert(Source value) {
    return static_cast<Target>(value);
}

// Non-type template parameters in functions
template<int N>
int add_constant(int value) {
    return value + N;
}

// ----------------------------------------------------------------------------
// Variadic Templates (Variable Number of Arguments)
// ----------------------------------------------------------------------------

// Base case for recursion
void print_all() {
    std::cout << "\n";
}

// Recursive variadic template
template<typename First, typename... Rest>
void print_all(const First& first, const Rest&... rest) {
    std::cout << first;
    if constexpr (sizeof...(rest) > 0) {
        std::cout << ", ";
    }
    print_all(rest...);
}

// Sum of any number of arguments
template<typename T>
T sum_all(T value) {
    return value;
}

template<typename T, typename... Rest>
T sum_all(T first, Rest... rest) {
    return first + sum_all(rest...);
}

// C++17 fold expression version (more concise)
template<typename... Args>
auto sum_fold(Args... args) {
    return (args + ...);  // Fold expression
}

// ----------------------------------------------------------------------------
// Main function demonstrating all concepts
// ----------------------------------------------------------------------------

int main() {
    std::cout << "=== Basic Function Templates ===\n";

    // Template argument deduction - compiler figures out T
    std::cout << "max_of(3, 7) = " << max_of(3, 7) << "\n";
    std::cout << "max_of(3.14, 2.71) = " << max_of(3.14, 2.71) << "\n";
    std::cout << "max_of('a', 'z') = " << max_of('a', 'z') << "\n";

    // Explicit template argument (when needed)
    std::cout << "max_of<double>(3, 7.5) = " << max_of<double>(3, 7.5) << "\n";

    std::cout << "\n=== Template Argument Deduction ===\n";
    print_type_info(42);
    print_type_info(3.14);
    print_type_info(std::string{"hello"});

    std::cout << "\n=== Multiple Template Parameters ===\n";
    std::cout << "add(3, 4.5) = " << add(3, 4.5) << "\n";
    std::cout << "multiply(3, 4.5) = " << multiply(3, 4.5) << "\n";

    std::cout << "\n=== Abbreviated Function Templates (C++20) ===\n";
    std::cout << "divide(10.0, 3.0) = " << divide(10.0, 3.0) << "\n";
    std::cout << "absolute(-42) = " << absolute(-42) << "\n";
    std::cout << "absolute(3.14) = " << absolute(3.14) << "\n";

    std::cout << "\n=== Container Algorithms ===\n";
    std::vector<int> vec = {1, 2, 3, 4, 5};
    std::array<double, 4> arr = {1.1, 2.2, 3.3, 4.4};

    print_container(vec, "vec");
    print_container(arr, "arr");

    std::cout << "sum(vec) = " << sum(vec) << "\n";
    std::cout << "sum(arr) = " << sum(arr) << "\n";

    std::cout << "\n=== Iterator-based Algorithms ===\n";
    auto it = my_find(vec.begin(), vec.end(), 3);
    if (it != vec.end()) {
        std::cout << "Found 3 at position " << (it - vec.begin()) << "\n";
    }

    std::vector<int> nums = {1, 2, 2, 3, 2, 4, 2};
    std::cout << "count of 2 in nums: " << my_count(nums.begin(), nums.end(), 2) << "\n";

    std::cout << "\n=== Predicate-based Algorithms ===\n";
    auto is_even = [](int x) { return x % 2 == 0; };
    auto first_even = my_find_if(vec.begin(), vec.end(), is_even);
    if (first_even != vec.end()) {
        std::cout << "First even number: " << *first_even << "\n";
    }

    std::cout << "Count of evens: " << my_count_if(vec.begin(), vec.end(), is_even) << "\n";

    // Transform example
    std::vector<int> squared(vec.size());
    my_transform(vec.begin(), vec.end(), squared.begin(), [](int x) { return x * x; });
    print_container(squared, "squared");

    std::cout << "\n=== Explicit Template Arguments ===\n";
    double pi = 3.14159;
    std::cout << "convert<int>(3.14159) = " << convert<int>(pi) << "\n";
    std::cout << "add_constant<10>(5) = " << add_constant<10>(5) << "\n";
    std::cout << "add_constant<100>(5) = " << add_constant<100>(5) << "\n";

    std::cout << "\n=== Variadic Templates ===\n";
    std::cout << "print_all: ";
    print_all(1, 2.5, "hello", 'x');

    std::cout << "sum_all(1, 2, 3, 4, 5) = " << sum_all(1, 2, 3, 4, 5) << "\n";
    std::cout << "sum_fold(1, 2, 3, 4, 5) = " << sum_fold(1, 2, 3, 4, 5) << "\n";

    std::cout << "\n=== Key Takeaways ===\n";
    std::cout << "1. Template argument deduction makes function templates easy to use\n";
    std::cout << "2. 'auto' parameters (C++20) create abbreviated function templates\n";
    std::cout << "3. Iterator-based algorithms work with any container\n";
    std::cout << "4. Predicates (lambdas/functions) add flexibility to algorithms\n";
    std::cout << "5. Variadic templates enable functions with any number of arguments\n";

    return 0;
}
