// Virtual Functions - Polymorphism and Dynamic Dispatch
// Book reference: 5.4 Virtual Functions
//
// Virtual functions enable runtime polymorphism:
// - The actual function called depends on the object's dynamic type
// - Implemented via vtable (virtual function table)
// - Use 'override' to explicitly mark overriding functions
// - Use 'final' to prevent further overriding

#include <iostream>
#include <memory>
#include <string>
#include <vector>

// ----------------------------------------------------------------------------
// Basic Virtual Function Mechanics
// Book reference: 5.4 Virtual Functions
// ----------------------------------------------------------------------------

class Animal {
public:
    virtual ~Animal() = default;

    // Virtual function - can be overridden
    virtual std::string speak() const {
        return "...";  // Default implementation
    }

    // Non-virtual function - cannot be overridden (statically bound)
    std::string type() const {
        return "Animal";
    }
};

class Dog : public Animal {
public:
    // 'override' ensures we're actually overriding a virtual function
    // If signature doesn't match, compiler will error
    std::string speak() const override {
        return "Woof!";
    }

    // This HIDES Animal::type(), doesn't override (type() is non-virtual)
    std::string type() const {
        return "Dog";
    }
};

class Cat : public Animal {
public:
    std::string speak() const override {
        return "Meow!";
    }
};

// Demonstrate virtual dispatch
void make_speak(const Animal& a) {
    // Virtual dispatch: calls the appropriate speak() based on actual type
    std::cout << "Speaking: " << a.speak() << "\n";

    // Static binding: always calls Animal::type()
    std::cout << "Type (static): " << a.type() << "\n";
}

// ----------------------------------------------------------------------------
// The 'override' Keyword (C++11)
// Book reference: 5.4 Virtual Functions
// ----------------------------------------------------------------------------

class Base {
public:
    virtual ~Base() = default;

    virtual void foo([[maybe_unused]] int x) { std::cout << "Base::foo(int)\n"; }
    virtual void bar() const { std::cout << "Base::bar() const\n"; }
    virtual void baz() { std::cout << "Base::baz()\n"; }
};

class DerivedCorrect : public Base {
public:
    // These correctly override base class functions
    void foo([[maybe_unused]] int x) override { std::cout << "DerivedCorrect::foo(int)\n"; }
    void bar() const override { std::cout << "DerivedCorrect::bar() const\n"; }
    void baz() override { std::cout << "DerivedCorrect::baz()\n"; }
};

// class DerivedBroken : public Base {
// public:
//     // These would FAIL to compile with 'override':
//
//     // void foo(double x) override { }  // ERROR: wrong parameter type
//     // void bar() override { }           // ERROR: missing const
//     // void bax() override { }           // ERROR: typo in name
//
//     // Without 'override', these compile but DON'T override - they hide!
// };

// ----------------------------------------------------------------------------
// The 'final' Keyword (C++11)
// Book reference: 5.4 Virtual Functions
// ----------------------------------------------------------------------------

class Widget {
public:
    virtual ~Widget() = default;

    virtual void draw() { std::cout << "Widget::draw()\n"; }
    virtual void resize([[maybe_unused]] int w, [[maybe_unused]] int h) { std::cout << "Widget::resize()\n"; }
};

class Button : public Widget {
public:
    // 'final' prevents further overriding in derived classes
    void draw() final { std::cout << "Button::draw()\n"; }

    // This can still be overridden
    void resize([[maybe_unused]] int w, [[maybe_unused]] int h) override { std::cout << "Button::resize()\n"; }
};

// 'final' on a class prevents ANY further inheritance
class FinalButton final : public Widget {
public:
    void draw() override { std::cout << "FinalButton::draw()\n"; }
};

class IconButton : public Button {
public:
    // void draw() override { }  // ERROR: Button::draw() is final

    // This is OK - resize() wasn't marked final
    void resize([[maybe_unused]] int w, [[maybe_unused]] int h) override { std::cout << "IconButton::resize()\n"; }
};

// class SpecialFinalButton : public FinalButton { };  // ERROR: FinalButton is final

// ----------------------------------------------------------------------------
// Virtual Function Tables (vtable) - How It Works
// ----------------------------------------------------------------------------

class Shape {
public:
    virtual ~Shape() = default;
    virtual double area() const = 0;
    virtual void describe() const {
        std::cout << "A shape with area " << area() << "\n";
    }
};

class Circle : public Shape {
    double radius;

public:
    explicit Circle(double r) : radius{r} {}

    double area() const override {
        return 3.14159 * radius * radius;
    }

    void describe() const override {
        std::cout << "Circle with radius " << radius << ", area " << area() << "\n";
    }
};

class Square : public Shape {
    double side;

public:
    explicit Square(double s) : side{s} {}

    double area() const override {
        return side * side;
    }

    // Uses default describe() from Shape
};

// ----------------------------------------------------------------------------
// Covariant Return Types
// A derived class can return a more derived type
// ----------------------------------------------------------------------------

class Document {
public:
    virtual ~Document() = default;

    // Clone pattern with covariant return
    virtual Document* clone() const {
        return new Document(*this);
    }

    virtual std::string content() const { return "Generic document"; }
};

class TextDocument : public Document {
    std::string text;

public:
    explicit TextDocument(std::string t = "") : text{std::move(t)} {}

    // Covariant return type: TextDocument* instead of Document*
    TextDocument* clone() const override {
        return new TextDocument(*this);
    }

    std::string content() const override { return text; }
};

// ----------------------------------------------------------------------------
// Calling Base Class Version
// ----------------------------------------------------------------------------

class Logger {
public:
    virtual ~Logger() = default;

    virtual void log(const std::string& message) {
        std::cout << "[LOG] " << message << "\n";
    }
};

class TimestampLogger : public Logger {
public:
    void log(const std::string& message) override {
        std::cout << "[2024-01-15 10:30:00] ";
        // Explicitly call base class version
        Logger::log(message);
    }
};

// ----------------------------------------------------------------------------
// Main demonstration
// ----------------------------------------------------------------------------

int main() {
    std::cout << "=== Virtual Dispatch Basics ===\n";
    std::cout << "Book reference: 5.4 Virtual Functions\n\n";

    Dog dog;
    Cat cat;

    // Through base class reference - virtual dispatch occurs
    std::cout << "Through Animal& reference:\n";
    make_speak(dog);
    std::cout << "\n";
    make_speak(cat);

    std::cout << "\n=== Virtual vs Non-Virtual ===\n\n";

    Animal* animal_ptr = &dog;

    // Virtual function - dynamic dispatch based on actual type
    std::cout << "animal_ptr->speak(): " << animal_ptr->speak() << " (Dog's version)\n";

    // Non-virtual function - static binding based on pointer type
    std::cout << "animal_ptr->type(): " << animal_ptr->type() << " (Animal's version)\n";

    // Direct call uses Dog's version
    std::cout << "dog.type(): " << dog.type() << " (Dog's version)\n";

    std::cout << "\n=== The 'override' Keyword ===\n\n";

    Base* base = new DerivedCorrect();
    base->foo(42);
    base->bar();
    base->baz();
    delete base;

    std::cout << "\n'override' catches errors at compile time:\n";
    std::cout << "- Wrong parameter types\n";
    std::cout << "- Missing const qualifiers\n";
    std::cout << "- Typos in function names\n";

    std::cout << "\n=== The 'final' Keyword ===\n\n";

    Button button;
    IconButton icon_button;

    Widget* w1 = &button;
    Widget* w2 = &icon_button;

    w1->draw();    // Button::draw()
    w2->draw();    // Still Button::draw() (inherited, can't override)
    w2->resize(100, 50);  // IconButton::resize()

    std::cout << "\n'final' prevents:\n";
    std::cout << "- Further overriding of a specific function\n";
    std::cout << "- Any inheritance from a class marked final\n";

    std::cout << "\n=== Polymorphic Shapes ===\n\n";

    std::vector<std::unique_ptr<Shape>> shapes;
    shapes.push_back(std::make_unique<Circle>(5.0));
    shapes.push_back(std::make_unique<Square>(4.0));
    shapes.push_back(std::make_unique<Circle>(3.0));

    for (const auto& shape : shapes) {
        shape->describe();  // Virtual dispatch to appropriate describe()
    }

    double total = 0;
    for (const auto& shape : shapes) {
        total += shape->area();  // Virtual dispatch to appropriate area()
    }
    std::cout << "Total area: " << total << "\n";

    std::cout << "\n=== Covariant Return Types ===\n\n";

    TextDocument original{"Hello, World!"};
    TextDocument* copy = original.clone();  // Returns TextDocument*, not Document*

    std::cout << "Original: " << original.content() << "\n";
    std::cout << "Clone: " << copy->content() << "\n";

    delete copy;

    std::cout << "\n=== Calling Base Class Version ===\n\n";

    TimestampLogger logger;
    logger.log("Application started");  // Adds timestamp, then calls base log()

    std::cout << "\n=== How Virtual Functions Work (Conceptually) ===\n\n";

    std::cout << "Each polymorphic class has a vtable (virtual function table):\n";
    std::cout << "\n";
    std::cout << "Animal vtable:          Dog vtable:           Cat vtable:\n";
    std::cout << "+---------------+       +---------------+     +---------------+\n";
    std::cout << "| ~Animal()     |       | ~Dog()        |     | ~Cat()        |\n";
    std::cout << "| Animal::speak |       | Dog::speak    |     | Cat::speak    |\n";
    std::cout << "+---------------+       +---------------+     +---------------+\n";
    std::cout << "\n";
    std::cout << "Each object has a hidden vptr pointing to its class's vtable.\n";
    std::cout << "Virtual call: object->vptr->function_slot()\n";

    std::cout << "\n=== Key Points ===\n";
    std::cout << "1. Virtual functions enable runtime polymorphism\n";
    std::cout << "2. ALWAYS use 'override' when overriding\n";
    std::cout << "3. Use 'final' to prevent unwanted overriding\n";
    std::cout << "4. Non-virtual functions use static binding\n";
    std::cout << "5. Virtual calls have small overhead (vtable lookup)\n";

    return 0;
}
