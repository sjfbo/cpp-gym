// Structures - Grouping Related Data
// Book reference: 2.2 Structures
//
// A struct is an aggregate of elements (members) of potentially different types.
// Members are public by default. Use struct for simple data aggregates.

#include <cmath>
#include <iostream>
#include <string>

// ----------------------------------------------------------------------------
// Basic structure
// ----------------------------------------------------------------------------

struct Point {
    double x;
    double y;
};

// ----------------------------------------------------------------------------
// Structure with member functions
// ----------------------------------------------------------------------------

struct Vector2D {
    double x = 0.0;  // In-class member initializer (C++11)
    double y = 0.0;

    // Member function
    double length() const {
        return std::sqrt(x * x + y * y);
    }

    // Member function that modifies the object
    void normalize() {
        double len = length();
        if (len > 0) {
            x /= len;
            y /= len;
        }
    }

    // Operator overloading as member function
    Vector2D operator+(const Vector2D& other) const {
        return {x + other.x, y + other.y};
    }

    Vector2D operator*(double scalar) const {
        return {x * scalar, y * scalar};
    }
};

// ----------------------------------------------------------------------------
// Structure with constructor
// ----------------------------------------------------------------------------

struct Rectangle {
    double width;
    double height;

    // Default constructor
    Rectangle() : width{0}, height{0} {}

    // Parameterized constructor
    Rectangle(double w, double h) : width{w}, height{h} {}

    double area() const {
        return width * height;
    }

    double perimeter() const {
        return 2 * (width + height);
    }
};

// ----------------------------------------------------------------------------
// Nested structures
// ----------------------------------------------------------------------------

struct Circle {
    Point center;  // Nested struct
    double radius;

    double area() const {
        return 3.14159265359 * radius * radius;
    }

    bool contains(const Point& p) const {
        double dx = p.x - center.x;
        double dy = p.y - center.y;
        return (dx * dx + dy * dy) <= (radius * radius);
    }
};

// ----------------------------------------------------------------------------
// Structure with string member
// ----------------------------------------------------------------------------

struct Person {
    std::string name;
    int age;

    void greet() const {
        std::cout << "Hello, I'm " << name << " and I'm " << age << " years old.\n";
    }
};

// ----------------------------------------------------------------------------
// Main demonstration
// ----------------------------------------------------------------------------

int main() {
    std::cout << "=== Basic Structure ===\n";

    // Different initialization styles
    Point p1;            // Default (uninitialized for built-in types!)
    Point p2 = {3.0, 4.0};  // Aggregate initialization
    Point p3{1.0, 2.0};     // List initialization (preferred)

    std::cout << "p2 = (" << p2.x << ", " << p2.y << ")\n";
    std::cout << "p3 = (" << p3.x << ", " << p3.y << ")\n";

    // Access and modify members
    p1.x = 5.0;
    p1.y = 12.0;
    std::cout << "p1 = (" << p1.x << ", " << p1.y << ")\n";

    std::cout << "\n=== Structure with Member Functions ===\n";

    Vector2D v1{3.0, 4.0};
    std::cout << "v1 = (" << v1.x << ", " << v1.y << ")\n";
    std::cout << "v1.length() = " << v1.length() << "\n";

    v1.normalize();
    std::cout << "After normalize: v1 = (" << v1.x << ", " << v1.y << ")\n";
    std::cout << "v1.length() = " << v1.length() << " (should be 1)\n";

    // Using operator overloading
    Vector2D v2{1.0, 0.0};
    Vector2D v3 = v1 + v2;
    std::cout << "v1 + v2 = (" << v3.x << ", " << v3.y << ")\n";

    Vector2D v4 = v2 * 5.0;
    std::cout << "v2 * 5 = (" << v4.x << ", " << v4.y << ")\n";

    std::cout << "\n=== Structure with Constructor ===\n";

    Rectangle r1;           // Default constructor
    Rectangle r2{10, 5};    // Parameterized constructor
    Rectangle r3(3.0, 4.0); // Alternative syntax

    std::cout << "r1: " << r1.width << " x " << r1.height << ", area = " << r1.area() << "\n";
    std::cout << "r2: " << r2.width << " x " << r2.height << ", area = " << r2.area() << "\n";
    std::cout << "r3: " << r3.width << " x " << r3.height << ", area = " << r3.area() << "\n";

    std::cout << "\n=== Nested Structure ===\n";

    Circle c{{0, 0}, 5.0};  // Center at origin, radius 5
    std::cout << "Circle area: " << c.area() << "\n";

    Point test1{3, 4};  // Inside (distance = 5)
    Point test2{6, 0};  // Outside (distance = 6)

    std::cout << "Point (3,4) inside circle: " << std::boolalpha << c.contains(test1) << "\n";
    std::cout << "Point (6,0) inside circle: " << c.contains(test2) << "\n";

    std::cout << "\n=== Structure with String ===\n";

    Person alice{"Alice", 30};
    Person bob{"Bob", 25};

    alice.greet();
    bob.greet();

    std::cout << "\n=== Designated Initializers (C++20) ===\n";

    // C++20 designated initializers (like C99)
    Point dp{.x = 10.0, .y = 20.0};
    std::cout << "Designated init: (" << dp.x << ", " << dp.y << ")\n";

    // Note: Rectangle has user-defined constructors so it's not an aggregate
    // Use constructor syntax instead
    Rectangle dr{0, 100};  // width = 0, height = 100
    std::cout << "Partial designated: " << dr.width << " x " << dr.height << "\n";

    return 0;
}
