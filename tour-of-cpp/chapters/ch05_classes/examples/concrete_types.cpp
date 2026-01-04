// Concrete Types - Value Semantics
// Book reference: 5.2 Concrete Types
//
// Concrete types behave like built-in types:
// - Can be placed on the stack, in static memory, or as members
// - Can be copied, moved, and compared
// - Used directly (not through pointers/references)
// - No virtual functions (no runtime polymorphism overhead)

#include <cmath>
#include <compare>
#include <iostream>
#include <numbers>

// ----------------------------------------------------------------------------
// Complex Number Class (Classic concrete type)
// Book reference: 5.2.1 An Arithmetic Type
// ----------------------------------------------------------------------------

class Complex {
    double re;
    double im;

public:
    // Constructors
    constexpr Complex(double r = 0, double i = 0) : re{r}, im{i} {}

    // Accessors (const - don't modify state)
    constexpr double real() const { return re; }
    constexpr double imag() const { return im; }

    // Mutators
    void real(double r) { re = r; }
    void imag(double i) { im = i; }

    // Arithmetic operators (return by value - value semantics)
    constexpr Complex operator+(Complex z) const {
        return {re + z.re, im + z.im};
    }

    constexpr Complex operator-(Complex z) const {
        return {re - z.re, im - z.im};
    }

    constexpr Complex operator*(Complex z) const {
        return {re * z.re - im * z.im, re * z.im + im * z.re};
    }

    Complex operator/(Complex z) const {
        double denom = z.re * z.re + z.im * z.im;
        return {(re * z.re + im * z.im) / denom,
                (im * z.re - re * z.im) / denom};
    }

    // Compound assignment operators
    Complex& operator+=(Complex z) {
        re += z.re;
        im += z.im;
        return *this;
    }

    Complex& operator-=(Complex z) {
        re -= z.re;
        im -= z.im;
        return *this;
    }

    // Unary minus
    constexpr Complex operator-() const { return {-re, -im}; }

    // Comparison (C++20 spaceship operator)
    bool operator==(const Complex& z) const = default;

    // Magnitude and phase
    double magnitude() const { return std::sqrt(re * re + im * im); }

    double phase() const { return std::atan2(im, re); }

    // Conjugate
    constexpr Complex conjugate() const { return {re, -im}; }
};

// Non-member function for output
std::ostream& operator<<(std::ostream& os, const Complex& c) {
    return os << "(" << c.real() << " + " << c.imag() << "i)";
}

// Non-member functions for mixed-type arithmetic
constexpr Complex operator+(double d, Complex z) {
    return {d + z.real(), z.imag()};
}

constexpr Complex operator*(double d, Complex z) {
    return {d * z.real(), d * z.imag()};
}

// ----------------------------------------------------------------------------
// Date Class (Another concrete type with invariants)
// Book reference: 5.2.2 A Container
// ----------------------------------------------------------------------------

class Date {
public:
    enum class Month { jan = 1, feb, mar, apr, may, jun,
                       jul, aug, sep, oct, nov, dec };

private:
    int y;
    Month m;
    int d;

    // Helper: check if year is a leap year
    static bool is_leap_year(int year) {
        return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
    }

    // Helper: days in a given month
    static int days_in_month(Month month, int year) {
        switch (month) {
            case Month::jan: case Month::mar: case Month::may:
            case Month::jul: case Month::aug: case Month::oct:
            case Month::dec:
                return 31;
            case Month::apr: case Month::jun: case Month::sep:
            case Month::nov:
                return 30;
            case Month::feb:
                return is_leap_year(year) ? 29 : 28;
        }
        return 0;  // Should never reach here
    }

    // Validate the date
    bool is_valid() const {
        if (y < 1 || static_cast<int>(m) < 1 || static_cast<int>(m) > 12) {
            return false;
        }
        return d >= 1 && d <= days_in_month(m, y);
    }

public:
    // Constructor enforces invariant
    Date(int year, Month month, int day) : y{year}, m{month}, d{day} {
        if (!is_valid()) {
            throw std::invalid_argument("Invalid date");
        }
    }

    // Accessors
    int year() const { return y; }
    Month month() const { return m; }
    int day() const { return d; }

    // Add days (simplified - handles month/year rollover)
    Date& add_days(int n) {
        d += n;
        while (d > days_in_month(m, y)) {
            d -= days_in_month(m, y);
            m = static_cast<Month>(static_cast<int>(m) + 1);
            if (static_cast<int>(m) > 12) {
                m = Month::jan;
                ++y;
            }
        }
        return *this;
    }

    // Comparison operators (C++20)
    auto operator<=>(const Date& other) const = default;
};

// Output operator
std::ostream& operator<<(std::ostream& os, const Date& d) {
    return os << d.year() << "-"
              << static_cast<int>(d.month()) << "-"
              << d.day();
}

// ----------------------------------------------------------------------------
// Point2D Class (Simple value type)
// ----------------------------------------------------------------------------

class Point2D {
    double x_;
    double y_;

public:
    constexpr Point2D(double x = 0, double y = 0) : x_{x}, y_{y} {}

    // Accessors
    constexpr double x() const { return x_; }
    constexpr double y() const { return y_; }

    // Vector operations (treating point as position vector)
    constexpr Point2D operator+(Point2D p) const {
        return {x_ + p.x_, y_ + p.y_};
    }

    constexpr Point2D operator-(Point2D p) const {
        return {x_ - p.x_, y_ - p.y_};
    }

    constexpr Point2D operator*(double s) const {
        return {x_ * s, y_ * s};
    }

    // Distance from origin
    double magnitude() const {
        return std::sqrt(x_ * x_ + y_ * y_);
    }

    // Distance to another point
    double distance_to(Point2D p) const {
        return (*this - p).magnitude();
    }

    // Comparison
    bool operator==(const Point2D& p) const = default;
    auto operator<=>(const Point2D& p) const = default;
};

std::ostream& operator<<(std::ostream& os, const Point2D& p) {
    return os << "(" << p.x() << ", " << p.y() << ")";
}

// Non-member scalar multiplication (allows 2.0 * point)
constexpr Point2D operator*(double s, Point2D p) {
    return p * s;
}

// ----------------------------------------------------------------------------
// Main demonstration
// ----------------------------------------------------------------------------

int main() {
    std::cout << "=== Complex Numbers (Concrete Type) ===\n";
    std::cout << "Book reference: 5.2.1 An Arithmetic Type\n\n";

    Complex z1{3, 4};
    Complex z2{1, 2};

    std::cout << "z1 = " << z1 << "\n";
    std::cout << "z2 = " << z2 << "\n";
    std::cout << "z1 + z2 = " << (z1 + z2) << "\n";
    std::cout << "z1 - z2 = " << (z1 - z2) << "\n";
    std::cout << "z1 * z2 = " << (z1 * z2) << "\n";
    std::cout << "z1 / z2 = " << (z1 / z2) << "\n";
    std::cout << "|z1| = " << z1.magnitude() << "\n";
    std::cout << "conjugate(z1) = " << z1.conjugate() << "\n";

    // Value semantics: copy creates independent object
    Complex z3 = z1;
    z3 += {1, 1};
    std::cout << "\nAfter z3 = z1; z3 += (1+1i):\n";
    std::cout << "z1 = " << z1 << " (unchanged)\n";
    std::cout << "z3 = " << z3 << " (modified copy)\n";

    // Compile-time computation (constexpr)
    constexpr Complex ct1{1, 2};
    constexpr Complex ct2{3, 4};
    constexpr Complex ct3 = ct1 + ct2;
    std::cout << "\nCompile-time: " << ct1 << " + " << ct2 << " = " << ct3 << "\n";

    std::cout << "\n=== Date Class (Concrete Type with Invariant) ===\n";

    Date today{2024, Date::Month::jan, 15};
    std::cout << "Today: " << today << "\n";

    Date future = today;
    future.add_days(30);
    std::cout << "30 days later: " << future << "\n";

    // Comparison
    if (today < future) {
        std::cout << today << " is before " << future << "\n";
    }

    // Invalid date throws
    try {
        Date bad{2024, Date::Month::feb, 30};  // February doesn't have 30 days
    } catch (const std::invalid_argument& e) {
        std::cout << "Invalid date caught: " << e.what() << "\n";
    }

    std::cout << "\n=== Point2D (Simple Value Type) ===\n";

    Point2D p1{3, 4};
    Point2D p2{1, 1};

    std::cout << "p1 = " << p1 << "\n";
    std::cout << "p2 = " << p2 << "\n";
    std::cout << "p1 + p2 = " << (p1 + p2) << "\n";
    std::cout << "p1 - p2 = " << (p1 - p2) << "\n";
    std::cout << "p1 * 2 = " << (p1 * 2) << "\n";
    std::cout << "2 * p1 = " << (2 * p1) << "\n";
    std::cout << "|p1| = " << p1.magnitude() << "\n";
    std::cout << "distance(p1, p2) = " << p1.distance_to(p2) << "\n";

    std::cout << "\n=== Key Characteristics of Concrete Types ===\n";
    std::cout << "1. Value semantics: copy creates independent objects\n";
    std::cout << "2. No virtual functions: no runtime overhead\n";
    std::cout << "3. Can be stack-allocated, embedded in other objects\n";
    std::cout << "4. Support arithmetic and comparison operators\n";
    std::cout << "5. Often constexpr-friendly for compile-time use\n";
    std::cout << "6. Invariants enforced through constructors\n";

    return 0;
}
