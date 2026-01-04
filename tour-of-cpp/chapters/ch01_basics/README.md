# Chapter 1: The Basics

## TL;DR

C++ is a statically-typed, compiled language where every entity has a type known at compile time. Programs are built from functions, with `main()` as the entry point. Use `const` and `constexpr` liberally, prefer references over pointers, and **always initialize everything**.

## Key Concepts

| Concept | One-liner | Example |
|---------|-----------|---------|
| Static typing | Types checked at compile time | `int x = 42;` |
| `const` | Value cannot change after init | `const int x = 42;` |
| `constexpr` | Evaluated at compile time | `constexpr int sq(int x) { return x*x; }` |
| Reference | Alias to existing object | `int& r = x;` |
| Pointer | Holds address of object | `int* p = &x;` |
| `nullptr` | Type-safe null pointer | `int* p = nullptr;` |
| `auto` | Type deduction | `auto x = 42;` // int |
| Range-for | Iterate over containers | `for (auto& x : vec) {}` |
| `{}` init | Uniform initialization | `int x{42};` |

## Things to Remember

- **Initialization**: Always initialize variables. Use `{}` for safety (detects narrowing).
- **`const` by default**: If it shouldn't change, make it `const`.
- **References vs Pointers**: Prefer references; use pointers only when "no object" is valid or when you need to reseat.
- **`nullptr` not `NULL`**: Always use `nullptr` for null pointers in modern C++.
- **Scope**: Variables live from declaration to end of their `{}` block.
- **`auto`**: Use it to avoid redundancy, but be explicit when clarity matters.
- **Value categories**: Understand lvalues (can take address) vs rvalues (temporaries).

## Modern C++ Highlights

What's new/better in C++20 compared to older styles:

| Feature | Old Style | Modern C++20 |
|---------|-----------|--------------|
| Type deduction | `std::vector<int>::iterator it = v.begin();` | `auto it = v.begin();` |
| Null pointer | `int* p = NULL;` or `int* p = 0;` | `int* p = nullptr;` |
| Compile-time | `#define SQUARE(x) ((x)*(x))` | `constexpr int square(int x) { return x*x; }` |
| Loop iteration | `for (int i = 0; i < v.size(); ++i)` | `for (const auto& x : v)` |
| Initialization | `int x = 7.8;` (truncates!) | `int x{7.8};` (compile error!) |

## Common Pitfalls

- **Uninitialized variables**: Undefined behavior! Always initialize.
  ```cpp
  int x;        // BAD: uninitialized, contains garbage
  int x = 0;    // GOOD: explicitly initialized
  int x{};      // GOOD: zero-initialized
  ```

- **Dangling references**: Never return a reference to a local variable.
  ```cpp
  int& bad() {
      int local = 42;
      return local;  // BAD: dangling reference!
  }
  ```

- **`=` vs `==`**: Assignment vs comparison in conditions.
  ```cpp
  if (x = 5) {}   // BAD: assigns 5 to x, then tests (always true)
  if (x == 5) {}  // GOOD: compares x with 5
  ```

- **Narrowing conversions**: `int x = 7.8;` silently truncates to 7.
  ```cpp
  int x = 7.8;   // Compiles, but x is 7 (data loss!)
  int x{7.8};    // Compile ERROR: narrowing conversion
  ```

- **Array decay**: Arrays decay to pointers, losing size info.
  ```cpp
  void f(int arr[]) {
      // arr is actually int*, sizeof(arr) is pointer size!
  }
  ```

## Quick Reference

```cpp
// Variable declarations
int i = 42;              // copy initialization
int j{42};               // direct-list initialization (preferred)
auto k = 42;             // type deduced as int
const auto PI = 3.14159; // const, type deduced as double

// Pointers and references
int* p = &i;             // pointer to i
int& r = i;              // reference to i (alias)
int* q = nullptr;        // null pointer

// Functions
double sqrt(double);                           // declaration
constexpr int square(int x) { return x * x; }  // compile-time capable
int add(int a, int b = 0);                     // default argument

// Range-based for
for (const auto& elem : container) {
    // use elem (read-only)
}
for (auto& elem : container) {
    // modify elem
}

// constexpr - computed at compile time when possible
constexpr int factorial(int n) {
    return (n <= 1) ? 1 : n * factorial(n - 1);
}
constexpr int f5 = factorial(5);  // computed at compile time: 120

// Structured bindings (C++17/20)
auto [x, y] = std::pair{1, 2};
auto [key, value] = *map.begin();
```

## Memory Model Basics

```
Stack                          Heap
+------------------+           +------------------+
| local variables  |           | dynamically      |
| function params  |           | allocated memory |
| return addresses |           | (new/delete)     |
+------------------+           +------------------+
    |                              ^
    | int x = 42;                  | int* p = new int{42};
    | (automatic lifetime)         | (manual lifetime)
```

- **Stack**: Fast, automatic lifetime, limited size
- **Heap**: Slower, manual management, larger capacity
- Modern C++: Use smart pointers (`unique_ptr`, `shared_ptr`) instead of raw `new`/`delete`

## Book Sections Covered

- **1.1** Introduction
- **1.2** Programs
- **1.3** Functions
- **1.4** Types, Variables, and Arithmetic
- **1.5** Scope and Lifetime
- **1.6** Constants
- **1.7** Pointers, Arrays, and References
- **1.8** Tests
- **1.9** Mapping to Hardware

## Exercises in This Chapter

| Exercise | Difficulty | Concepts Practiced |
|----------|------------|-------------------|
| ex01_hello | ★☆☆ | Basic I/O, main(), return values |
| ex02_functions | ★☆☆ | Function definition, parameters, overloading |
| ex03_types | ★☆☆ | Built-in types, initialization, auto |
| ex04_pointers_refs | ★★☆ | Pointers, references, nullptr |
| ex05_scope | ★★☆ | Scope rules, lifetime, shadowing |

## Running the Examples

```bash
# Build all chapter 1 examples
cmake --build build --target ch01_examples

# Run individual examples
./build/bin/ch01_hello_world
./build/bin/ch01_functions
./build/bin/ch01_types_variables
./build/bin/ch01_pointers_arrays_refs
./build/bin/ch01_control_flow
./build/bin/ch01_constants

# Run chapter 1 tests
ctest --test-dir build -R ch01
```
