// Class Hierarchies - Inheritance and Composition
// Book reference: 5.5 Class Hierarchies
//
// Class hierarchies organize related classes through inheritance:
// - Single inheritance: one base class
// - Multiple inheritance: multiple base classes
// - Object slicing: beware of copying derived to base
// - Use references/pointers for polymorphism

#include <iostream>
#include <memory>
#include <string>
#include <vector>

// ----------------------------------------------------------------------------
// Single Inheritance Hierarchy
// Book reference: 5.5 Class Hierarchies
// ----------------------------------------------------------------------------

class Employee {
    std::string name_;
    int id_;

public:
    Employee(std::string name, int id) : name_{std::move(name)}, id_{id} {}

    virtual ~Employee() = default;

    // Accessors
    const std::string& name() const { return name_; }
    int id() const { return id_; }

    // Virtual function for polymorphic behavior
    virtual double calculate_pay() const = 0;

    virtual void print_info() const {
        std::cout << "Employee #" << id_ << ": " << name_ << "\n";
    }
};

class SalariedEmployee : public Employee {
    double annual_salary_;

public:
    SalariedEmployee(std::string name, int id, double salary)
        : Employee{std::move(name), id}, annual_salary_{salary} {}

    double calculate_pay() const override {
        return annual_salary_ / 12;  // Monthly pay
    }

    void print_info() const override {
        Employee::print_info();  // Call base version
        std::cout << "  Type: Salaried, Annual: $" << annual_salary_ << "\n";
    }
};

class HourlyEmployee : public Employee {
    double hourly_rate_;
    double hours_worked_;

public:
    HourlyEmployee(std::string name, int id, double rate, double hours = 0)
        : Employee{std::move(name), id}, hourly_rate_{rate}, hours_worked_{hours} {}

    void set_hours(double hours) { hours_worked_ = hours; }

    double calculate_pay() const override {
        return hourly_rate_ * hours_worked_;
    }

    void print_info() const override {
        Employee::print_info();
        std::cout << "  Type: Hourly, Rate: $" << hourly_rate_
                  << "/hr, Hours: " << hours_worked_ << "\n";
    }
};

// Manager is-a SalariedEmployee with additional responsibilities
class Manager : public SalariedEmployee {
    std::vector<Employee*> team_;

public:
    Manager(std::string name, int id, double salary)
        : SalariedEmployee{std::move(name), id, salary} {}

    void add_team_member(Employee* e) {
        team_.push_back(e);
    }

    int team_size() const { return static_cast<int>(team_.size()); }

    void print_info() const override {
        SalariedEmployee::print_info();
        std::cout << "  Team size: " << team_.size() << "\n";
    }
};

// ----------------------------------------------------------------------------
// Multiple Inheritance
// Book reference: 5.5 Class Hierarchies
// ----------------------------------------------------------------------------

// Interface for objects that can be serialized
class Serializable {
public:
    virtual ~Serializable() = default;
    virtual std::string serialize() const = 0;
    virtual void deserialize(const std::string& data) = 0;
};

// Interface for objects that can be compared
class Comparable {
public:
    virtual ~Comparable() = default;
    virtual int compare_to(const Comparable& other) const = 0;
};

// A class implementing multiple interfaces
class Document : public Serializable, public Comparable {
    std::string title_;
    std::string content_;
    int version_;

public:
    Document(std::string title = "", std::string content = "", int version = 1)
        : title_{std::move(title)}, content_{std::move(content)}, version_{version} {}

    // Implement Serializable
    std::string serialize() const override {
        return title_ + "|" + content_ + "|" + std::to_string(version_);
    }

    void deserialize(const std::string& data) override {
        // Simplified parsing
        size_t pos1 = data.find('|');
        size_t pos2 = data.find('|', pos1 + 1);
        if (pos1 != std::string::npos && pos2 != std::string::npos) {
            title_ = data.substr(0, pos1);
            content_ = data.substr(pos1 + 1, pos2 - pos1 - 1);
            version_ = std::stoi(data.substr(pos2 + 1));
        }
    }

    // Implement Comparable
    int compare_to(const Comparable& other) const override {
        const auto* doc = dynamic_cast<const Document*>(&other);
        if (!doc) return 0;
        return version_ - doc->version_;
    }

    // Accessors
    const std::string& title() const { return title_; }
    int version() const { return version_; }
};

// ----------------------------------------------------------------------------
// Diamond Inheritance Problem and Virtual Inheritance
// ----------------------------------------------------------------------------

class Animal {
protected:
    std::string name_;

public:
    explicit Animal(std::string name = "Unknown") : name_{std::move(name)} {
        std::cout << "  Animal(" << name_ << ") constructed\n";
    }

    virtual ~Animal() {
        std::cout << "  Animal(" << name_ << ") destroyed\n";
    }

    virtual void speak() const { std::cout << name_ << " says: ...\n"; }
};

// Virtual inheritance: only one Animal base in the final object
class Mammal : virtual public Animal {
public:
    explicit Mammal(std::string name = "Unknown") : Animal{name} {
        std::cout << "  Mammal constructed\n";
    }

    void nurse() const { std::cout << name_ << " nurses young\n"; }
};

class WingedAnimal : virtual public Animal {
public:
    explicit WingedAnimal(std::string name = "Unknown") : Animal{name} {
        std::cout << "  WingedAnimal constructed\n";
    }

    void fly() const { std::cout << name_ << " flies\n"; }
};

// Bat inherits from both Mammal and WingedAnimal
// With virtual inheritance, there's only ONE Animal base
class Bat : public Mammal, public WingedAnimal {
public:
    // Must initialize the virtual base directly
    explicit Bat(std::string name = "Bat")
        : Animal{name}, Mammal{name}, WingedAnimal{name} {
        std::cout << "  Bat constructed\n";
    }

    void speak() const override {
        std::cout << name_ << " says: Squeak!\n";
    }
};

// ----------------------------------------------------------------------------
// Object Slicing Demonstration
// ----------------------------------------------------------------------------

class Base {
protected:
    int base_value_;

public:
    explicit Base(int v = 0) : base_value_{v} {}
    virtual ~Base() = default;

    virtual void print() const {
        std::cout << "Base: " << base_value_ << "\n";
    }

    int value() const { return base_value_; }
};

class Derived : public Base {
    int derived_value_;

public:
    Derived(int bv, int dv) : Base{bv}, derived_value_{dv} {}

    void print() const override {
        std::cout << "Derived: base=" << base_value_
                  << ", derived=" << derived_value_ << "\n";
    }
};

// Takes by value - causes slicing!
void process_by_value(Base b) {
    b.print();  // Always calls Base::print(), derived part is sliced off
}

// Takes by reference - preserves polymorphism
void process_by_reference(const Base& b) {
    b.print();  // Calls appropriate print() based on actual type
}

// ----------------------------------------------------------------------------
// Protected Members
// ----------------------------------------------------------------------------

class Shape {
protected:
    // Protected: accessible in derived classes, not outside
    std::string color_ = "black";

public:
    virtual ~Shape() = default;

    // Public interface
    void set_color(const std::string& c) { color_ = c; }
    std::string color() const { return color_; }

    virtual double area() const = 0;
};

class ColoredCircle : public Shape {
    double radius_;

public:
    explicit ColoredCircle(double r, const std::string& color = "black")
        : radius_{r} {
        color_ = color;  // Can access protected member
    }

    double area() const override {
        return 3.14159 * radius_ * radius_;
    }

    void describe() const {
        // Can use protected member color_ directly
        std::cout << "A " << color_ << " circle with radius " << radius_ << "\n";
    }
};

// ----------------------------------------------------------------------------
// Main demonstration
// ----------------------------------------------------------------------------

int main() {
    std::cout << "=== Single Inheritance Hierarchy ===\n";
    std::cout << "Book reference: 5.5 Class Hierarchies\n\n";

    std::vector<std::unique_ptr<Employee>> employees;
    employees.push_back(std::make_unique<SalariedEmployee>("Alice", 1001, 75000));
    employees.push_back(std::make_unique<HourlyEmployee>("Bob", 1002, 25, 160));
    employees.push_back(std::make_unique<Manager>("Carol", 1003, 95000));

    // Add team members to manager
    Manager* manager = static_cast<Manager*>(employees[2].get());
    manager->add_team_member(employees[0].get());
    manager->add_team_member(employees[1].get());

    // Polymorphic processing
    double total_payroll = 0;
    for (const auto& emp : employees) {
        emp->print_info();
        double pay = emp->calculate_pay();
        std::cout << "  Monthly pay: $" << pay << "\n\n";
        total_payroll += pay;
    }
    std::cout << "Total monthly payroll: $" << total_payroll << "\n";

    std::cout << "\n=== Multiple Inheritance (Interfaces) ===\n\n";

    Document doc1{"Report", "Quarterly results...", 1};
    Document doc2{"Report", "Updated results...", 2};

    std::cout << "Document 1: " << doc1.title() << " v" << doc1.version() << "\n";
    std::cout << "Serialized: " << doc1.serialize() << "\n";

    std::cout << "Document 2: " << doc2.title() << " v" << doc2.version() << "\n";

    if (doc1.compare_to(doc2) < 0) {
        std::cout << "Document 1 is older than Document 2\n";
    }

    // Deserialize
    Document doc3;
    doc3.deserialize(doc1.serialize());
    std::cout << "Deserialized: " << doc3.title() << " v" << doc3.version() << "\n";

    std::cout << "\n=== Virtual Inheritance (Diamond Problem) ===\n\n";

    std::cout << "Creating Bat with virtual inheritance:\n";
    {
        Bat bat{"Stellaluna"};
        std::cout << "\nBat behavior:\n";
        bat.speak();
        bat.nurse();   // From Mammal
        bat.fly();     // From WingedAnimal
        std::cout << "\nDestroying Bat:\n";
    }

    std::cout << "\n=== Object Slicing ===\n\n";

    Derived d{10, 20};

    std::cout << "Original object:\n";
    d.print();

    std::cout << "\nPassed by value (SLICED!):\n";
    process_by_value(d);  // derived_value_ is lost!

    std::cout << "\nPassed by reference (preserved):\n";
    process_by_reference(d);  // Full object, virtual dispatch works

    std::cout << "\nCopied to Base variable (SLICED!):\n";
    Base b = d;  // Slicing assignment
    b.print();   // Calls Base::print()

    std::cout << "\n=== Protected Members ===\n\n";

    ColoredCircle circle{5.0, "red"};
    circle.describe();
    std::cout << "Area: " << circle.area() << "\n";

    // circle.color_ = "blue";  // ERROR: protected, not accessible from outside
    circle.set_color("blue");   // OK: through public interface
    circle.describe();

    std::cout << "\n=== Key Points ===\n";
    std::cout << "1. Use inheritance for 'is-a' relationships\n";
    std::cout << "2. Multiple inheritance works well for interfaces\n";
    std::cout << "3. Virtual inheritance solves the diamond problem\n";
    std::cout << "4. Beware of object slicing - use references/pointers\n";
    std::cout << "5. Protected members accessible in derived classes only\n";
    std::cout << "6. Prefer composition over inheritance when possible\n";

    return 0;
}
