# Chapter 17: Numerics

## TL;DR

C++ provides robust numerical support through `<cmath>` for math functions, `<complex>` for complex arithmetic, `<random>` for quality random numbers, and `<limits>` for type properties. Always use `<random>` instead of `rand()` for serious applications. For floating-point comparisons, consider epsilon-based approaches rather than exact equality.

## Key Concepts

| Concept | One-liner | Example |
|---------|-----------|---------|
| `<cmath>` | Mathematical functions | `std::sqrt(2.0)`, `std::sin(x)` |
| `<complex>` | Complex number type | `std::complex<double> z{3.0, 4.0};` |
| `<random>` | Random number generation | `std::mt19937`, `std::uniform_int_distribution` |
| `<limits>` | Numeric type properties | `std::numeric_limits<int>::max()` |
| Random engine | Source of randomness | `std::mt19937`, `std::random_device` |
| Distribution | Shape randomness into values | `std::uniform_int_distribution<>`, `std::normal_distribution<>` |
| `std::lerp` | Linear interpolation (C++20) | `std::lerp(a, b, t)` |
| `std::midpoint` | Safe midpoint (C++20) | `std::midpoint(a, b)` |

## Things to Remember

- **Never use `rand()`**: Use `<random>` for quality random numbers.
- **Seed once**: Create one engine, seed it once, reuse it.
- **`std::random_device`**: Use for seeding, not for bulk random numbers (may be slow).
- **Distributions are stateful**: They may cache values; don't recreate them in loops.
- **Floating-point equality**: Use epsilon comparisons, not `==`.
- **`numeric_limits`**: Query type properties at compile time.
- **Complex numbers**: `std::complex<double>` is the standard choice.

## Modern C++ Highlights

| Feature | Old Style | Modern C++20 |
|---------|-----------|--------------|
| Random numbers | `rand() % n` | `uniform_int_distribution<>(0, n-1)(gen)` |
| Midpoint | `(a + b) / 2` (overflow!) | `std::midpoint(a, b)` |
| Linear interp | `a + t * (b - a)` | `std::lerp(a, b, t)` |
| Math constants | `#define PI 3.14159...` | `std::numbers::pi` (C++20) |
| Bit operations | Manual bit twiddling | `<bit>` header (C++20) |

## Common Pitfalls

- **Using `rand()`**:
  ```cpp
  // BAD: Poor quality, not thread-safe
  int r = rand() % 100;

  // GOOD: Use <random>
  std::mt19937 gen{std::random_device{}()};
  std::uniform_int_distribution<> dist{0, 99};
  int r = dist(gen);
  ```

- **Integer overflow in midpoint**:
  ```cpp
  int a = INT_MAX - 1;
  int b = INT_MAX;
  int mid = (a + b) / 2;  // OVERFLOW!
  int mid = std::midpoint(a, b);  // Safe
  ```

- **Floating-point equality**:
  ```cpp
  double x = 0.1 + 0.2;
  if (x == 0.3) { ... }  // May fail!

  // Better: epsilon comparison
  if (std::abs(x - 0.3) < 1e-9) { ... }
  ```

- **Creating distribution in loop**:
  ```cpp
  // BAD: Recreating distribution each iteration
  for (int i = 0; i < 1000; ++i) {
      std::uniform_int_distribution<> d{0, 99};
      int r = d(gen);
  }

  // GOOD: Create once, reuse
  std::uniform_int_distribution<> d{0, 99};
  for (int i = 0; i < 1000; ++i) {
      int r = d(gen);
  }
  ```

## Quick Reference

```cpp
#include <cmath>
#include <complex>
#include <limits>
#include <numbers>
#include <random>

// --- Math functions ---
std::sqrt(x);           // Square root
std::pow(base, exp);    // Power
std::exp(x);            // e^x
std::log(x);            // Natural log
std::log10(x);          // Log base 10
std::sin(x); std::cos(x); std::tan(x);  // Trigonometry
std::asin(x); std::acos(x); std::atan(x);
std::abs(x);            // Absolute value
std::floor(x);          // Round down
std::ceil(x);           // Round up
std::round(x);          // Round to nearest
std::fmod(x, y);        // Floating-point modulo
std::hypot(x, y);       // sqrt(x^2 + y^2) safely
std::lerp(a, b, t);     // Linear interpolation (C++20)
std::midpoint(a, b);    // Safe midpoint (C++20)

// --- Math constants (C++20) ---
std::numbers::pi;       // 3.14159...
std::numbers::e;        // 2.71828...
std::numbers::sqrt2;    // 1.41421...
std::numbers::ln2;      // 0.69314...

// --- Complex numbers ---
std::complex<double> z{3.0, 4.0};  // 3 + 4i
z.real();               // Real part (3.0)
z.imag();               // Imaginary part (4.0)
std::abs(z);            // Magnitude (5.0)
std::arg(z);            // Phase angle
std::conj(z);           // Conjugate (3 - 4i)
std::norm(z);           // |z|^2 (25.0)
z1 + z2; z1 * z2;       // Arithmetic operations
std::exp(z); std::log(z);  // Complex exp/log

// --- Random numbers ---
std::random_device rd;              // True randomness (for seeding)
std::mt19937 gen{rd()};             // Mersenne Twister engine
std::mt19937_64 gen64{rd()};        // 64-bit version

// Distributions
std::uniform_int_distribution<> int_dist{1, 100};      // [1, 100]
std::uniform_real_distribution<> real_dist{0.0, 1.0};  // [0, 1)
std::normal_distribution<> normal{mean, stddev};
std::bernoulli_distribution bernoulli{0.7};            // true 70% of time
std::poisson_distribution<> poisson{lambda};
std::exponential_distribution<> exp_dist{lambda};

int r = int_dist(gen);              // Generate random int
double d = real_dist(gen);          // Generate random double

// Shuffle a container
std::shuffle(v.begin(), v.end(), gen);

// --- Numeric limits ---
std::numeric_limits<int>::max();         // Maximum int value
std::numeric_limits<int>::min();         // Minimum int value
std::numeric_limits<int>::digits;        // Binary digits
std::numeric_limits<double>::epsilon();  // Smallest x where 1+x != 1
std::numeric_limits<double>::infinity(); // Infinity
std::numeric_limits<double>::quiet_NaN();// NaN
std::numeric_limits<float>::is_integer;  // false
std::numeric_limits<int>::is_signed;     // true

// Check for special values
std::isnan(x);          // Is NaN?
std::isinf(x);          // Is infinity?
std::isfinite(x);       // Is finite?
```

## Distribution Selection Guide

| Distribution | Use Case |
|--------------|----------|
| `uniform_int_distribution` | Equal chance for each integer in range |
| `uniform_real_distribution` | Equal chance for each real in range |
| `normal_distribution` | Bell curve (heights, test scores) |
| `bernoulli_distribution` | Yes/no with probability p |
| `binomial_distribution` | Number of successes in n trials |
| `poisson_distribution` | Events in fixed interval (rare events) |
| `exponential_distribution` | Time between events |
| `discrete_distribution` | Custom weights for outcomes |

## Book Sections Covered

- **17.1** Introduction
- **17.2** Mathematical Functions
- **17.3** Numerical Algorithms
- **17.4** Complex Numbers
- **17.5** Random Numbers
- **17.6** Vector Arithmetic
- **17.7** Numeric Limits
- **17.8** Type Aliases

## Exercises in This Chapter

| Exercise | Difficulty | Concepts Practiced |
|----------|------------|-------------------|
| ex01_random | ** | Random engines, distributions, shuffling |
| ex02_math | ** | Math functions, numeric limits |

## Running the Examples

```bash
# Build all chapter 17 examples
cmake --build build --target ch17_examples

# Run individual examples
./build/bin/ch17_math_functions
./build/bin/ch17_complex_numbers
./build/bin/ch17_random
./build/bin/ch17_numeric_limits

# Run chapter 17 tests
ctest --test-dir build -R ch17
```
