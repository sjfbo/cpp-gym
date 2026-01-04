// Mathematical Functions
// Book reference: 17.2 Mathematical Functions
//
// The <cmath> header provides mathematical functions for floating-point
// arithmetic, trigonometry, exponentials, and more.

#include <cmath>
#include <format>
#include <iostream>
#include <numbers>  // C++20 math constants
#include <numeric>  // C++20 midpoint

// std::midpoint is C++20 but missing from Apple libc++
// Provide a simple implementation as fallback
#if !__cpp_lib_interpolate
namespace std {
template<typename T>
constexpr T midpoint(T a, T b) noexcept {
    return a + (b - a) / 2;
}
}
#endif

int main() {
    std::cout << "=== Mathematical Functions ===\n\n";

    // =========================================================================
    // Basic arithmetic functions
    // =========================================================================
    std::cout << "--- Basic Arithmetic ---\n";

    double x = 2.0;
    double y = 3.0;

    std::cout << std::format("abs(-5.5) = {}\n", std::abs(-5.5));
    std::cout << std::format("fabs(-5.5) = {}\n", std::fabs(-5.5));
    std::cout << std::format("sqrt({}) = {}\n", x, std::sqrt(x));
    std::cout << std::format("cbrt(27) = {}\n", std::cbrt(27.0));
    std::cout << std::format("pow({}, {}) = {}\n", x, y, std::pow(x, y));
    std::cout << std::format("hypot(3, 4) = {}\n", std::hypot(3.0, 4.0));
    std::cout << std::format("fmod(5.5, 2.0) = {}\n", std::fmod(5.5, 2.0));
    std::cout << std::format("remainder(5.5, 2.0) = {}\n", std::remainder(5.5, 2.0));
    std::cout << '\n';

    // =========================================================================
    // Exponential and logarithmic functions
    // =========================================================================
    std::cout << "--- Exponential and Logarithmic ---\n";

    std::cout << std::format("exp(1) = {} (e)\n", std::exp(1.0));
    std::cout << std::format("exp2(10) = {}\n", std::exp2(10.0));
    std::cout << std::format("expm1(0.001) = {}\n", std::expm1(0.001));  // More precise for small x
    std::cout << std::format("log(e) = {}\n", std::log(std::numbers::e));
    std::cout << std::format("log10(100) = {}\n", std::log10(100.0));
    std::cout << std::format("log2(1024) = {}\n", std::log2(1024.0));
    std::cout << std::format("log1p(0.001) = {}\n", std::log1p(0.001));  // More precise for small x
    std::cout << '\n';

    // =========================================================================
    // Trigonometric functions
    // =========================================================================
    std::cout << "--- Trigonometry ---\n";

    double pi = std::numbers::pi;
    double angle = pi / 4;  // 45 degrees

    std::cout << std::format("sin(pi/4) = {}\n", std::sin(angle));
    std::cout << std::format("cos(pi/4) = {}\n", std::cos(angle));
    std::cout << std::format("tan(pi/4) = {}\n", std::tan(angle));
    std::cout << std::format("asin(0.5) = {} rad\n", std::asin(0.5));
    std::cout << std::format("acos(0.5) = {} rad\n", std::acos(0.5));
    std::cout << std::format("atan(1) = {} rad\n", std::atan(1.0));
    std::cout << std::format("atan2(1, 1) = {} rad\n", std::atan2(1.0, 1.0));
    std::cout << '\n';

    // =========================================================================
    // Hyperbolic functions
    // =========================================================================
    std::cout << "--- Hyperbolic Functions ---\n";

    std::cout << std::format("sinh(1) = {}\n", std::sinh(1.0));
    std::cout << std::format("cosh(1) = {}\n", std::cosh(1.0));
    std::cout << std::format("tanh(1) = {}\n", std::tanh(1.0));
    std::cout << std::format("asinh(1) = {}\n", std::asinh(1.0));
    std::cout << '\n';

    // =========================================================================
    // Rounding functions
    // =========================================================================
    std::cout << "--- Rounding ---\n";

    double values[] = {2.3, 2.5, 2.7, -2.3, -2.5, -2.7};

    std::cout << "Value\tfloor\tceil\ttrunc\tround\n";
    for (double v : values) {
        std::cout << std::format("{}\t{}\t{}\t{}\t{}\n",
            v, std::floor(v), std::ceil(v), std::trunc(v), std::round(v));
    }
    std::cout << '\n';

    // =========================================================================
    // C++20: lerp and midpoint
    // =========================================================================
    std::cout << "--- C++20: lerp and midpoint ---\n";

    double a = 10.0, b = 20.0;

    std::cout << std::format("lerp({}, {}, 0.0) = {}\n", a, b, std::lerp(a, b, 0.0));
    std::cout << std::format("lerp({}, {}, 0.5) = {}\n", a, b, std::lerp(a, b, 0.5));
    std::cout << std::format("lerp({}, {}, 1.0) = {}\n", a, b, std::lerp(a, b, 1.0));
    std::cout << std::format("lerp({}, {}, 0.25) = {}\n", a, b, std::lerp(a, b, 0.25));
    std::cout << std::format("midpoint({}, {}) = {}\n", a, b, std::midpoint(a, b));

    // midpoint avoids overflow
    int i1 = 2'000'000'000;
    int i2 = 2'000'000'001;
    std::cout << std::format("midpoint({}, {}) = {}\n", i1, i2, std::midpoint(i1, i2));
    std::cout << '\n';

    // =========================================================================
    // C++20: Math constants
    // =========================================================================
    std::cout << "--- C++20 Math Constants (std::numbers) ---\n";

    std::cout << std::format("e = {}\n", std::numbers::e);
    std::cout << std::format("pi = {}\n", std::numbers::pi);
    std::cout << std::format("sqrt2 = {}\n", std::numbers::sqrt2);
    std::cout << std::format("sqrt3 = {}\n", std::numbers::sqrt3);
    std::cout << std::format("ln2 = {}\n", std::numbers::ln2);
    std::cout << std::format("ln10 = {}\n", std::numbers::ln10);
    std::cout << std::format("phi (golden ratio) = {}\n", std::numbers::phi);
    std::cout << '\n';

    // =========================================================================
    // Special values
    // =========================================================================
    std::cout << "--- Special Values ---\n";

    double inf = std::numeric_limits<double>::infinity();
    double nan = std::nan("");

    std::cout << std::format("infinity = {}\n", inf);
    std::cout << std::format("-infinity = {}\n", -inf);
    std::cout << std::format("NaN = {}\n", nan);
    std::cout << std::format("1.0 / 0.0 = {}\n", 1.0 / 0.0);
    std::cout << std::format("0.0 / 0.0 = {}\n", 0.0 / 0.0);

    std::cout << std::format("isinf(inf) = {}\n", std::isinf(inf));
    std::cout << std::format("isnan(nan) = {}\n", std::isnan(nan));
    std::cout << std::format("isfinite(42) = {}\n", std::isfinite(42.0));
    std::cout << std::format("isfinite(inf) = {}\n", std::isfinite(inf));
    std::cout << '\n';

    // =========================================================================
    // Floating-point classification
    // =========================================================================
    std::cout << "--- Floating-Point Classification ---\n";

    auto classify = [](double v) -> std::string {
        switch (std::fpclassify(v)) {
            case FP_INFINITE: return "infinite";
            case FP_NAN: return "NaN";
            case FP_NORMAL: return "normal";
            case FP_SUBNORMAL: return "subnormal";
            case FP_ZERO: return "zero";
            default: return "unknown";
        }
    };

    double test_values[] = {42.0, 0.0, inf, nan, 1e-320};
    for (double v : test_values) {
        std::cout << std::format("{} is {}\n", v, classify(v));
    }

    return 0;
}

// Key points:
// - <cmath> provides comprehensive math functions
// - C++20 adds std::numbers for mathematical constants
// - Use std::lerp and std::midpoint for safe interpolation
// - Check for NaN/infinity with isnan/isinf/isfinite
// - Many functions have special versions for precision (expm1, log1p)
