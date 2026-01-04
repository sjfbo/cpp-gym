# Chapter 14: Ranges

## TL;DR

C++20 ranges revolutionize how we work with sequences by introducing lazy, composable views. Instead of copying data between algorithm calls, views transform elements on-demand. Use the pipe operator (`|`) to chain transformations into readable pipelines. Ranges eliminate iterator-pair boilerplate and enable cleaner, more expressive code.

## Key Concepts

| Concept | One-liner | Example |
|---------|-----------|---------|
| Range | Anything with `begin()` and `end()` | `std::vector<int>`, `std::string`, arrays |
| View | Lazy, non-owning range transformation | `std::views::filter`, `std::views::transform` |
| Range adaptor | Function that creates a view | `std::views::take(5)` |
| Pipe operator | Compose adaptors left-to-right | `v \| filter(f) \| transform(g)` |
| `std::ranges` | Namespace for range algorithms | `std::ranges::sort(v)` |
| Sentinel | Generalized end marker | Can be different type than iterator |
| Projection | Transform element before comparison | `std::ranges::sort(v, {}, &Person::name)` |

## Things to Remember

- **Views are lazy**: No work is done until you iterate or materialize.
- **Views don't own data**: They reference the underlying range; ensure it outlives the view.
- **Pipe reads left-to-right**: `v | filter(f) | transform(g)` applies filter first, then transform.
- **Use `std::ranges::` algorithms**: They accept ranges directly, no need for `begin()`/`end()`.
- **Projections simplify comparisons**: Sort by member without writing a custom comparator.
- **`to<Container>()`**: Materialize a view into a container (C++23, or use ranges-v3).
- **Views are cheap to copy**: They're lightweight, holding only references and state.

## Modern C++ Highlights

| Feature | Old Style | Modern C++20 |
|---------|-----------|--------------|
| Algorithm call | `std::sort(v.begin(), v.end())` | `std::ranges::sort(v)` |
| Filter + transform | Two separate loops or copies | `v \| filter(f) \| transform(g)` |
| Take first N | `std::copy_n(...)` | `v \| take(n)` |
| Sort by member | `sort(v.begin(), v.end(), [](a,b){ return a.x < b.x; })` | `ranges::sort(v, {}, &T::x)` |
| Enumerate | Manual index tracking | `views::enumerate` (C++23) |
| Zip ranges | Manual parallel iteration | `views::zip` (C++23) |

## Common Pitfalls

- **Dangling views**:
  ```cpp
  auto get_filtered() {
      std::vector<int> v{1, 2, 3};
      return v | std::views::filter([](int x) { return x > 1; });
      // BAD: v is destroyed, view dangles!
  }
  ```

- **Views are not containers**:
  ```cpp
  auto view = v | std::views::transform(f);
  view.push_back(42);  // ERROR: views don't own data
  ```

- **Forgetting to materialize**:
  ```cpp
  auto filtered = v | std::views::filter(pred);
  // filtered is still lazy - iterating multiple times may be inefficient
  // If you need a container: auto result = filtered | std::ranges::to<std::vector>();
  ```

- **Mutating through views unexpectedly**:
  ```cpp
  for (auto& x : v | std::views::filter(pred)) {
      x *= 2;  // This modifies the original vector!
  }
  ```

- **Non-const lambdas in views**:
  ```cpp
  int count = 0;
  auto view = v | std::views::filter([&count](int x) {
      ++count;  // Mutable capture - may cause issues with view copying
      return x > 0;
  });
  ```

## Quick Reference

```cpp
#include <ranges>

namespace views = std::views;
namespace ranges = std::ranges;

std::vector<int> v{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

// --- View adaptors ---
auto filtered = v | views::filter([](int x) { return x % 2 == 0; });
auto transformed = v | views::transform([](int x) { return x * x; });
auto first_five = v | views::take(5);
auto skip_two = v | views::drop(2);
auto reversed = v | views::reverse;

// --- Chaining views (pipelines) ---
auto pipeline = v
    | views::filter([](int x) { return x > 3; })
    | views::transform([](int x) { return x * 2; })
    | views::take(3);
// Lazy: computes [8, 10, 12] only when iterated

// --- Iterating ---
for (int x : pipeline) {
    std::cout << x << ' ';
}

// --- Range algorithms ---
ranges::sort(v);                          // Sort entire range
ranges::sort(v, std::greater{});          // Sort descending
ranges::sort(v, {}, &Person::age);        // Sort by projection

auto it = ranges::find(v, 5);             // Find element
auto it2 = ranges::find_if(v, pred);      // Find with predicate

bool all = ranges::all_of(v, pred);       // Check predicates
bool any = ranges::any_of(v, pred);
bool none = ranges::none_of(v, pred);

auto count = ranges::count(v, 5);         // Count occurrences
auto count2 = ranges::count_if(v, pred);

ranges::copy(v, output_it);               // Copy range
ranges::fill(v, 42);                      // Fill range

// --- Factory views ---
auto iota = views::iota(1, 10);           // [1, 2, 3, ..., 9]
auto infinite = views::iota(1);           // [1, 2, 3, ...] infinite!
auto repeated = views::repeat(42, 5);     // [42, 42, 42, 42, 42] (C++23)
auto empty = views::empty<int>;           // Empty view
auto single = views::single(42);          // View of one element

// --- String views ---
std::string s = "hello,world,foo,bar";
for (auto word : s | views::split(',')) {
    // word is a subrange
}

// --- Composing custom adaptors ---
auto even_squares = views::filter([](int x) { return x % 2 == 0; })
                  | views::transform([](int x) { return x * x; });
auto result = v | even_squares;

// --- Materialization (C++23 or ranges-v3) ---
// auto vec = v | views::filter(pred) | ranges::to<std::vector>();
```

## View Adaptor Reference

| Adaptor | Description | Example |
|---------|-------------|---------|
| `filter(pred)` | Keep elements where pred is true | `v \| filter(is_even)` |
| `transform(f)` | Apply f to each element | `v \| transform(square)` |
| `take(n)` | First n elements | `v \| take(5)` |
| `drop(n)` | Skip first n elements | `v \| drop(2)` |
| `take_while(pred)` | Take while pred is true | `v \| take_while(is_positive)` |
| `drop_while(pred)` | Drop while pred is true | `v \| drop_while(is_negative)` |
| `reverse` | Reverse the range | `v \| reverse` |
| `elements<N>` | Get Nth element of tuples | `pairs \| elements<0>` |
| `keys` | Get keys from pairs/maps | `map \| keys` |
| `values` | Get values from pairs/maps | `map \| values` |
| `join` | Flatten nested ranges | `nested \| join` |
| `split(delim)` | Split by delimiter | `str \| split(',')` |
| `common` | Make sentinel same as iterator | `v \| common` |

## Book Sections Covered

- **14.1** Introduction
- **14.2** Views
- **14.3** Generators
- **14.4** Pipelines
- **14.5** Concepts Overview

## Exercises in This Chapter

| Exercise | Difficulty | Concepts Practiced |
|----------|------------|-------------------|
| ex01_ranges | *** | Views, pipelines, range algorithms |

## Running the Examples

```bash
# Build all chapter 14 examples
cmake --build build --target ch14_examples

# Run individual examples
./build/bin/ch14_views
./build/bin/ch14_generators
./build/bin/ch14_pipelines

# Run chapter 14 tests
ctest --test-dir build -R ch14
```
