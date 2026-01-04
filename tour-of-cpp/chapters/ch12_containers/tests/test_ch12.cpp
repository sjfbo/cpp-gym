// Chapter 12 Tests: Containers
// These tests verify understanding of C++ Standard Library containers.

#include <algorithm>
#include <catch2/catch_test_macros.hpp>
#include <deque>
#include <list>
#include <map>
#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

// ============================================================================
// Section 12.2: std::vector
// ============================================================================

TEST_CASE("vector: construction and initialization", "[ch12][vector]") {
    std::vector<int> v1;
    REQUIRE(v1.empty());
    REQUIRE(v1.size() == 0);

    std::vector<int> v2(5);
    REQUIRE(v2.size() == 5);
    REQUIRE(v2[0] == 0);  // Default initialized

    std::vector<int> v3(5, 42);
    REQUIRE(v3.size() == 5);
    REQUIRE(v3[0] == 42);
    REQUIRE(v3[4] == 42);

    std::vector<int> v4{1, 2, 3, 4, 5};
    REQUIRE(v4.size() == 5);
    REQUIRE(v4.front() == 1);
    REQUIRE(v4.back() == 5);
}

TEST_CASE("vector: capacity management", "[ch12][vector]") {
    std::vector<int> v;

    v.reserve(100);
    REQUIRE(v.capacity() >= 100);
    REQUIRE(v.size() == 0);  // size unchanged

    for (int i = 0; i < 50; ++i) {
        v.push_back(i);
    }
    REQUIRE(v.size() == 50);
    REQUIRE(v.capacity() >= 100);  // No reallocation
}

TEST_CASE("vector: element access", "[ch12][vector]") {
    std::vector<int> v{10, 20, 30, 40, 50};

    REQUIRE(v[0] == 10);
    REQUIRE(v.at(2) == 30);
    REQUIRE(v.front() == 10);
    REQUIRE(v.back() == 50);

    REQUIRE_THROWS_AS(v.at(100), std::out_of_range);
}

TEST_CASE("vector: modifying operations", "[ch12][vector]") {
    std::vector<int> v{1, 2, 3};

    v.push_back(4);
    REQUIRE(v.back() == 4);

    v.pop_back();
    REQUIRE(v.back() == 3);

    v.insert(v.begin(), 0);
    REQUIRE(v.front() == 0);

    v.erase(v.begin());
    REQUIRE(v.front() == 1);
}

TEST_CASE("vector: C++20 erase functions", "[ch12][vector]") {
    std::vector<int> v{1, 2, 3, 2, 4, 2, 5};

    auto count = std::erase(v, 2);
    REQUIRE(count == 3);
    REQUIRE(v.size() == 4);
    REQUIRE(std::find(v.begin(), v.end(), 2) == v.end());

    std::vector<int> v2{1, 2, 3, 4, 5, 6};
    std::erase_if(v2, [](int x) { return x % 2 == 0; });
    REQUIRE(v2 == std::vector<int>{1, 3, 5});
}

// ============================================================================
// Section 12.3: std::list
// ============================================================================

TEST_CASE("list: construction and basic operations", "[ch12][list]") {
    std::list<int> l{1, 2, 3, 4, 5};

    REQUIRE(l.size() == 5);
    REQUIRE(l.front() == 1);
    REQUIRE(l.back() == 5);

    l.push_front(0);
    REQUIRE(l.front() == 0);

    l.push_back(6);
    REQUIRE(l.back() == 6);
}

TEST_CASE("list: splice operation", "[ch12][list]") {
    std::list<int> l1{1, 2, 3};
    std::list<int> l2{10, 20, 30};

    auto it = l1.begin();
    std::advance(it, 1);  // Points to 2
    l1.splice(it, l2);

    REQUIRE(l2.empty());
    REQUIRE(l1.size() == 6);
    // l1 is now: 1, 10, 20, 30, 2, 3
    REQUIRE(l1.front() == 1);
}

TEST_CASE("list: merge sorted lists", "[ch12][list]") {
    std::list<int> l1{1, 3, 5};
    std::list<int> l2{2, 4, 6};

    l1.merge(l2);

    REQUIRE(l2.empty());
    REQUIRE(l1.size() == 6);

    std::vector<int> expected{1, 2, 3, 4, 5, 6};
    std::vector<int> actual(l1.begin(), l1.end());
    REQUIRE(actual == expected);
}

TEST_CASE("list: remove and unique", "[ch12][list]") {
    std::list<int> l{1, 2, 2, 3, 3, 3, 4};

    l.remove(2);
    REQUIRE(l.size() == 5);

    l.unique();  // Removes consecutive duplicates
    REQUIRE(l.size() == 3);  // 1, 3, 4
}

// ============================================================================
// Section 12.5: std::map
// ============================================================================

TEST_CASE("map: construction and insertion", "[ch12][map]") {
    std::map<std::string, int> m;

    m["one"] = 1;
    m["two"] = 2;
    m.emplace("three", 3);
    m.insert({"four", 4});

    REQUIRE(m.size() == 4);
    REQUIRE(m["one"] == 1);
    REQUIRE(m.at("three") == 3);
}

TEST_CASE("map: ordered iteration", "[ch12][map]") {
    std::map<std::string, int> m{
        {"cherry", 3}, {"apple", 1}, {"banana", 2}
    };

    std::vector<std::string> keys;
    for (const auto& [k, v] : m) {
        keys.push_back(k);
        (void)v;
    }

    // Map maintains sorted order by key
    REQUIRE(keys == std::vector<std::string>{"apple", "banana", "cherry"});
}

TEST_CASE("map: structured bindings", "[ch12][map]") {
    std::map<std::string, int> m{{"a", 1}, {"b", 2}, {"c", 3}};

    int sum = 0;
    for (const auto& [key, value] : m) {
        sum += value;
        (void)key;
    }
    REQUIRE(sum == 6);

    // Modifying through structured bindings
    for (auto& [key, value] : m) {
        value *= 10;
        (void)key;
    }
    REQUIRE(m["a"] == 10);
    REQUIRE(m["b"] == 20);
}

TEST_CASE("map: contains and find (C++20)", "[ch12][map]") {
    std::map<std::string, int> m{{"a", 1}, {"b", 2}};

    REQUIRE(m.contains("a"));
    REQUIRE_FALSE(m.contains("z"));

    auto it = m.find("a");
    REQUIRE(it != m.end());
    REQUIRE(it->second == 1);

    it = m.find("z");
    REQUIRE(it == m.end());
}

TEST_CASE("map: insert_or_assign and try_emplace", "[ch12][map]") {
    std::map<std::string, int> m;

    // insert_or_assign: always sets the value
    m.insert_or_assign("key", 1);
    REQUIRE(m["key"] == 1);
    m.insert_or_assign("key", 2);
    REQUIRE(m["key"] == 2);

    // try_emplace: only inserts if key doesn't exist
    auto [it1, inserted1] = m.try_emplace("key", 100);
    REQUIRE_FALSE(inserted1);
    REQUIRE(it1->second == 2);  // Unchanged

    auto [it2, inserted2] = m.try_emplace("new_key", 100);
    REQUIRE(inserted2);
    REQUIRE(it2->second == 100);
}

// ============================================================================
// Section 12.5: std::multimap
// ============================================================================

TEST_CASE("multimap: multiple values per key", "[ch12][multimap]") {
    std::multimap<std::string, int> mm;

    mm.emplace("a", 1);
    mm.emplace("a", 2);
    mm.emplace("a", 3);
    mm.emplace("b", 10);

    REQUIRE(mm.size() == 4);
    REQUIRE(mm.count("a") == 3);
    REQUIRE(mm.count("b") == 1);
}

TEST_CASE("multimap: equal_range", "[ch12][multimap]") {
    std::multimap<std::string, int> mm{
        {"a", 1}, {"a", 2}, {"b", 10}, {"a", 3}
    };

    auto [begin, end] = mm.equal_range("a");

    std::vector<int> values;
    for (auto it = begin; it != end; ++it) {
        values.push_back(it->second);
    }

    REQUIRE(values.size() == 3);
    // Values are in insertion order within the same key
}

// ============================================================================
// Section 12.6: std::unordered_map
// ============================================================================

TEST_CASE("unordered_map: basic operations", "[ch12][unordered_map]") {
    std::unordered_map<std::string, int> um;

    um["one"] = 1;
    um["two"] = 2;
    um.emplace("three", 3);

    REQUIRE(um.size() == 3);
    REQUIRE(um["one"] == 1);
    REQUIRE(um.contains("two"));
    REQUIRE_FALSE(um.contains("four"));
}

TEST_CASE("unordered_map: hash table properties", "[ch12][unordered_map]") {
    std::unordered_map<int, int> um;

    um.reserve(100);
    REQUIRE(um.bucket_count() >= 100);

    for (int i = 0; i < 50; ++i) {
        um[i] = i * 2;
    }

    REQUIRE(um.load_factor() <= um.max_load_factor());
}

TEST_CASE("unordered_map: custom hash", "[ch12][unordered_map]") {
    struct Point {
        int x, y;
        bool operator==(const Point& other) const {
            return x == other.x && y == other.y;
        }
    };

    struct PointHash {
        std::size_t operator()(const Point& p) const {
            return std::hash<int>{}(p.x) ^ (std::hash<int>{}(p.y) << 1);
        }
    };

    std::unordered_map<Point, std::string, PointHash> pm;
    pm[{0, 0}] = "origin";
    pm[{1, 1}] = "diagonal";

    REQUIRE(pm.size() == 2);
    REQUIRE(pm[{0, 0}] == "origin");
    REQUIRE(pm.contains({1, 1}));
}

// ============================================================================
// std::set and std::unordered_set
// ============================================================================

TEST_CASE("set: unique sorted elements", "[ch12][set]") {
    std::set<int> s{5, 2, 8, 2, 1, 9, 5};

    REQUIRE(s.size() == 5);  // Duplicates removed

    std::vector<int> v(s.begin(), s.end());
    REQUIRE(v == std::vector<int>{1, 2, 5, 8, 9});  // Sorted
}

TEST_CASE("unordered_set: unique elements O(1) lookup", "[ch12][unordered_set]") {
    std::unordered_set<std::string> us{"apple", "banana", "cherry"};

    REQUIRE(us.contains("apple"));
    REQUIRE_FALSE(us.contains("date"));

    us.insert("date");
    REQUIRE(us.contains("date"));
    REQUIRE(us.size() == 4);
}

// ============================================================================
// Container comparison
// ============================================================================

TEST_CASE("container: comparison operations", "[ch12][container]") {
    std::vector<int> v1{1, 2, 3};
    std::vector<int> v2{1, 2, 3};
    std::vector<int> v3{1, 2, 4};

    REQUIRE(v1 == v2);
    REQUIRE(v1 != v3);
    REQUIRE(v1 < v3);  // Lexicographic comparison

    std::map<std::string, int> m1{{"a", 1}};
    std::map<std::string, int> m2{{"a", 1}};
    REQUIRE(m1 == m2);
}

// ============================================================================
// Iterator categories
// ============================================================================

TEST_CASE("iterators: random access (vector)", "[ch12][iterator]") {
    std::vector<int> v{10, 20, 30, 40, 50};

    auto it = v.begin();
    REQUIRE(*it == 10);

    it += 2;
    REQUIRE(*it == 30);

    it -= 1;
    REQUIRE(*it == 20);

    REQUIRE(it[2] == 40);  // Random access
    REQUIRE(v.end() - v.begin() == 5);
}

TEST_CASE("iterators: bidirectional (list)", "[ch12][iterator]") {
    std::list<int> l{10, 20, 30};

    auto it = l.begin();
    REQUIRE(*it == 10);

    ++it;
    REQUIRE(*it == 20);

    --it;
    REQUIRE(*it == 10);

    // Note: it += 2 would NOT compile for list (not random access)
}

TEST_CASE("iterators: const iterators", "[ch12][iterator]") {
    const std::vector<int> v{1, 2, 3};

    // Only cbegin/cend available on const container
    auto it = v.cbegin();
    REQUIRE(*it == 1);
    // *it = 10;  // Would not compile: const iterator

    // Non-const container can also use cbegin/cend
    std::vector<int> v2{1, 2, 3};
    auto cit = v2.cbegin();
    REQUIRE(*cit == 1);
}
