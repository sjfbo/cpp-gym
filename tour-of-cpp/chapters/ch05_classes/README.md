# Chapter 5: Classes

## TL;DR

Use **concrete types** for value semantics (copy, assign, compare like built-in types). Use **abstract types** (pure virtual functions) to define interfaces when you need runtime polymorphism. Use **virtual functions** to enable derived classes to customize behavior. Always declare destructors `virtual` in base classes meant for inheritance.

## Key Concepts

| Concept | One-liner | Example |
|---------|-----------|---------|
| Concrete type | A type that behaves like a built-in type (value semantics) | `class Complex { double re, im; };` |
| Abstract type | A type that defines an interface with pure virtual functions | `class Container { virtual int size() const = 0; };` |
| Virtual function | A function that can be overridden in derived classes | `virtual void draw() { }` |
| Pure virtual | A virtual function with no implementation (forces override) | `virtual double area() const = 0;` |
| `override` | Explicitly mark a function as overriding a base virtual | `double area() const override { return pi*r*r; }` |
| `final` | Prevent further overriding or inheritance | `void draw() final;` or `class Leaf final : Base {};` |

## Things to Remember

- **Virtual destructor**: Always declare destructor `virtual` in any class intended as a base class. Without it, deleting a derived object through a base pointer causes undefined behavior.
  ```cpp
  class Base {
  public:
      virtual ~Base() = default;  // REQUIRED for polymorphic base classes
  };
  ```

- **Use `override`**: Always use `override` when overriding virtual functions. It catches signature mismatches at compile time.
  ```cpp
  class Derived : public Base {
      void foo() const override;  // Compiler error if Base::foo() doesn't exist
  };
  ```

- **Liskov Substitution Principle (LSP)**: A derived class should be usable wherever its base class is expected. If `Derived` is-a `Base`, any code using `Base*` should work correctly with `Derived*`.

- **Prefer composition over inheritance**: Inheritance creates tight coupling. Use it when you truly have an "is-a" relationship with polymorphic behavior.

- **Abstract classes cannot be instantiated**: A class with at least one pure virtual function is abstract and can only be used as a base class.

## Modern C++ Highlights

| Feature | Old Style | Modern C++20 |
|---------|-----------|--------------|
| Override | `virtual void f() {}` (hope signature matches) | `void f() override {}` (compiler-checked) |
| Prevent override | Documentation/comments | `void f() final;` |
| Prevent inheritance | Documentation/comments | `class Leaf final : Base {};` |
| Default implementations | Write empty body | `virtual ~Base() = default;` |
| Delete functions | Make private | `Base(const Base&) = delete;` |

## Common Pitfalls

- **Object slicing**: Copying a derived object into a base object loses derived data.
  ```cpp
  class Base { int x; };
  class Derived : public Base { int y; };  // y gets sliced off!

  Derived d;
  Base b = d;  // BAD: slicing! Only x is copied, y is lost
  Base& ref = d;  // OK: reference preserves polymorphism
  Base* ptr = &d; // OK: pointer preserves polymorphism
  ```

- **Forgetting virtual destructor**: Deleting through base pointer without virtual destructor is undefined behavior.
  ```cpp
  class Base { ~Base() {} };           // BAD: non-virtual destructor
  class Derived : public Base { ~Derived() {} };
  Base* p = new Derived;
  delete p;  // UNDEFINED BEHAVIOR! Derived destructor never called
  ```

- **Hiding vs overriding**: Using the wrong signature hides the base function instead of overriding.
  ```cpp
  class Base {
      virtual void f(int) {}
  };
  class Derived : public Base {
      void f(double) {}  // BAD: hides Base::f(int), doesn't override!
      void f(int) override {}  // GOOD: actually overrides
  };
  ```

- **Calling virtual functions in constructors/destructors**: Virtual dispatch doesn't work as expected during construction/destruction.
  ```cpp
  class Base {
      Base() { init(); }  // Calls Base::init(), even for Derived objects
      virtual void init() {}
  };
  ```

## Quick Reference

```cpp
// Interface pattern (abstract type)
class Shape {
public:
    virtual ~Shape() = default;  // Virtual destructor is essential
    virtual double area() const = 0;      // Pure virtual
    virtual double perimeter() const = 0; // Pure virtual
    virtual void draw() const = 0;        // Pure virtual
};

// Concrete implementation
class Circle : public Shape {
    double radius;
public:
    explicit Circle(double r) : radius{r} {}

    double area() const override { return 3.14159 * radius * radius; }
    double perimeter() const override { return 2 * 3.14159 * radius; }
    void draw() const override { /* ... */ }
};

// Virtual dispatch
void render(const Shape& s) {
    s.draw();  // Calls the appropriate draw() based on actual type
}

Circle c{5.0};
render(c);  // Calls Circle::draw()
```

```cpp
// Concrete type (value semantics)
class Complex {
    double re, im;
public:
    Complex(double r = 0, double i = 0) : re{r}, im{i} {}

    double real() const { return re; }
    double imag() const { return im; }

    Complex operator+(Complex z) const { return {re + z.re, im + z.im}; }
    Complex operator-(Complex z) const { return {re - z.re, im - z.im}; }
    Complex operator*(Complex z) const {
        return {re*z.re - im*z.im, re*z.im + im*z.re};
    }

    bool operator==(Complex z) const { return re == z.re && im == z.im; }
};
```

## Class Design Decision Tree

```
Should this type support polymorphism?
|
+-- NO --> Concrete type (value semantics)
|          - No virtual functions
|          - Copy/move operations defined
|          - Example: Complex, Date, Point
|
+-- YES --> Abstract type (reference semantics)
            |
            +-- Define interface with pure virtual functions
            |
            +-- Virtual destructor (= default)
            |
            +-- Derived classes use override
            |
            +-- Pass by reference or pointer, never by value
```

## Book Sections Covered

- **5.1** Introduction
- **5.2** Concrete Types (value semantics, arithmetic types)
- **5.3** Abstract Types (interfaces, pure virtual functions)
- **5.4** Virtual Functions (polymorphism, vtable)
- **5.5** Class Hierarchies (inheritance, multiple inheritance basics)

## Exercises in This Chapter

| Exercise | Difficulty | Concepts Practiced |
|----------|------------|-------------------|
| ex01_shape | ★★☆ | Shape hierarchy with Circle and Rectangle |
| ex02_container | ★★☆ | Container interface implementation |

## Running the Examples

```bash
# Build all chapter 5 examples
cmake --build build --target ch05_examples

# Run individual examples
./build/bin/ch05_concrete_types
./build/bin/ch05_abstract_types
./build/bin/ch05_virtual_functions
./build/bin/ch05_class_hierarchies

# Run chapter 5 tests
ctest --test-dir build -R ch05
```
