// Complex Numbers
// Book reference: 17.4 Complex Numbers
//
// The <complex> header provides the std::complex template for
// complex number arithmetic.

#include <cmath>
#include <complex>
#include <iostream>
#include <numbers>

using namespace std::complex_literals;  // For 1i, 2.0i etc.

int main() {
    std::cout << "=== Complex Numbers ===\n\n";

    // =========================================================================
    // Construction
    // =========================================================================
    std::cout << "--- Construction ---\n";

    std::complex<double> z1{3.0, 4.0};  // 3 + 4i
    std::complex<double> z2 = 2.0 + 3.0i;  // Using literal
    std::complex<double> z3 = 5.0;  // Real number (5 + 0i)
    std::complex<double> z4 = 2.0i;  // Pure imaginary (0 + 2i)

    std::cout << "z1 = " << z1 << "\n";
    std::cout << "z2 = " << z2 << "\n";
    std::cout << "z3 = " << z3 << " (real only)\n";
    std::cout << "z4 = " << z4 << " (imaginary only)\n";
    std::cout << '\n';

    // =========================================================================
    // Accessing parts
    // =========================================================================
    std::cout << "--- Accessing Parts ---\n";

    std::complex<double> z{3.0, 4.0};

    std::cout << "z = " << z << "\n";
    std::cout << "z.real() = " << z.real() << "\n";
    std::cout << "z.imag() = " << z.imag() << "\n";
    std::cout << "std::real(z) = " << std::real(z) << "\n";
    std::cout << "std::imag(z) = " << std::imag(z) << "\n";
    std::cout << '\n';

    // =========================================================================
    // Basic arithmetic
    // =========================================================================
    std::cout << "--- Basic Arithmetic ---\n";

    std::complex<double> a{1.0, 2.0};
    std::complex<double> b{3.0, 4.0};

    std::cout << "a = " << a << "\n";
    std::cout << "b = " << b << "\n";
    std::cout << "a + b = " << (a + b) << "\n";
    std::cout << "a - b = " << (a - b) << "\n";
    std::cout << "a * b = " << (a * b) << "\n";
    std::cout << "a / b = " << (a / b) << "\n";
    std::cout << "-a = " << (-a) << "\n";
    std::cout << '\n';

    // =========================================================================
    // Magnitude and phase
    // =========================================================================
    std::cout << "--- Magnitude and Phase ---\n";

    std::complex<double> w{3.0, 4.0};

    std::cout << "w = " << w << "\n";
    std::cout << "abs(w) = " << std::abs(w) << " (magnitude/modulus)\n";
    std::cout << "norm(w) = " << std::norm(w) << " (squared magnitude)\n";
    std::cout << "arg(w) = " << std::arg(w) << " rad (phase angle)\n";
    std::cout << "arg(w) = " << (std::arg(w) * 180.0 / std::numbers::pi) << " degrees\n";
    std::cout << '\n';

    // =========================================================================
    // Conjugate
    // =========================================================================
    std::cout << "--- Conjugate ---\n";

    std::complex<double> c{3.0, 4.0};

    std::cout << "c = " << c << "\n";
    std::cout << "conj(c) = " << std::conj(c) << "\n";
    std::cout << "c * conj(c) = " << (c * std::conj(c)) << " (equals norm(c))\n";
    std::cout << '\n';

    // =========================================================================
    // Polar form
    // =========================================================================
    std::cout << "--- Polar Form ---\n";

    // Create from polar coordinates
    double r = 5.0;
    double theta = std::numbers::pi / 4;  // 45 degrees
    auto polar_z = std::polar(r, theta);

    std::cout << "polar(" << r << ", pi/4) = " << polar_z << "\n";
    std::cout << "  magnitude: " << std::abs(polar_z) << "\n";
    std::cout << "  phase: " << std::arg(polar_z) << " rad\n";

    // Convert back
    std::complex<double> p{3.0, 4.0};
    std::cout << "p = " << p << " -> polar(" << std::abs(p) << ", " << std::arg(p) << ")\n";
    std::cout << '\n';

    // =========================================================================
    // Exponential and logarithm
    // =========================================================================
    std::cout << "--- Exponential and Logarithm ---\n";

    std::complex<double> e1{1.0, 0.0};
    std::complex<double> e2{0.0, std::numbers::pi};

    std::cout << "exp(" << e1 << ") = " << std::exp(e1) << "\n";
    std::cout << "exp(i*pi) = " << std::exp(e2) << " (Euler's identity)\n";
    std::cout << "log(" << std::complex<double>{2.0, 0.0} << ") = "
              << std::log(std::complex<double>{2.0, 0.0}) << "\n";
    std::cout << "log(e) = " << std::log(std::complex<double>{std::numbers::e, 0.0}) << "\n";
    std::cout << '\n';

    // =========================================================================
    // Power and square root
    // =========================================================================
    std::cout << "--- Power and Square Root ---\n";

    std::complex<double> base{2.0, 3.0};

    std::cout << "base = " << base << "\n";
    std::cout << "pow(base, 2) = " << std::pow(base, 2.0) << "\n";
    std::cout << "pow(base, base) = " << std::pow(base, base) << "\n";
    std::cout << "sqrt(base) = " << std::sqrt(base) << "\n";
    std::cout << "sqrt(-1) = " << std::sqrt(std::complex<double>{-1.0, 0.0}) << "\n";
    std::cout << '\n';

    // =========================================================================
    // Trigonometric functions
    // =========================================================================
    std::cout << "--- Trigonometric Functions ---\n";

    std::complex<double> t{1.0, 1.0};

    std::cout << "t = " << t << "\n";
    std::cout << "sin(t) = " << std::sin(t) << "\n";
    std::cout << "cos(t) = " << std::cos(t) << "\n";
    std::cout << "tan(t) = " << std::tan(t) << "\n";
    std::cout << "sinh(t) = " << std::sinh(t) << "\n";
    std::cout << "cosh(t) = " << std::cosh(t) << "\n";
    std::cout << '\n';

    // =========================================================================
    // Practical example: Quadratic formula with complex roots
    // =========================================================================
    std::cout << "--- Quadratic Formula (Complex Roots) ---\n";

    // Solve ax^2 + bx + c = 0
    auto solve_quadratic = [](double qa, double qb, double qc) {
        std::complex<double> discriminant{qb * qb - 4 * qa * qc, 0.0};
        std::complex<double> sqrt_disc = std::sqrt(discriminant);
        std::complex<double> x1 = (-qb + sqrt_disc) / (2.0 * qa);
        std::complex<double> x2 = (-qb - sqrt_disc) / (2.0 * qa);
        return std::pair{x1, x2};
    };

    // x^2 - 5x + 6 = 0 (real roots: 2, 3)
    auto [r1, r2] = solve_quadratic(1, -5, 6);
    std::cout << "x^2 - 5x + 6 = 0: x = " << r1 << ", " << r2 << "\n";

    // x^2 + 1 = 0 (complex roots: i, -i)
    auto [c1, c2] = solve_quadratic(1, 0, 1);
    std::cout << "x^2 + 1 = 0: x = " << c1 << ", " << c2 << "\n";

    // x^2 - 2x + 5 = 0 (complex roots: 1+2i, 1-2i)
    auto [d1, d2] = solve_quadratic(1, -2, 5);
    std::cout << "x^2 - 2x + 5 = 0: x = " << d1 << ", " << d2 << "\n";

    return 0;
}

// Key points:
// - std::complex<T> where T is float, double, or long double
// - Use complex literals: 2.0 + 3.0i (with using directive)
// - abs() gives magnitude, arg() gives phase angle
// - std::polar() creates from polar coordinates
// - All standard math functions work with complex numbers
// - Euler's identity: e^(i*pi) + 1 = 0
