# Chapter 15: Pointers and Containers

## TL;DR

Modern C++ replaces raw pointers with smart pointers (`unique_ptr`, `shared_ptr`) for automatic memory management. Use `std::span` for non-owning views of contiguous data, and `std::optional`, `std::variant`, and `std::any` for type-safe handling of optional values and discriminated unions. **Prefer unique_ptr by default; use shared_ptr only when ownership is truly shared.**

## Key Concepts

| Concept | One-liner | Example |
|---------|-----------|---------|
| `unique_ptr` | Exclusive ownership, automatic cleanup | `auto p = std::make_unique<int>(42);` |
| `shared_ptr` | Reference-counted shared ownership | `auto p = std::make_shared<int>(42);` |
| `weak_ptr` | Non-owning observer of shared_ptr | `std::weak_ptr<int> w = shared;` |
| `make_unique` | Safe factory for unique_ptr | `std::make_unique<Widget>(args...)` |
| `make_shared` | Efficient factory for shared_ptr | `std::make_shared<Widget>(args...)` |
| `std::span` | Non-owning view of contiguous data | `std::span<int> s{vec};` |
| `std::optional` | May or may not contain a value | `std::optional<int> maybe;` |
| `std::variant` | Type-safe discriminated union | `std::variant<int, string> v;` |
| `std::any` | Type-safe container for any type | `std::any a = 42;` |
| Custom deleter | Custom cleanup for smart pointers | `unique_ptr<FILE, decltype(&fclose)>` |

## Things to Remember

- **unique_ptr by default**: Use `unique_ptr` for single ownership; it has zero overhead compared to raw pointers.
- **shared_ptr for sharing**: Only use when multiple owners truly need to share lifetime control.
- **weak_ptr breaks cycles**: Use `weak_ptr` to break reference cycles in shared_ptr graphs.
- **Avoid raw new/delete**: Always use `make_unique` or `make_shared` instead of raw `new`.
- **span is non-owning**: `std::span` does not own its data; ensure the underlying container outlives the span.
- **optional vs pointer**: Use `optional<T>` instead of `T*` when "no value" is a valid state.
- **variant over union**: Prefer `std::variant` over C-style unions for type safety.
- **any for type erasure**: Use `std::any` sparingly; prefer `variant` when types are known.

## Modern C++ Highlights

What's new/better in C++20 compared to older styles:

| Feature | Old Style | Modern C++20 |
|---------|-----------|--------------|
| Memory management | `int* p = new int(42); delete p;` | `auto p = std::make_unique<int>(42);` |
| Array passing | `void f(int* arr, size_t n)` | `void f(std::span<int> arr)` |
| Optional values | `int* result = nullptr;` | `std::optional<int> result;` |
| Discriminated union | `union { int i; string s; };` | `std::variant<int, std::string>` |
| Type-safe any | `void* data;` | `std::any data;` |
| Null check | `if (ptr != nullptr)` | `if (ptr)` or `if (opt.has_value())` |

## Common Pitfalls

- **Dangling span**: Span outlives the container it references.
  ```cpp
  std::span<int> bad() {
      std::vector<int> v{1, 2, 3};
      return std::span{v};  // BAD: dangling span!
  }
  ```

- **Circular shared_ptr**: Creates memory leak.
  ```cpp
  struct Node {
      std::shared_ptr<Node> next;  // Can create cycles!
  };
  // Use weak_ptr for back-references
  struct Node {
      std::weak_ptr<Node> parent;  // GOOD: won't leak
  };
  ```

- **Accessing empty optional**: Undefined behavior.
  ```cpp
  std::optional<int> opt;
  int x = *opt;  // BAD: undefined behavior!
  int x = opt.value_or(0);  // GOOD: safe default
  ```

- **Wrong variant access**: Throws std::bad_variant_access.
  ```cpp
  std::variant<int, string> v = "hello";
  int x = std::get<int>(v);  // BAD: throws exception!
  if (auto* p = std::get_if<string>(&v)) {  // GOOD: safe access
      std::cout << *p;
  }
  ```

- **any_cast failure**: Throws std::bad_any_cast.
  ```cpp
  std::any a = 42;
  auto s = std::any_cast<std::string>(a);  // BAD: throws!
  if (auto* p = std::any_cast<int>(&a)) {  // GOOD: safe
      std::cout << *p;
  }
  ```

## Quick Reference

```cpp
// unique_ptr - exclusive ownership
auto up = std::make_unique<int>(42);      // Create
auto up2 = std::move(up);                  // Transfer ownership
int* raw = up2.get();                      // Get raw pointer (don't delete!)
up2.reset();                               // Release and delete
up2.reset(new int{100});                   // Replace managed object

// Custom deleter
auto file = std::unique_ptr<FILE, decltype(&fclose)>(
    fopen("file.txt", "r"), &fclose);

// shared_ptr - shared ownership
auto sp = std::make_shared<int>(42);       // Create (efficient)
auto sp2 = sp;                             // Share ownership
sp.use_count();                            // Reference count
sp.reset();                                // Release (deletes if last)

// weak_ptr - non-owning observer
std::weak_ptr<int> wp = sp;                // Observe
if (auto locked = wp.lock()) {             // Try to access
    // Use locked (shared_ptr)
}
wp.expired();                              // Check if dead

// span - non-owning view
std::vector<int> vec{1, 2, 3, 4, 5};
std::span<int> s{vec};                     // View of entire vector
std::span<int, 3> fixed{vec.data(), 3};    // Fixed-size span
for (int& x : s) { x *= 2; }               // Modify through span

// optional - maybe a value
std::optional<int> opt;                    // Empty
std::optional<int> opt2 = 42;              // Has value
opt = std::nullopt;                        // Clear
if (opt2) { /* has value */ }
int x = opt2.value_or(0);                  // Safe access with default
int y = *opt2;                             // Unchecked access

// variant - type-safe union
std::variant<int, std::string> v = 42;
v = "hello";                               // Change type
std::visit([](auto&& arg) {                // Visit pattern
    std::cout << arg;
}, v);
if (std::holds_alternative<std::string>(v)) { /* is string */ }

// any - type-safe any
std::any a = 42;
a = std::string{"hello"};                  // Change type
if (a.has_value()) { /* not empty */ }
a.type();                                  // Get type_info
std::any_cast<std::string>(a);             // Throws if wrong type
std::any_cast<std::string>(&a);            // Returns nullptr if wrong
```

## Book Sections Covered

- **15.1** Introduction
- **15.2** Pointers
  - **15.2.1** unique_ptr
  - **15.2.2** shared_ptr
  - **15.2.3** weak_ptr
- **15.3** Containers
  - **15.3.1** span
  - **15.3.2** array
- **15.4** Alternatives
  - **15.4.1** optional
  - **15.4.2** variant
  - **15.4.3** any

## Exercises in This Chapter

| Exercise | Difficulty | Concepts Practiced |
|----------|------------|-------------------|
| ex01_smart_ptrs | ★★☆ | unique_ptr, shared_ptr, ownership semantics |
| ex02_optional | ★★☆ | optional, variant, type-safe alternatives |

## Running the Examples

```bash
# Build all chapter 15 examples
cmake --build build --target ch15_examples

# Run individual examples
./build/bin/ch15_unique_ptr
./build/bin/ch15_shared_ptr
./build/bin/ch15_span
./build/bin/ch15_optional_variant

# Run chapter 15 tests
ctest --test-dir build -R ch15
```
