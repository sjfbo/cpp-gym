# Chapter 9: Library Overview

## TL;DR

The C++ standard library provides a rich set of components organized into headers. It includes **containers** (vector, map, set), **algorithms** (sort, find, transform), **strings**, **I/O streams**, and **utilities** (smart pointers, optional, variant). Use the standard library instead of reinventing the wheel - it's well-tested, efficient, and portable.

## Key Concepts

| Concept | One-liner | Example |
|---------|-----------|---------|
| Headers | Organize library components | `#include <vector>` |
| Namespace std | Contains all standard library names | `std::vector<int>` |
| Containers | Store collections of objects | `std::vector`, `std::map` |
| Algorithms | Generic operations on ranges | `std::sort`, `std::find` |
| Iterators | Abstract pointer-like access | `vec.begin()`, `vec.end()` |
| Strings | Text handling | `std::string`, `std::string_view` |
| I/O Streams | Input/output operations | `std::cout`, `std::ifstream` |
| Utilities | General-purpose tools | `std::optional`, `std::pair` |

## Things to Remember

- **Use the standard library**: It's tested, optimized, and portable. Don't reinvent wheels.
- **Know your containers**: `vector` for sequences, `map` for key-value, `set` for unique values.
- **Algorithms over loops**: Prefer `std::find`, `std::sort`, `std::transform` over hand-written loops.
- **Iterators**: The glue between containers and algorithms - learn the iterator categories.
- **Headers matter**: Include what you use, use what you include.
- **`using namespace std;`**: Avoid in headers, acceptable in small source files.
- **Check before use**: Many operations have preconditions (e.g., non-empty container for `front()`).

## Modern C++ Highlights

What's new/better in C++20 compared to older styles:

| Feature | Old Style | Modern C++20 |
|---------|-----------|--------------|
| Ranges | `sort(v.begin(), v.end())` | `ranges::sort(v)` |
| Concepts | Template errors pages long | `template<typename T> requires integral<T>` |
| Spans | Pass pointer + size | `std::span<int>` |
| Format | `printf`/streams | `std::format("{}", value)` |
| Calendar | Manual date handling | `std::chrono::year_month_day` |
| Source location | `__FILE__`, `__LINE__` | `std::source_location::current()` |

## Common Pitfalls

- **Iterator invalidation**: Modifying containers while iterating.
  ```cpp
  for (auto it = vec.begin(); it != vec.end(); ++it) {
      if (*it == 42) vec.erase(it);  // BAD: invalidates iterator!
  }
  // Use std::erase_if(vec, [](int x) { return x == 42; }); // C++20
  ```

- **Missing includes**: Each component needs its header.
  ```cpp
  std::vector<int> v;  // BAD if <vector> not included
  std::cout << x;      // BAD if <iostream> not included
  ```

- **Forgetting std::**: Without `using namespace std;`, qualify names.
  ```cpp
  vector<int> v;       // BAD: needs std::vector
  std::vector<int> v;  // GOOD
  ```

- **Empty container access**: Accessing elements in empty containers is UB.
  ```cpp
  std::vector<int> v;
  int x = v.front();   // BAD: undefined behavior!
  int x = v.empty() ? 0 : v.front();  // GOOD: check first
  ```

- **Confusing size types**: Container sizes are `size_t` (unsigned).
  ```cpp
  for (int i = 0; i < vec.size(); ++i)  // Warning: signed/unsigned comparison
  for (size_t i = 0; i < vec.size(); ++i)  // OK
  for (auto& x : vec)  // Best: range-based for
  ```

## Quick Reference

```cpp
// ============================================================================
// Containers
// ============================================================================
#include <vector>       // Dynamic array
#include <array>        // Fixed-size array
#include <deque>        // Double-ended queue
#include <list>         // Doubly-linked list
#include <forward_list> // Singly-linked list
#include <set>          // Ordered unique elements
#include <map>          // Ordered key-value pairs
#include <unordered_set>// Hash-based unique elements
#include <unordered_map>// Hash-based key-value pairs
#include <stack>        // LIFO adapter
#include <queue>        // FIFO adapter
#include <span>         // Non-owning view (C++20)

// ============================================================================
// Algorithms
// ============================================================================
#include <algorithm>    // sort, find, transform, copy, etc.
#include <numeric>      // accumulate, iota, gcd, lcm
#include <ranges>       // Range-based algorithms (C++20)

// Common algorithms
std::sort(v.begin(), v.end());
std::find(v.begin(), v.end(), target);
std::count_if(v.begin(), v.end(), predicate);
std::transform(in.begin(), in.end(), out.begin(), func);
std::accumulate(v.begin(), v.end(), 0);

// ============================================================================
// Strings
// ============================================================================
#include <string>       // std::string
#include <string_view>  // Non-owning string reference (C++17)
#include <regex>        // Regular expressions

// ============================================================================
// I/O
// ============================================================================
#include <iostream>     // cin, cout, cerr
#include <fstream>      // File streams
#include <sstream>      // String streams
#include <iomanip>      // I/O manipulators
#include <format>       // std::format (C++20)
#include <filesystem>   // File system operations (C++17)

// ============================================================================
// Utilities
// ============================================================================
#include <memory>       // unique_ptr, shared_ptr
#include <utility>      // pair, move, forward
#include <optional>     // Optional values (C++17)
#include <variant>      // Type-safe union (C++17)
#include <any>          // Type-erased container (C++17)
#include <tuple>        // Heterogeneous fixed-size collection
#include <functional>   // function, bind, reference_wrapper
#include <chrono>       // Time utilities
#include <random>       // Random number generation

// ============================================================================
// Concurrency
// ============================================================================
#include <thread>       // Thread management
#include <mutex>        // Mutual exclusion
#include <atomic>       // Atomic operations
#include <future>       // Async operations
#include <condition_variable>  // Synchronization

// ============================================================================
// Other
// ============================================================================
#include <concepts>     // Concept definitions (C++20)
#include <expected>     // Expected values (C++23)
#include <source_location>  // Source location (C++20)
```

## Standard Library Organization

```
Standard Library
|
+-- Containers
|   +-- Sequence: vector, array, deque, list, forward_list
|   +-- Associative: set, map, multiset, multimap
|   +-- Unordered: unordered_set, unordered_map, ...
|   +-- Adaptors: stack, queue, priority_queue
|   +-- Views: span, string_view
|
+-- Algorithms
|   +-- Non-modifying: find, count, all_of, any_of, none_of
|   +-- Modifying: copy, transform, fill, replace, remove
|   +-- Sorting: sort, stable_sort, partial_sort
|   +-- Searching: binary_search, lower_bound, upper_bound
|   +-- Numeric: accumulate, inner_product, partial_sum
|
+-- Strings & Regex
|   +-- string, wstring, u8string, u16string, u32string
|   +-- string_view
|   +-- regex, regex_match, regex_search, regex_replace
|
+-- I/O
|   +-- Streams: iostream, fstream, sstream
|   +-- Formatting: format, iomanip
|   +-- Filesystem: path, directory_iterator, file operations
|
+-- Utilities
|   +-- Memory: unique_ptr, shared_ptr, weak_ptr, allocator
|   +-- General: pair, tuple, optional, variant, any
|   +-- Time: chrono (durations, time_points, clocks)
|   +-- Random: random engines, distributions
|
+-- Concurrency
    +-- Threads: thread, jthread
    +-- Synchronization: mutex, lock_guard, condition_variable
    +-- Atomics: atomic<T>
    +-- Futures: future, promise, async
```

## Book Sections Covered

- **9.1** Introduction
- **9.2** Standard-Library Components
- **9.3** Standard-Library Headers and Namespace
- **9.4** Advice

## Exercises in This Chapter

This chapter is primarily conceptual - see later chapters for hands-on exercises with specific library components:
- Chapter 10: Strings and Regular Expressions
- Chapter 11: Input and Output
- Chapter 12: Containers
- Chapter 13: Algorithms

## Running the Examples

```bash
# Build all chapter 9 examples
cmake --build build --target ch09_examples

# Run the standard library tour
./build/bin/ch09_std_library_tour

# Run chapter 9 tests
ctest --test-dir build -R ch09
```
