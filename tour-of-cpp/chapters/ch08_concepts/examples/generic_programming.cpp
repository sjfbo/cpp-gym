// Generic Programming with Concepts
// Book reference: 8.3 Generic Programming
//
// This example demonstrates how concepts improve generic programming
// by constraining templates for algorithms and data structures,
// enabling concept-based overloading, and documenting requirements.

#include <algorithm>
#include <concepts>
#include <iostream>
#include <iterator>
#include <list>
#include <numeric>
#include <ranges>
#include <string>
#include <vector>

// ============================================================================
// Iterator concepts for algorithm constraints
// ============================================================================

// Concept for input iterators
template<typename I>
concept InputIter = std::input_iterator<I>;

// Concept for forward iterators
template<typename I>
concept ForwardIter = std::forward_iterator<I>;

// Concept for random access iterators
template<typename I>
concept RandomAccessIter = std::random_access_iterator<I>;

// ============================================================================
// Concept-constrained algorithms
// ============================================================================

// Find algorithm that works with any input iterator
template<InputIter I, std::sentinel_for<I> S, typename T>
    requires std::equality_comparable_with<std::iter_value_t<I>, T>
I my_find(I first, S last, const T& value) {
    while (first != last) {
        if (*first == value) {
            return first;
        }
        ++first;
    }
    return first;
}

// Count algorithm
template<InputIter I, std::sentinel_for<I> S, typename T>
    requires std::equality_comparable_with<std::iter_value_t<I>, T>
std::iter_difference_t<I> my_count(I first, S last, const T& value) {
    std::iter_difference_t<I> count = 0;
    while (first != last) {
        if (*first == value) {
            ++count;
        }
        ++first;
    }
    return count;
}

// Accumulate with concept constraints
template<InputIter I, std::sentinel_for<I> S, typename T>
    requires requires(T acc, std::iter_value_t<I> val) { acc + val; }
T my_accumulate(I first, S last, T init) {
    while (first != last) {
        init = init + *first;
        ++first;
    }
    return init;
}

// ============================================================================
// Concept-based overloading for optimization
// ============================================================================

// Generic distance (works with any input iterator)
template<InputIter I, std::sentinel_for<I> S>
std::iter_difference_t<I> my_distance(I first, S last) {
    std::iter_difference_t<I> n = 0;
    while (first != last) {
        ++first;
        ++n;
    }
    return n;
}

// Optimized distance for random access iterators
template<RandomAccessIter I, std::sentinel_for<I> S>
    requires std::sized_sentinel_for<S, I>
std::iter_difference_t<I> my_distance(I first, S last) {
    return last - first;  // O(1) instead of O(n)
}

// ============================================================================
// Generic container algorithms
// ============================================================================

// Concept for ranges/containers
template<typename R>
concept Range = requires(R& r) {
    std::ranges::begin(r);
    std::ranges::end(r);
};

// Sum elements in any range
template<Range R>
    requires std::integral<std::ranges::range_value_t<R>>
          || std::floating_point<std::ranges::range_value_t<R>>
auto sum(const R& range) {
    using ValueType = std::ranges::range_value_t<R>;
    ValueType result{};
    for (const auto& elem : range) {
        result += elem;
    }
    return result;
}

// Find max element in any range
template<Range R>
    requires std::totally_ordered<std::ranges::range_value_t<R>>
auto max_element(const R& range) -> std::ranges::range_value_t<R> {
    auto it = std::ranges::begin(range);
    auto end = std::ranges::end(range);

    if (it == end) {
        return std::ranges::range_value_t<R>{};
    }

    auto max_val = *it;
    ++it;
    while (it != end) {
        if (*it > max_val) {
            max_val = *it;
        }
        ++it;
    }
    return max_val;
}

// ============================================================================
// Sortable concept and sorting
// ============================================================================

template<typename T>
concept Sortable = std::totally_ordered<T> && std::movable<T>;

// Simple bubble sort (for demonstration)
template<Range R>
    requires Sortable<std::ranges::range_value_t<R>>
void bubble_sort(R& range) {
    auto begin = std::ranges::begin(range);
    auto end = std::ranges::end(range);

    for (auto i = begin; i != end; ++i) {
        for (auto j = begin; j != end; ++j) {
            auto next = j;
            ++next;
            if (next != end && *next < *j) {
                std::swap(*j, *next);
            }
        }
    }
}

// ============================================================================
// Template with multiple concept constraints
// ============================================================================

// Transform with concept constraints
template<Range R, typename F>
    requires std::invocable<F, std::ranges::range_value_t<R>>
auto transform_copy(const R& range, F func) {
    using InputType = std::ranges::range_value_t<R>;
    using OutputType = std::invoke_result_t<F, InputType>;

    std::vector<OutputType> result;
    for (const auto& elem : range) {
        result.push_back(func(elem));
    }
    return result;
}

// Filter with predicate concept
template<Range R, typename Pred>
    requires std::predicate<Pred, std::ranges::range_value_t<R>>
auto filter_copy(const R& range, Pred pred) {
    using ValueType = std::ranges::range_value_t<R>;

    std::vector<ValueType> result;
    for (const auto& elem : range) {
        if (pred(elem)) {
            result.push_back(elem);
        }
    }
    return result;
}

// ============================================================================
// Demonstrating subsumption
// ============================================================================

// Base case: any integral
template<std::integral T>
std::string describe_type(T) {
    return "integral";
}

// More specific: signed integral (preferred over integral for signed types)
template<std::signed_integral T>
std::string describe_type(T) {
    return "signed_integral";
}

// Even more specific: same_as int AND signed_integral (subsumes signed_integral)
template<typename T>
    requires std::signed_integral<T> && std::same_as<T, int>
std::string describe_type(T) {
    return "int";
}

// ============================================================================
// Helper for printing containers
// ============================================================================

template<Range R>
void print_range(const std::string& label, const R& range) {
    std::cout << label << ": [";
    bool first = true;
    for (const auto& elem : range) {
        if (!first) std::cout << ", ";
        std::cout << elem;
        first = false;
    }
    std::cout << "]\n";
}

// ============================================================================
// Main demonstration
// ============================================================================

int main() {
    std::cout << "=== Generic Programming with Concepts ===\n\n";

    // Test data
    std::vector<int> vec = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3};
    std::list<int> lst = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3};

    print_range("Vector", vec);
    print_range("List", lst);

    // Custom algorithms
    std::cout << "\n--- Custom Algorithms ---\n";

    auto it = my_find(vec.begin(), vec.end(), 5);
    std::cout << "my_find(vec, 5): found at index "
              << std::distance(vec.begin(), it) << "\n";

    std::cout << "my_count(vec, 1): " << my_count(vec.begin(), vec.end(), 1) << "\n";
    std::cout << "my_accumulate(vec, 0): "
              << my_accumulate(vec.begin(), vec.end(), 0) << "\n";

    // Distance with concept-based overloading
    std::cout << "\n--- Distance (Concept-Based Overloading) ---\n";
    std::cout << "my_distance on vector (O(1)): "
              << my_distance(vec.begin(), vec.end()) << "\n";
    std::cout << "my_distance on list (O(n)): "
              << my_distance(lst.begin(), lst.end()) << "\n";

    // Range algorithms
    std::cout << "\n--- Range Algorithms ---\n";
    std::cout << "sum(vec): " << sum(vec) << "\n";
    std::cout << "max_element(vec): " << max_element(vec) << "\n";

    std::vector<double> doubles = {1.5, 2.5, 3.5};
    std::cout << "sum(doubles): " << sum(doubles) << "\n";
    std::cout << "max_element(doubles): " << max_element(doubles) << "\n";

    // Sorting
    std::cout << "\n--- Sorting ---\n";
    std::vector<int> to_sort = {5, 2, 8, 1, 9};
    print_range("Before sort", to_sort);
    bubble_sort(to_sort);
    print_range("After sort", to_sort);

    // Transform
    std::cout << "\n--- Transform ---\n";
    auto squared = transform_copy(vec, [](int x) { return x * x; });
    print_range("Squared", squared);

    auto as_strings = transform_copy(vec, [](int x) { return std::to_string(x); });
    print_range("As strings", as_strings);

    // Filter
    std::cout << "\n--- Filter ---\n";
    auto evens = filter_copy(vec, [](int x) { return x % 2 == 0; });
    print_range("Even numbers", evens);

    auto greater_than_4 = filter_copy(vec, [](int x) { return x > 4; });
    print_range("Greater than 4", greater_than_4);

    // Subsumption demonstration
    std::cout << "\n--- Subsumption (Most Specific Overload) ---\n";
    std::cout << "describe_type(42): " << describe_type(42) << "\n";
    std::cout << "describe_type(42L): " << describe_type(42L) << "\n";
    std::cout << "describe_type(42u): " << describe_type(42u) << "\n";
    std::cout << "describe_type(short(42)): " << describe_type(short(42)) << "\n";

    // Compile-time checks
    std::cout << "\n=== Compile-Time Checks ===\n";
    static_assert(InputIter<std::vector<int>::iterator>);
    static_assert(RandomAccessIter<std::vector<int>::iterator>);
    static_assert(ForwardIter<std::list<int>::iterator>);
    static_assert(!RandomAccessIter<std::list<int>::iterator>);
    static_assert(Range<std::vector<int>>);
    static_assert(Range<std::string>);
    static_assert(Sortable<int>);
    static_assert(Sortable<std::string>);
    std::cout << "All compile-time checks passed!\n";

    return 0;
}
