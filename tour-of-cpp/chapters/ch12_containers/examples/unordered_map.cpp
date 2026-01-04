// std::unordered_map - Hash-Based Associative Container
// Book reference: 12.6 unordered_map
//
// std::unordered_map uses hash tables for O(1) average-case lookup.
// Keys are not stored in any particular order.
// Custom types require a hash function.

#include <functional>
#include <iostream>
#include <string>
#include <unordered_map>

// Helper to print unordered_map
template <typename K, typename V>
void print_umap(const std::string& label, const std::unordered_map<K, V>& m) {
    std::cout << label << ": {";
    bool first = true;
    for (const auto& [key, value] : m) {
        if (!first) std::cout << ", ";
        std::cout << key << ": " << value;
        first = false;
    }
    std::cout << "} (size=" << m.size() << ")\n";
}

// -------------------------------------------------------------------------
// Custom hash function examples
// -------------------------------------------------------------------------

// Example 1: Simple struct with custom hash
struct Point {
    int x;
    int y;

    bool operator==(const Point& other) const {
        return x == other.x && y == other.y;
    }
};

// Functor-based hash
struct PointHash {
    std::size_t operator()(const Point& p) const {
        // Combine hashes using a common technique
        std::size_t h1 = std::hash<int>{}(p.x);
        std::size_t h2 = std::hash<int>{}(p.y);
        return h1 ^ (h2 << 1);  // Simple combination
    }
};

// Example 2: Better hash combination (boost-style)
struct PointHashBetter {
    std::size_t operator()(const Point& p) const {
        std::size_t seed = 0;
        // Hash combine pattern
        seed ^= std::hash<int>{}(p.x) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        seed ^= std::hash<int>{}(p.y) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        return seed;
    }
};

// Example 3: Specializing std::hash (alternative approach)
struct Employee {
    int id;
    std::string name;

    bool operator==(const Employee& other) const {
        return id == other.id && name == other.name;
    }
};

// Specialization of std::hash for Employee
template <>
struct std::hash<Employee> {
    std::size_t operator()(const Employee& e) const {
        std::size_t h1 = std::hash<int>{}(e.id);
        std::size_t h2 = std::hash<std::string>{}(e.name);
        return h1 ^ (h2 << 1);
    }
};

int main() {
    std::cout << "=== std::unordered_map Operations ===\n\n";

    // -------------------------------------------------------------------------
    // Basic operations (similar to std::map)
    // -------------------------------------------------------------------------
    std::cout << "--- Basic Operations ---\n";

    std::unordered_map<std::string, int> word_count;

    // Insertion
    word_count["hello"] = 1;
    word_count["world"] = 2;
    word_count.emplace("foo", 3);
    word_count.insert({"bar", 4});

    print_umap("word_count", word_count);

    // Access
    std::cout << "word_count['hello'] = " << word_count["hello"] << "\n";
    std::cout << "word_count.at('world') = " << word_count.at("world") << "\n";

    // Lookup
    std::cout << "contains('foo'): " << (word_count.contains("foo") ? "yes" : "no") << "\n";
    std::cout << "contains('baz'): " << (word_count.contains("baz") ? "yes" : "no") << "\n";

    // -------------------------------------------------------------------------
    // Order is NOT guaranteed
    // -------------------------------------------------------------------------
    std::cout << "\n--- Iteration Order ---\n";

    std::unordered_map<int, std::string> nums{
        {1, "one"}, {2, "two"}, {3, "three"},
        {4, "four"}, {5, "five"}
    };

    std::cout << "Elements (order may vary on different runs):\n";
    for (const auto& [k, v] : nums) {
        std::cout << "  " << k << " -> " << v << "\n";
    }

    // -------------------------------------------------------------------------
    // Hash table internals
    // -------------------------------------------------------------------------
    std::cout << "\n--- Hash Table Internals ---\n";

    std::unordered_map<std::string, int> demo;

    std::cout << "Empty map:\n";
    std::cout << "  bucket_count: " << demo.bucket_count() << "\n";
    std::cout << "  load_factor: " << demo.load_factor() << "\n";
    std::cout << "  max_load_factor: " << demo.max_load_factor() << "\n";

    // Add elements
    for (int i = 0; i < 100; ++i) {
        demo[std::to_string(i)] = i;
    }

    std::cout << "\nAfter adding 100 elements:\n";
    std::cout << "  size: " << demo.size() << "\n";
    std::cout << "  bucket_count: " << demo.bucket_count() << "\n";
    std::cout << "  load_factor: " << demo.load_factor() << "\n";

    // Reserve buckets for expected elements
    std::unordered_map<std::string, int> reserved;
    reserved.reserve(1000);  // Prepare for ~1000 elements
    std::cout << "\nAfter reserve(1000):\n";
    std::cout << "  bucket_count: " << reserved.bucket_count() << "\n";

    // -------------------------------------------------------------------------
    // Bucket inspection
    // -------------------------------------------------------------------------
    std::cout << "\n--- Bucket Inspection ---\n";

    std::unordered_map<std::string, int> bucket_demo{
        {"apple", 1}, {"banana", 2}, {"cherry", 3},
        {"date", 4}, {"elderberry", 5}
    };

    std::cout << "Bucket distribution:\n";
    for (std::size_t i = 0; i < bucket_demo.bucket_count(); ++i) {
        if (bucket_demo.bucket_size(i) > 0) {
            std::cout << "  Bucket " << i << " has "
                      << bucket_demo.bucket_size(i) << " element(s): ";
            for (auto it = bucket_demo.begin(i); it != bucket_demo.end(i); ++it) {
                std::cout << it->first << " ";
            }
            std::cout << "\n";
        }
    }

    // Which bucket is a key in?
    std::cout << "\n'apple' is in bucket: " << bucket_demo.bucket("apple") << "\n";
    std::cout << "'banana' is in bucket: " << bucket_demo.bucket("banana") << "\n";

    // -------------------------------------------------------------------------
    // Custom hash function for user-defined types
    // -------------------------------------------------------------------------
    std::cout << "\n--- Custom Hash Functions ---\n";

    // Using PointHash functor
    std::unordered_map<Point, std::string, PointHash> point_map;
    point_map[{0, 0}] = "origin";
    point_map[{1, 0}] = "unit x";
    point_map[{0, 1}] = "unit y";
    point_map[{1, 1}] = "diagonal";

    std::cout << "Point map:\n";
    for (const auto& [p, name] : point_map) {
        std::cout << "  (" << p.x << ", " << p.y << ") -> " << name << "\n";
    }

    // Lookup
    Point target{1, 1};
    if (point_map.contains(target)) {
        std::cout << "Found (1,1): " << point_map[target] << "\n";
    }

    // Using std::hash specialization (no need to specify hash template arg)
    std::unordered_map<Employee, double> salaries;
    salaries[{1, "Alice"}] = 75000.0;
    salaries[{2, "Bob"}] = 80000.0;

    std::cout << "\nEmployee salaries:\n";
    for (const auto& [emp, salary] : salaries) {
        std::cout << "  " << emp.name << " (ID: " << emp.id << "): $" << salary << "\n";
    }

    // -------------------------------------------------------------------------
    // Lambda-based hash (C++20 style)
    // -------------------------------------------------------------------------
    std::cout << "\n--- Lambda Hash (Modern C++) ---\n";

    auto pair_hash = [](const std::pair<int, int>& p) {
        return std::hash<int>{}(p.first) ^ (std::hash<int>{}(p.second) << 1);
    };

    std::unordered_map<std::pair<int, int>, std::string,
                       decltype(pair_hash)> coord_names(10, pair_hash);

    coord_names[{0, 0}] = "origin";
    coord_names[{1, 2}] = "point A";
    coord_names[{3, 4}] = "point B";

    std::cout << "Coordinate names:\n";
    for (const auto& [coord, name] : coord_names) {
        std::cout << "  (" << coord.first << ", " << coord.second << ") = " << name << "\n";
    }

    // -------------------------------------------------------------------------
    // Performance comparison hint
    // -------------------------------------------------------------------------
    std::cout << "\n--- When to Use unordered_map vs map ---\n";
    std::cout << "Use std::unordered_map when:\n";
    std::cout << "  - O(1) average lookup is important\n";
    std::cout << "  - Order doesn't matter\n";
    std::cout << "  - Good hash function available\n";
    std::cout << "  - Large datasets\n\n";

    std::cout << "Use std::map when:\n";
    std::cout << "  - Ordered iteration is needed\n";
    std::cout << "  - Range queries (lower_bound, upper_bound)\n";
    std::cout << "  - Worst-case O(log n) guarantee needed\n";
    std::cout << "  - No good hash function for key type\n";

    // -------------------------------------------------------------------------
    // Heterogeneous lookup (C++20)
    // -------------------------------------------------------------------------
    std::cout << "\n--- Heterogeneous Lookup (C++20) ---\n";

    // With transparent hash and equal, can lookup with string_view
    // without creating a temporary std::string
    struct StringHash {
        using is_transparent = void;  // Enable heterogeneous lookup

        std::size_t operator()(std::string_view sv) const {
            return std::hash<std::string_view>{}(sv);
        }
        std::size_t operator()(const std::string& s) const {
            return std::hash<std::string>{}(s);
        }
    };

    struct StringEqual {
        using is_transparent = void;

        bool operator()(std::string_view a, std::string_view b) const {
            return a == b;
        }
    };

    std::unordered_map<std::string, int, StringHash, StringEqual> het_map;
    het_map["hello"] = 1;
    het_map["world"] = 2;

    // Can lookup with string_view (no temporary string created)
    std::string_view sv = "hello";
    if (auto it = het_map.find(sv); it != het_map.end()) {
        std::cout << "Found via string_view: " << it->second << "\n";
    }

    return 0;
}

// Key takeaways:
// - unordered_map has O(1) average lookup vs map's O(log n)
// - Elements are not stored in any particular order
// - Custom types need hash function + equality operator
// - reserve() pre-allocates buckets for better performance
// - Use transparent comparators (C++20) for heterogeneous lookup
// - Watch load_factor for performance tuning
