# Chapter 3: Modularity

## TL;DR

Modularity in C++ is achieved through separate compilation (headers + implementation files), namespaces for organizing code and avoiding name collisions, and C++20 modules for better encapsulation. Use header files to declare interfaces, implementation files for definitions, and namespaces to group related functionality logically.

## Key Concepts

| Concept | One-liner | Example |
|---------|-----------|---------|
| Separate compilation | Split code into headers (.h) and implementation (.cpp) | `#include "vector.h"` |
| Header files | Declare interfaces (classes, functions, constants) | `class Vector { ... };` |
| Include guards | Prevent multiple inclusion of headers | `#ifndef VECTOR_H` / `#pragma once` |
| Namespaces | Group related declarations to avoid name collisions | `namespace Math { ... }` |
| C++20 Modules | Modern alternative to headers with better encapsulation | `export module Math;` |
| Function arguments | Pass by value, reference, const ref, or pointer | `void f(const T& x)` |

## Things to Remember

- **Header guards are mandatory**: Use `#pragma once` (modern) or `#ifndef`/`#define`/`#endif` guards in every header.
- **Include order matters**: System headers, then third-party, then project headers. This helps catch missing includes.
- **Namespace usage**:
  - Never use `using namespace std;` in headers
  - Prefer `using` declarations (`using std::string;`) over directives
  - Use namespace aliases for long names (`namespace fs = std::filesystem;`)
- **One Definition Rule (ODR)**: Every entity must have exactly one definition across all translation units.
- **Declare in headers, define in .cpp**: Except for templates and inline functions.

## Modern C++ Highlights

| Feature | Old Style | Modern C++20 |
|---------|-----------|--------------|
| Include protection | `#ifndef`/`#define`/`#endif` | `#pragma once` (widely supported) |
| Code organization | Headers only | Modules: `export module X;` |
| Nested namespaces | `namespace A { namespace B { } }` | `namespace A::B { }` |
| Inline namespaces | N/A | `inline namespace v2 { }` for versioning |
| Structured bindings | Manual unpacking | `auto [a, b] = get_pair();` |

## Common Pitfalls

- **ODR (One Definition Rule) violations**:
  ```cpp
  // BAD: Defining in header (included multiple times = multiple definitions)
  // vector.h
  int global_count = 0;  // ERROR: multiple definitions

  // GOOD: Declare in header, define in .cpp
  // vector.h
  extern int global_count;  // Declaration only
  // vector.cpp
  int global_count = 0;     // Single definition
  ```

- **Circular includes**:
  ```cpp
  // a.h
  #include "b.h"  // b.h includes a.h -> circular!
  class A { B* b; };

  // SOLUTION: Use forward declarations
  // a.h
  class B;  // Forward declaration
  class A { B* b; };  // OK: only using pointer
  ```

- **Using namespace std in headers**:
  ```cpp
  // BAD: header.h
  using namespace std;  // Pollutes all files that include this header!

  // GOOD: Use qualified names or specific using declarations
  std::string name;
  // or in .cpp file only:
  using std::string;
  ```

- **Including unnecessary headers**:
  ```cpp
  // BAD: Slows compilation, increases dependencies
  #include <iostream>  // Only need for implementation

  // GOOD: Forward declare when possible, include only what you need
  class ostream;  // Forward declaration (in headers)
  ```

## Quick Reference

### Header/Implementation Pattern

```cpp
// === math_utils.h ===
#pragma once  // See 3.2

namespace math {

// Function declarations
double square(double x);
double cube(double x);

// Class declaration
class Calculator {
    double memory = 0;
public:
    void store(double val);
    double recall() const;
    void clear();
};

}  // namespace math

// === math_utils.cpp ===
#include "math_utils.h"

namespace math {

double square(double x) { return x * x; }
double cube(double x) { return x * x * x; }

void Calculator::store(double val) { memory = val; }
double Calculator::recall() const { return memory; }
void Calculator::clear() { memory = 0; }

}  // namespace math
```

### Namespace Usage

```cpp
// Namespace declaration - See 3.3
namespace graphics {
    class Point { int x, y; };
    class Line { Point start, end; };
}

// Nested namespace (C++17)
namespace company::project::module {
    void func();
}

// Namespace alias
namespace gfx = graphics;

// Anonymous namespace (internal linkage)
namespace {
    int helper_function() { return 42; }  // Only visible in this file
}

// Using declarations (prefer over using directives)
using std::string;
using std::vector;
```

### Function Argument Patterns

```cpp
// See 3.4
void by_value(int x);              // Copy, for small types
void by_reference(int& x);         // Modify caller's value
void by_const_ref(const int& x);   // Read-only, no copy
void by_pointer(int* x);           // Optional/nullable, or C API
void by_rvalue_ref(int&& x);       // Move semantics

// Guidelines:
// - Small types (int, double, ptr): pass by value
// - Large types for reading: pass by const reference
// - Need to modify: pass by reference
// - Optional parameter: pass by pointer (can be nullptr)
```

## Modules Preview (C++20)

```cpp
// math.cppm (module interface)
export module math;

export namespace math {
    double square(double x) { return x * x; }
    double cube(double x) { return x * x * x; }
}

// main.cpp
import math;

int main() {
    auto result = math::square(5);
}
```

Note: Module support varies by compiler. Headers remain the standard approach for most projects.

## Book Sections Covered

- **3.1** Introduction
- **3.2** Separate Compilation
- **3.3** Namespaces
- **3.4** Function Arguments and Return Values

## Exercises in This Chapter

| Exercise | Difficulty | Concepts Practiced |
|----------|------------|-------------------|
| ex01_header | ★★☆ | Header/implementation separation, include guards |
| ex02_namespace | ★☆☆ | Namespace organization, using declarations |

## Running the Examples

```bash
# Build all chapter 3 examples
cmake --build build --target ch03_examples

# Run individual examples
./build/bin/ch03_separate_compilation
./build/bin/ch03_namespaces
./build/bin/ch03_function_args

# Run chapter 3 tests
ctest --test-dir build -R ch03
```
