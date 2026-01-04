// Concept Definitions - Creating Custom Concepts
// Book reference: 8.2.4 Definition of Concepts
//
// This example demonstrates how to define custom concepts using
// requires expressions, compound requirements, and concept composition.

#include <concepts>
#include <cstddef>
#include <functional>
#include <iostream>
#include <string>
#include <type_traits>
#include <vector>

// ============================================================================
// Simple concept definitions
// ============================================================================

// Concept using existing type traits
template<typename T>
concept Numeric = std::integral<T> || std::floating_point<T>;

// Concept checking for a specific operation
template<typename T>
concept Addable = requires(T a, T b) {
    a + b;  // Expression must be valid
};

// Concept with multiple operations
template<typename T>
concept Arithmetic = requires(T a, T b) {
    a + b;
    a - b;
    a * b;
    a / b;
};

// ============================================================================
// Requires expressions with compound requirements
// ============================================================================

// Compound requirement: checks expression validity AND return type
template<typename T>
concept Hashable = requires(T a) {
    { std::hash<T>{}(a) } -> std::convertible_to<std::size_t>;
};

// Multiple compound requirements
template<typename T>
concept StringLike = requires(T s) {
    { s.size() } -> std::convertible_to<std::size_t>;
    { s.empty() } -> std::convertible_to<bool>;
    { s[0] } -> std::convertible_to<char>;
    { s.c_str() } -> std::same_as<const char*>;
};

// ============================================================================
// Type requirements in concepts
// ============================================================================

// Check for nested types
template<typename T>
concept HasValueType = requires {
    typename T::value_type;
};

// Container concept with type and expression requirements
template<typename T>
concept Container = requires(T c) {
    typename T::value_type;      // Must have value_type
    typename T::iterator;        // Must have iterator type
    typename T::size_type;       // Must have size_type

    { c.begin() } -> std::same_as<typename T::iterator>;
    { c.end() } -> std::same_as<typename T::iterator>;
    { c.size() } -> std::convertible_to<typename T::size_type>;
    { c.empty() } -> std::convertible_to<bool>;
};

// ============================================================================
// Nested requirements
// ============================================================================

// Concept that requires another concept
template<typename T>
concept OrderedContainer = Container<T> && requires(T c) {
    requires std::totally_ordered<typename T::value_type>;
};

// Using nested requires for complex conditions
template<typename T>
concept SmallType = requires {
    requires sizeof(T) <= 8;
    requires std::is_trivially_copyable_v<T>;
};

// ============================================================================
// Concept composition
// ============================================================================

// Combine concepts with logical operators
template<typename T>
concept NumericAndOrdered = Numeric<T> && std::totally_ordered<T>;

template<typename T>
concept IntOrFloat = std::integral<T> || std::floating_point<T>;

// Build complex concepts from simpler ones
template<typename T>
concept Regular = std::copyable<T> && std::default_initializable<T>
               && std::equality_comparable<T>;

// ============================================================================
// Callable concepts
// ============================================================================

// Check if something is callable with specific arguments
template<typename F, typename... Args>
concept CallableWith = std::invocable<F, Args...>;

// Callable that returns a specific type
template<typename F, typename R, typename... Args>
concept CallableReturning = std::invocable<F, Args...>
    && std::convertible_to<std::invoke_result_t<F, Args...>, R>;

// Predicate concept (callable returning bool)
template<typename F, typename T>
concept Predicate = CallableReturning<F, bool, T>;

// ============================================================================
// Practical custom concepts
// ============================================================================

// Printable: can be output to ostream
template<typename T>
concept Printable = requires(std::ostream& os, const T& value) {
    { os << value } -> std::same_as<std::ostream&>;
};

// Incrementable: supports ++ operator
template<typename T>
concept Incrementable = requires(T t) {
    { ++t } -> std::same_as<T&>;
    { t++ } -> std::same_as<T>;
};

// Indexable: supports [] operator
template<typename T, typename Index = std::size_t>
concept Indexable = requires(T t, Index i) {
    t[i];
};

// ============================================================================
// Using custom concepts
// ============================================================================

// Function using Numeric concept
template<Numeric T>
T average(T a, T b) {
    return (a + b) / 2;
}

// Function using Hashable concept
template<Hashable T>
std::size_t get_hash(const T& value) {
    return std::hash<T>{}(value);
}

// Function using Container concept
template<Container C>
void print_container(const C& container) {
    std::cout << "[";
    bool first = true;
    for (const auto& elem : container) {
        if (!first) std::cout << ", ";
        std::cout << elem;
        first = false;
    }
    std::cout << "]\n";
}

// Function using Predicate concept
template<typename T, Predicate<T> P>
bool all_match(const std::vector<T>& vec, P pred) {
    for (const auto& elem : vec) {
        if (!pred(elem)) return false;
    }
    return true;
}

// ============================================================================
// Demonstrating concept satisfaction checks
// ============================================================================

void demonstrate_concept_checks() {
    std::cout << "=== Concept Satisfaction Checks ===\n\n";

    std::cout << "--- Numeric concept ---\n";
    std::cout << "Numeric<int>: " << Numeric<int> << "\n";
    std::cout << "Numeric<double>: " << Numeric<double> << "\n";
    std::cout << "Numeric<std::string>: " << Numeric<std::string> << "\n";

    std::cout << "\n--- Addable concept ---\n";
    std::cout << "Addable<int>: " << Addable<int> << "\n";
    std::cout << "Addable<std::string>: " << Addable<std::string> << "\n";

    std::cout << "\n--- Hashable concept ---\n";
    std::cout << "Hashable<int>: " << Hashable<int> << "\n";
    std::cout << "Hashable<std::string>: " << Hashable<std::string> << "\n";

    std::cout << "\n--- Container concept ---\n";
    std::cout << "Container<std::vector<int>>: " << Container<std::vector<int>> << "\n";
    std::cout << "Container<std::string>: " << Container<std::string> << "\n";
    std::cout << "Container<int>: " << Container<int> << "\n";

    std::cout << "\n--- SmallType concept ---\n";
    std::cout << "SmallType<int>: " << SmallType<int> << "\n";
    std::cout << "SmallType<char>: " << SmallType<char> << "\n";
    std::cout << "SmallType<std::string>: " << SmallType<std::string> << "\n";

    std::cout << "\n--- Printable concept ---\n";
    std::cout << "Printable<int>: " << Printable<int> << "\n";
    std::cout << "Printable<std::string>: " << Printable<std::string> << "\n";
}

// ============================================================================
// Main demonstration
// ============================================================================

int main() {
    std::cout << "=== Custom Concept Definitions ===\n\n";

    // Using Numeric concept
    std::cout << "--- Numeric Average ---\n";
    std::cout << "average(10, 20): " << average(10, 20) << "\n";
    std::cout << "average(3.0, 5.0): " << average(3.0, 5.0) << "\n";

    // Using Hashable concept
    std::cout << "\n--- Hashable ---\n";
    std::cout << "get_hash(42): " << get_hash(42) << "\n";
    std::cout << "get_hash(\"hello\"s): " << get_hash(std::string("hello")) << "\n";

    // Using Container concept
    std::cout << "\n--- Container ---\n";
    std::vector<int> numbers = {1, 2, 3, 4, 5};
    std::cout << "print_container: ";
    print_container(numbers);

    std::vector<std::string> words = {"hello", "world"};
    std::cout << "print_container: ";
    print_container(words);

    // Using Predicate concept
    std::cout << "\n--- Predicate ---\n";
    auto is_positive = [](int x) { return x > 0; };
    auto is_even = [](int x) { return x % 2 == 0; };

    std::vector<int> positive_nums = {1, 2, 3, 4, 5};
    std::vector<int> even_nums = {2, 4, 6, 8};

    std::cout << "all_match(positive_nums, is_positive): "
              << all_match(positive_nums, is_positive) << "\n";
    std::cout << "all_match(positive_nums, is_even): "
              << all_match(positive_nums, is_even) << "\n";
    std::cout << "all_match(even_nums, is_even): "
              << all_match(even_nums, is_even) << "\n";

    // Concept checks
    std::cout << "\n";
    demonstrate_concept_checks();

    // Static assertions for compile-time verification
    std::cout << "\n=== Static Assertions ===\n";
    static_assert(Numeric<int>);
    static_assert(Numeric<float>);
    static_assert(!Numeric<std::string>);
    static_assert(Addable<int>);
    static_assert(Addable<std::string>);  // string supports +
    static_assert(Container<std::vector<int>>);
    static_assert(Hashable<int>);
    static_assert(Hashable<std::string>);
    static_assert(Printable<int>);
    static_assert(SmallType<int>);
    static_assert(!SmallType<std::string>);  // string is not trivially copyable
    std::cout << "All static assertions passed!\n";

    return 0;
}
