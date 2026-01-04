// Exercise 13.2: Implementing Custom Algorithms
// Book reference: 13.5 Algorithm Overview
// Difficulty: ***
//
// Instructions:
// Implement your own versions of common STL algorithms.
// This helps you understand how algorithms work internally.
// Use iterator concepts and follow the STL algorithm patterns.

#include <algorithm>
#include <functional>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

// ============================================================================
// Part 1: Non-modifying algorithms
// ============================================================================

// TODO 1: Implement my_find
// Find the first element equal to value in range [first, last).
// Return iterator to the element, or last if not found.

template <typename InputIt, typename T>
InputIt my_find(InputIt first, InputIt last, const T& value) {
    // TODO: Implement
    (void)first;
    (void)value;
    return last;
}


// TODO 2: Implement my_find_if
// Find the first element for which predicate returns true.
// Return iterator to the element, or last if not found.

template <typename InputIt, typename Predicate>
InputIt my_find_if(InputIt first, InputIt last, Predicate pred) {
    // TODO: Implement
    (void)first;
    (void)pred;
    return last;
}


// TODO 3: Implement my_count
// Count elements equal to value in range [first, last).

template <typename InputIt, typename T>
typename std::iterator_traits<InputIt>::difference_type
my_count(InputIt first, InputIt last, const T& value) {
    // TODO: Implement
    (void)first;
    (void)last;
    (void)value;
    return 0;
}


// TODO 4: Implement my_count_if
// Count elements for which predicate returns true.

template <typename InputIt, typename Predicate>
typename std::iterator_traits<InputIt>::difference_type
my_count_if(InputIt first, InputIt last, Predicate pred) {
    // TODO: Implement
    (void)first;
    (void)last;
    (void)pred;
    return 0;
}


// TODO 5: Implement my_all_of
// Return true if predicate returns true for all elements.
// Return true for empty range.

template <typename InputIt, typename Predicate>
bool my_all_of(InputIt first, InputIt last, Predicate pred) {
    // TODO: Implement
    (void)first;
    (void)last;
    (void)pred;
    return false;
}


// TODO 6: Implement my_any_of
// Return true if predicate returns true for at least one element.
// Return false for empty range.

template <typename InputIt, typename Predicate>
bool my_any_of(InputIt first, InputIt last, Predicate pred) {
    // TODO: Implement
    (void)first;
    (void)last;
    (void)pred;
    return false;
}


// TODO 7: Implement my_none_of
// Return true if predicate returns false for all elements.
// Return true for empty range.

template <typename InputIt, typename Predicate>
bool my_none_of(InputIt first, InputIt last, Predicate pred) {
    // TODO: Implement
    (void)first;
    (void)last;
    (void)pred;
    return false;
}


// ============================================================================
// Part 2: Modifying algorithms
// ============================================================================

// TODO 8: Implement my_transform
// Apply unary operation to each element in [first, last) and write to d_first.
// Return iterator past the last element written.

template <typename InputIt, typename OutputIt, typename UnaryOp>
OutputIt my_transform(InputIt first, InputIt last, OutputIt d_first, UnaryOp op) {
    // TODO: Implement
    (void)first;
    (void)last;
    (void)op;
    return d_first;
}


// TODO 9: Implement my_copy_if
// Copy elements for which predicate returns true to d_first.
// Return iterator past the last element copied.

template <typename InputIt, typename OutputIt, typename Predicate>
OutputIt my_copy_if(InputIt first, InputIt last, OutputIt d_first, Predicate pred) {
    // TODO: Implement
    (void)first;
    (void)last;
    (void)pred;
    return d_first;
}


// TODO 10: Implement my_fill
// Assign value to all elements in range [first, last).

template <typename ForwardIt, typename T>
void my_fill(ForwardIt first, ForwardIt last, const T& value) {
    // TODO: Implement
    (void)first;
    (void)last;
    (void)value;
}


// TODO 11: Implement my_replace_if
// Replace elements for which predicate returns true with new_value.

template <typename ForwardIt, typename Predicate, typename T>
void my_replace_if(ForwardIt first, ForwardIt last, Predicate pred,
                   const T& new_value) {
    // TODO: Implement
    (void)first;
    (void)last;
    (void)pred;
    (void)new_value;
}


// ============================================================================
// Part 3: Reduction algorithms
// ============================================================================

// TODO 12: Implement my_accumulate
// Fold left: compute init op a0 op a1 op ... op an.
// Default operation is addition.

template <typename InputIt, typename T, typename BinaryOp = std::plus<>>
T my_accumulate(InputIt first, InputIt last, T init, BinaryOp op = BinaryOp{}) {
    // TODO: Implement
    (void)first;
    (void)last;
    (void)op;
    return init;
}


// TODO 13: Implement my_for_each
// Apply function to each element in range [first, last).
// Return the function object (it may have accumulated state).

template <typename InputIt, typename UnaryFunc>
UnaryFunc my_for_each(InputIt first, InputIt last, UnaryFunc f) {
    // TODO: Implement
    (void)first;
    (void)last;
    return f;
}


// ============================================================================
// Part 4: Advanced algorithms
// ============================================================================

// TODO 14: Implement my_partition
// Reorder elements so that elements for which predicate returns true
// come before elements for which it returns false.
// Return iterator to the first element of the second group.
// Hint: Use two-pointer technique from both ends.

template <typename ForwardIt, typename Predicate>
ForwardIt my_partition(ForwardIt first, ForwardIt last, Predicate pred) {
    // TODO: Implement
    // This is trickier - think about swapping elements
    (void)pred;
    (void)last;
    return first;
}


// TODO 15: Implement my_rotate
// Rotate elements so that the element pointed by middle becomes first.
// Elements [first, middle) move after [middle, last).
// Return iterator to the element originally pointed to by first.
// Hint: This can be done with three reverses.

template <typename ForwardIt>
ForwardIt my_rotate(ForwardIt first, ForwardIt middle, ForwardIt last) {
    // TODO: Implement
    // Hint: reverse(first, middle), reverse(middle, last), reverse(first, last)
    (void)middle;
    (void)last;
    return first;
}


// ============================================================================
// Tests
// ============================================================================

template <typename T>
void print_vector(const std::string& label, const std::vector<T>& v) {
    std::cout << label << ": [";
    for (std::size_t i = 0; i < v.size(); ++i) {
        std::cout << v[i];
        if (i < v.size() - 1) std::cout << ", ";
    }
    std::cout << "]\n";
}

int main() {
    std::cout << "=== Exercise 13.2: Custom Algorithm Implementations ===\n\n";

    // Test data
    std::vector<int> nums{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    auto is_even = [](int x) { return x % 2 == 0; };
    auto is_positive = [](int x) { return x > 0; };

    // Test 1: my_find
    std::cout << "--- Test 1: my_find ---\n";
    auto it = my_find(nums.begin(), nums.end(), 5);
    if (it != nums.end()) {
        std::cout << "Found 5 at index: " << std::distance(nums.begin(), it) << "\n";
    }
    // Expected: index 4

    // Test 2: my_find_if
    std::cout << "\n--- Test 2: my_find_if ---\n";
    auto even_it = my_find_if(nums.begin(), nums.end(), is_even);
    if (even_it != nums.end()) {
        std::cout << "First even: " << *even_it << "\n";
    }
    // Expected: 2

    // Test 3: my_count
    std::cout << "\n--- Test 3: my_count ---\n";
    std::vector<int> with_dups{1, 2, 2, 3, 2, 4, 2};
    std::cout << "Count of 2: " << my_count(with_dups.begin(), with_dups.end(), 2) << "\n";
    // Expected: 4

    // Test 4: my_count_if
    std::cout << "\n--- Test 4: my_count_if ---\n";
    std::cout << "Count of evens in 1-10: "
              << my_count_if(nums.begin(), nums.end(), is_even) << "\n";
    // Expected: 5

    // Test 5-7: my_all_of, my_any_of, my_none_of
    std::cout << "\n--- Test 5-7: all_of, any_of, none_of ---\n";
    std::vector<int> all_pos{1, 2, 3, 4, 5};
    std::vector<int> some_pos{-1, 0, 1, 2};
    std::vector<int> no_pos{-5, -4, -3, -2, -1};

    std::cout << "all_positive({1,2,3,4,5}): "
              << (my_all_of(all_pos.begin(), all_pos.end(), is_positive) ? "true" : "false") << "\n";
    std::cout << "any_positive({-1,0,1,2}): "
              << (my_any_of(some_pos.begin(), some_pos.end(), is_positive) ? "true" : "false") << "\n";
    std::cout << "none_positive({-5,-4,-3,-2,-1}): "
              << (my_none_of(no_pos.begin(), no_pos.end(), is_positive) ? "true" : "false") << "\n";

    // Test 8: my_transform
    std::cout << "\n--- Test 8: my_transform ---\n";
    std::vector<int> src{1, 2, 3, 4, 5};
    std::vector<int> dst(5);
    my_transform(src.begin(), src.end(), dst.begin(), [](int x) { return x * x; });
    print_vector("squared", dst);
    // Expected: [1, 4, 9, 16, 25]

    // Test 9: my_copy_if
    std::cout << "\n--- Test 9: my_copy_if ---\n";
    std::vector<int> evens;
    my_copy_if(nums.begin(), nums.end(), std::back_inserter(evens), is_even);
    print_vector("evens", evens);
    // Expected: [2, 4, 6, 8, 10]

    // Test 10: my_fill
    std::cout << "\n--- Test 10: my_fill ---\n";
    std::vector<int> filled(5);
    my_fill(filled.begin(), filled.end(), 42);
    print_vector("filled", filled);
    // Expected: [42, 42, 42, 42, 42]

    // Test 11: my_replace_if
    std::cout << "\n--- Test 11: my_replace_if ---\n";
    std::vector<int> to_replace{1, -2, 3, -4, 5, -6};
    my_replace_if(to_replace.begin(), to_replace.end(),
                  [](int x) { return x < 0; }, 0);
    print_vector("replaced negatives", to_replace);
    // Expected: [1, 0, 3, 0, 5, 0]

    // Test 12: my_accumulate
    std::cout << "\n--- Test 12: my_accumulate ---\n";
    std::vector<int> to_sum{1, 2, 3, 4, 5};
    std::cout << "Sum: " << my_accumulate(to_sum.begin(), to_sum.end(), 0) << "\n";
    std::cout << "Product: " << my_accumulate(to_sum.begin(), to_sum.end(), 1,
                                               std::multiplies<>{}) << "\n";
    // Expected: Sum: 15, Product: 120

    // Test 13: my_for_each
    std::cout << "\n--- Test 13: my_for_each ---\n";
    struct Counter {
        int count = 0;
        void operator()(int) { ++count; }
    };
    Counter c = my_for_each(nums.begin(), nums.end(), Counter{});
    std::cout << "Visited " << c.count << " elements\n";
    // Expected: 10

    // Test 14: my_partition
    std::cout << "\n--- Test 14: my_partition ---\n";
    std::vector<int> to_partition{1, 2, 3, 4, 5, 6, 7, 8};
    auto part_point = my_partition(to_partition.begin(), to_partition.end(), is_even);
    print_vector("partitioned (evens first)", to_partition);
    std::cout << "Partition point at index: "
              << std::distance(to_partition.begin(), part_point) << "\n";
    // Expected: evens before odds, partition point at index 4

    // Test 15: my_rotate
    std::cout << "\n--- Test 15: my_rotate ---\n";
    std::vector<int> to_rotate{1, 2, 3, 4, 5};
    my_rotate(to_rotate.begin(), to_rotate.begin() + 2, to_rotate.end());
    print_vector("rotated by 2", to_rotate);
    // Expected: [3, 4, 5, 1, 2]

    std::cout << "\nImplementation complete!\n";

    return 0;
}

// Tips for implementing:
// 1. Start simple - get the basic loop right first
// 2. Use iterator dereference (*it) to access elements
// 3. Use pre-increment (++it) to advance iterators
// 4. Remember that [first, last) is half-open interval
// 5. Handle edge cases like empty ranges
// 6. Test with different container types (vector, list, etc.)
