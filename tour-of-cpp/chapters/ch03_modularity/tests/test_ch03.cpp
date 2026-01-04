// Chapter 3 Tests: Modularity
// Book reference: 3.2-3.4

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <string>
#include <vector>

// ============================================================================
// Section 3.2: Separate Compilation Concepts
// ============================================================================

// Simulating a header-defined class (normally would #include "myclass.h")
namespace separate_compilation {

// This demonstrates the pattern of header declarations
class Counter {
    int value_ = 0;

public:
    Counter() = default;
    explicit Counter(int initial) : value_{initial} {}

    void increment() { ++value_; }
    void decrement() { --value_; }
    [[nodiscard]] int get() const { return value_; }
    void reset() { value_ = 0; }
};

// Free function declarations (would be in header)
[[nodiscard]] int add(int a, int b);
[[nodiscard]] int multiply(int a, int b);

// Implementations (would be in .cpp file)
int add(int a, int b) { return a + b; }
int multiply(int a, int b) { return a * b; }

}  // namespace separate_compilation

TEST_CASE("Separate compilation: class usage", "[ch03][separate]") {
    using separate_compilation::Counter;

    Counter c;
    REQUIRE(c.get() == 0);

    c.increment();
    c.increment();
    REQUIRE(c.get() == 2);

    c.decrement();
    REQUIRE(c.get() == 1);

    Counter c2{100};
    REQUIRE(c2.get() == 100);
}

TEST_CASE("Separate compilation: free functions", "[ch03][separate]") {
    using namespace separate_compilation;

    REQUIRE(add(2, 3) == 5);
    REQUIRE(multiply(4, 5) == 20);
}

// ============================================================================
// Section 3.3: Namespaces
// ============================================================================

namespace physics {

struct Vector2D {
    double x = 0;
    double y = 0;
};

double magnitude(const Vector2D& v) {
    return std::sqrt(v.x * v.x + v.y * v.y);
}

Vector2D add(const Vector2D& a, const Vector2D& b) {
    return {a.x + b.x, a.y + b.y};
}

}  // namespace physics

namespace graphics {

struct Vector2D {  // Same name, different namespace
    float x = 0;
    float y = 0;
    float z = 0;  // Different structure
};

}  // namespace graphics

// Nested namespace (C++17 style)
namespace company::project::utils {

std::string to_upper(std::string s) {
    for (char& c : s) {
        c = static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
    }
    return s;
}

}  // namespace company::project::utils

TEST_CASE("Namespaces: prevent naming conflicts", "[ch03][namespace]") {
    // Same name Vector2D in different namespaces
    physics::Vector2D pv{3.0, 4.0};
    graphics::Vector2D gv{1.0f, 2.0f, 3.0f};

    REQUIRE(pv.x == 3.0);
    REQUIRE(gv.z == 3.0f);

    // They're completely different types
    static_assert(!std::is_same_v<physics::Vector2D, graphics::Vector2D>);
}

TEST_CASE("Namespaces: using declaration", "[ch03][namespace]") {
    using physics::Vector2D;
    using physics::magnitude;

    Vector2D v{3.0, 4.0};
    REQUIRE_THAT(magnitude(v), Catch::Matchers::WithinAbs(5.0, 0.001));
}

TEST_CASE("Namespaces: nested namespace access", "[ch03][namespace]") {
    REQUIRE(company::project::utils::to_upper("hello") == "HELLO");

    // Namespace alias
    namespace cpu = company::project::utils;
    REQUIRE(cpu::to_upper("world") == "WORLD");
}

TEST_CASE("Namespaces: namespace operations", "[ch03][namespace]") {
    physics::Vector2D a{1.0, 2.0};
    physics::Vector2D b{3.0, 4.0};

    auto sum = physics::add(a, b);
    REQUIRE(sum.x == 4.0);
    REQUIRE(sum.y == 6.0);
}

// Anonymous namespace test (internal linkage)
namespace {
    int internal_counter = 0;

    int get_and_increment() {
        return internal_counter++;
    }
}

TEST_CASE("Namespaces: anonymous namespace", "[ch03][namespace]") {
    internal_counter = 0;

    REQUIRE(get_and_increment() == 0);
    REQUIRE(get_and_increment() == 1);
    REQUIRE(get_and_increment() == 2);
}

// Inline namespace test (versioning)
namespace api {

inline namespace v2 {
    struct Config {
        int version = 2;
        std::string name;
    };
}

namespace v1 {
    struct Config {
        int version = 1;
    };
}

}  // namespace api

TEST_CASE("Namespaces: inline namespace versioning", "[ch03][namespace]") {
    // Default version (v2 via inline)
    api::Config current;
    REQUIRE(current.version == 2);

    // Explicit old version
    api::v1::Config old;
    REQUIRE(old.version == 1);

    // Explicit new version (same as default)
    api::v2::Config explicit_new;
    REQUIRE(explicit_new.version == 2);
}

// ============================================================================
// Section 3.4: Function Arguments
// ============================================================================

namespace args_test {

// Pass by value
int double_value(int x) {
    return x * 2;
}

// Pass by reference (modifies original)
void triple_in_place(int& x) {
    x *= 3;
}

// Pass by const reference (read-only, no copy)
int sum_vector(const std::vector<int>& v) {
    int total = 0;
    for (int x : v) total += x;
    return total;
}

// Pass by pointer (nullable)
bool try_divide(int a, int b, int* result) {
    if (b == 0 || result == nullptr) return false;
    *result = a / b;
    return true;
}

// Return by value (RVO applies)
std::vector<int> make_sequence(int n) {
    std::vector<int> result;
    for (int i = 0; i < n; ++i) {
        result.push_back(i);
    }
    return result;
}

// Move semantics
class Buffer {
    std::vector<int> data_;

public:
    Buffer() = default;

    void set_data(std::vector<int>&& d) {
        data_ = std::move(d);
    }

    [[nodiscard]] const std::vector<int>& data() const { return data_; }
    [[nodiscard]] std::size_t size() const { return data_.size(); }
};

}  // namespace args_test

TEST_CASE("Function args: pass by value", "[ch03][args]") {
    int x = 5;
    int result = args_test::double_value(x);

    REQUIRE(result == 10);
    REQUIRE(x == 5);  // Original unchanged
}

TEST_CASE("Function args: pass by reference", "[ch03][args]") {
    int x = 5;
    args_test::triple_in_place(x);

    REQUIRE(x == 15);  // Original modified
}

TEST_CASE("Function args: pass by const reference", "[ch03][args]") {
    std::vector<int> v{1, 2, 3, 4, 5};
    int sum = args_test::sum_vector(v);

    REQUIRE(sum == 15);
    REQUIRE(v.size() == 5);  // Original unchanged
}

TEST_CASE("Function args: pass by pointer", "[ch03][args]") {
    int result = 0;

    REQUIRE(args_test::try_divide(10, 2, &result));
    REQUIRE(result == 5);

    REQUIRE_FALSE(args_test::try_divide(10, 0, &result));  // Division by zero
    REQUIRE_FALSE(args_test::try_divide(10, 2, nullptr)); // Null pointer
}

TEST_CASE("Function args: return by value (RVO)", "[ch03][args]") {
    auto seq = args_test::make_sequence(5);

    REQUIRE(seq.size() == 5);
    REQUIRE(seq[0] == 0);
    REQUIRE(seq[4] == 4);
}

TEST_CASE("Function args: move semantics", "[ch03][args]") {
    args_test::Buffer buf;

    std::vector<int> data{1, 2, 3, 4, 5};
    buf.set_data(std::move(data));

    REQUIRE(buf.size() == 5);
    REQUIRE(buf.data()[2] == 3);

    // data is now in moved-from state (valid but unspecified)
    // We shouldn't rely on its contents, but size() should work
}

TEST_CASE("Function args: temporary as rvalue", "[ch03][args]") {
    args_test::Buffer buf;

    // Temporary vector binds directly to rvalue reference
    buf.set_data({10, 20, 30});

    REQUIRE(buf.size() == 3);
    REQUIRE(buf.data()[1] == 20);
}

// ============================================================================
// Comprehensive: Combining Concepts
// ============================================================================

namespace math_lib {

class Matrix {
    std::vector<double> data_;
    std::size_t rows_, cols_;

public:
    Matrix(std::size_t rows, std::size_t cols)
        : data_(rows * cols, 0.0), rows_{rows}, cols_{cols} {}

    double& at(std::size_t r, std::size_t c) {
        return data_[r * cols_ + c];
    }

    [[nodiscard]] double at(std::size_t r, std::size_t c) const {
        return data_[r * cols_ + c];
    }

    [[nodiscard]] std::size_t rows() const { return rows_; }
    [[nodiscard]] std::size_t cols() const { return cols_; }
};

// Free function in same namespace
[[nodiscard]] double trace(const Matrix& m) {
    double sum = 0;
    std::size_t n = std::min(m.rows(), m.cols());
    for (std::size_t i = 0; i < n; ++i) {
        sum += m.at(i, i);
    }
    return sum;
}

}  // namespace math_lib

TEST_CASE("Comprehensive: namespace + class + args", "[ch03][comprehensive]") {
    math_lib::Matrix m(3, 3);

    // Set diagonal elements
    m.at(0, 0) = 1.0;
    m.at(1, 1) = 2.0;
    m.at(2, 2) = 3.0;

    // Use free function (ADL could find it, but we qualify for clarity)
    double t = math_lib::trace(m);

    REQUIRE_THAT(t, Catch::Matchers::WithinAbs(6.0, 0.001));
}
