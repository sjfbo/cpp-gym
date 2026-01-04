# Chapter 8: Concepts

## TL;DR

C++20 concepts constrain templates at compile time, providing better error messages and self-documenting code. They express requirements on template arguments as first-class language features. Use concepts to document what operations a type must support, and prefer standard library concepts when available.

## Key Concepts

| Concept | One-liner | Example |
|---------|-----------|---------|
| `concept` | Named compile-time predicate on types | `template<typename T> concept Numeric = std::integral<T> \|\| std::floating_point<T>;` |
| `requires` clause | Constrain template with concept | `template<std::integral T> T add(T a, T b);` |
| `requires` expression | Check if expressions are valid | `requires(T a) { a + a; }` |
| Standard concepts | Pre-defined concepts in `<concepts>` | `std::integral`, `std::copyable`, `std::invocable` |
| Variadic templates | Templates with variable arguments | `template<typename... Ts> void print(Ts... args);` |
| Fold expressions | Expand parameter packs with operators | `(args + ...)` |
| `sizeof...` | Count elements in parameter pack | `sizeof...(args)` |

## Things to Remember

- **Concept = compile-time predicate**: A concept evaluates to `true` or `false` at compile time based on type properties.
- **Use standard concepts first**: `<concepts>` provides well-tested concepts like `std::integral`, `std::copyable`, `std::same_as`.
- **Concepts improve error messages**: Instead of deep template errors, you get clear "concept not satisfied" messages.
- **Requires expressions test validity**: They check if code would compile, not if it runs correctly.
- **Multiple syntax forms**: Constrain with `requires` clause, template parameter, or abbreviated function templates.
- **Variadic templates enable type-safe variadics**: Safer and more powerful than C-style variadic functions.
- **Fold expressions simplify pack operations**: Replace recursive templates with concise `(... op args)` syntax.

## Modern C++ Highlights

What's new/better in C++20 compared to older styles:

| Feature | Old Style (C++17 and earlier) | Modern C++20 |
|---------|-------------------------------|--------------|
| Type constraints | `enable_if`, SFINAE | `template<std::integral T>` |
| Error messages | Deep template instantiation errors | Clear concept violation messages |
| Requirements | Comments or documentation | `requires` expressions |
| Abbreviated templates | `template<typename T> void f(T x)` | `void f(std::integral auto x)` |
| Concept definitions | Complex SFINAE traits | `concept Name = expression;` |
| Subsumption | Manual overload resolution | Automatic "more constrained" selection |

## Common Pitfalls

- **Over-constraining templates**: Adding unnecessary constraints reduces reusability.
  ```cpp
  // Over-constrained: requires both integral AND signed
  template<std::integral T> requires std::signed_integral<T>
  T abs_value(T x);  // Won't work with unsigned!

  // Better: just require what you need
  template<std::integral T>
  auto abs_value(T x) -> T;
  ```

- **Forgetting subsumption rules**: More constrained overloads are preferred.
  ```cpp
  template<std::integral T> void f(T);       // Less constrained
  template<std::signed_integral T> void f(T); // More constrained

  f(42);   // Calls signed_integral version (more specific)
  f(42u);  // Calls integral version (unsigned doesn't match signed)
  ```

- **Requires expression vs requires clause confusion**:
  ```cpp
  // requires CLAUSE (constrains template)
  template<typename T> requires std::integral<T>
  void f(T);

  // requires EXPRESSION (defines what's needed)
  template<typename T>
  concept Addable = requires(T a, T b) { a + b; };
  ```

- **Not checking return types in requires expressions**:
  ```cpp
  // Only checks that + compiles, not what it returns
  concept Addable = requires(T a, T b) { a + b; };

  // Better: verify return type
  concept Addable = requires(T a, T b) {
      { a + b } -> std::convertible_to<T>;
  };
  ```

- **Variadic pack expansion mistakes**:
  ```cpp
  // WRONG: missing ... for pack expansion
  template<typename... Ts>
  void print(Ts... args) {
      (std::cout << args);  // Error: args not expanded
  }

  // CORRECT: fold expression with ...
  template<typename... Ts>
  void print(Ts... args) {
      (std::cout << ... << args);  // Fold expression
  }
  ```

## Quick Reference

```cpp
// Concept definition
template<typename T>
concept Numeric = std::integral<T> || std::floating_point<T>;

// Concept with requires expression
template<typename T>
concept Hashable = requires(T a) {
    { std::hash<T>{}(a) } -> std::convertible_to<std::size_t>;
};

// Multiple requirements
template<typename T>
concept Container = requires(T c) {
    typename T::value_type;           // Has nested type
    typename T::iterator;             // Has iterator type
    { c.begin() } -> std::same_as<typename T::iterator>;
    { c.end() } -> std::same_as<typename T::iterator>;
    { c.size() } -> std::convertible_to<std::size_t>;
};

// ---- Constraint syntax options ----

// 1. Requires clause after template
template<typename T> requires std::integral<T>
T square(T x) { return x * x; }

// 2. Constraint in template parameter
template<std::integral T>
T cube(T x) { return x * x * x; }

// 3. Trailing requires clause
template<typename T>
T negate(T x) requires std::signed_integral<T> { return -x; }

// 4. Abbreviated function template (C++20)
auto twice(std::integral auto x) { return x * 2; }

// ---- Variadic templates ----

// Parameter pack
template<typename... Ts>
void ignore(Ts...) {}

// sizeof... for pack size
template<typename... Ts>
constexpr std::size_t count() { return sizeof...(Ts); }

// Fold expressions (C++17/20)
template<typename... Ts>
auto sum(Ts... args) { return (args + ...); }        // Right fold

template<typename... Ts>
auto sum_init(Ts... args) { return (0 + ... + args); } // Left fold with init

// Variadic print with fold
template<typename... Ts>
void print(Ts&&... args) {
    ((std::cout << args << ' '), ...);  // Comma fold
    std::cout << '\n';
}
```

## Book Sections Covered

- **8.1** Introduction to Concepts
- **8.2** Concepts
  - 8.2.1 Use of Concepts
  - 8.2.2 Concept-based Overloading
  - 8.2.3 Valid Code
  - 8.2.4 Definition of Concepts
  - 8.2.5 Concepts and `auto`
- **8.3** Generic Programming
  - 8.3.1 Use of Concepts
  - 8.3.2 Abstraction Using Templates
- **8.4** Variadic Templates
  - 8.4.1 Fold Expressions
  - 8.4.2 Forwarding Arguments

## Exercises in This Chapter

| Exercise | Difficulty | Concepts Practiced |
|----------|------------|-------------------|
| ex01_concepts | ** | Define custom concepts for numeric types, use requires expressions |
| ex02_variadic | ** | Implement variadic functions with fold expressions |

## Running the Examples

```bash
# Build all chapter 8 examples
cmake --build build --target ch08_examples

# Run individual examples
./build/bin/ch08_concepts_intro
./build/bin/ch08_concept_definitions
./build/bin/ch08_generic_programming
./build/bin/ch08_variadic_templates

# Run chapter 8 tests
ctest --test-dir build -R ch08
```
