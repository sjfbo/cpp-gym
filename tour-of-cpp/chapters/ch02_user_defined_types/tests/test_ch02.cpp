// Chapter 2 Tests: User-Defined Types

#include <catch2/catch_test_macros.hpp>
#include <string>
#include <variant>

// ============================================================================
// Section 2.2: Structures
// ============================================================================

struct Point {
    double x = 0;
    double y = 0;

    double distance_from_origin() const {
        return std::sqrt(x * x + y * y);
    }
};

struct Rectangle {
    double width = 0;
    double height = 0;

    Rectangle() = default;
    Rectangle(double w, double h) : width{w}, height{h} {}

    double area() const { return width * height; }
    double perimeter() const { return 2 * (width + height); }
};

TEST_CASE("Structures: basic aggregate", "[ch02][struct]") {
    Point p1;
    REQUIRE(p1.x == 0);
    REQUIRE(p1.y == 0);

    Point p2{3.0, 4.0};
    REQUIRE(p2.x == 3.0);
    REQUIRE(p2.y == 4.0);
}

TEST_CASE("Structures: member functions", "[ch02][struct]") {
    Point p{3.0, 4.0};
    REQUIRE(p.distance_from_origin() == 5.0);
}

TEST_CASE("Structures: with constructors", "[ch02][struct]") {
    Rectangle r1;
    REQUIRE(r1.area() == 0);

    Rectangle r2{10, 5};
    REQUIRE(r2.area() == 50);
    REQUIRE(r2.perimeter() == 30);
}

// ============================================================================
// Section 2.3: Classes
// ============================================================================

class Counter {
    int value = 0;

public:
    Counter() = default;
    explicit Counter(int initial) : value{initial} {}

    void increment() { ++value; }
    void decrement() { --value; }
    int get() const { return value; }
    void reset() { value = 0; }
};

class BoundedCounter {
    int value;
    int min_val;
    int max_val;

public:
    BoundedCounter(int min, int max, int initial = 0)
        : value{initial}, min_val{min}, max_val{max} {
        if (value < min_val) value = min_val;
        if (value > max_val) value = max_val;
    }

    void increment() {
        if (value < max_val) ++value;
    }

    void decrement() {
        if (value > min_val) --value;
    }

    int get() const { return value; }
    int min() const { return min_val; }
    int max() const { return max_val; }
};

TEST_CASE("Classes: basic counter", "[ch02][class]") {
    Counter c;
    REQUIRE(c.get() == 0);

    c.increment();
    c.increment();
    REQUIRE(c.get() == 2);

    c.decrement();
    REQUIRE(c.get() == 1);

    c.reset();
    REQUIRE(c.get() == 0);
}

TEST_CASE("Classes: counter with initial value", "[ch02][class]") {
    Counter c{10};
    REQUIRE(c.get() == 10);

    c.increment();
    REQUIRE(c.get() == 11);
}

TEST_CASE("Classes: bounded counter invariant", "[ch02][class]") {
    BoundedCounter bc{0, 5, 3};
    REQUIRE(bc.get() == 3);

    bc.increment();
    bc.increment();
    REQUIRE(bc.get() == 5);

    bc.increment();  // Should not exceed max
    REQUIRE(bc.get() == 5);

    bc.decrement();
    bc.decrement();
    bc.decrement();
    bc.decrement();
    bc.decrement();
    REQUIRE(bc.get() == 0);

    bc.decrement();  // Should not go below min
    REQUIRE(bc.get() == 0);
}

// ============================================================================
// Section 2.4: Enumerations
// ============================================================================

enum class Color { Red, Green, Blue };

enum class HttpStatus { Ok = 200, NotFound = 404, InternalError = 500 };

enum class Priority : int { Low = -1, Normal = 0, High = 1 };

TEST_CASE("Enumerations: enum class basics", "[ch02][enum]") {
    Color c = Color::Red;
    REQUIRE(c == Color::Red);
    REQUIRE(c != Color::Blue);

    c = Color::Green;
    REQUIRE(c == Color::Green);
}

TEST_CASE("Enumerations: explicit values", "[ch02][enum]") {
    REQUIRE(static_cast<int>(HttpStatus::Ok) == 200);
    REQUIRE(static_cast<int>(HttpStatus::NotFound) == 404);
    REQUIRE(static_cast<int>(HttpStatus::InternalError) == 500);
}

TEST_CASE("Enumerations: underlying type", "[ch02][enum]") {
    static_assert(std::is_same_v<std::underlying_type_t<Priority>, int>);

    REQUIRE(static_cast<int>(Priority::Low) == -1);
    REQUIRE(static_cast<int>(Priority::Normal) == 0);
    REQUIRE(static_cast<int>(Priority::High) == 1);
}

TEST_CASE("Enumerations: switch statement", "[ch02][enum]") {
    auto color_name = [](Color c) -> std::string {
        switch (c) {
            case Color::Red:
                return "red";
            case Color::Green:
                return "green";
            case Color::Blue:
                return "blue";
        }
        return "unknown";
    };

    REQUIRE(color_name(Color::Red) == "red");
    REQUIRE(color_name(Color::Green) == "green");
    REQUIRE(color_name(Color::Blue) == "blue");
}

// ============================================================================
// Section 2.5: Unions and std::variant
// ============================================================================

TEST_CASE("std::variant: basic usage", "[ch02][variant]") {
    std::variant<int, double, std::string> v;

    v = 42;
    REQUIRE(std::holds_alternative<int>(v));
    REQUIRE(std::get<int>(v) == 42);

    v = 3.14;
    REQUIRE(std::holds_alternative<double>(v));
    REQUIRE(std::get<double>(v) == 3.14);

    v = std::string{"hello"};
    REQUIRE(std::holds_alternative<std::string>(v));
    REQUIRE(std::get<std::string>(v) == "hello");
}

TEST_CASE("std::variant: index and get_if", "[ch02][variant]") {
    std::variant<int, double, std::string> v = 42;

    REQUIRE(v.index() == 0);  // int is at index 0

    auto* pi = std::get_if<int>(&v);
    REQUIRE(pi != nullptr);
    REQUIRE(*pi == 42);

    auto* pd = std::get_if<double>(&v);
    REQUIRE(pd == nullptr);
}

TEST_CASE("std::variant: visit", "[ch02][variant]") {
    std::variant<int, double> v = 42;

    auto result = std::visit(
        [](auto&& arg) -> double {
            return static_cast<double>(arg) * 2;
        },
        v);

    REQUIRE(result == 84.0);

    v = 3.14;
    result = std::visit(
        [](auto&& arg) -> double {
            return static_cast<double>(arg) * 2;
        },
        v);

    REQUIRE(result == 6.28);
}

TEST_CASE("std::variant: bad_variant_access", "[ch02][variant]") {
    std::variant<int, double> v = 42;

    REQUIRE_THROWS_AS(std::get<double>(v), std::bad_variant_access);
}
