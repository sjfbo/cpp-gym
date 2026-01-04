// Vector Class Declaration (Header File)
// Book reference: 3.2 Separate Compilation
//
// Headers contain declarations: class definitions, function prototypes,
// constants, and type aliases. They define the interface.
//
// Key points:
// - Use #pragma once or include guards to prevent multiple inclusion
// - Only declare, don't define (except inline/constexpr functions)
// - Include only what you need

#pragma once  // Modern include guard - See 3.2

#include <cstddef>    // For std::size_t
#include <stdexcept>  // For std::out_of_range
#include <initializer_list>

namespace containers {

// A simple dynamic array class demonstrating header/implementation separation
// Invariant: elem points to an array of sz doubles, or elem is nullptr and sz is 0
class Vector {
public:
    // Constructors
    Vector();                             // Default: empty vector
    explicit Vector(std::size_t size);    // Create with given size, zero-initialized
    Vector(std::initializer_list<double> lst);  // Initializer list

    // Destructor
    ~Vector();

    // Copy operations (Rule of Three/Five)
    Vector(const Vector& other);
    Vector& operator=(const Vector& other);

    // Move operations (C++11)
    Vector(Vector&& other) noexcept;
    Vector& operator=(Vector&& other) noexcept;

    // Element access
    double& operator[](std::size_t i);
    const double& operator[](std::size_t i) const;

    double& at(std::size_t i);  // Bounds-checked access
    const double& at(std::size_t i) const;

    // Capacity
    [[nodiscard]] std::size_t size() const noexcept;
    [[nodiscard]] bool empty() const noexcept;

    // Iterators (pointer-based for simplicity)
    double* begin() noexcept;
    double* end() noexcept;
    const double* begin() const noexcept;
    const double* end() const noexcept;

private:
    double* elem = nullptr;  // Pointer to elements
    std::size_t sz = 0;      // Number of elements
};

// Non-member functions (part of the interface)
[[nodiscard]] double sum(const Vector& v);
[[nodiscard]] double dot_product(const Vector& a, const Vector& b);

}  // namespace containers
