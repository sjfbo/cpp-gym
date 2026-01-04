# Chapter 12: Containers

## TL;DR

The C++ Standard Library provides a rich set of container types for storing collections of elements. Choose `std::vector` for most use cases (contiguous memory, cache-friendly). Use `std::map`/`std::unordered_map` for key-value associations, `std::list` only when you need stable iterators and frequent middle insertions. Modern C++ encourages structured bindings for clean iteration over maps.

## Key Concepts

| Concept | One-liner | Example |
|---------|-----------|---------|
| `std::vector` | Dynamic contiguous array | `std::vector<int> v{1, 2, 3};` |
| `std::list` | Doubly-linked list | `std::list<int> l{1, 2, 3};` |
| `std::map` | Ordered key-value (red-black tree) | `std::map<std::string, int> m;` |
| `std::unordered_map` | Hash-based key-value | `std::unordered_map<std::string, int> um;` |
| Iterator | Generalized pointer to elements | `auto it = v.begin();` |
| Capacity vs Size | Reserved space vs actual elements | `v.capacity()` vs `v.size()` |
| Structured bindings | Unpack pairs/tuples | `for (auto& [k, v] : map) {}` |
| Custom hash | User-defined hash function | `std::unordered_map<Key, Val, Hash>` |

## Things to Remember

- **Default to `std::vector`**: It's cache-friendly and efficient for most operations.
- **Reserve when you know the size**: `v.reserve(n)` avoids repeated reallocations.
- **`map` vs `unordered_map`**: Use `map` when you need ordering; `unordered_map` for O(1) average lookup.
- **Iterator invalidation**: Know when operations invalidate iterators (e.g., `vector::push_back` can invalidate all iterators if reallocation occurs).
- **Use structured bindings**: `for (const auto& [key, value] : map)` is cleaner than `.first`/`.second`.
- **`emplace` vs `insert`**: Prefer `emplace_back` to construct in-place and avoid copies.
- **Custom hash**: For user-defined types in `unordered_map`, provide a hash function.

## Modern C++ Highlights

| Feature | Old Style | Modern C++20 |
|---------|-----------|--------------|
| Iteration | `for (auto it = m.begin(); it != m.end(); ++it)` | `for (const auto& [k, v] : m)` |
| Insertion | `m.insert(std::make_pair(k, v));` | `m.emplace(k, v);` or `m[k] = v;` |
| Contains check | `if (m.find(k) != m.end())` | `if (m.contains(k))` (C++20) |
| In-place construct | `v.push_back(MyClass(a, b));` | `v.emplace_back(a, b);` |
| Erase-remove idiom | `v.erase(std::remove(...), v.end());` | `std::erase(v, value);` (C++20) |

## Common Pitfalls

- **Iterator invalidation with vector**:
  ```cpp
  std::vector<int> v{1, 2, 3};
  auto it = v.begin();
  v.push_back(4);  // MAY invalidate 'it' if reallocation occurs!
  // *it is now undefined behavior
  ```

- **Using `[]` on map creates entries**:
  ```cpp
  std::map<std::string, int> m;
  int val = m["key"];  // Creates m["key"] = 0 if not exists!
  // Use m.at("key") or m.find("key") to avoid this
  ```

- **Forgetting to provide hash for custom types**:
  ```cpp
  struct Point { int x, y; };
  std::unordered_map<Point, int> m;  // ERROR: no hash function!
  // Must provide std::hash<Point> specialization or custom hasher
  ```

- **Inefficient lookup in vector**:
  ```cpp
  // BAD: O(n) lookup each time
  for (auto& item : items) {
      if (std::find(v.begin(), v.end(), item) != v.end()) { ... }
  }
  // GOOD: Use set or unordered_set for O(1)/O(log n) lookup
  ```

- **Modifying map keys**:
  ```cpp
  std::map<std::string, int> m;
  for (auto& [key, val] : m) {
      key = "new";  // ERROR: keys are const!
  }
  ```

## Quick Reference

```cpp
// Vector - dynamic array
std::vector<int> v;
v.push_back(1);              // Add to end
v.emplace_back(2);           // Construct in-place
v.reserve(100);              // Pre-allocate capacity
v.resize(50);                // Change size (adds/removes elements)
v.size();                    // Number of elements
v.capacity();                // Allocated space
v.empty();                   // Check if empty
v.clear();                   // Remove all elements
v.front(); v.back();         // First/last element
v[i]; v.at(i);               // Access (at() has bounds check)

// List - doubly-linked list
std::list<int> l{1, 2, 3};
l.push_front(0);             // Add to front (O(1))
l.splice(it, other_list);    // Move elements from another list
l.merge(sorted_list);        // Merge sorted lists
l.remove(value);             // Remove all matching values
l.sort();                    // Sort the list

// Map - ordered key-value
std::map<std::string, int> m;
m["key"] = 42;               // Insert/update
m.at("key");                 // Access with bounds check
m.emplace("k", v);           // Construct in-place
m.contains("key");           // C++20: check existence
m.find("key");               // Returns iterator (or end())
m.erase("key");              // Remove by key
m.count("key");              // 0 or 1 for map

// Multimap - multiple values per key
std::multimap<std::string, int> mm;
mm.emplace("key", 1);
mm.emplace("key", 2);        // Same key, different value OK
auto [begin, end] = mm.equal_range("key");  // Get all values for key

// Unordered map - hash table
std::unordered_map<std::string, int> um;
um["key"] = 42;              // Same interface as map
um.bucket_count();           // Number of buckets
um.load_factor();            // Elements per bucket
um.reserve(100);             // Reserve buckets for n elements

// Structured bindings with maps
for (const auto& [key, value] : m) {
    std::cout << key << ": " << value << '\n';
}

// Custom hash for user types
struct Point { int x, y; };
struct PointHash {
    std::size_t operator()(const Point& p) const {
        return std::hash<int>{}(p.x) ^ (std::hash<int>{}(p.y) << 1);
    }
};
std::unordered_map<Point, int, PointHash> point_map;
```

## Container Selection Guide

```
Need fast random access?
  YES -> std::vector or std::array

Need fast insertion/removal at both ends?
  YES -> std::deque

Need stable iterators and fast middle insertion?
  YES -> std::list

Need key-value association?
  YES -> Need ordering?
         YES -> std::map
         NO  -> std::unordered_map (usually faster)

Need unique values only (no duplicates)?
  YES -> std::set or std::unordered_set

Need multiple values per key?
  YES -> std::multimap or std::unordered_multimap
```

## Book Sections Covered

- **12.1** Introduction
- **12.2** `vector`
- **12.3** `list`
- **12.4** `forward_list`
- **12.5** `map`
- **12.6** `unordered_map`
- **12.7** Allocators
- **12.8** Container Overview

## Exercises in This Chapter

| Exercise | Difficulty | Concepts Practiced |
|----------|------------|-------------------|
| ex01_containers | ** | Choosing the right container for the job |
| ex02_map | ** | Map operations, structured bindings, custom comparators |

## Running the Examples

```bash
# Build all chapter 12 examples
cmake --build build --target ch12_examples

# Run individual examples
./build/bin/ch12_vector
./build/bin/ch12_list
./build/bin/ch12_map
./build/bin/ch12_unordered_map

# Run chapter 12 tests
ctest --test-dir build -R ch12
```
