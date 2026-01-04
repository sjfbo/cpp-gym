// Classes - Types with Invariants
// Book reference: 2.3 Classes
//
// A class is like a struct but with private members by default.
// Use class when you need to maintain invariants (rules that must
// always be true about the object's state).

#include <cassert>
#include <iostream>
#include <stdexcept>

// ----------------------------------------------------------------------------
// A simple class with an invariant
// Invariant: sz >= 0 and elem points to sz integers
// ----------------------------------------------------------------------------

class Vector {
private:
    int* elem;  // Pointer to elements
    int sz;     // Number of elements

public:
    // Constructor - establishes the invariant
    Vector(int s) : elem{new int[static_cast<std::size_t>(s)]}, sz{s} {
        if (s < 0) {
            throw std::length_error("Vector size must be non-negative");
        }
        for (int i = 0; i < s; ++i) {
            elem[i] = 0;  // Initialize to zero
        }
    }

    // Destructor - cleans up resources
    ~Vector() {
        delete[] elem;
    }

    // Access element (with bounds checking)
    int& operator[](int i) {
        if (i < 0 || i >= sz) {
            throw std::out_of_range("Vector index out of range");
        }
        return elem[i];
    }

    // Const version for read-only access
    const int& operator[](int i) const {
        if (i < 0 || i >= sz) {
            throw std::out_of_range("Vector index out of range");
        }
        return elem[i];
    }

    // Get size
    int size() const {
        return sz;
    }
};

// ----------------------------------------------------------------------------
// Class with multiple access levels
// ----------------------------------------------------------------------------

class Date {
public:
    // Public interface
    Date(int y, int m, int d);
    void add_day(int n);

    int year() const { return y; }
    int month() const { return m; }
    int day() const { return d; }

private:
    // Private implementation
    int y, m, d;

    // Private helper
    bool is_valid() const {
        return y > 0 && m >= 1 && m <= 12 && d >= 1 && d <= 31;
    }
};

// Constructor definition outside class
Date::Date(int year, int month, int day) : y{year}, m{month}, d{day} {
    if (!is_valid()) {
        throw std::invalid_argument("Invalid date");
    }
}

void Date::add_day(int n) {
    d += n;
    // Simplified: doesn't handle month/year rollover
    while (d > 31) {
        d -= 31;
        m++;
        if (m > 12) {
            m = 1;
            y++;
        }
    }
}

// ----------------------------------------------------------------------------
// Class with const member functions
// ----------------------------------------------------------------------------

class Complex {
    double re, im;

public:
    Complex(double r = 0, double i = 0) : re{r}, im{i} {}

    // Accessor functions (const - don't modify object)
    double real() const { return re; }
    double imag() const { return im; }

    // Mutator functions (non-const - modify object)
    void set_real(double r) { re = r; }
    void set_imag(double i) { im = i; }

    // Operators
    Complex operator+(Complex z) const {
        return {re + z.re, im + z.im};
    }

    Complex operator-(Complex z) const {
        return {re - z.re, im - z.im};
    }

    Complex operator*(Complex z) const {
        return {re * z.re - im * z.im, re * z.im + im * z.re};
    }
};

// Non-member function
std::ostream& operator<<(std::ostream& os, const Complex& c) {
    return os << "(" << c.real() << ", " << c.imag() << "i)";
}

// ----------------------------------------------------------------------------
// Class with static members
// ----------------------------------------------------------------------------

class Counter {
    int count;
    static int total_count;  // Shared across all instances

public:
    Counter() : count{0} {
        total_count++;
    }

    ~Counter() {
        total_count--;
    }

    void increment() { count++; }
    int get_count() const { return count; }

    static int get_total() { return total_count; }
};

// Static member definition (required outside class)
int Counter::total_count = 0;

// ----------------------------------------------------------------------------
// Main demonstration
// ----------------------------------------------------------------------------

int main() {
    std::cout << "=== Vector Class (RAII) ===\n";

    Vector v(5);
    std::cout << "Vector size: " << v.size() << "\n";

    // Set values
    for (int i = 0; i < v.size(); ++i) {
        v[i] = i * 10;
    }

    // Read values
    std::cout << "Elements: ";
    for (int i = 0; i < v.size(); ++i) {
        std::cout << v[i] << " ";
    }
    std::cout << "\n";

    // Bounds checking
    try {
        v[10] = 100;  // Out of range
    } catch (const std::out_of_range& e) {
        std::cout << "Caught: " << e.what() << "\n";
    }

    std::cout << "\n=== Date Class ===\n";

    Date today{2024, 1, 15};
    std::cout << "Today: " << today.year() << "-" << today.month() << "-" << today.day() << "\n";

    today.add_day(20);
    std::cout << "20 days later: " << today.year() << "-" << today.month() << "-" << today.day()
              << "\n";

    // Invalid date throws
    try {
        Date bad{2024, 13, 1};  // Invalid month
    } catch (const std::invalid_argument& e) {
        std::cout << "Invalid date caught: " << e.what() << "\n";
    }

    std::cout << "\n=== Complex Class ===\n";

    Complex c1{3, 4};
    Complex c2{1, 2};

    std::cout << "c1 = " << c1 << "\n";
    std::cout << "c2 = " << c2 << "\n";
    std::cout << "c1 + c2 = " << (c1 + c2) << "\n";
    std::cout << "c1 * c2 = " << (c1 * c2) << "\n";

    std::cout << "\n=== Static Members ===\n";

    std::cout << "Total counters: " << Counter::get_total() << "\n";

    {
        Counter a;
        Counter b;
        std::cout << "After creating 2: " << Counter::get_total() << "\n";

        a.increment();
        a.increment();
        b.increment();

        std::cout << "a.count = " << a.get_count() << "\n";
        std::cout << "b.count = " << b.get_count() << "\n";
    }

    std::cout << "After scope ends: " << Counter::get_total() << "\n";

    std::cout << "\n=== Key Points ===\n";
    std::cout << "1. Classes maintain invariants through private members\n";
    std::cout << "2. Constructors establish invariants\n";
    std::cout << "3. Destructors clean up resources (RAII)\n";
    std::cout << "4. const member functions don't modify object state\n";
    std::cout << "5. Static members are shared across all instances\n";

    return 0;
}
