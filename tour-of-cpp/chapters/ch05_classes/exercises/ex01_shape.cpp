// Exercise 5.1: Shape Hierarchy
// Book reference: 5.3-5.4 Abstract Types and Virtual Functions
// Difficulty: **
//
// Instructions:
// Implement a Shape hierarchy with Circle and Rectangle classes.
//
// 1. Complete the Shape abstract base class:
//    - Virtual destructor
//    - Pure virtual: area(), perimeter(), name()
//    - Virtual: describe() that prints name, area, and perimeter
//
// 2. Implement Circle class:
//    - Constructor taking radius
//    - Implement all pure virtual functions
//    - area = pi * r^2
//    - perimeter = 2 * pi * r
//
// 3. Implement Rectangle class:
//    - Constructor taking width and height
//    - Implement all pure virtual functions
//    - area = width * height
//    - perimeter = 2 * (width + height)
//
// 4. Implement Square as a special Rectangle (width == height)
//
// Expected output:
//   Circle: area=78.5398, perimeter=31.4159
//   Rectangle: area=24, perimeter=20
//   Square: area=25, perimeter=20
//   Total area: 127.54

#include <cmath>
#include <iostream>
#include <memory>
#include <numbers>
#include <vector>

// ----------------------------------------------------------------------------
// TODO: Complete the Shape base class
// ----------------------------------------------------------------------------

class Shape {
public:
    // TODO: Virtual destructor

    // TODO: Pure virtual functions
    // virtual double area() const = 0;
    // virtual double perimeter() const = 0;
    // virtual std::string name() const = 0;

    // TODO: Virtual describe() function that prints shape info
    // Default implementation should print: name(): area=X, perimeter=Y
};

// ----------------------------------------------------------------------------
// TODO: Implement Circle class
// ----------------------------------------------------------------------------

class Circle /* : public Shape */ {
    // TODO: Add radius member

public:
    // TODO: Constructor taking radius

    // TODO: Override area() - returns pi * r * r

    // TODO: Override perimeter() - returns 2 * pi * r

    // TODO: Override name() - returns "Circle"

    // Optional: Add radius() accessor
};

// ----------------------------------------------------------------------------
// TODO: Implement Rectangle class
// ----------------------------------------------------------------------------

class Rectangle /* : public Shape */ {
    // TODO: Add width and height members

public:
    // TODO: Constructor taking width and height

    // TODO: Override area() - returns width * height

    // TODO: Override perimeter() - returns 2 * (width + height)

    // TODO: Override name() - returns "Rectangle"

    // Optional: Add width() and height() accessors
};

// ----------------------------------------------------------------------------
// TODO: Implement Square class (inherits from Rectangle)
// Hint: A square is a rectangle where width == height
// ----------------------------------------------------------------------------

class Square /* : public Rectangle */ {
public:
    // TODO: Constructor taking side length
    // Hint: Initialize Rectangle with (side, side)

    // TODO: Override name() - returns "Square"
};

// ----------------------------------------------------------------------------
// Utility function - calculates total area of shapes
// ----------------------------------------------------------------------------

// TODO: Implement this function
// double total_area(const std::vector<std::unique_ptr<Shape>>& shapes) {
//     double sum = 0;
//     for (const auto& shape : shapes) {
//         sum += shape->area();
//     }
//     return sum;
// }

// ----------------------------------------------------------------------------
// Main - uncomment after implementing the classes
// ----------------------------------------------------------------------------

int main() {
    std::cout << "Exercise 5.1: Shape Hierarchy\n";
    std::cout << "==============================\n\n";

    // Uncomment after implementing:

    // std::vector<std::unique_ptr<Shape>> shapes;
    //
    // shapes.push_back(std::make_unique<Circle>(5.0));
    // shapes.push_back(std::make_unique<Rectangle>(4.0, 6.0));
    // shapes.push_back(std::make_unique<Square>(5.0));
    //
    // for (const auto& shape : shapes) {
    //     shape->describe();
    // }
    //
    // std::cout << "\nTotal area: " << total_area(shapes) << "\n";

    std::cout << "TODO: Implement Shape, Circle, Rectangle, and Square classes\n";

    return 0;
}

// ----------------------------------------------------------------------------
// Bonus challenges:
// ----------------------------------------------------------------------------
//
// 1. Add a Triangle class using Heron's formula:
//    area = sqrt(s*(s-a)*(s-b)*(s-c)) where s = (a+b+c)/2
//
// 2. Add a clone() method using covariant return types:
//    virtual Shape* clone() const = 0;
//    Circle* clone() const override { return new Circle(*this); }
//
// 3. Add a scale(double factor) method that scales the shape
//
// 4. Add comparison operators based on area
