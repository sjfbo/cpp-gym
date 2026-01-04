// std::map and std::multimap - Ordered Associative Containers
// Book reference: 12.5 map
//
// std::map stores unique key-value pairs in sorted order (by key).
// std::multimap allows duplicate keys.
// Both are typically implemented as red-black trees: O(log n) operations.

#include <algorithm>
#include <iostream>
#include <map>
#include <string>

// Helper to print map contents
template <typename K, typename V>
void print_map(const std::string& label, const std::map<K, V>& m) {
    std::cout << label << ": {";
    bool first = true;
    for (const auto& [key, value] : m) {
        if (!first) std::cout << ", ";
        std::cout << key << ": " << value;
        first = false;
    }
    std::cout << "} (size=" << m.size() << ")\n";
}

template <typename K, typename V>
void print_multimap(const std::string& label, const std::multimap<K, V>& mm) {
    std::cout << label << ": {";
    bool first = true;
    for (const auto& [key, value] : mm) {
        if (!first) std::cout << ", ";
        std::cout << key << ": " << value;
        first = false;
    }
    std::cout << "} (size=" << mm.size() << ")\n";
}

int main() {
    std::cout << "=== std::map Operations ===\n\n";

    // -------------------------------------------------------------------------
    // Construction and initialization
    // -------------------------------------------------------------------------
    std::cout << "--- Construction ---\n";

    std::map<std::string, int> m1;  // Empty map
    std::map<std::string, int> m2{  // Initializer list
        {"apple", 5},
        {"banana", 3},
        {"cherry", 8}
    };

    print_map("m1 (empty)", m1);
    print_map("m2 (init list)", m2);

    // -------------------------------------------------------------------------
    // Insertion
    // -------------------------------------------------------------------------
    std::cout << "\n--- Insertion ---\n";

    std::map<std::string, int> inventory;

    // Using operator[] - creates entry if not exists
    inventory["apples"] = 50;
    inventory["bananas"] = 30;
    print_map("After operator[]", inventory);

    // Using insert() with pair
    inventory.insert({"cherries", 25});
    inventory.insert(std::make_pair("dates", 15));
    print_map("After insert()", inventory);

    // Using emplace() - constructs in-place
    inventory.emplace("elderberries", 10);
    print_map("After emplace()", inventory);

    // insert() doesn't overwrite existing keys
    auto [iter, inserted] = inventory.insert({"apples", 999});
    std::cout << "Tried to insert apples=999: "
              << (inserted ? "inserted" : "already exists")
              << ", current value=" << iter->second << "\n";

    // insert_or_assign() (C++17) - inserts or updates
    inventory.insert_or_assign("apples", 100);
    print_map("After insert_or_assign(apples, 100)", inventory);

    // try_emplace() (C++17) - emplace only if key doesn't exist
    inventory.try_emplace("figs", 20);     // Inserted
    inventory.try_emplace("apples", 999);  // Not inserted (key exists)
    print_map("After try_emplace()", inventory);

    // -------------------------------------------------------------------------
    // Access
    // -------------------------------------------------------------------------
    std::cout << "\n--- Access ---\n";

    std::map<std::string, int> scores{
        {"Alice", 95},
        {"Bob", 87},
        {"Charlie", 92}
    };

    // operator[] - WARNING: creates entry if not found!
    std::cout << "Alice's score: " << scores["Alice"] << "\n";
    std::cout << "After accessing 'Unknown': " << scores["Unknown"]
              << " (created with default value!)\n";
    print_map("Map now has 'Unknown'", scores);

    // at() - throws if not found
    try {
        std::cout << "Bob's score via at(): " << scores.at("Bob") << "\n";
        [[maybe_unused]] auto val = scores.at("NonExistent");
    } catch (const std::out_of_range& e) {
        std::cout << "Caught out_of_range: " << e.what() << "\n";
    }

    // -------------------------------------------------------------------------
    // Lookup
    // -------------------------------------------------------------------------
    std::cout << "\n--- Lookup ---\n";

    std::map<std::string, int> lookup_demo{
        {"one", 1}, {"two", 2}, {"three", 3}
    };

    // contains() - C++20
    std::cout << "contains('two'): " << (lookup_demo.contains("two") ? "yes" : "no") << "\n";
    std::cout << "contains('four'): " << (lookup_demo.contains("four") ? "yes" : "no") << "\n";

    // find() - returns iterator
    auto it = lookup_demo.find("two");
    if (it != lookup_demo.end()) {
        std::cout << "Found 'two': " << it->first << " -> " << it->second << "\n";
    }

    it = lookup_demo.find("four");
    if (it == lookup_demo.end()) {
        std::cout << "'four' not found\n";
    }

    // count() - returns 0 or 1 for map (useful for multimap)
    std::cout << "count('one'): " << lookup_demo.count("one") << "\n";
    std::cout << "count('five'): " << lookup_demo.count("five") << "\n";

    // -------------------------------------------------------------------------
    // Structured Bindings (Modern C++ iteration)
    // -------------------------------------------------------------------------
    std::cout << "\n--- Structured Bindings (C++17/20) ---\n";

    std::map<std::string, double> prices{
        {"coffee", 4.50},
        {"tea", 3.00},
        {"juice", 5.25}
    };

    // Old style (pre-C++17)
    std::cout << "Old style (first/second):\n";
    for (const auto& pair : prices) {
        std::cout << "  " << pair.first << ": $" << pair.second << "\n";
    }

    // Modern style with structured bindings
    std::cout << "Modern style (structured bindings):\n";
    for (const auto& [item, price] : prices) {
        std::cout << "  " << item << ": $" << price << "\n";
    }

    // Modifying values through structured bindings
    for (auto& [item, price] : prices) {
        price *= 1.1;  // 10% price increase
    }
    std::cout << "After 10% increase:\n";
    for (const auto& [item, price] : prices) {
        std::cout << "  " << item << ": $" << price << "\n";
    }

    // -------------------------------------------------------------------------
    // Removal
    // -------------------------------------------------------------------------
    std::cout << "\n--- Removal ---\n";

    std::map<std::string, int> remove_demo{
        {"a", 1}, {"b", 2}, {"c", 3}, {"d", 4}, {"e", 5}
    };
    print_map("Initial", remove_demo);

    // Erase by key
    auto erased_count = remove_demo.erase("c");
    std::cout << "Erased 'c': " << erased_count << " element(s)\n";
    print_map("After erase('c')", remove_demo);

    // Erase by iterator
    remove_demo.erase(remove_demo.begin());
    print_map("After erase(begin())", remove_demo);

    // Erase with condition (C++20)
    std::map<std::string, int> erase_if_demo{
        {"a", 1}, {"b", 2}, {"c", 3}, {"d", 4}
    };
    std::erase_if(erase_if_demo, [](const auto& pair) {
        return pair.second % 2 == 0;  // Remove even values
    });
    print_map("After erase_if (even values)", erase_if_demo);

    // -------------------------------------------------------------------------
    // Custom comparator
    // -------------------------------------------------------------------------
    std::cout << "\n--- Custom Comparator ---\n";

    // Case-insensitive string comparison
    auto case_insensitive = [](const std::string& a, const std::string& b) {
        return std::lexicographical_compare(
            a.begin(), a.end(),
            b.begin(), b.end(),
            [](char ca, char cb) {
                return std::tolower(ca) < std::tolower(cb);
            }
        );
    };

    std::map<std::string, int, decltype(case_insensitive)> ci_map(case_insensitive);
    ci_map["Apple"] = 1;
    ci_map["apple"] = 2;  // Same key (case-insensitive), overwrites
    ci_map["BANANA"] = 3;

    std::cout << "Case-insensitive map:\n";
    for (const auto& [k, v] : ci_map) {
        std::cout << "  " << k << ": " << v << "\n";
    }

    // -------------------------------------------------------------------------
    // std::multimap - Multiple values per key
    // -------------------------------------------------------------------------
    std::cout << "\n=== std::multimap ===\n";

    std::multimap<std::string, std::string> phonebook;

    phonebook.emplace("John", "555-1234");
    phonebook.emplace("John", "555-5678");  // Same key, different value - OK!
    phonebook.emplace("Jane", "555-9999");
    phonebook.emplace("John", "555-0000");

    print_multimap("Phonebook", phonebook);

    // count() returns actual count for multimap
    std::cout << "count('John'): " << phonebook.count("John") << "\n";

    // equal_range() - get all entries for a key
    auto [range_begin, range_end] = phonebook.equal_range("John");
    std::cout << "All numbers for John:\n";
    for (auto range_it = range_begin; range_it != range_end; ++range_it) {
        std::cout << "  " << range_it->second << "\n";
    }

    // lower_bound and upper_bound
    auto lower = phonebook.lower_bound("John");  // First "John"
    auto upper = phonebook.upper_bound("John");  // Past last "John"
    std::cout << "Range for 'John': " << std::distance(lower, upper) << " entries\n";

    return 0;
}

// Key takeaways:
// - std::map stores unique keys in sorted order
// - operator[] creates entries - use at() or find() to avoid this
// - contains() (C++20) is the cleanest way to check existence
// - Use structured bindings for clean iteration: for (auto& [k, v] : m)
// - std::multimap allows duplicate keys; use equal_range() to get all values
// - Custom comparators enable case-insensitive maps, reverse order, etc.
// - insert_or_assign() and try_emplace() (C++17) provide more control
