// Chapter 6: Essential Operations - Operator Overloading
//
// This example demonstrates:
// - Arithmetic operators (+, -, *, /)
// - Comparison operators (==, !=, <, <=, >, >=, <=>)
// - Stream operators (<<, >>)
// - Subscript operator ([])
// - Function call operator ()
// - Conversion operators
// - Best practices for operator overloading

#include <compare>
#include <cmath>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

// =============================================================================
// 1. Arithmetic and Comparison Operators: Complex Number
// =============================================================================

class Complex {
    double re_;
    double im_;

public:
    // Constructors
    constexpr Complex(double re = 0.0, double im = 0.0) : re_{re}, im_{im} {}

    // Accessors
    [[nodiscard]] constexpr double real() const { return re_; }
    [[nodiscard]] constexpr double imag() const { return im_; }
    [[nodiscard]] double magnitude() const { return std::sqrt(re_ * re_ + im_ * im_); }

    // Compound assignment operators (members)
    constexpr Complex& operator+=(const Complex& other) {
        re_ += other.re_;
        im_ += other.im_;
        return *this;
    }

    constexpr Complex& operator-=(const Complex& other) {
        re_ -= other.re_;
        im_ -= other.im_;
        return *this;
    }

    constexpr Complex& operator*=(const Complex& other) {
        double new_re = re_ * other.re_ - im_ * other.im_;
        double new_im = re_ * other.im_ + im_ * other.re_;
        re_ = new_re;
        im_ = new_im;
        return *this;
    }

    // Unary operators
    constexpr Complex operator-() const { return Complex{-re_, -im_}; }
    constexpr Complex operator+() const { return *this; }

    // Comparison using C++20 spaceship operator
    // For Complex numbers, we compare by magnitude (partial ordering)
    // Note: True mathematical complex numbers have no natural ordering,
    // this is just for demonstration
    [[nodiscard]] auto operator<=>(const Complex& other) const {
        return magnitude() <=> other.magnitude();
    }

    // Equality comparison (exact equality of components)
    [[nodiscard]] constexpr bool operator==(const Complex& other) const {
        return re_ == other.re_ && im_ == other.im_;
    }
};

// Binary arithmetic operators (non-members for symmetry)
constexpr Complex operator+(Complex lhs, const Complex& rhs) {
    return lhs += rhs;
}

constexpr Complex operator-(Complex lhs, const Complex& rhs) {
    return lhs -= rhs;
}

constexpr Complex operator*(Complex lhs, const Complex& rhs) {
    return lhs *= rhs;
}

// Stream operators (non-members)
std::ostream& operator<<(std::ostream& os, const Complex& c) {
    os << "(" << c.real();
    if (c.imag() >= 0) {
        os << "+";
    }
    os << c.imag() << "i)";
    return os;
}

std::istream& operator>>(std::istream& is, Complex& c) {
    double re, im;
    char plus_or_minus, i_char;
    is >> re >> plus_or_minus >> im >> i_char;
    if (plus_or_minus == '-') {
        im = -im;
    }
    c = Complex{re, im};
    return is;
}

// =============================================================================
// 2. Subscript Operator: Safe Array
// =============================================================================

class SafeArray {
    std::vector<int> data_;

public:
    explicit SafeArray(std::size_t size) : data_(size, 0) {}

    SafeArray(std::initializer_list<int> init) : data_{init} {}

    // Subscript operator with bounds checking
    int& operator[](std::size_t index) {
        if (index >= data_.size()) {
            throw std::out_of_range{"SafeArray index out of range"};
        }
        return data_[index];
    }

    const int& operator[](std::size_t index) const {
        if (index >= data_.size()) {
            throw std::out_of_range{"SafeArray index out of range"};
        }
        return data_[index];
    }

    [[nodiscard]] std::size_t size() const { return data_.size(); }

    // Stream operator
    friend std::ostream& operator<<(std::ostream& os, const SafeArray& arr) {
        os << "[";
        for (std::size_t i = 0; i < arr.size(); ++i) {
            if (i > 0) os << ", ";
            os << arr[i];
        }
        os << "]";
        return os;
    }
};

// =============================================================================
// 3. Function Call Operator: Functor
// =============================================================================

// A functor that computes polynomial values
class Polynomial {
    std::vector<double> coefficients_;  // a0 + a1*x + a2*x^2 + ...

public:
    Polynomial(std::initializer_list<double> coeffs) : coefficients_{coeffs} {}

    // Function call operator - makes object callable like a function
    double operator()(double x) const {
        double result = 0.0;
        double x_power = 1.0;
        for (double coeff : coefficients_) {
            result += coeff * x_power;
            x_power *= x;
        }
        return result;
    }

    // Stream operator
    friend std::ostream& operator<<(std::ostream& os, const Polynomial& p) {
        bool first = true;
        for (std::size_t i = 0; i < p.coefficients_.size(); ++i) {
            double c = p.coefficients_[i];
            if (c == 0.0) continue;

            if (!first && c > 0) os << " + ";
            else if (c < 0) os << " - ";

            if (i == 0 || std::abs(c) != 1.0) {
                os << std::abs(c);
            }
            if (i == 1) os << "x";
            else if (i > 1) os << "x^" << i;

            first = false;
        }
        if (first) os << "0";
        return os;
    }
};

// =============================================================================
// 4. Conversion Operators
// =============================================================================

class Rational {
    int numerator_;
    int denominator_;

    static int gcd(int a, int b) {
        a = std::abs(a);
        b = std::abs(b);
        while (b != 0) {
            int temp = b;
            b = a % b;
            a = temp;
        }
        return a;
    }

    void normalize() {
        if (denominator_ < 0) {
            numerator_ = -numerator_;
            denominator_ = -denominator_;
        }
        int g = gcd(numerator_, denominator_);
        if (g > 1) {
            numerator_ /= g;
            denominator_ /= g;
        }
    }

public:
    Rational(int num = 0, int denom = 1) : numerator_{num}, denominator_{denom} {
        if (denom == 0) {
            throw std::invalid_argument{"Denominator cannot be zero"};
        }
        normalize();
    }

    // Explicit conversion to double (prevents accidental conversions)
    explicit operator double() const {
        return static_cast<double>(numerator_) / denominator_;
    }

    // Explicit conversion to bool (is non-zero?)
    explicit operator bool() const {
        return numerator_ != 0;
    }

    // Arithmetic
    Rational& operator+=(const Rational& other) {
        numerator_ = numerator_ * other.denominator_ + other.numerator_ * denominator_;
        denominator_ *= other.denominator_;
        normalize();
        return *this;
    }

    Rational& operator*=(const Rational& other) {
        numerator_ *= other.numerator_;
        denominator_ *= other.denominator_;
        normalize();
        return *this;
    }

    // Comparison
    auto operator<=>(const Rational& other) const {
        return numerator_ * other.denominator_ <=> other.numerator_ * denominator_;
    }

    bool operator==(const Rational& other) const = default;

    // Stream operator
    friend std::ostream& operator<<(std::ostream& os, const Rational& r) {
        os << r.numerator_;
        if (r.denominator_ != 1) {
            os << "/" << r.denominator_;
        }
        return os;
    }
};

Rational operator+(Rational lhs, const Rational& rhs) {
    return lhs += rhs;
}

Rational operator*(Rational lhs, const Rational& rhs) {
    return lhs *= rhs;
}

// =============================================================================
// 5. C++20 Spaceship Operator Demo
// =============================================================================

struct Point3D {
    double x, y, z;

    // Defaulted spaceship operator: generates all comparison operators
    auto operator<=>(const Point3D&) const = default;
};

// =============================================================================
// Main Demonstration
// =============================================================================

int main() {
    std::cout << "=== Operator Overloading Demo ===\n";

    // -------------------------------------------------------------------------
    std::cout << "\n--- Complex Number Operations ---\n";
    {
        Complex c1{3.0, 4.0};
        Complex c2{1.0, 2.0};

        std::cout << "c1 = " << c1 << "\n";
        std::cout << "c2 = " << c2 << "\n";
        std::cout << "c1 + c2 = " << (c1 + c2) << "\n";
        std::cout << "c1 - c2 = " << (c1 - c2) << "\n";
        std::cout << "c1 * c2 = " << (c1 * c2) << "\n";
        std::cout << "-c1 = " << (-c1) << "\n";
        std::cout << "|c1| = " << c1.magnitude() << "\n";

        std::cout << "c1 == c2: " << (c1 == c2 ? "true" : "false") << "\n";
        std::cout << "|c1| > |c2|: " << (c1 > c2 ? "true" : "false") << "\n";

        // Parsing from string
        Complex parsed;
        std::istringstream iss{"5+3i"};
        iss >> parsed;
        std::cout << "Parsed: " << parsed << "\n";
    }

    // -------------------------------------------------------------------------
    std::cout << "\n--- Safe Array Operations ---\n";
    {
        SafeArray arr{1, 2, 3, 4, 5};
        std::cout << "arr = " << arr << "\n";
        std::cout << "arr[2] = " << arr[2] << "\n";

        arr[2] = 100;
        std::cout << "After arr[2] = 100: " << arr << "\n";

        try {
            std::cout << arr[10] << "\n";  // Out of bounds
        } catch (const std::out_of_range& e) {
            std::cout << "Caught exception: " << e.what() << "\n";
        }
    }

    // -------------------------------------------------------------------------
    std::cout << "\n--- Polynomial Functor ---\n";
    {
        // p(x) = 1 + 2x + 3x^2
        Polynomial p{1.0, 2.0, 3.0};
        std::cout << "p(x) = " << p << "\n";
        std::cout << "p(0) = " << p(0) << "\n";
        std::cout << "p(1) = " << p(1) << "\n";
        std::cout << "p(2) = " << p(2) << "\n";

        // Quadratic: x^2 - 5x + 6 = (x-2)(x-3)
        Polynomial quadratic{6.0, -5.0, 1.0};
        std::cout << "q(x) = " << quadratic << "\n";
        std::cout << "q(2) = " << quadratic(2) << " (root)\n";
        std::cout << "q(3) = " << quadratic(3) << " (root)\n";
    }

    // -------------------------------------------------------------------------
    std::cout << "\n--- Rational Number Operations ---\n";
    {
        Rational r1{1, 2};
        Rational r2{1, 3};

        std::cout << "r1 = " << r1 << "\n";
        std::cout << "r2 = " << r2 << "\n";
        std::cout << "r1 + r2 = " << (r1 + r2) << "\n";
        std::cout << "r1 * r2 = " << (r1 * r2) << "\n";

        // Explicit conversion to double
        std::cout << "double(r1) = " << static_cast<double>(r1) << "\n";

        // Explicit conversion to bool
        Rational zero{0};
        std::cout << "bool(r1) = " << (r1 ? "true" : "false") << "\n";
        std::cout << "bool(zero) = " << (zero ? "true" : "false") << "\n";

        // Comparison
        std::cout << "r1 > r2: " << (r1 > r2 ? "true" : "false") << "\n";
        std::cout << "r1 < r2: " << (r1 < r2 ? "true" : "false") << "\n";
    }

    // -------------------------------------------------------------------------
    std::cout << "\n--- C++20 Spaceship Operator ---\n";
    {
        Point3D p1{1.0, 2.0, 3.0};
        Point3D p2{1.0, 2.0, 3.0};
        Point3D p3{1.0, 2.0, 4.0};

        std::cout << "p1 == p2: " << (p1 == p2 ? "true" : "false") << "\n";
        std::cout << "p1 == p3: " << (p1 == p3 ? "true" : "false") << "\n";
        std::cout << "p1 < p3: " << (p1 < p3 ? "true" : "false") << "\n";
        std::cout << "p1 != p3: " << (p1 != p3 ? "true" : "false") << "\n";

        // All six comparison operators generated from <=>
        std::cout << "p1 <= p2: " << (p1 <= p2 ? "true" : "false") << "\n";
        std::cout << "p1 >= p2: " << (p1 >= p2 ? "true" : "false") << "\n";
    }

    std::cout << "\n=== End of Demo ===\n";
    return 0;
}
