# Chapter 16: Utilities

## TL;DR

The C++ standard library provides essential utilities for time handling (`<chrono>`), callable wrappers (`std::function`), type traits for metaprogramming (`<type_traits>`), and perfect forwarding (`std::move`/`std::forward`). These tools enable writing generic, efficient, and expressive code. **Prefer lambdas over std::bind, and use perfect forwarding for generic wrappers.**

## Key Concepts

| Concept | One-liner | Example |
|---------|-----------|---------|
| `std::chrono` | Type-safe time handling | `std::chrono::seconds{5}` |
| Time point | Instant in time | `std::chrono::system_clock::now()` |
| Duration | Time interval | `std::chrono::milliseconds{100}` |
| `std::function` | Type-erased callable wrapper | `std::function<int(int)> f;` |
| `std::bind` | Partial function application | `std::bind(f, 42, _1)` |
| Type traits | Compile-time type queries | `std::is_integral_v<T>` |
| `std::enable_if` | SFINAE-based overloading | `enable_if_t<is_integral_v<T>>` |
| `std::move` | Cast to rvalue reference | `std::move(obj)` |
| `std::forward` | Perfect forwarding | `std::forward<T>(arg)` |
| Concepts | C++20 type constraints | `template<std::integral T>` |

## Things to Remember

- **Chrono is strongly typed**: Durations carry their unit (seconds, milliseconds); no implicit conversions lose precision.
- **Prefer lambdas over bind**: Lambdas are clearer, more efficient, and easier to debug.
- **function has overhead**: std::function involves type erasure; use templates for hot paths.
- **Type traits are constexpr**: Use `_v` suffix (e.g., `is_integral_v<T>`) for cleaner code.
- **move doesn't move**: It casts to rvalue reference; actual moving happens in move constructors.
- **forward preserves value category**: Use in templates to forward arguments exactly as received.
- **Concepts replace enable_if**: In C++20, prefer concepts for cleaner constraint syntax.

## Modern C++ Highlights

What's new/better in C++20 compared to older styles:

| Feature | Old Style | Modern C++20 |
|---------|-----------|--------------|
| Callable wrapper | `std::bind(f, 42, _1)` | `[f](auto x) { return f(42, x); }` |
| Type constraints | `enable_if_t<is_integral_v<T>>` | `template<std::integral T>` |
| Time literals | `std::chrono::seconds{5}` | `using namespace std::chrono; 5s` |
| Duration comparison | Manual conversion | `1h > 59min` (direct comparison) |
| Calendar | Manual calculation | `std::chrono::year_month_day` |
| Time zones | OS-specific APIs | `std::chrono::current_zone()` |

## Common Pitfalls

- **Chrono precision loss**: Implicit narrowing truncates.
  ```cpp
  auto ms = std::chrono::milliseconds{1500};
  auto sec = std::chrono::duration_cast<std::chrono::seconds>(ms);
  // sec is 1s, not 1.5s - precision lost!
  ```

- **Dangling references in bind**: Captured by value unintentionally.
  ```cpp
  int x = 42;
  auto f = std::bind(print, x);  // Copies x, not reference
  auto f = std::bind(print, std::ref(x));  // Reference wrapper
  ```

- **std::function from temporary lambda**: Can dangle.
  ```cpp
  std::function<int()> dangerous() {
      int local = 42;
      return [&local]() { return local; };  // Dangling!
  }
  ```

- **move from const**: Silently copies instead of moving.
  ```cpp
  const std::string s = "hello";
  auto s2 = std::move(s);  // Actually copies (const blocks move)
  ```

- **Forwarding non-forwarding reference**: Breaks forwarding.
  ```cpp
  template<typename T>
  void bad(T&& t) {
      use(t);  // Always lvalue! Should be std::forward<T>(t)
  }
  ```

## Quick Reference

```cpp
// ============================================================================
// Chrono - Time Handling
// ============================================================================

using namespace std::chrono;
using namespace std::chrono_literals;  // For 5s, 100ms, etc.

// Durations
auto dur1 = seconds{5};
auto dur2 = 100ms;
auto dur3 = 2h + 30min;

// Duration arithmetic
auto total = dur1 + dur2;
auto doubled = dur1 * 2;

// Duration cast (explicit for lossy conversions)
auto ms = duration_cast<milliseconds>(dur3);

// Time points
auto now = system_clock::now();
auto later = now + 5s;

// Measuring elapsed time
auto start = steady_clock::now();
// ... do work ...
auto end = steady_clock::now();
auto elapsed = duration_cast<milliseconds>(end - start);

// C++20 Calendar
auto today = year_month_day{floor<days>(system_clock::now())};
auto xmas = 2024y/December/25;

// ============================================================================
// Function Adaption
// ============================================================================

// std::function - type-erased callable
std::function<int(int, int)> f = [](int a, int b) { return a + b; };
std::function<void()> callback;  // Can be empty

// Invoke any callable
std::invoke(f, 1, 2);  // Works with functions, lambdas, member pointers

// std::bind (prefer lambdas)
using namespace std::placeholders;
auto bound = std::bind(f, 10, _1);  // f(10, x)
bound(5);  // Returns f(10, 5) = 15

// std::ref for reference wrapper
int x = 0;
auto inc = std::bind([](int& n) { ++n; }, std::ref(x));
inc();  // x is now 1

// ============================================================================
// Type Traits
// ============================================================================

// Query type properties
static_assert(std::is_integral_v<int>);
static_assert(std::is_floating_point_v<double>);
static_assert(std::is_pointer_v<int*>);
static_assert(std::is_same_v<int, int>);

// Type transformations
using NoRef = std::remove_reference_t<int&>;     // int
using NoConst = std::remove_const_t<const int>;  // int
using AddPtr = std::add_pointer_t<int>;          // int*
using Decay = std::decay_t<const int&>;          // int

// Conditional type selection
using T = std::conditional_t<sizeof(int) == 4, int32_t, int64_t>;

// SFINAE with enable_if (prefer concepts in C++20)
template<typename T, std::enable_if_t<std::is_integral_v<T>, int> = 0>
T twice(T x) { return x * 2; }

// C++20 Concepts (preferred)
template<std::integral T>
T thrice(T x) { return x * 3; }

// ============================================================================
// Move and Forward
// ============================================================================

// std::move - cast to rvalue reference
std::string s = "hello";
std::string s2 = std::move(s);  // s is now empty (moved-from)

// Perfect forwarding
template<typename T>
void wrapper(T&& arg) {
    target(std::forward<T>(arg));  // Forward as lvalue or rvalue
}

// Forward with multiple args
template<typename... Args>
auto make_wrapper(Args&&... args) {
    return std::make_unique<Widget>(std::forward<Args>(args)...);
}

// Move semantics in containers
std::vector<std::string> vec;
std::string item = "large string";
vec.push_back(std::move(item));  // Moves instead of copies
```

## Book Sections Covered

- **16.1** Introduction
- **16.2** Time
  - **16.2.1** Duration
  - **16.2.2** Time Points
  - **16.2.3** Clocks
  - **16.2.4** Time Zones
- **16.3** Function Adaption
  - **16.3.1** function
  - **16.3.2** bind
  - **16.3.3** mem_fn
- **16.4** Type Functions
  - **16.4.1** Type Traits
  - **16.4.2** conditional
  - **16.4.3** enable_if
- **16.5** move() and forward()

## Exercises in This Chapter

| Exercise | Difficulty | Concepts Practiced |
|----------|------------|-------------------|
| ex01_chrono | ★★☆ | Durations, time points, measuring time |
| ex02_type_traits | ★★★ | Type traits, SFINAE, concepts |

## Running the Examples

```bash
# Build all chapter 16 examples
cmake --build build --target ch16_examples

# Run individual examples
./build/bin/ch16_chrono
./build/bin/ch16_function_adaption
./build/bin/ch16_type_functions
./build/bin/ch16_move_forward

# Run chapter 16 tests
ctest --test-dir build -R ch16
```
