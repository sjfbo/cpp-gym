// Vector Class Implementation
// Book reference: 3.2 Separate Compilation
//
// Implementation files contain definitions: function bodies, static member
// initialization, etc. They implement the interface declared in headers.
//
// Key points:
// - Include the corresponding header first (catches missing includes)
// - Use the same namespace as the header
// - Define all declared functions

#include "vector.h"  // Include our header first - See 3.2

#include <algorithm>  // For std::copy, std::fill
#include <numeric>    // For std::accumulate
#include <utility>    // For std::exchange

namespace containers {

// ----------------------------------------------------------------------------
// Constructors
// ----------------------------------------------------------------------------

Vector::Vector() = default;  // Uses in-class member initializers

Vector::Vector(std::size_t size)
    : elem{size > 0 ? new double[size] : nullptr}
    , sz{size}
{
    // Zero-initialize all elements
    std::fill(elem, elem + sz, 0.0);
}

Vector::Vector(std::initializer_list<double> lst)
    : elem{lst.size() > 0 ? new double[lst.size()] : nullptr}
    , sz{lst.size()}
{
    std::copy(lst.begin(), lst.end(), elem);
}

// ----------------------------------------------------------------------------
// Destructor
// ----------------------------------------------------------------------------

Vector::~Vector() {
    delete[] elem;
}

// ----------------------------------------------------------------------------
// Copy operations
// ----------------------------------------------------------------------------

Vector::Vector(const Vector& other)
    : elem{other.sz > 0 ? new double[other.sz] : nullptr}
    , sz{other.sz}
{
    std::copy(other.elem, other.elem + sz, elem);
}

Vector& Vector::operator=(const Vector& other) {
    if (this != &other) {
        // Create new array first (exception safety)
        double* new_elem = other.sz > 0 ? new double[other.sz] : nullptr;
        std::copy(other.elem, other.elem + other.sz, new_elem);

        // Clean up old data
        delete[] elem;

        // Take ownership of new data
        elem = new_elem;
        sz = other.sz;
    }
    return *this;
}

// ----------------------------------------------------------------------------
// Move operations
// ----------------------------------------------------------------------------

Vector::Vector(Vector&& other) noexcept
    : elem{std::exchange(other.elem, nullptr)}
    , sz{std::exchange(other.sz, 0)}
{
    // other is now in a valid but empty state
}

Vector& Vector::operator=(Vector&& other) noexcept {
    if (this != &other) {
        delete[] elem;
        elem = std::exchange(other.elem, nullptr);
        sz = std::exchange(other.sz, 0);
    }
    return *this;
}

// ----------------------------------------------------------------------------
// Element access
// ----------------------------------------------------------------------------

double& Vector::operator[](std::size_t i) {
    return elem[i];  // No bounds checking for performance
}

const double& Vector::operator[](std::size_t i) const {
    return elem[i];
}

double& Vector::at(std::size_t i) {
    if (i >= sz) {
        throw std::out_of_range("Vector::at: index out of range");
    }
    return elem[i];
}

const double& Vector::at(std::size_t i) const {
    if (i >= sz) {
        throw std::out_of_range("Vector::at: index out of range");
    }
    return elem[i];
}

// ----------------------------------------------------------------------------
// Capacity
// ----------------------------------------------------------------------------

std::size_t Vector::size() const noexcept {
    return sz;
}

bool Vector::empty() const noexcept {
    return sz == 0;
}

// ----------------------------------------------------------------------------
// Iterators
// ----------------------------------------------------------------------------

double* Vector::begin() noexcept { return elem; }
double* Vector::end() noexcept { return elem + sz; }
const double* Vector::begin() const noexcept { return elem; }
const double* Vector::end() const noexcept { return elem + sz; }

// ----------------------------------------------------------------------------
// Non-member functions
// ----------------------------------------------------------------------------

double sum(const Vector& v) {
    return std::accumulate(v.begin(), v.end(), 0.0);
}

double dot_product(const Vector& a, const Vector& b) {
    if (a.size() != b.size()) {
        throw std::invalid_argument("dot_product: vectors must have same size");
    }

    double result = 0.0;
    for (std::size_t i = 0; i < a.size(); ++i) {
        result += a[i] * b[i];
    }
    return result;
}

}  // namespace containers
