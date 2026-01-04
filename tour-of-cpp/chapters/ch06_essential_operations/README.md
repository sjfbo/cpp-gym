# Chapter 6: Essential Operations

## TL;DR

C++ gives you fine-grained control over object lifecycle through special member functions. Follow the **Rule of Zero** (let the compiler generate them) when possible, or the **Rule of Five** (implement all five) when managing resources manually. Use **RAII** (Resource Acquisition Is Initialization) to tie resource lifetime to object lifetime. Leverage **move semantics** for efficiency when transferring ownership of resources.

## Key Concepts

| Concept | One-liner | Example |
|---------|-----------|---------|
| Copy constructor | Creates new object as copy of existing | `T(const T& other)` |
| Move constructor | Transfers resources from temporary | `T(T&& other) noexcept` |
| Copy assignment | Replaces contents with copy | `T& operator=(const T& other)` |
| Move assignment | Replaces contents via transfer | `T& operator=(T&& other) noexcept` |
| Destructor | Cleans up when object is destroyed | `~T()` |
| RAII | Resource lifetime = object lifetime | `unique_ptr`, `lock_guard` |
| Rule of Zero | Use types that manage themselves | `std::string`, `std::vector` members |
| Rule of Five | If you define one, define all five | Custom resource-managing classes |

## Things to Remember

- **Rule of Zero preferred**: If your class only contains members that manage themselves (like `std::string`, `std::vector`, smart pointers), you don't need to write any special members.
- **When to implement special members**: Only when directly managing a resource (raw pointer, file handle, socket, etc.).
- **Always mark move operations `noexcept`**: This enables important optimizations (e.g., `std::vector` reallocation).
- **Use `= default`**: Explicitly request compiler-generated implementations when you want them.
- **Use `= delete`**: Explicitly disable operations that don't make sense (e.g., non-copyable types).
- **Copy-and-swap idiom**: A safe pattern for implementing assignment operators.
- **`std::move` doesn't move**: It's just a cast to rvalue reference; the actual move happens in the constructor/assignment.

## Modern C++ Highlights

| Feature | Old Style | Modern C++20 |
|---------|-----------|--------------|
| Default implementation | Write out trivial implementations | `T(const T&) = default;` |
| Disable operation | Private with no implementation | `T(const T&) = delete;` |
| Resource ownership | Raw `new`/`delete` | `std::unique_ptr<T>` |
| Shared ownership | Manual reference counting | `std::shared_ptr<T>` |
| Move semantics | Copy everything | `T(T&& other) noexcept` |
| Exception safety | Complex manual handling | noexcept move + RAII |

## Common Pitfalls

- **Forgetting self-assignment check**: Assignment operators must handle `x = x` correctly.
  ```cpp
  T& operator=(const T& other) {
      if (this == &other) return *this;  // Self-assignment check
      // ... rest of implementation
  }
  ```

- **Missing `noexcept` on move operations**: Without `noexcept`, containers fall back to copying.
  ```cpp
  T(T&& other);                    // BAD: vector will copy instead of move
  T(T&& other) noexcept;           // GOOD: enables move optimization
  ```

- **Resource leaks in assignment**: Must free existing resources before acquiring new ones.
  ```cpp
  T& operator=(const T& other) {
      delete[] data;               // Free old resource first!
      data = new int[other.size];  // Then allocate new
      // ...
  }
  ```

- **Moved-from objects left in invalid state**: After move, object should be in a valid (though unspecified) state.
  ```cpp
  T(T&& other) noexcept : data{other.data}, size{other.size} {
      other.data = nullptr;        // Leave other in valid state!
      other.size = 0;
  }
  ```

- **Not implementing all five**: If you need a custom destructor, you likely need all five special members.

- **Slicing**: Copying derived class through base class reference loses derived data.
  ```cpp
  Derived d;
  Base b = d;  // Slicing! Derived parts are lost
  ```

## Quick Reference

### Rule of Five Pattern

```cpp
class Resource {
    int* data;
    std::size_t size;

public:
    // Constructor
    explicit Resource(std::size_t n)
        : data{new int[n]{}}, size{n} {}

    // Destructor
    ~Resource() {
        delete[] data;
    }

    // Copy constructor
    Resource(const Resource& other)
        : data{new int[other.size]}, size{other.size} {
        std::copy(other.data, other.data + size, data);
    }

    // Move constructor
    Resource(Resource&& other) noexcept
        : data{other.data}, size{other.size} {
        other.data = nullptr;
        other.size = 0;
    }

    // Copy assignment
    Resource& operator=(const Resource& other) {
        if (this != &other) {
            delete[] data;
            data = new int[other.size];
            size = other.size;
            std::copy(other.data, other.data + size, data);
        }
        return *this;
    }

    // Move assignment
    Resource& operator=(Resource&& other) noexcept {
        if (this != &other) {
            delete[] data;
            data = other.data;
            size = other.size;
            other.data = nullptr;
            other.size = 0;
        }
        return *this;
    }
};
```

### Copy-and-Swap Idiom

```cpp
class Buffer {
    int* data;
    std::size_t size;

public:
    // Swap function (friend for ADL)
    friend void swap(Buffer& a, Buffer& b) noexcept {
        using std::swap;
        swap(a.data, b.data);
        swap(a.size, b.size);
    }

    // Unified assignment operator (handles both copy and move!)
    Buffer& operator=(Buffer other) noexcept {  // Note: pass by value
        swap(*this, other);
        return *this;
    }
};
```

### Rule of Zero Example

```cpp
// Preferred: Let standard library types manage resources
class Person {
    std::string name;
    std::vector<std::string> addresses;
    std::unique_ptr<Account> account;

public:
    // No destructor, copy/move constructors, or assignment operators needed!
    // Compiler generates correct implementations automatically.

    Person(std::string n) : name{std::move(n)} {}
};
```

### RAII Pattern

```cpp
// Generic RAII wrapper
template<typename T, typename Deleter>
class ScopedResource {
    T resource;
    Deleter deleter;

public:
    explicit ScopedResource(T res, Deleter del)
        : resource{res}, deleter{del} {}

    ~ScopedResource() {
        deleter(resource);
    }

    // Non-copyable
    ScopedResource(const ScopedResource&) = delete;
    ScopedResource& operator=(const ScopedResource&) = delete;

    // Movable
    ScopedResource(ScopedResource&& other) noexcept
        : resource{other.resource}, deleter{std::move(other.deleter)} {
        other.resource = T{};
    }

    T get() const { return resource; }
};
```

## Book Sections Covered

- **6.1** Introduction
- **6.2** Copy and Move
- **6.3** Resource Management
- **6.4** Operator Overloading
- **6.5** Conventional Operations
- **6.6** User-Defined Literals

## Exercises in This Chapter

| Exercise | Difficulty | Concepts Practiced |
|----------|------------|-------------------|
| ex01_string | ★★☆ | Implement simple string class with Rule of Five |
| ex02_raii | ★★☆ | Implement RAII wrapper for file handle |

## Running the Examples

```bash
# Build all chapter 6 examples
cmake --build build --target ch06_examples

# Run individual examples
./build/bin/ch06_copy_move
./build/bin/ch06_resource_management
./build/bin/ch06_operator_overloading
./build/bin/ch06_user_defined_literals

# Run chapter 6 tests
ctest --test-dir build -R ch06
```
