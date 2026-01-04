# Chapter 13: Algorithms

## TL;DR

The C++ Standard Library provides over 100 algorithms that work with any container through iterators. Use lambdas as predicates for clean, inline logic. C++17 introduced parallel execution policies (`std::execution::par`) to automatically parallelize algorithms. Modern C++ (C++20) adds ranges that provide a more elegant alternative to iterator pairs.

## Key Concepts

| Concept | One-liner | Example |
|---------|-----------|---------|
| Iterator | Generalized pointer to traverse containers | `auto it = v.begin();` |
| Range | Iterator pair `[begin, end)` | `std::sort(v.begin(), v.end());` |
| Predicate | Function returning bool | `[](int x) { return x > 0; }` |
| Comparator | Binary predicate for ordering | `[](int a, int b) { return a > b; }` |
| Transform | Apply function to each element | `std::transform(src, dst, fn);` |
| Accumulate | Fold/reduce operation | `std::accumulate(begin, end, init);` |
| Execution policy | Parallel/sequential control | `std::execution::par` |

## Things to Remember

- **Algorithms work on iterators, not containers**: Pass `begin()` and `end()`, not the container itself.
- **Use lambdas for predicates**: Cleaner than named functions for simple logic.
- **`const auto&` for read-only**: Use const reference in lambdas when not modifying.
- **Check iterator validity**: Many algorithms invalidate iterators (e.g., `remove` + `erase`).
- **`std::ranges` in C++20**: Prefer ranges for cleaner syntax when available.
- **Parallel algorithms need `<execution>`**: Include the header and link appropriately.
- **Return values matter**: Many algorithms return iterators (e.g., `find`, `remove`).

## Modern C++ Highlights

| Feature | Old Style | Modern C++20 |
|---------|-----------|--------------|
| Sorting | `std::sort(v.begin(), v.end());` | `std::ranges::sort(v);` |
| Finding | `std::find(v.begin(), v.end(), x);` | `std::ranges::find(v, x);` |
| Predicate | `bool pred(int x) { return x > 0; }` | `[](int x) { return x > 0; }` |
| Custom compare | `struct Cmp { bool operator()(...) };` | `[](auto a, auto b) { return a > b; }` |
| Parallel sort | (manual threading) | `std::sort(std::execution::par, ...)` |
| Range check | `std::all_of(v.begin(), v.end(), p)` | `std::ranges::all_of(v, p)` |

## Common Pitfalls

- **Forgetting the `erase` after `remove`**:
  ```cpp
  // BAD: remove doesn't actually erase
  std::remove(v.begin(), v.end(), val);  // Elements still there!

  // GOOD: erase-remove idiom
  v.erase(std::remove(v.begin(), v.end(), val), v.end());

  // BEST (C++20): std::erase
  std::erase(v, val);
  ```

- **Using `std::accumulate` with wrong initial value type**:
  ```cpp
  std::vector<double> v{1.5, 2.5, 3.5};
  auto sum = std::accumulate(v.begin(), v.end(), 0);    // Returns 6 (int!)
  auto sum = std::accumulate(v.begin(), v.end(), 0.0);  // Returns 7.5 (double)
  ```

- **Modifying container during iteration**:
  ```cpp
  // BAD: undefined behavior
  for (auto it = v.begin(); it != v.end(); ++it) {
      if (*it < 0) v.erase(it);  // Invalidates iterator!
  }

  // GOOD: use return value of erase
  for (auto it = v.begin(); it != v.end(); ) {
      if (*it < 0) it = v.erase(it);
      else ++it;
  }

  // BEST (C++20)
  std::erase_if(v, [](int x) { return x < 0; });
  ```

- **Assuming sorted input for `binary_search`**:
  ```cpp
  std::vector<int> v{5, 2, 8, 1};
  bool found = std::binary_search(v.begin(), v.end(), 2);  // UB! Not sorted
  std::sort(v.begin(), v.end());
  found = std::binary_search(v.begin(), v.end(), 2);  // OK
  ```

## Quick Reference

```cpp
// Sorting
std::sort(v.begin(), v.end());                    // Ascending
std::sort(v.begin(), v.end(), std::greater<>{});  // Descending
std::stable_sort(v.begin(), v.end());             // Preserves equal order
std::partial_sort(v.begin(), v.begin()+3, v.end()); // Top 3 sorted

// Searching
auto it = std::find(v.begin(), v.end(), val);     // Linear search
auto it = std::find_if(v.begin(), v.end(), pred); // Find by predicate
bool found = std::binary_search(v.begin(), v.end(), val);  // Sorted range
auto [lo, hi] = std::equal_range(v.begin(), v.end(), val); // Range of val

// Counting
int n = std::count(v.begin(), v.end(), val);
int n = std::count_if(v.begin(), v.end(), pred);

// Checking conditions
bool all = std::all_of(v.begin(), v.end(), pred);
bool any = std::any_of(v.begin(), v.end(), pred);
bool none = std::none_of(v.begin(), v.end(), pred);

// Transforming
std::transform(src.begin(), src.end(), dst.begin(), fn);
std::for_each(v.begin(), v.end(), fn);  // For side effects

// Accumulating
auto sum = std::accumulate(v.begin(), v.end(), init);
auto product = std::accumulate(v.begin(), v.end(), 1, std::multiplies<>{});
auto result = std::reduce(v.begin(), v.end());  // Parallel-friendly

// Modifying
std::fill(v.begin(), v.end(), val);
std::generate(v.begin(), v.end(), gen);
std::replace(v.begin(), v.end(), old_val, new_val);
std::reverse(v.begin(), v.end());
std::rotate(v.begin(), middle, v.end());
std::shuffle(v.begin(), v.end(), rng);

// Removing (returns new logical end)
auto new_end = std::remove(v.begin(), v.end(), val);
auto new_end = std::remove_if(v.begin(), v.end(), pred);
auto new_end = std::unique(v.begin(), v.end());  // Remove consecutive dups

// Copying
std::copy(src.begin(), src.end(), dst.begin());
std::copy_if(src.begin(), src.end(), dst.begin(), pred);
std::copy_n(src.begin(), n, dst.begin());

// Min/Max
auto it = std::min_element(v.begin(), v.end());
auto it = std::max_element(v.begin(), v.end());
auto [min_it, max_it] = std::minmax_element(v.begin(), v.end());

// Parallel execution (C++17)
#include <execution>
std::sort(std::execution::par, v.begin(), v.end());
std::for_each(std::execution::par_unseq, v.begin(), v.end(), fn);
```

## Iterator Categories

```
                    Input Iterator
                          |
                    Forward Iterator
                          |
                  Bidirectional Iterator
                          |
                  Random Access Iterator
                          |
                  Contiguous Iterator (C++20)

Container Iterator Types:
- vector, array, deque, string: Random Access (Contiguous for vector/array)
- list: Bidirectional
- forward_list: Forward
- set, map: Bidirectional
- unordered_set, unordered_map: Forward
- istream_iterator: Input
- ostream_iterator: Output
```

## Execution Policies (C++17)

```cpp
#include <execution>

// Sequential (default) - single threaded
std::sort(std::execution::seq, v.begin(), v.end());

// Parallel - may use multiple threads
std::sort(std::execution::par, v.begin(), v.end());

// Parallel unsequenced - may vectorize and parallelize
std::sort(std::execution::par_unseq, v.begin(), v.end());

// Unsequenced (C++20) - may vectorize, single thread
std::sort(std::execution::unseq, v.begin(), v.end());
```

## Book Sections Covered

- **13.1** Introduction
- **13.2** Use of Iterators
- **13.3** Iterator Types
- **13.4** Use of Predicates
- **13.5** Algorithm Overview
- **13.6** Parallel Algorithms

## Exercises in This Chapter

| Exercise | Difficulty | Concepts Practiced |
|----------|------------|-------------------|
| ex01_algorithms | ** | Common algorithm usage with predicates |
| ex02_custom_algorithm | *** | Writing your own algorithm templates |

## Running the Examples

```bash
# Build all chapter 13 examples
cmake --build build --target ch13_examples

# Run individual examples
./build/bin/ch13_iterators
./build/bin/ch13_predicates
./build/bin/ch13_algorithm_overview
./build/bin/ch13_parallel_algorithms

# Run chapter 13 tests
ctest --test-dir build -R ch13
```
