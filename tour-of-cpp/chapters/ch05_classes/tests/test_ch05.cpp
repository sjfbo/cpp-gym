// Chapter 5 Tests: Classes
// Book reference: 5.1-5.5

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <cmath>
#include <memory>
#include <numbers>
#include <string>
#include <vector>

// ============================================================================
// Section 5.2: Concrete Types
// ============================================================================

class Complex {
    double re;
    double im;

public:
    constexpr Complex(double r = 0, double i = 0) : re{r}, im{i} {}

    constexpr double real() const { return re; }
    constexpr double imag() const { return im; }

    constexpr Complex operator+(Complex z) const {
        return {re + z.re, im + z.im};
    }

    constexpr Complex operator-(Complex z) const {
        return {re - z.re, im - z.im};
    }

    constexpr Complex operator*(Complex z) const {
        return {re * z.re - im * z.im, re * z.im + im * z.re};
    }

    bool operator==(const Complex&) const = default;

    double magnitude() const { return std::sqrt(re * re + im * im); }
};

TEST_CASE("Concrete types: Complex arithmetic", "[ch05][concrete]") {
    Complex z1{3, 4};
    Complex z2{1, 2};

    SECTION("construction and access") {
        REQUIRE(z1.real() == 3);
        REQUIRE(z1.imag() == 4);
    }

    SECTION("addition") {
        Complex sum = z1 + z2;
        REQUIRE(sum.real() == 4);
        REQUIRE(sum.imag() == 6);
    }

    SECTION("subtraction") {
        Complex diff = z1 - z2;
        REQUIRE(diff.real() == 2);
        REQUIRE(diff.imag() == 2);
    }

    SECTION("multiplication") {
        Complex prod = z1 * z2;
        // (3+4i)(1+2i) = 3 + 6i + 4i + 8i^2 = 3 + 10i - 8 = -5 + 10i
        REQUIRE(prod.real() == -5);
        REQUIRE(prod.imag() == 10);
    }

    SECTION("magnitude") {
        REQUIRE(z1.magnitude() == 5.0);  // 3-4-5 triangle
    }

    SECTION("equality") {
        Complex z3{3, 4};
        REQUIRE(z1 == z3);
        REQUIRE_FALSE(z1 == z2);
    }
}

TEST_CASE("Concrete types: value semantics", "[ch05][concrete]") {
    Complex original{1, 2};
    Complex copy = original;  // Copy

    // Modifying copy doesn't affect original
    copy = copy + Complex{1, 1};

    REQUIRE(original.real() == 1);
    REQUIRE(original.imag() == 2);
    REQUIRE(copy.real() == 2);
    REQUIRE(copy.imag() == 3);
}

TEST_CASE("Concrete types: constexpr", "[ch05][concrete]") {
    constexpr Complex c1{1, 2};
    constexpr Complex c2{3, 4};
    constexpr Complex c3 = c1 + c2;

    static_assert(c3.real() == 4);
    static_assert(c3.imag() == 6);

    REQUIRE(c3.real() == 4);
}

// ============================================================================
// Section 5.3: Abstract Types
// ============================================================================

class Shape {
public:
    virtual ~Shape() = default;
    virtual double area() const = 0;
    virtual double perimeter() const = 0;
    virtual std::string name() const = 0;
};

class Circle : public Shape {
    double radius;

public:
    explicit Circle(double r) : radius{r} {}

    double area() const override {
        return std::numbers::pi * radius * radius;
    }

    double perimeter() const override {
        return 2 * std::numbers::pi * radius;
    }

    std::string name() const override { return "Circle"; }

    double get_radius() const { return radius; }
};

class Rectangle : public Shape {
    double width;
    double height;

public:
    Rectangle(double w, double h) : width{w}, height{h} {}

    double area() const override { return width * height; }

    double perimeter() const override { return 2 * (width + height); }

    std::string name() const override { return "Rectangle"; }
};

TEST_CASE("Abstract types: Shape interface", "[ch05][abstract]") {
    Circle c{5.0};
    Rectangle r{4.0, 6.0};

    SECTION("Circle") {
        REQUIRE(c.name() == "Circle");
        REQUIRE_THAT(c.area(), Catch::Matchers::WithinRel(78.54, 0.01));
        REQUIRE_THAT(c.perimeter(), Catch::Matchers::WithinRel(31.42, 0.01));
    }

    SECTION("Rectangle") {
        REQUIRE(r.name() == "Rectangle");
        REQUIRE(r.area() == 24.0);
        REQUIRE(r.perimeter() == 20.0);
    }
}

TEST_CASE("Abstract types: polymorphism", "[ch05][abstract]") {
    std::vector<std::unique_ptr<Shape>> shapes;
    shapes.push_back(std::make_unique<Circle>(1.0));
    shapes.push_back(std::make_unique<Rectangle>(2.0, 3.0));

    double total_area = 0;
    for (const auto& shape : shapes) {
        total_area += shape->area();
    }

    // pi + 6
    REQUIRE_THAT(total_area, Catch::Matchers::WithinRel(9.14, 0.01));
}

TEST_CASE("Abstract types: interface through reference", "[ch05][abstract]") {
    Circle c{2.0};
    const Shape& shape_ref = c;

    REQUIRE(shape_ref.name() == "Circle");
    REQUIRE_THAT(shape_ref.area(), Catch::Matchers::WithinRel(12.57, 0.01));
}

// ============================================================================
// Section 5.4: Virtual Functions
// ============================================================================

class Animal {
public:
    virtual ~Animal() = default;
    virtual std::string speak() const { return "..."; }
    std::string type() const { return "Animal"; }  // Non-virtual
};

class Dog : public Animal {
public:
    std::string speak() const override { return "Woof!"; }
    std::string type() const { return "Dog"; }  // Hides, not overrides
};

class Cat : public Animal {
public:
    std::string speak() const override { return "Meow!"; }
};

TEST_CASE("Virtual functions: dynamic dispatch", "[ch05][virtual]") {
    Dog dog;
    Cat cat;
    Animal* animals[] = {&dog, &cat};

    REQUIRE(animals[0]->speak() == "Woof!");
    REQUIRE(animals[1]->speak() == "Meow!");
}

TEST_CASE("Virtual functions: static vs dynamic binding", "[ch05][virtual]") {
    Dog dog;
    Animal* animal_ptr = &dog;

    // Virtual function - dynamic dispatch
    REQUIRE(animal_ptr->speak() == "Woof!");

    // Non-virtual function - static binding
    REQUIRE(animal_ptr->type() == "Animal");  // NOT "Dog"!

    // Direct call uses Dog's version
    REQUIRE(dog.type() == "Dog");
}

// Test override and final
class Widget {
public:
    virtual ~Widget() = default;
    virtual void draw() {}
    virtual void resize(int, int) {}
};

class Button : public Widget {
public:
    void draw() final {}  // Cannot be overridden further
    void resize(int, int) override {}
};

class IconButton : public Button {
public:
    // void draw() override {}  // Would fail: Button::draw() is final
    void resize(int, int) override {}
};

TEST_CASE("Virtual functions: final keyword", "[ch05][virtual]") {
    IconButton ib;
    Widget* w = &ib;

    // Just verify the hierarchy compiles and runs
    w->draw();
    w->resize(100, 100);
    REQUIRE(true);
}

// ============================================================================
// Section 5.5: Class Hierarchies
// ============================================================================

class Employee {
    std::string name_;
    int id_;

public:
    Employee(std::string name, int id) : name_{std::move(name)}, id_{id} {}
    virtual ~Employee() = default;

    const std::string& name() const { return name_; }
    int id() const { return id_; }

    virtual double monthly_pay() const = 0;
};

class SalariedEmployee : public Employee {
    double annual_salary_;

public:
    SalariedEmployee(std::string name, int id, double salary)
        : Employee{std::move(name), id}, annual_salary_{salary} {}

    double monthly_pay() const override { return annual_salary_ / 12; }
};

class HourlyEmployee : public Employee {
    double hourly_rate_;
    double hours_;

public:
    HourlyEmployee(std::string name, int id, double rate, double hours)
        : Employee{std::move(name), id}, hourly_rate_{rate}, hours_{hours} {}

    double monthly_pay() const override { return hourly_rate_ * hours_; }
};

TEST_CASE("Class hierarchies: inheritance", "[ch05][hierarchy]") {
    SalariedEmployee alice{"Alice", 1, 60000};
    HourlyEmployee bob{"Bob", 2, 25, 160};

    REQUIRE(alice.name() == "Alice");
    REQUIRE(alice.id() == 1);
    REQUIRE(alice.monthly_pay() == 5000);

    REQUIRE(bob.name() == "Bob");
    REQUIRE(bob.monthly_pay() == 4000);
}

TEST_CASE("Class hierarchies: polymorphic collection", "[ch05][hierarchy]") {
    std::vector<std::unique_ptr<Employee>> employees;
    employees.push_back(std::make_unique<SalariedEmployee>("A", 1, 60000));
    employees.push_back(std::make_unique<HourlyEmployee>("B", 2, 25, 160));

    double total = 0;
    for (const auto& emp : employees) {
        total += emp->monthly_pay();
    }

    REQUIRE(total == 9000);
}

// Object slicing test
class Base {
protected:
    int value_;

public:
    explicit Base(int v = 0) : value_{v} {}
    virtual ~Base() = default;
    virtual int get_value() const { return value_; }
};

class Derived : public Base {
    int extra_;

public:
    Derived(int v, int e) : Base{v}, extra_{e} {}
    int get_value() const override { return value_ + extra_; }
    int get_extra() const { return extra_; }
};

TEST_CASE("Class hierarchies: object slicing", "[ch05][hierarchy]") {
    Derived d{10, 5};

    // Through reference - no slicing, virtual dispatch works
    const Base& ref = d;
    REQUIRE(ref.get_value() == 15);

    // Copied to base - SLICING occurs
    Base b = d;
    REQUIRE(b.get_value() == 10);  // Only Base::get_value(), extra_ is lost
}

// Multiple inheritance
class Printable {
public:
    virtual ~Printable() = default;
    virtual std::string to_string() const = 0;
};

class Drawable {
public:
    virtual ~Drawable() = default;
    virtual void draw() const = 0;
};

class TextBox : public Printable, public Drawable {
    std::string text_;

public:
    explicit TextBox(std::string text) : text_{std::move(text)} {}

    std::string to_string() const override { return "TextBox: " + text_; }
    void draw() const override {}
};

TEST_CASE("Class hierarchies: multiple inheritance", "[ch05][hierarchy]") {
    TextBox tb{"Hello"};

    // Can use as Printable
    const Printable& p = tb;
    REQUIRE(p.to_string() == "TextBox: Hello");

    // Can use as Drawable
    const Drawable& d = tb;
    d.draw();  // Just verify it compiles

    REQUIRE(true);
}

// ============================================================================
// Edge cases and common pitfalls
// ============================================================================

TEST_CASE("Pitfall: virtual destructor necessity", "[ch05][pitfall]") {
    // This test verifies proper cleanup through base pointer
    // With virtual destructor, derived destructor is called

    bool derived_destroyed = false;

    class TestBase {
    public:
        virtual ~TestBase() = default;
    };

    class TestDerived : public TestBase {
        bool& flag_;

    public:
        explicit TestDerived(bool& flag) : flag_{flag} {}
        ~TestDerived() override { flag_ = true; }
    };

    {
        std::unique_ptr<TestBase> ptr = std::make_unique<TestDerived>(derived_destroyed);
    }  // ptr goes out of scope

    REQUIRE(derived_destroyed);  // Derived destructor was called
}

TEST_CASE("Pitfall: hiding vs overriding", "[ch05][pitfall]") {
    class Parent {
    public:
        virtual ~Parent() = default;
        virtual void foo(int) {}
        void bar(int) {}  // Non-virtual
    };

    class Child : public Parent {
    public:
        // void foo(double) override {}  // Would fail: signature mismatch
        void foo(int) override {}  // Correct: matches exactly

        void bar(int) {}  // Hides Parent::bar, doesn't override
    };

    Child c;
    Parent* p = &c;

    // Virtual function - uses Child's version
    p->foo(1);

    // Non-virtual - always uses Parent's version through Parent*
    p->bar(1);

    REQUIRE(true);  // Just verify compilation
}
