// Abstract Types - Interfaces and Runtime Polymorphism
// Book reference: 5.3 Abstract Types
//
// Abstract types:
// - Define interfaces with pure virtual functions (= 0)
// - Cannot be instantiated directly
// - Accessed through pointers or references
// - Enable runtime polymorphism (different implementations, same interface)
// - MUST have virtual destructor

#include <cmath>
#include <iostream>
#include <memory>
#include <numbers>
#include <vector>

// ----------------------------------------------------------------------------
// Container Interface (Abstract Type)
// Book reference: 5.3 Abstract Types
// ----------------------------------------------------------------------------

class Container {
public:
    // Virtual destructor is ESSENTIAL for abstract types
    // Without it, deleting through base pointer is undefined behavior
    virtual ~Container() = default;

    // Pure virtual functions define the interface
    // = 0 means "must be implemented by derived classes"
    virtual double& operator[](int index) = 0;
    virtual const double& operator[](int index) const = 0;
    virtual int size() const = 0;

    // Non-pure virtual function with default implementation
    virtual bool empty() const { return size() == 0; }
};

// Concrete implementation: Vector-based container
class VectorContainer : public Container {
    std::vector<double> elements;

public:
    VectorContainer() = default;

    explicit VectorContainer(std::initializer_list<double> init)
        : elements{init} {}

    explicit VectorContainer(int n, double value = 0.0)
        : elements(static_cast<std::size_t>(n), value) {}

    // Implementing the interface
    double& operator[](int index) override {
        return elements.at(static_cast<std::size_t>(index));  // at() provides bounds checking
    }

    const double& operator[](int index) const override {
        return elements.at(static_cast<std::size_t>(index));
    }

    int size() const override {
        return static_cast<int>(elements.size());
    }

    // Additional functionality specific to this implementation
    void push_back(double value) {
        elements.push_back(value);
    }
};

// Another implementation: Fixed-size array container
class ArrayContainer : public Container {
    static constexpr int max_size = 100;
    double elements[max_size];
    int count = 0;

public:
    ArrayContainer() = default;

    explicit ArrayContainer(std::initializer_list<double> init) {
        for (double v : init) {
            if (count < max_size) {
                elements[count++] = v;
            }
        }
    }

    double& operator[](int index) override {
        if (index < 0 || index >= count) {
            throw std::out_of_range("ArrayContainer index out of range");
        }
        return elements[index];
    }

    const double& operator[](int index) const override {
        if (index < 0 || index >= count) {
            throw std::out_of_range("ArrayContainer index out of range");
        }
        return elements[index];
    }

    int size() const override { return count; }
};

// Function that works with ANY Container implementation
void print_container(const Container& c) {
    std::cout << "[";
    for (int i = 0; i < c.size(); ++i) {
        if (i > 0) std::cout << ", ";
        std::cout << c[i];
    }
    std::cout << "]";
}

double sum_container(const Container& c) {
    double total = 0;
    for (int i = 0; i < c.size(); ++i) {
        total += c[i];
    }
    return total;
}

// ----------------------------------------------------------------------------
// Shape Interface (Classic polymorphism example)
// Book reference: 5.3 Abstract Types
// ----------------------------------------------------------------------------

class Shape {
public:
    virtual ~Shape() = default;

    // Pure virtual - every shape must implement these
    virtual double area() const = 0;
    virtual double perimeter() const = 0;
    virtual void draw() const = 0;

    // Virtual with default implementation
    virtual std::string name() const { return "Shape"; }
};

class Circle : public Shape {
    double radius;

public:
    explicit Circle(double r) : radius{r} {
        if (r <= 0) throw std::invalid_argument("Radius must be positive");
    }

    double area() const override {
        return std::numbers::pi * radius * radius;
    }

    double perimeter() const override {
        return 2 * std::numbers::pi * radius;
    }

    void draw() const override {
        std::cout << "Drawing Circle with radius " << radius << "\n";
    }

    std::string name() const override { return "Circle"; }

    // Circle-specific method
    double get_radius() const { return radius; }
};

class Rectangle : public Shape {
    double width;
    double height;

public:
    Rectangle(double w, double h) : width{w}, height{h} {
        if (w <= 0 || h <= 0) {
            throw std::invalid_argument("Dimensions must be positive");
        }
    }

    double area() const override {
        return width * height;
    }

    double perimeter() const override {
        return 2 * (width + height);
    }

    void draw() const override {
        std::cout << "Drawing Rectangle " << width << "x" << height << "\n";
    }

    std::string name() const override { return "Rectangle"; }
};

class Triangle : public Shape {
    double a, b, c;  // Three sides

public:
    Triangle(double side_a, double side_b, double side_c)
        : a{side_a}, b{side_b}, c{side_c} {
        // Triangle inequality: sum of any two sides > third side
        if (a + b <= c || b + c <= a || a + c <= b) {
            throw std::invalid_argument("Invalid triangle sides");
        }
    }

    double area() const override {
        // Heron's formula
        double s = (a + b + c) / 2;
        return std::sqrt(s * (s - a) * (s - b) * (s - c));
    }

    double perimeter() const override {
        return a + b + c;
    }

    void draw() const override {
        std::cout << "Drawing Triangle with sides " << a << ", " << b << ", " << c << "\n";
    }

    std::string name() const override { return "Triangle"; }
};

// Polymorphic function - works with any Shape
void describe_shape(const Shape& s) {
    std::cout << s.name() << ": area=" << s.area()
              << ", perimeter=" << s.perimeter() << "\n";
}

// ----------------------------------------------------------------------------
// Drawable Interface (Multiple interfaces pattern)
// ----------------------------------------------------------------------------

class Drawable {
public:
    virtual ~Drawable() = default;
    virtual void render() const = 0;
};

class Printable {
public:
    virtual ~Printable() = default;
    virtual void print(std::ostream& os) const = 0;
};

// A class can implement multiple interfaces
class TextBox : public Drawable, public Printable {
    std::string text;
    int x, y;

public:
    TextBox(std::string t, int px, int py) : text{std::move(t)}, x{px}, y{py} {}

    void render() const override {
        std::cout << "Rendering TextBox at (" << x << "," << y << ")\n";
    }

    void print(std::ostream& os) const override {
        os << "TextBox[\"" << text << "\" at (" << x << "," << y << ")]";
    }
};

// ----------------------------------------------------------------------------
// Main demonstration
// ----------------------------------------------------------------------------

int main() {
    std::cout << "=== Container Interface ===\n";
    std::cout << "Book reference: 5.3 Abstract Types\n\n";

    // Using different implementations through the same interface
    VectorContainer vc{1.0, 2.0, 3.0, 4.0, 5.0};
    ArrayContainer ac{10.0, 20.0, 30.0};

    std::cout << "VectorContainer: ";
    print_container(vc);
    std::cout << " sum=" << sum_container(vc) << "\n";

    std::cout << "ArrayContainer: ";
    print_container(ac);
    std::cout << " sum=" << sum_container(ac) << "\n";

    // Polymorphism through pointer
    Container* containers[] = {&vc, &ac};
    std::cout << "\nIterating through Container pointers:\n";
    for (const Container* c : containers) {
        std::cout << "  size=" << c->size() << ", empty=" << std::boolalpha << c->empty() << "\n";
    }

    std::cout << "\n=== Shape Interface ===\n\n";

    // Create shapes using smart pointers (modern C++ practice)
    std::vector<std::unique_ptr<Shape>> shapes;
    shapes.push_back(std::make_unique<Circle>(5.0));
    shapes.push_back(std::make_unique<Rectangle>(4.0, 6.0));
    shapes.push_back(std::make_unique<Triangle>(3.0, 4.0, 5.0));

    // Polymorphic iteration
    for (const auto& shape : shapes) {
        describe_shape(*shape);
        shape->draw();
        std::cout << "\n";
    }

    // Calculate total area of all shapes
    double total_area = 0;
    for (const auto& shape : shapes) {
        total_area += shape->area();
    }
    std::cout << "Total area of all shapes: " << total_area << "\n";

    std::cout << "\n=== Multiple Interfaces ===\n\n";

    TextBox tb{"Hello, World!", 100, 50};

    // Use as Drawable
    const Drawable& drawable = tb;
    drawable.render();

    // Use as Printable
    const Printable& printable = tb;
    printable.print(std::cout);
    std::cout << "\n";

    std::cout << "\n=== Why Virtual Destructor Matters ===\n";
    std::cout << "When deleting through base pointer, virtual destructor ensures\n";
    std::cout << "the derived class destructor is called first.\n";
    std::cout << "Without it: undefined behavior, resource leaks, corruption.\n";

    // This is safe because Shape has virtual destructor
    Shape* shape_ptr = new Circle(3.0);
    delete shape_ptr;  // Correctly calls Circle destructor, then Shape destructor
    std::cout << "Safely deleted Circle through Shape pointer.\n";

    std::cout << "\n=== Key Points ===\n";
    std::cout << "1. Abstract types define interfaces with pure virtual functions (= 0)\n";
    std::cout << "2. Cannot instantiate abstract types directly\n";
    std::cout << "3. Access through pointers or references for polymorphism\n";
    std::cout << "4. ALWAYS declare virtual destructor in base classes\n";
    std::cout << "5. Use std::unique_ptr for ownership of polymorphic objects\n";

    return 0;
}
