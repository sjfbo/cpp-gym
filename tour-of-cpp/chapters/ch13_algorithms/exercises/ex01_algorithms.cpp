// Exercise 13.1: Common Algorithm Usage
// Book reference: 13.5 Algorithm Overview
// Difficulty: **
//
// Instructions:
// Practice using standard library algorithms with predicates.
// Implement each function using the most appropriate algorithm.
// Prefer algorithms over raw loops.

#include <algorithm>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

// ============================================================================
// Part 1: Searching and Counting
// ============================================================================

// TODO 1: Find the first negative number in a vector.
// Return an iterator to it, or end() if not found.
// Use: std::find_if

std::vector<int>::const_iterator find_first_negative(const std::vector<int>& v) {
    // TODO: Implement using std::find_if
    (void)v;
    return v.end();
}


// TODO 2: Count how many strings have length greater than n.
// Use: std::count_if

int count_longer_than(const std::vector<std::string>& strings, std::size_t n) {
    // TODO: Implement using std::count_if
    (void)strings;
    (void)n;
    return 0;
}


// TODO 3: Check if all elements in a vector are positive.
// Use: std::all_of

bool all_positive(const std::vector<int>& v) {
    // TODO: Implement using std::all_of
    (void)v;
    return false;
}


// TODO 4: Check if any string contains a specific character.
// Use: std::any_of and std::string::find

bool any_contains_char(const std::vector<std::string>& strings, char c) {
    // TODO: Implement using std::any_of
    (void)strings;
    (void)c;
    return false;
}


// ============================================================================
// Part 2: Transforming
// ============================================================================

// TODO 5: Square all elements in a vector and return the result.
// Use: std::transform

std::vector<int> square_all(const std::vector<int>& v) {
    // TODO: Implement using std::transform
    (void)v;
    return {};
}


// TODO 6: Convert all strings to uppercase.
// Use: std::transform (twice - once for vector, once for each string)
// Hint: std::toupper for characters

std::vector<std::string> to_uppercase(const std::vector<std::string>& strings) {
    // TODO: Implement using std::transform
    (void)strings;
    return {};
}


// TODO 7: Compute element-wise sum of two vectors.
// Use: std::transform with binary operation

std::vector<int> add_vectors(const std::vector<int>& a, const std::vector<int>& b) {
    // TODO: Implement using std::transform
    // Assume a and b have the same size
    (void)a;
    (void)b;
    return {};
}


// ============================================================================
// Part 3: Accumulating
// ============================================================================

// TODO 8: Compute the product of all elements.
// Use: std::accumulate with custom operation

long long product_of(const std::vector<int>& v) {
    // TODO: Implement using std::accumulate
    (void)v;
    return 0;
}


// TODO 9: Concatenate all strings with a separator.
// Example: {"a", "b", "c"} with sep=" - " -> "a - b - c"
// Use: std::accumulate

std::string join_strings(const std::vector<std::string>& strings,
                        const std::string& separator) {
    // TODO: Implement using std::accumulate
    // Hint: Handle the first element specially to avoid leading separator
    (void)strings;
    (void)separator;
    return "";
}


// TODO 10: Compute the average of a vector of doubles.
// Return 0.0 for empty vector.
// Use: std::accumulate

double average(const std::vector<double>& v) {
    // TODO: Implement using std::accumulate
    (void)v;
    return 0.0;
}


// ============================================================================
// Part 4: Sorting and Ordering
// ============================================================================

// TODO 11: Sort a vector of pairs by the second element.
// Use: std::sort with custom comparator

void sort_by_second(std::vector<std::pair<std::string, int>>& pairs) {
    // TODO: Implement using std::sort
    (void)pairs;
}


// TODO 12: Get the top N elements from a vector (sorted descending).
// Use: std::partial_sort or std::sort

std::vector<int> top_n(const std::vector<int>& v, std::size_t n) {
    // TODO: Implement
    // Return at most n elements, sorted from largest to smallest
    (void)v;
    (void)n;
    return {};
}


// TODO 13: Partition a vector into even and odd numbers.
// Return the partition point (iterator to first odd number).
// Use: std::partition

std::vector<int>::iterator partition_even_odd(std::vector<int>& v) {
    // TODO: Implement using std::partition
    // Even numbers should come before odd numbers
    (void)v;
    return v.begin();
}


// ============================================================================
// Part 5: Removing and Modifying
// ============================================================================

// TODO 14: Remove all occurrences of a value from a vector.
// Use: std::erase (C++20) or erase-remove idiom

void remove_value(std::vector<int>& v, int value) {
    // TODO: Implement
    (void)v;
    (void)value;
}


// TODO 15: Remove consecutive duplicates from a sorted vector.
// Use: std::unique and erase

void remove_consecutive_duplicates(std::vector<int>& v) {
    // TODO: Implement using std::unique
    (void)v;
}


// TODO 16: Replace all negative values with zero.
// Use: std::replace_if

void replace_negatives_with_zero(std::vector<int>& v) {
    // TODO: Implement using std::replace_if
    (void)v;
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
    std::cout << "=== Exercise 13.1: Common Algorithm Usage ===\n\n";

    // Test 1: find_first_negative
    std::cout << "--- Test 1: find_first_negative ---\n";
    std::vector<int> nums1{5, 3, -2, 8, -1};
    auto neg_it = find_first_negative(nums1);
    if (neg_it != nums1.end()) {
        std::cout << "First negative: " << *neg_it << "\n";
    }
    // Expected: -2

    // Test 2: count_longer_than
    std::cout << "\n--- Test 2: count_longer_than ---\n";
    std::vector<std::string> words{"apple", "cat", "banana", "dog", "elephant"};
    std::cout << "Strings longer than 4: " << count_longer_than(words, 4) << "\n";
    // Expected: 3 (apple, banana, elephant)

    // Test 3: all_positive
    std::cout << "\n--- Test 3: all_positive ---\n";
    std::vector<int> pos{1, 2, 3, 4, 5};
    std::vector<int> mixed{1, -2, 3};
    std::cout << "all_positive({1,2,3,4,5}): " << (all_positive(pos) ? "true" : "false") << "\n";
    std::cout << "all_positive({1,-2,3}): " << (all_positive(mixed) ? "true" : "false") << "\n";

    // Test 4: any_contains_char
    std::cout << "\n--- Test 4: any_contains_char ---\n";
    std::cout << "any contains 'a': " << (any_contains_char(words, 'a') ? "true" : "false") << "\n";
    std::cout << "any contains 'z': " << (any_contains_char(words, 'z') ? "true" : "false") << "\n";

    // Test 5: square_all
    std::cout << "\n--- Test 5: square_all ---\n";
    std::vector<int> to_square{1, 2, 3, 4, 5};
    print_vector("squared", square_all(to_square));
    // Expected: [1, 4, 9, 16, 25]

    // Test 6: to_uppercase
    std::cout << "\n--- Test 6: to_uppercase ---\n";
    std::vector<std::string> lower{"hello", "world"};
    auto upper = to_uppercase(lower);
    print_vector("uppercase", upper);
    // Expected: [HELLO, WORLD]

    // Test 7: add_vectors
    std::cout << "\n--- Test 7: add_vectors ---\n";
    std::vector<int> a{1, 2, 3};
    std::vector<int> b{10, 20, 30};
    print_vector("a + b", add_vectors(a, b));
    // Expected: [11, 22, 33]

    // Test 8: product_of
    std::cout << "\n--- Test 8: product_of ---\n";
    std::vector<int> factors{2, 3, 4, 5};
    std::cout << "Product: " << product_of(factors) << "\n";
    // Expected: 120

    // Test 9: join_strings
    std::cout << "\n--- Test 9: join_strings ---\n";
    std::vector<std::string> parts{"apple", "banana", "cherry"};
    std::cout << "Joined: " << join_strings(parts, " - ") << "\n";
    // Expected: "apple - banana - cherry"

    // Test 10: average
    std::cout << "\n--- Test 10: average ---\n";
    std::vector<double> scores{80.0, 90.0, 85.0, 95.0};
    std::cout << "Average: " << average(scores) << "\n";
    // Expected: 87.5

    // Test 11: sort_by_second
    std::cout << "\n--- Test 11: sort_by_second ---\n";
    std::vector<std::pair<std::string, int>> pairs{
        {"apple", 3}, {"banana", 1}, {"cherry", 2}
    };
    sort_by_second(pairs);
    for (const auto& [name, val] : pairs) {
        std::cout << name << ":" << val << " ";
    }
    std::cout << "\n";
    // Expected: banana:1 cherry:2 apple:3

    // Test 12: top_n
    std::cout << "\n--- Test 12: top_n ---\n";
    std::vector<int> top_test{3, 1, 4, 1, 5, 9, 2, 6};
    print_vector("top 3", top_n(top_test, 3));
    // Expected: [9, 6, 5]

    // Test 13: partition_even_odd
    std::cout << "\n--- Test 13: partition_even_odd ---\n";
    std::vector<int> part_test{1, 2, 3, 4, 5, 6, 7, 8};
    auto part_point = partition_even_odd(part_test);
    print_vector("partitioned", part_test);
    std::cout << "First odd at index: " << std::distance(part_test.begin(), part_point) << "\n";

    // Test 14: remove_value
    std::cout << "\n--- Test 14: remove_value ---\n";
    std::vector<int> remove_test{1, 2, 3, 2, 4, 2, 5};
    remove_value(remove_test, 2);
    print_vector("after removing 2s", remove_test);
    // Expected: [1, 3, 4, 5]

    // Test 15: remove_consecutive_duplicates
    std::cout << "\n--- Test 15: remove_consecutive_duplicates ---\n";
    std::vector<int> dup_test{1, 1, 2, 2, 2, 3, 3, 4};
    remove_consecutive_duplicates(dup_test);
    print_vector("unique", dup_test);
    // Expected: [1, 2, 3, 4]

    // Test 16: replace_negatives_with_zero
    std::cout << "\n--- Test 16: replace_negatives_with_zero ---\n";
    std::vector<int> neg_test{1, -2, 3, -4, 5, -6};
    replace_negatives_with_zero(neg_test);
    print_vector("replaced", neg_test);
    // Expected: [1, 0, 3, 0, 5, 0]

    return 0;
}
