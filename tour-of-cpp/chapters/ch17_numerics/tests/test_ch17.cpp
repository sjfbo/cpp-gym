// Chapter 17 Tests: Numerics
// These tests verify understanding of numerical operations in C++.

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <cmath>
#include <complex>
#include <limits>
#include <numbers>
#include <random>

using Catch::Matchers::WithinRel;
using Catch::Matchers::WithinAbs;

// ============================================================================
// Section 17.2: Mathematical Functions
// ============================================================================

TEST_CASE("math: basic functions", "[ch17][math]") {
    REQUIRE_THAT(std::sqrt(4.0), WithinRel(2.0));
    REQUIRE_THAT(std::pow(2.0, 3.0), WithinRel(8.0));
    REQUIRE_THAT(std::abs(-5.0), WithinRel(5.0));
    REQUIRE_THAT(std::cbrt(27.0), WithinRel(3.0));
}

TEST_CASE("math: trigonometric functions", "[ch17][math]") {
    double pi = std::numbers::pi;

    REQUIRE_THAT(std::sin(0.0), WithinAbs(0.0, 1e-10));
    REQUIRE_THAT(std::sin(pi / 2), WithinRel(1.0));
    REQUIRE_THAT(std::cos(0.0), WithinRel(1.0));
    REQUIRE_THAT(std::cos(pi), WithinRel(-1.0));
    REQUIRE_THAT(std::tan(pi / 4), WithinRel(1.0));
}

TEST_CASE("math: exponential and logarithm", "[ch17][math]") {
    double e = std::numbers::e;

    REQUIRE_THAT(std::exp(1.0), WithinRel(e));
    REQUIRE_THAT(std::log(e), WithinRel(1.0));
    REQUIRE_THAT(std::log10(100.0), WithinRel(2.0));
    REQUIRE_THAT(std::log2(1024.0), WithinRel(10.0));
}

TEST_CASE("math: rounding functions", "[ch17][math]") {
    REQUIRE(std::floor(2.7) == 2.0);
    REQUIRE(std::floor(-2.7) == -3.0);
    REQUIRE(std::ceil(2.3) == 3.0);
    REQUIRE(std::ceil(-2.3) == -2.0);
    REQUIRE(std::trunc(2.7) == 2.0);
    REQUIRE(std::trunc(-2.7) == -2.0);
    REQUIRE(std::round(2.5) == 3.0);
    REQUIRE(std::round(-2.5) == -3.0);
}

TEST_CASE("math: hypot", "[ch17][math]") {
    // 3-4-5 triangle
    REQUIRE_THAT(std::hypot(3.0, 4.0), WithinRel(5.0));

    // Handles very large/small values without overflow
    double big = 1e200;
    REQUIRE(std::isfinite(std::hypot(big, big)));
}

TEST_CASE("math: lerp and midpoint (C++20)", "[ch17][math]") {
    REQUIRE_THAT(std::lerp(0.0, 10.0, 0.0), WithinRel(0.0));
    REQUIRE_THAT(std::lerp(0.0, 10.0, 1.0), WithinRel(10.0));
    REQUIRE_THAT(std::lerp(0.0, 10.0, 0.5), WithinRel(5.0));

    REQUIRE(std::midpoint(0, 10) == 5);
    REQUIRE(std::midpoint(1, 4) == 2);

    // midpoint avoids overflow
    int max = std::numeric_limits<int>::max();
    REQUIRE(std::midpoint(max, max - 2) == max - 1);
}

TEST_CASE("math: C++20 constants", "[ch17][math]") {
    REQUIRE_THAT(std::numbers::pi, WithinRel(3.14159265358979, 1e-10));
    REQUIRE_THAT(std::numbers::e, WithinRel(2.71828182845904, 1e-10));
    REQUIRE_THAT(std::numbers::sqrt2, WithinRel(std::sqrt(2.0)));
}

// ============================================================================
// Section 17.4: Complex Numbers
// ============================================================================

TEST_CASE("complex: construction", "[ch17][complex]") {
    std::complex<double> z1{3.0, 4.0};
    REQUIRE(z1.real() == 3.0);
    REQUIRE(z1.imag() == 4.0);

    using namespace std::complex_literals;
    auto z2 = 2.0 + 3.0i;
    REQUIRE(z2.real() == 2.0);
    REQUIRE(z2.imag() == 3.0);
}

TEST_CASE("complex: arithmetic", "[ch17][complex]") {
    std::complex<double> a{1.0, 2.0};
    std::complex<double> b{3.0, 4.0};

    auto sum = a + b;
    REQUIRE(sum.real() == 4.0);
    REQUIRE(sum.imag() == 6.0);

    auto diff = a - b;
    REQUIRE(diff.real() == -2.0);
    REQUIRE(diff.imag() == -2.0);

    auto prod = a * b;  // (1+2i)(3+4i) = 3 + 4i + 6i + 8i^2 = -5 + 10i
    REQUIRE(prod.real() == -5.0);
    REQUIRE(prod.imag() == 10.0);
}

TEST_CASE("complex: magnitude and phase", "[ch17][complex]") {
    std::complex<double> z{3.0, 4.0};

    REQUIRE_THAT(std::abs(z), WithinRel(5.0));
    REQUIRE_THAT(std::norm(z), WithinRel(25.0));  // |z|^2
    REQUIRE_THAT(std::arg(z), WithinRel(std::atan2(4.0, 3.0)));
}

TEST_CASE("complex: conjugate", "[ch17][complex]") {
    std::complex<double> z{3.0, 4.0};
    auto conj = std::conj(z);

    REQUIRE(conj.real() == 3.0);
    REQUIRE(conj.imag() == -4.0);

    // z * conj(z) = |z|^2
    auto product = z * conj;
    REQUIRE_THAT(product.real(), WithinRel(25.0));
    REQUIRE_THAT(product.imag(), WithinAbs(0.0, 1e-10));
}

TEST_CASE("complex: polar form", "[ch17][complex]") {
    auto z = std::polar(5.0, std::numbers::pi / 4);

    REQUIRE_THAT(std::abs(z), WithinRel(5.0));
    REQUIRE_THAT(std::arg(z), WithinRel(std::numbers::pi / 4));
}

TEST_CASE("complex: Euler's identity", "[ch17][complex]") {
    // e^(i*pi) + 1 = 0
    std::complex<double> i_pi{0.0, std::numbers::pi};
    auto result = std::exp(i_pi) + 1.0;

    REQUIRE_THAT(result.real(), WithinAbs(0.0, 1e-10));
    REQUIRE_THAT(result.imag(), WithinAbs(0.0, 1e-10));
}

// ============================================================================
// Section 17.5: Random Numbers
// ============================================================================

TEST_CASE("random: uniform_int_distribution", "[ch17][random]") {
    std::mt19937 gen{42};  // Fixed seed for reproducibility
    std::uniform_int_distribution<> dist{1, 6};

    bool all_in_range = true;
    for (int i = 0; i < 100; ++i) {
        int val = dist(gen);
        if (val < 1 || val > 6) {
            all_in_range = false;
            break;
        }
    }

    REQUIRE(all_in_range);
}

TEST_CASE("random: uniform_real_distribution", "[ch17][random]") {
    std::mt19937 gen{42};
    std::uniform_real_distribution<> dist{0.0, 1.0};

    bool all_in_range = true;
    for (int i = 0; i < 100; ++i) {
        double val = dist(gen);
        if (val < 0.0 || val >= 1.0) {
            all_in_range = false;
            break;
        }
    }

    REQUIRE(all_in_range);
}

TEST_CASE("random: normal_distribution", "[ch17][random]") {
    std::mt19937 gen{42};
    std::normal_distribution<> dist{100.0, 15.0};

    double sum = 0.0;
    constexpr int N = 10000;
    for (int i = 0; i < N; ++i) {
        sum += dist(gen);
    }

    double mean = sum / N;
    REQUIRE_THAT(mean, WithinAbs(100.0, 1.0));  // Should be close to 100
}

TEST_CASE("random: bernoulli_distribution", "[ch17][random]") {
    std::mt19937 gen{42};
    std::bernoulli_distribution dist{0.7};

    int true_count = 0;
    constexpr int N = 10000;
    for (int i = 0; i < N; ++i) {
        if (dist(gen)) ++true_count;
    }

    double ratio = static_cast<double>(true_count) / N;
    REQUIRE_THAT(ratio, WithinAbs(0.7, 0.02));
}

TEST_CASE("random: deterministic seeding", "[ch17][random]") {
    std::mt19937 gen1{12345};
    std::mt19937 gen2{12345};

    // Same seed should produce same sequence
    for (int i = 0; i < 10; ++i) {
        REQUIRE(gen1() == gen2());
    }
}

// ============================================================================
// Section 17.7: Numeric Limits
// ============================================================================

TEST_CASE("limits: integer properties", "[ch17][limits]") {
    REQUIRE(std::numeric_limits<int>::is_integer);
    REQUIRE(std::numeric_limits<int>::is_signed);
    REQUIRE_FALSE(std::numeric_limits<unsigned int>::is_signed);
}

TEST_CASE("limits: floating-point properties", "[ch17][limits]") {
    REQUIRE_FALSE(std::numeric_limits<double>::is_integer);
    REQUIRE(std::numeric_limits<double>::is_signed);
    REQUIRE(std::numeric_limits<double>::has_infinity);
    REQUIRE(std::numeric_limits<double>::has_quiet_NaN);
}

TEST_CASE("limits: int range", "[ch17][limits]") {
    constexpr int max_int = std::numeric_limits<int>::max();
    constexpr int min_int = std::numeric_limits<int>::min();

    REQUIRE(max_int > 0);
    REQUIRE(min_int < 0);
    REQUIRE(max_int >= 2147483647);  // At least 32-bit
}

TEST_CASE("limits: special values", "[ch17][limits]") {
    double inf = std::numeric_limits<double>::infinity();
    double nan = std::numeric_limits<double>::quiet_NaN();

    REQUIRE(std::isinf(inf));
    REQUIRE(std::isinf(-inf));
    REQUIRE(std::isnan(nan));
    REQUIRE_FALSE(std::isfinite(inf));
    REQUIRE_FALSE(std::isfinite(nan));
    REQUIRE(std::isfinite(42.0));
}

TEST_CASE("limits: epsilon", "[ch17][limits]") {
    double eps = std::numeric_limits<double>::epsilon();

    // 1.0 + epsilon != 1.0
    REQUIRE(1.0 + eps != 1.0);

    // 1.0 + epsilon/2 == 1.0 (for typical implementations)
    REQUIRE(1.0 + eps / 2 == 1.0);
}

// ============================================================================
// Practical Applications
// ============================================================================

TEST_CASE("practical: floating-point comparison", "[ch17][practical]") {
    double x = 0.1 + 0.2;
    double y = 0.3;

    // Direct comparison may fail
    // REQUIRE(x == y);  // May fail!

    // Use epsilon comparison
    double eps = std::numeric_limits<double>::epsilon() * 100;
    REQUIRE(std::abs(x - y) < eps);
}

TEST_CASE("practical: safe midpoint", "[ch17][practical]") {
    // (a + b) / 2 can overflow
    int a = std::numeric_limits<int>::max();
    int b = std::numeric_limits<int>::max() - 2;

    // Unsafe: int mid = (a + b) / 2;  // Overflow!

    // Safe: use std::midpoint
    int mid = std::midpoint(a, b);
    REQUIRE(mid == std::numeric_limits<int>::max() - 1);
}
