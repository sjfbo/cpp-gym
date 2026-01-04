// Exercise 12.2: Map Operations and Structured Bindings
// Book reference: 12.5 map, 12.6 unordered_map
// Difficulty: **
//
// Instructions:
// Practice map operations including insertion, lookup, iteration
// with structured bindings, and custom comparators.

#include <iostream>
#include <map>
#include <string>
#include <unordered_map>
#include <vector>

// ============================================================================
// Part 1: Basic Map Operations
// ============================================================================

// TODO 1: Implement a function that takes a vector of pairs and creates a map.
// If a key appears multiple times, keep only the LAST value.
//
// Example: {{"a", 1}, {"b", 2}, {"a", 3}}
// Result: {{"a", 3}, {"b", 2}}

std::map<std::string, int> create_map(
    const std::vector<std::pair<std::string, int>>& pairs) {
    // TODO: Implement
    (void)pairs;
    return {};
}


// TODO 2: Implement a function that merges two maps.
// If a key exists in both, sum the values.
//
// Example: {{"a", 1}, {"b", 2}} + {{"b", 3}, {"c", 4}}
// Result: {{"a", 1}, {"b", 5}, {"c", 4}}

std::map<std::string, int> merge_maps(
    const std::map<std::string, int>& m1,
    const std::map<std::string, int>& m2) {
    // TODO: Implement using structured bindings
    (void)m1;
    (void)m2;
    return {};
}


// TODO 3: Implement a function that inverts a map (swap keys and values).
// Assumes values are unique.
//
// Example: {{"a", 1}, {"b", 2}, {"c", 3}}
// Result: {{1, "a"}, {2, "b"}, {3, "c"}}

std::map<int, std::string> invert_map(const std::map<std::string, int>& m) {
    // TODO: Implement using structured bindings
    (void)m;
    return {};
}


// ============================================================================
// Part 2: Working with Structured Bindings
// ============================================================================

// TODO 4: Implement a function that finds all keys where value exceeds threshold.
// Use structured bindings for clean iteration.
//
// Example: {{"a", 10}, {"b", 5}, {"c", 15}}, threshold=7
// Result: {"a", "c"}

std::vector<std::string> find_keys_above(
    const std::map<std::string, int>& m,
    int threshold) {
    // TODO: Implement using structured bindings
    (void)m;
    (void)threshold;
    return {};
}


// TODO 5: Implement a function that modifies all values in a map by a multiplier.
// Use structured bindings to iterate and modify.
//
// Example: {{"a", 2}, {"b", 3}}, multiplier=10
// Result: {{"a", 20}, {"b", 30}}

void multiply_values(std::map<std::string, int>& m, int multiplier) {
    // TODO: Implement using structured bindings
    // Hint: Use auto& [key, value] to get modifiable references
    (void)m;
    (void)multiplier;
}


// ============================================================================
// Part 3: Custom Comparators
// ============================================================================

// TODO 6: Create a map that stores strings in reverse alphabetical order.
// Use a custom comparator.
//
// After inserting: "apple", "banana", "cherry"
// Iteration should give: "cherry", "banana", "apple"

// Define the custom comparator type
// Hint: std::greater<std::string> or a lambda

auto create_reverse_map() {
    // TODO: Create and return a map with reverse ordering
    // You can use std::greater<std::string> as the comparator
    std::map<std::string, int> placeholder;  // Replace with correct type
    return placeholder;
}


// TODO 7: Create a case-insensitive string map.
// "Apple" and "apple" should be treated as the same key.

struct CaseInsensitiveCompare {
    bool operator()(const std::string& a, const std::string& b) const {
        // TODO: Implement case-insensitive comparison
        // Hint: Compare character by character using std::tolower
        (void)a;
        (void)b;
        return false;
    }
};

void test_case_insensitive_map() {
    std::map<std::string, int, CaseInsensitiveCompare> ci_map;

    // TODO: Insert values and verify case-insensitivity
    // ci_map["Apple"] = 1;
    // ci_map["APPLE"] = 2;  // Should overwrite, not create new entry
    // Expected size: 1
    // Expected value for "apple": 2
}


// ============================================================================
// Part 4: Using contains() and try_emplace() (C++20/17)
// ============================================================================

// TODO 8: Implement a function that safely gets a value with a default.
// If key exists, return its value. Otherwise return the default.
// Do NOT modify the map (don't use operator[]).

int get_or_default(
    const std::map<std::string, int>& m,
    const std::string& key,
    int default_value) {
    // TODO: Implement using contains() or find()
    (void)m;
    (void)key;
    return default_value;
}


// TODO 9: Implement a function that increments a counter for a key.
// If key doesn't exist, initialize to 1.
// Use try_emplace() for efficiency.

void increment_counter(
    std::map<std::string, int>& counters,
    const std::string& key) {
    // TODO: Implement
    // Hint: try_emplace returns pair<iterator, bool>
    // If inserted, value is already 1 (or 0, then increment)
    // If not inserted, increment the existing value
    (void)counters;
    (void)key;
}


// ============================================================================
// Part 5: Group By (Advanced)
// ============================================================================

// TODO 10: Implement a group_by function.
// Group a vector of pairs by their first element, collecting all second elements.
//
// Example: {{"a", 1}, {"b", 2}, {"a", 3}, {"b", 4}, {"a", 5}}
// Result: {{"a", [1, 3, 5]}, {"b", [2, 4]}}

std::map<std::string, std::vector<int>> group_by(
    const std::vector<std::pair<std::string, int>>& pairs) {
    // TODO: Implement
    (void)pairs;
    return {};
}


// ============================================================================
// Test Functions
// ============================================================================

template <typename K, typename V>
void print_map(const std::string& label, const std::map<K, V>& m) {
    std::cout << label << ": {";
    bool first = true;
    for (const auto& [k, v] : m) {
        if (!first) std::cout << ", ";
        std::cout << k << ": " << v;
        first = false;
    }
    std::cout << "}\n";
}

int main() {
    std::cout << "=== Exercise 12.2: Map Operations ===\n\n";

    // Test 1: create_map
    std::cout << "--- Test 1: create_map ---\n";
    std::vector<std::pair<std::string, int>> pairs1{
        {"a", 1}, {"b", 2}, {"a", 3}
    };
    auto m1 = create_map(pairs1);
    print_map("Created map", m1);
    // Expected: {a: 3, b: 2}
    std::cout << "\n";

    // Test 2: merge_maps
    std::cout << "--- Test 2: merge_maps ---\n";
    std::map<std::string, int> map_a{{"a", 1}, {"b", 2}};
    std::map<std::string, int> map_b{{"b", 3}, {"c", 4}};
    auto merged = merge_maps(map_a, map_b);
    print_map("Merged map", merged);
    // Expected: {a: 1, b: 5, c: 4}
    std::cout << "\n";

    // Test 3: invert_map
    std::cout << "--- Test 3: invert_map ---\n";
    std::map<std::string, int> to_invert{{"a", 1}, {"b", 2}, {"c", 3}};
    auto inverted = invert_map(to_invert);
    print_map("Inverted map", inverted);
    // Expected: {1: a, 2: b, 3: c}
    std::cout << "\n";

    // Test 4: find_keys_above
    std::cout << "--- Test 4: find_keys_above ---\n";
    std::map<std::string, int> scores{{"alice", 85}, {"bob", 72}, {"charlie", 91}};
    auto high_scorers = find_keys_above(scores, 80);
    std::cout << "Keys above 80: ";
    for (const auto& k : high_scorers) std::cout << k << " ";
    std::cout << "\n";
    // Expected: alice charlie
    std::cout << "\n";

    // Test 5: multiply_values
    std::cout << "--- Test 5: multiply_values ---\n";
    std::map<std::string, int> values{{"x", 2}, {"y", 3}, {"z", 4}};
    print_map("Before multiply", values);
    multiply_values(values, 10);
    print_map("After multiply by 10", values);
    // Expected: {x: 20, y: 30, z: 40}
    std::cout << "\n";

    // Test 6: reverse_map
    std::cout << "--- Test 6: reverse_map ---\n";
    auto rev_map = create_reverse_map();
    // TODO: Uncomment when implemented
    // rev_map["apple"] = 1;
    // rev_map["banana"] = 2;
    // rev_map["cherry"] = 3;
    // for (const auto& [k, v] : rev_map) {
    //     std::cout << k << ": " << v << "\n";
    // }
    // Expected order: cherry, banana, apple
    std::cout << "TODO: Implement reverse map\n\n";

    // Test 7: case_insensitive_map
    std::cout << "--- Test 7: case_insensitive_map ---\n";
    test_case_insensitive_map();
    std::cout << "TODO: Implement case-insensitive map\n\n";

    // Test 8: get_or_default
    std::cout << "--- Test 8: get_or_default ---\n";
    std::map<std::string, int> defaults_test{{"a", 10}, {"b", 20}};
    std::cout << "get 'a' (exists): " << get_or_default(defaults_test, "a", -1) << "\n";
    std::cout << "get 'c' (missing): " << get_or_default(defaults_test, "c", -1) << "\n";
    // Expected: 10, -1
    std::cout << "\n";

    // Test 9: increment_counter
    std::cout << "--- Test 9: increment_counter ---\n";
    std::map<std::string, int> counters;
    increment_counter(counters, "visits");
    increment_counter(counters, "visits");
    increment_counter(counters, "clicks");
    increment_counter(counters, "visits");
    print_map("Counters", counters);
    // Expected: {clicks: 1, visits: 3}
    std::cout << "\n";

    // Test 10: group_by
    std::cout << "--- Test 10: group_by ---\n";
    std::vector<std::pair<std::string, int>> to_group{
        {"fruit", 1}, {"veg", 2}, {"fruit", 3}, {"veg", 4}, {"fruit", 5}
    };
    auto grouped = group_by(to_group);
    for (const auto& [key, values] : grouped) {
        std::cout << key << ": [";
        for (std::size_t i = 0; i < values.size(); ++i) {
            std::cout << values[i];
            if (i < values.size() - 1) std::cout << ", ";
        }
        std::cout << "]\n";
    }
    // Expected: fruit: [1, 3, 5], veg: [2, 4]

    return 0;
}
