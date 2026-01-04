# Chapter 2: User-Defined Types

## TL;DR

C++ lets you create your own types to model domain concepts. Use `struct` for simple data aggregates with public members, `class` for types with invariants (where you need to enforce rules), `enum class` for type-safe enumerations, and rarely use `union` (prefer `std::variant`).

## Key Concepts

| Concept | One-liner | Example |
|---------|-----------|---------|
| `struct` | Aggregate of data, members public by default | `struct Point { int x, y; };` |
| `class` | Like struct but private by default, typically has invariants | `class Vector { int* elem; int sz; };` |
| Constructor | Initializes object, same name as class | `Point(int x, int y) : x{x}, y{y} {}` |
| `enum class` | Type-safe enumeration | `enum class Color { Red, Green, Blue };` |
| `union` | Overlapping storage for different types | `union Value { int i; double d; };` |
| Member functions | Functions that operate on object state | `double length() const { return ...; }` |

## Things to Remember

- **struct vs class**: Only difference is default access (`struct` = public, `class` = private). Use `struct` for POD-like types, `class` for types with invariants.
- **Always use `enum class`**: Never use plain `enum` in modern C++. `enum class` is type-safe and scoped.
- **Initialize members**: Use member initializer lists in constructors, not assignment in body.
- **Keep invariants**: A class should maintain its invariants through all operations.
- **const member functions**: Mark functions that don't modify state as `const`.
- **Avoid unions**: Use `std::variant` instead for type-safe discriminated unions.

## Modern C++ Highlights

| Feature | Old Style | Modern C++20 |
|---------|-----------|--------------|
| Enumerations | `enum Color { Red, Green };` | `enum class Color { Red, Green };` |
| Initialization | Constructor body assignment | Member initializer list |
| Discriminated union | `union` + manual tag | `std::variant<int, double, string>` |
| Default members | Initialize in constructor | In-class member initializers |

## Common Pitfalls

- **Unscoped enum pollution**: Plain `enum` pollutes namespace with its enumerators.
  ```cpp
  enum Color { Red };    // BAD: Red is now global
  enum class Color { Red };  // GOOD: must use Color::Red
  ```

- **Forgetting to initialize members**:
  ```cpp
  class Bad {
      int x;  // Uninitialized if no constructor!
  public:
      Bad() {}  // x contains garbage
  };

  class Good {
      int x = 0;  // In-class initializer
  public:
      Good() = default;  // x is 0
  };
  ```

- **Using union unsafely**:
  ```cpp
  union U { int i; double d; };
  U u;
  u.d = 3.14;
  int x = u.i;  // UB! Reading wrong member
  // Use std::variant instead
  ```

## Quick Reference

```cpp
// Struct - simple aggregate
struct Point {
    double x = 0;  // In-class initializer
    double y = 0;

    double distance_from_origin() const {
        return std::sqrt(x*x + y*y);
    }
};

// Class with invariant
class Vector {
    int* elem;
    int sz;
public:
    Vector(int s) : elem{new int[s]}, sz{s} {
        for (int i = 0; i < s; ++i) elem[i] = 0;
    }
    ~Vector() { delete[] elem; }

    int size() const { return sz; }
    int& operator[](int i) { return elem[i]; }
};

// Enum class (type-safe)
enum class Color { Red, Green, Blue };
Color c = Color::Red;
// int x = c;  // ERROR: no implicit conversion

// Enum with underlying type
enum class Byte : unsigned char { };

// Struct with constructor
struct Complex {
    double re, im;

    Complex(double r = 0, double i = 0) : re{r}, im{i} {}

    Complex operator+(Complex other) const {
        return {re + other.re, im + other.im};
    }
};
```

## Class Design Guidelines

```
+------------------+     +------------------+
|  Use struct if:  |     |  Use class if:   |
+------------------+     +------------------+
| - All members    |     | - Has invariants |
|   are public     |     |   to maintain    |
| - No invariants  |     | - Needs private  |
| - Simple data    |     |   implementation |
|   aggregate      |     | - Complex        |
|                  |     |   construction   |
+------------------+     +------------------+
```

## Book Sections Covered

- **2.1** Introduction
- **2.2** Structures
- **2.3** Classes
- **2.4** Enumerations
- **2.5** Unions

## Exercises in This Chapter

| Exercise | Difficulty | Concepts Practiced |
|----------|------------|-------------------|
| ex01_struct | ★☆☆ | Struct definition, member functions |
| ex02_class | ★★☆ | Class with invariant, constructor |
| ex03_enum | ★☆☆ | Enum class, switch statements |

## Running the Examples

```bash
# Build all chapter 2 examples
cmake --build build --target ch02_examples

# Run individual examples
./build/bin/ch02_structures
./build/bin/ch02_classes
./build/bin/ch02_enumerations
./build/bin/ch02_unions

# Run chapter 2 tests
ctest --test-dir build -R ch02
```
