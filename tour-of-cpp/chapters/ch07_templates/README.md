# Chapter 7: Templates

## TL;DR

Templates enable **generic programming** in C++, allowing you to write code that works with any type. The compiler generates type-safe, specialized code at compile time - you get the flexibility of generics with the performance of hand-written type-specific code. Templates are the foundation of the STL and modern C++ libraries.

## Key Concepts

| Concept | One-liner | Example |
|---------|-----------|---------|
| Class template | Parameterized type definition | `template<typename T> class Vector { T* elem; };` |
| Function template | Generic function for any type | `template<typename T> T max(T a, T b);` |
| Template argument deduction | Compiler infers types from arguments | `max(3, 7)` deduces `T = int` |
| Specialization | Custom implementation for specific types | `template<> class Vector<bool> { ... };` |
| CTAD | Class Template Argument Deduction (C++17) | `std::pair p{1, 2.0};` deduces `pair<int, double>` |
| `if constexpr` | Compile-time conditional (C++17) | `if constexpr (std::is_integral_v<T>) { ... }` |
| Non-type parameters | Values as template arguments | `template<typename T, int N> class Buffer;` |
| Variadic templates | Variable number of template args | `template<typename... Args> void print(Args...);` |

## Things to Remember

- **Templates belong in headers**: The compiler needs the full template definition to instantiate it, so put template code in `.hpp` files (not `.cpp`).
- **Instantiation on demand**: Templates generate code only when used with specific types - unused combinations cost nothing.
- **Type requirements**: Templates implicitly require operations on `T` (like `<` for sorting). C++20 concepts make these explicit.
- **SFINAE basics**: "Substitution Failure Is Not An Error" - invalid template substitutions are silently discarded during overload resolution.
- **Prefer `typename` over `class`**: In template parameters, `typename` is clearer (both work identically).
- **Two-phase lookup**: Template code is checked twice - once when defined, once when instantiated.

## Modern C++ Highlights

| Feature | Old Style | Modern C++17/20 |
|---------|-----------|-----------------|
| Class template usage | `std::pair<int, double> p(1, 2.0);` | `std::pair p{1, 2.0};` (CTAD) |
| Type traits conditional | `std::enable_if<...>` | `if constexpr (...)` |
| Generic functions | `template<typename T> void f(T x)` | `void f(auto x)` (C++20) |
| Generic lambdas | Not available (C++11) | `[](auto x) { ... }` (C++14+) |
| Template lambdas | Not available | `[]<typename T>(T x) { ... }` (C++20) |
| Explicit requirements | SFINAE + enable_if | `requires` clauses (C++20) |

## Common Pitfalls

- **Templates in .cpp files**: Linker errors! Templates must be in headers or explicitly instantiated.
  ```cpp
  // BAD: in vector.cpp - linker error when used elsewhere
  template<typename T>
  void Vector<T>::push_back(const T& x) { ... }

  // GOOD: in vector.hpp - available for instantiation
  template<typename T>
  void Vector<T>::push_back(const T& x) { ... }
  ```

- **Cryptic error messages**: Template errors can be verbose. Read from the bottom up, look for the actual type mismatch.
  ```cpp
  // Error message may be hundreds of lines, but the cause is simple:
  std::sort(lst.begin(), lst.end());  // list iterators aren't random access!
  ```

- **Code bloat**: Each template instantiation generates code. `Vector<int>` and `Vector<double>` are completely separate types.
  ```cpp
  // These create 3 separate class instantiations:
  Vector<int> v1;
  Vector<double> v2;
  Vector<std::string> v3;
  ```

- **Dependent names**: Use `typename` for dependent types, `template` for dependent template members.
  ```cpp
  template<typename T>
  void foo() {
      typename T::value_type x;           // 'typename' required
      obj.template bar<int>();            // 'template' required
  }
  ```

- **ADL complications**: Argument-dependent lookup can find unexpected functions with templates.

## Quick Reference

```cpp
// Class template
template<typename T>
class Vector {
    T* elements;
    size_t sz;
public:
    Vector(size_t n) : elements{new T[n]}, sz{n} {}
    ~Vector() { delete[] elements; }

    T& operator[](size_t i) { return elements[i]; }
    const T& operator[](size_t i) const { return elements[i]; }
    size_t size() const { return sz; }
};

// Multiple template parameters
template<typename T, typename U>
struct Pair {
    T first;
    U second;
};

// Non-type template parameter
template<typename T, size_t N>
class Array {
    T data[N];
public:
    constexpr size_t size() const { return N; }
};

// Function template
template<typename T>
T max(T a, T b) {
    return (a > b) ? a : b;
}

// Abbreviated function template (C++20)
auto min(auto a, auto b) {
    return (a < b) ? a : b;
}

// Template with if constexpr
template<typename T>
auto process(T value) {
    if constexpr (std::is_integral_v<T>) {
        return value * 2;
    } else if constexpr (std::is_floating_point_v<T>) {
        return value * 2.0;
    } else {
        return value;
    }
}

// Generic lambda (C++14)
auto print = [](const auto& x) { std::cout << x << '\n'; };

// Template lambda (C++20)
auto cast = []<typename T>(auto value) { return static_cast<T>(value); };

// CTAD (C++17) - compiler deduces template arguments
std::vector v = {1, 2, 3};          // vector<int>
std::pair p = {42, "hello"};        // pair<int, const char*>
auto arr = std::array{1, 2, 3, 4};  // array<int, 4>
```

## Book Sections Covered

- **7.1** Introduction to Templates
- **7.2** Parameterized Types (class templates)
- **7.3** Function Templates
- **7.4** Template Mechanisms (specialization, SFINAE)

## Exercises in This Chapter

| Exercise | Difficulty | Concepts Practiced |
|----------|------------|-------------------|
| ex01_stack | ** | Implement a generic Stack<T> class template with push, pop, top, empty |
| ex02_algorithms | ** | Implement generic find and count algorithms using iterators |

## Running the Examples

```bash
# Build all chapter 7 examples
cmake --build build --target ch07_examples

# Run individual examples
./build/bin/ch07_parameterized_types
./build/bin/ch07_function_templates
./build/bin/ch07_lambda_templates
./build/bin/ch07_template_mechanisms

# Run chapter 7 tests
ctest --test-dir build -R ch07
```
