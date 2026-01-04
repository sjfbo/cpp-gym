// Exercise 14.1: Working with Ranges
// Book reference: 14.2-14.4
// Difficulty: ***
//
// Instructions:
// Implement each function using C++20 ranges and views.
// Prefer pipelines over raw loops.
// Use range algorithms where appropriate.

#include <algorithm>
#include <iostream>
#include <map>
#include <numeric>
#include <ranges>
#include <string>
#include <vector>

namespace views = std::views;
namespace ranges = std::ranges;

// ============================================================================
// Part 1: Basic View Operations
// ============================================================================

// TODO 1: Return the first n even numbers from a vector.
// Use: views::filter and views::take

std::vector<int> first_n_evens(const std::vector<int>& v, int n) {
    // TODO: Implement using views::filter and views::take
    // Hint: You'll need to materialize the view into a vector
    (void)v;
    (void)n;
    return {};
}


// TODO 2: Square all numbers, then keep only those > 10.
// Use: views::transform and views::filter

std::vector<int> squares_greater_than_10(const std::vector<int>& v) {
    // TODO: Implement using a pipeline
    (void)v;
    return {};
}


// TODO 3: Get every nth element from a vector.
// Example: every_nth({1,2,3,4,5,6,7,8,9}, 3) -> {1,4,7}
// Use: views::filter or views::iota with transform

std::vector<int> every_nth(const std::vector<int>& v, int n) {
    // TODO: Implement
    (void)v;
    (void)n;
    return {};
}


// ============================================================================
// Part 2: String Processing
// ============================================================================

// TODO 4: Split a string by delimiter and return non-empty parts as vector.
// Use: views::split

std::vector<std::string> split_string(const std::string& s, char delim) {
    // TODO: Implement using views::split
    (void)s;
    (void)delim;
    return {};
}


// TODO 5: Get words longer than n characters from a sentence.
// Use: views::split and views::filter

std::vector<std::string> words_longer_than(const std::string& sentence, std::size_t n) {
    // TODO: Implement
    (void)sentence;
    (void)n;
    return {};
}


// ============================================================================
// Part 3: Working with Structs
// ============================================================================

struct Product {
    std::string name;
    double price;
    int quantity;
};

// TODO 6: Get names of products with price > threshold.
// Use: views::filter and views::transform

std::vector<std::string> expensive_product_names(
    const std::vector<Product>& products, double threshold) {
    // TODO: Implement using a pipeline
    (void)products;
    (void)threshold;
    return {};
}


// TODO 7: Calculate total inventory value (sum of price * quantity).
// Use: views::transform and std::accumulate or ranges algorithm

double total_inventory_value(const std::vector<Product>& products) {
    // TODO: Implement
    (void)products;
    return 0.0;
}


// TODO 8: Sort products by price (descending) and return top n names.
// Note: You'll need to copy first since views can't sort

std::vector<std::string> top_n_by_price(
    std::vector<Product> products, int n) {
    // TODO: Sort products by price descending, return top n names
    (void)products;
    (void)n;
    return {};
}


// ============================================================================
// Part 4: Combining Views
// ============================================================================

// TODO 9: Flatten a 2D vector and return unique sorted elements.
// Use: views::join, then sort and unique on result

std::vector<int> flatten_unique_sorted(
    const std::vector<std::vector<int>>& nested) {
    // TODO: Implement
    (void)nested;
    return {};
}


// TODO 10: Generate first n Fizz-Buzz strings.
// 1, 2, Fizz, 4, Buzz, Fizz, 7, 8, Fizz, Buzz, 11, Fizz, 13, 14, FizzBuzz...
// Use: views::iota and views::transform

std::vector<std::string> fizzbuzz(int n) {
    // TODO: Implement using views::iota
    (void)n;
    return {};
}


// ============================================================================
// Part 5: Advanced Operations
// ============================================================================

// TODO 11: Find the longest streak of consecutive increasing numbers.
// Example: {1,2,5,6,7,3,4} -> streak of 3 (5,6,7)
// This is harder to do with views alone - use algorithms

int longest_increasing_streak(const std::vector<int>& v) {
    // TODO: Implement
    (void)v;
    return 0;
}


// TODO 12: Sliding window maximum.
// For each position, find max in window of size k.
// Example: sliding_max({1,3,1,5,2}, 3) -> {3,5,5}
// (windows: [1,3,1], [3,1,5], [1,5,2])

std::vector<int> sliding_window_max(const std::vector<int>& v, int k) {
    // TODO: Implement
    (void)v;
    (void)k;
    return {};
}


// ============================================================================
// Tests
// ============================================================================

template <typename T>
void print_vector(const std::string& label, const std::vector<T>& v) {
    std::cout << label << ": [";
    for (std::size_t i = 0; i < v.size(); ++i) {
        if constexpr (std::is_same_v<T, std::string>) {
            std::cout << '"' << v[i] << '"';
        } else {
            std::cout << v[i];
        }
        if (i < v.size() - 1) std::cout << ", ";
    }
    std::cout << "]\n";
}

int main() {
    std::cout << "=== Exercise 14.1: Working with Ranges ===\n\n";

    // Test 1: first_n_evens
    std::cout << "--- Test 1: first_n_evens ---\n";
    std::vector<int> nums{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    print_vector("first_n_evens({1..10}, 3)", first_n_evens(nums, 3));
    // Expected: [2, 4, 6]

    // Test 2: squares_greater_than_10
    std::cout << "\n--- Test 2: squares_greater_than_10 ---\n";
    std::vector<int> small{1, 2, 3, 4, 5};
    print_vector("squares_greater_than_10({1..5})", squares_greater_than_10(small));
    // Expected: [16, 25] (4^2=16, 5^2=25)

    // Test 3: every_nth
    std::cout << "\n--- Test 3: every_nth ---\n";
    std::vector<int> seq{1, 2, 3, 4, 5, 6, 7, 8, 9};
    print_vector("every_nth({1..9}, 3)", every_nth(seq, 3));
    // Expected: [1, 4, 7]

    // Test 4: split_string
    std::cout << "\n--- Test 4: split_string ---\n";
    print_vector("split_string(\"a,b,c,d\", ',')", split_string("a,b,c,d", ','));
    // Expected: ["a", "b", "c", "d"]

    // Test 5: words_longer_than
    std::cout << "\n--- Test 5: words_longer_than ---\n";
    print_vector("words_longer_than(\"The quick brown fox\", 3)",
                 words_longer_than("The quick brown fox", 3));
    // Expected: ["quick", "brown"]

    // Test 6-8: Product operations
    std::cout << "\n--- Test 6-8: Product operations ---\n";
    std::vector<Product> products{
        {"Widget", 25.0, 10},
        {"Gadget", 50.0, 5},
        {"Gizmo", 15.0, 20},
        {"Doohickey", 100.0, 2}
    };

    print_vector("expensive_product_names(>30)", expensive_product_names(products, 30.0));
    // Expected: ["Gadget", "Doohickey"]

    std::cout << "total_inventory_value: " << total_inventory_value(products) << "\n";
    // Expected: 25*10 + 50*5 + 15*20 + 100*2 = 250 + 250 + 300 + 200 = 1000

    print_vector("top_n_by_price(2)", top_n_by_price(products, 2));
    // Expected: ["Doohickey", "Gadget"]

    // Test 9: flatten_unique_sorted
    std::cout << "\n--- Test 9: flatten_unique_sorted ---\n";
    std::vector<std::vector<int>> nested{{3, 1, 2}, {2, 4}, {1, 5, 3}};
    print_vector("flatten_unique_sorted", flatten_unique_sorted(nested));
    // Expected: [1, 2, 3, 4, 5]

    // Test 10: fizzbuzz
    std::cout << "\n--- Test 10: fizzbuzz ---\n";
    print_vector("fizzbuzz(15)", fizzbuzz(15));
    // Expected: ["1", "2", "Fizz", "4", "Buzz", "Fizz", "7", "8", "Fizz", "Buzz",
    //            "11", "Fizz", "13", "14", "FizzBuzz"]

    // Test 11: longest_increasing_streak
    std::cout << "\n--- Test 11: longest_increasing_streak ---\n";
    std::vector<int> streak_test{1, 2, 5, 6, 7, 3, 4};
    std::cout << "longest_increasing_streak({1,2,5,6,7,3,4}): "
              << longest_increasing_streak(streak_test) << "\n";
    // Expected: 3

    // Test 12: sliding_window_max
    std::cout << "\n--- Test 12: sliding_window_max ---\n";
    std::vector<int> window_test{1, 3, 1, 5, 2};
    print_vector("sliding_window_max({1,3,1,5,2}, 3)", sliding_window_max(window_test, 3));
    // Expected: [3, 5, 5]

    return 0;
}
