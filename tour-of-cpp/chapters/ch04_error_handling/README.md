# Chapter 4: Error Handling

## TL;DR

- **Exceptions** are for recoverable errors that cannot be handled locally (e.g., file not found, network failure)
- **Assertions** are for programming errors and invariant violations that should never happen in correct code
- Use **RAII** to ensure resources are cleaned up even when exceptions are thrown
- Mark functions `noexcept` when they cannot throw to enable optimizations

---

## Key Concepts

| Concept | Description |
|---------|-------------|
| **exception** | An object thrown to signal an error condition that cannot be handled locally |
| **throw** | Statement that raises an exception, transferring control to the nearest matching catch handler |
| **try/catch** | Mechanism to handle exceptions; `try` block contains code that may throw, `catch` blocks handle specific exception types |
| **noexcept** | Specifier indicating a function will not throw exceptions; enables optimizations and is checked at compile-time |
| **RAII** | Resource Acquisition Is Initialization - tie resource lifetime to object lifetime for automatic cleanup |
| **invariants** | Conditions that must always be true for an object to be in a valid state |
| **assertions** | Runtime checks (`assert()`) or compile-time checks (`static_assert()`) that verify assumptions |

---

## Things to Remember

1. **Exception Safety Guarantees**
   - **No-throw guarantee**: Operation will not throw (mark with `noexcept`)
   - **Strong guarantee**: Operation succeeds completely or has no effect
   - **Basic guarantee**: No resources are leaked, objects remain in valid (but possibly changed) state

2. **Use RAII for Resource Cleanup**
   - Destructors run automatically during stack unwinding
   - Smart pointers (`unique_ptr`, `shared_ptr`) manage memory
   - Lock guards (`lock_guard`, `unique_lock`) manage mutexes
   - File streams close automatically on destruction

3. **noexcept Matters**
   - Move constructors/assignment should be `noexcept` when possible
   - `std::vector` uses `noexcept` move operations for efficiency
   - Destructors are implicitly `noexcept`
   - Termination occurs if a `noexcept` function throws

4. **Catch by Reference**
   - Always catch exceptions by const reference: `catch (const std::exception& e)`
   - Prevents slicing and unnecessary copies

5. **Don't Throw from Destructors**
   - Destructors are implicitly `noexcept`
   - Throwing during stack unwinding causes `std::terminate()`

---

## Modern C++ Highlights

### noexcept Specifications (C++11/17)

```cpp
// Unconditional noexcept
void safe_operation() noexcept;

// Conditional noexcept (C++17 - noexcept is part of type system)
template<typename T>
void swap(T& a, T& b) noexcept(noexcept(a.swap(b)));

// Check if expression can throw
static_assert(noexcept(std::declval<int>() + 1));
```

### std::expected (C++23)

```cpp
#include <expected>

// Return value OR error without exceptions
std::expected<int, std::string> parse_int(std::string_view str) {
    try {
        return std::stoi(std::string(str));
    } catch (...) {
        return std::unexpected("Invalid integer");
    }
}

// Usage
auto result = parse_int("42");
if (result) {
    std::cout << "Value: " << *result << '\n';
} else {
    std::cout << "Error: " << result.error() << '\n';
}
```

### std::source_location (C++20)

```cpp
#include <source_location>

void log_error(std::string_view msg,
               std::source_location loc = std::source_location::current()) {
    std::cerr << loc.file_name() << ":" << loc.line()
              << " - " << msg << '\n';
}
```

---

## Common Pitfalls

| Pitfall | Problem | Solution |
|---------|---------|----------|
| **Catching by value** | Causes slicing with inheritance hierarchies | Catch by `const&` |
| **Exception in destructor** | Can cause `std::terminate()` during stack unwinding | Never throw from destructors; absorb or log errors |
| **Naked new/delete** | Memory leaks if exception thrown between allocation and cleanup | Use smart pointers (`unique_ptr`, `shared_ptr`) |
| **Catching `...` without rethrowing** | Swallows all errors including unexpected ones | Rethrow unknown exceptions or catch specific types |
| **Empty catch blocks** | Silently ignores errors | At minimum, log the error |
| **Exception specifications (deprecated)** | `throw()` is deprecated and problematic | Use `noexcept` instead |

---

## Quick Reference

### try/catch Syntax

```cpp
try {
    // Code that may throw
    throw std::runtime_error("Something went wrong");
}
catch (const std::runtime_error& e) {
    // Handle runtime_error
    std::cerr << "Runtime error: " << e.what() << '\n';
}
catch (const std::exception& e) {
    // Handle any other std::exception
    std::cerr << "Exception: " << e.what() << '\n';
}
catch (...) {
    // Handle any other throwable (use sparingly)
    std::cerr << "Unknown exception\n";
    throw;  // Rethrow to preserve the exception
}
```

### Custom Exception Class

```cpp
#include <stdexcept>
#include <string>

class FileError : public std::runtime_error {
public:
    explicit FileError(const std::string& filename, const std::string& msg)
        : std::runtime_error("File '" + filename + "': " + msg)
        , filename_(filename)
    {}

    const std::string& filename() const noexcept { return filename_; }

private:
    std::string filename_;
};
```

### static_assert (Compile-Time Checks)

```cpp
// Type requirements
template<typename T>
class Container {
    static_assert(std::is_default_constructible_v<T>,
                  "Container requires default-constructible type");
    // ...
};

// Size/alignment checks
static_assert(sizeof(int) == 4, "Expected 4-byte int");
static_assert(alignof(double) == 8, "Expected 8-byte alignment");

// Constant expression validation
constexpr int array_size = 100;
static_assert(array_size > 0, "Array size must be positive");
```

### assert (Runtime Checks)

```cpp
#include <cassert>

void process(int* ptr, size_t count) {
    assert(ptr != nullptr && "Pointer must not be null");
    assert(count > 0 && "Count must be positive");
    // ... process data
}
```

---

## Book Sections Covered

| Section | Topic | Example File |
|---------|-------|--------------|
| 4.1 | Exceptions | `examples/exceptions.cpp` |
| 4.2 | Invariants | `examples/invariants.cpp` |
| 4.3 | Error-Handling Alternatives | `examples/exceptions.cpp` |
| 4.4 | Assertions | `examples/assertions.cpp` |
| 4.5 | Advice | All examples |

---

## Exercises

| Exercise | Description | Concepts Practiced |
|----------|-------------|-------------------|
| `ex01_exceptions.cpp` | Implement a file parser with proper exception handling | throw, try/catch, custom exceptions, RAII |
| `ex02_invariants.cpp` | Implement a Date class with enforced invariants | Class invariants, preconditions, postconditions |

---

## Building and Running

```bash
# Create build directory
mkdir build && cd build

# Configure and build
cmake ..
cmake --build .

# Run examples
./exceptions
./invariants
./assertions

# Run tests
ctest --output-on-failure
# or
./test_ch04
```
