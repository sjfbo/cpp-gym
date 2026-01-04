// Predicates and Function Objects
// Book reference: 13.4 Use of Predicates
//
// Predicates are functions (or function-like objects) that return bool.
// They're used with algorithms like find_if, count_if, sort, etc.
// Modern C++ prefers lambdas for most predicate use cases.

#include <algorithm>
#include <functional>
#include <iostream>
#include <string>
#include <vector>

// ============================================================================
// Traditional function objects (functors)
// ============================================================================

// Unary predicate: takes one argument, returns bool
struct IsEven {
    bool operator()(int x) const {
        return x % 2 == 0;
    }
};

// Configurable predicate with state
struct IsGreaterThan {
    int threshold;

    explicit IsGreaterThan(int t) : threshold{t} {}

    bool operator()(int x) const {
        return x > threshold;
    }
};

// Binary predicate: takes two arguments, returns bool
struct AbsoluteCompare {
    bool operator()(int a, int b) const {
        return std::abs(a) < std::abs(b);
    }
};

// Function object (transformer, not predicate)
struct Square {
    int operator()(int x) const {
        return x * x;
    }
};

// ============================================================================
// Main demonstration
// ============================================================================

int main() {
    std::cout << "=== Predicates and Function Objects ===\n\n";

    std::vector<int> nums{-5, 3, -2, 8, -1, 7, 4, -9, 6, 0};

    // -------------------------------------------------------------------------
    // Using function objects
    // -------------------------------------------------------------------------
    std::cout << "--- Function Objects (Functors) ---\n";

    // Count even numbers using functor
    auto even_count = std::count_if(nums.begin(), nums.end(), IsEven{});
    std::cout << "Even numbers (functor): " << even_count << "\n";

    // Find first element > 5
    auto it = std::find_if(nums.begin(), nums.end(), IsGreaterThan{5});
    if (it != nums.end()) {
        std::cout << "First element > 5: " << *it << "\n";
    }

    // Sort by absolute value
    std::vector<int> sorted_abs = nums;
    std::sort(sorted_abs.begin(), sorted_abs.end(), AbsoluteCompare{});
    std::cout << "Sorted by abs value: ";
    for (int x : sorted_abs) std::cout << x << " ";
    std::cout << "\n";

    // -------------------------------------------------------------------------
    // Using lambdas (modern, preferred)
    // -------------------------------------------------------------------------
    std::cout << "\n--- Lambda Expressions ---\n";

    // Basic lambda predicate
    auto even_count_lambda = std::count_if(nums.begin(), nums.end(),
        [](int x) { return x % 2 == 0; });
    std::cout << "Even numbers (lambda): " << even_count_lambda << "\n";

    // Lambda with capture (replaces configurable functor)
    int threshold = 5;
    auto it_lambda = std::find_if(nums.begin(), nums.end(),
        [threshold](int x) { return x > threshold; });
    if (it_lambda != nums.end()) {
        std::cout << "First element > " << threshold << ": " << *it_lambda << "\n";
    }

    // Mutable lambda (can modify captured variables)
    int call_count = 0;
    std::for_each(nums.begin(), nums.end(),
        [&call_count](int x) mutable {
            ++call_count;
            (void)x;  // Suppress unused warning
        });
    std::cout << "for_each called " << call_count << " times\n";

    // -------------------------------------------------------------------------
    // Lambda capture modes
    // -------------------------------------------------------------------------
    std::cout << "\n--- Lambda Capture Modes ---\n";

    int a = 10, b = 20;

    // Capture by value [=]
    auto by_value = [=]() { return a + b; };
    a = 100;  // Doesn't affect lambda
    std::cout << "[=] capture: " << by_value() << " (a changed but lambda has copy)\n";

    // Capture by reference [&]
    a = 10;  // Reset
    auto by_ref = [&]() { return a + b; };
    a = 100;  // Affects lambda
    std::cout << "[&] capture: " << by_ref() << " (a changed, lambda sees it)\n";

    // Mixed capture
    a = 10;
    auto mixed = [&a, b]() { return a + b; };  // a by ref, b by value
    a = 100;
    std::cout << "[&a, b] capture: " << mixed() << "\n";

    // Init capture (C++14) - create new variable in capture
    auto init_cap = [sum = a + b]() { return sum; };
    std::cout << "[sum = a + b] init capture: " << init_cap() << "\n";

    // -------------------------------------------------------------------------
    // Generic lambdas (auto parameters)
    // -------------------------------------------------------------------------
    std::cout << "\n--- Generic Lambdas (C++14/20) ---\n";

    // auto parameter - works with any type
    auto print_elem = [](const auto& x) { std::cout << x << " "; };

    std::vector<int> ints{1, 2, 3};
    std::vector<std::string> strs{"hello", "world"};

    std::cout << "Ints: ";
    std::for_each(ints.begin(), ints.end(), print_elem);
    std::cout << "\nStrings: ";
    std::for_each(strs.begin(), strs.end(), print_elem);
    std::cout << "\n";

    // Generic comparator
    auto generic_less = [](const auto& a, const auto& b) { return a < b; };
    std::sort(ints.begin(), ints.end(), generic_less);
    std::sort(strs.begin(), strs.end(), generic_less);

    // -------------------------------------------------------------------------
    // Standard library function objects
    // -------------------------------------------------------------------------
    std::cout << "\n--- Standard Library Function Objects ---\n";

    std::vector<int> to_sort{5, 2, 8, 1, 9};

    // std::greater for descending sort
    std::sort(to_sort.begin(), to_sort.end(), std::greater<int>{});
    std::cout << "Sorted descending (greater<int>): ";
    for (int x : to_sort) std::cout << x << " ";
    std::cout << "\n";

    // std::greater<> (transparent comparator, C++14)
    std::sort(to_sort.begin(), to_sort.end(), std::greater<>{});

    // Arithmetic function objects
    std::vector<int> v1{1, 2, 3, 4};
    std::vector<int> v2{10, 20, 30, 40};
    std::vector<int> result(4);

    std::transform(v1.begin(), v1.end(), v2.begin(), result.begin(),
                   std::plus<>{});
    std::cout << "v1 + v2: ";
    for (int x : result) std::cout << x << " ";
    std::cout << "\n";

    std::transform(v1.begin(), v1.end(), v2.begin(), result.begin(),
                   std::multiplies<>{});
    std::cout << "v1 * v2: ";
    for (int x : result) std::cout << x << " ";
    std::cout << "\n";

    // -------------------------------------------------------------------------
    // std::function for type-erased callable
    // -------------------------------------------------------------------------
    std::cout << "\n--- std::function ---\n";

    // Store any callable in std::function
    std::function<bool(int)> predicate;

    // Assign a lambda
    predicate = [](int x) { return x > 0; };
    std::cout << "Lambda predicate(5): " << predicate(5) << "\n";

    // Assign a functor
    predicate = IsEven{};
    std::cout << "Functor predicate(4): " << predicate(4) << "\n";

    // Assign a plain function pointer
    predicate = [](int x) { return x == 0; };
    std::cout << "Function predicate(0): " << predicate(0) << "\n";

    // Use in algorithm
    std::vector<std::function<int(int)>> transforms = {
        [](int x) { return x * 2; },
        [](int x) { return x + 10; },
        [](int x) { return x * x; }
    };

    int value = 5;
    std::cout << "Applying transforms to " << value << ": ";
    for (const auto& fn : transforms) {
        std::cout << fn(value) << " ";
    }
    std::cout << "\n";

    // -------------------------------------------------------------------------
    // Common predicate patterns
    // -------------------------------------------------------------------------
    std::cout << "\n--- Common Predicate Patterns ---\n";

    std::vector<int> data{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    // Check all elements match condition
    bool all_positive = std::all_of(data.begin(), data.end(),
        [](int x) { return x > 0; });
    std::cout << "All positive: " << (all_positive ? "yes" : "no") << "\n";

    // Check any element matches
    bool any_even = std::any_of(data.begin(), data.end(),
        [](int x) { return x % 2 == 0; });
    std::cout << "Any even: " << (any_even ? "yes" : "no") << "\n";

    // Check no element matches
    bool none_negative = std::none_of(data.begin(), data.end(),
        [](int x) { return x < 0; });
    std::cout << "None negative: " << (none_negative ? "yes" : "no") << "\n";

    // Partition: move elements matching predicate to front
    std::vector<int> to_partition{1, 4, 2, 5, 3, 6};
    auto partition_point = std::partition(to_partition.begin(), to_partition.end(),
        [](int x) { return x % 2 == 0; });
    std::cout << "After partition (even first): ";
    for (int x : to_partition) std::cout << x << " ";
    std::cout << "\nPartition point at index: "
              << std::distance(to_partition.begin(), partition_point) << "\n";

    return 0;
}

// Key takeaways:
// - Predicates are functions returning bool, used for filtering/comparison
// - Lambdas are preferred over function objects for most cases
// - Use [&] for reference capture, [=] for value capture
// - Generic lambdas (auto params) work with any type
// - Standard library provides greater<>, less<>, plus<>, etc.
// - std::function provides type-erased callable storage (with overhead)
