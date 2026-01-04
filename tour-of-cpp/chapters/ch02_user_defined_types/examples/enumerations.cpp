// Enumerations - Named Constants
// Book reference: 2.4 Enumerations
//
// Enumerations define a type with a set of named values.
// Always use enum class (scoped enums) in modern C++.

#include <iostream>
#include <string>

// ----------------------------------------------------------------------------
// enum class (scoped enumeration) - PREFERRED
// ----------------------------------------------------------------------------

enum class Color { Red, Green, Blue };

enum class TrafficLight { Red, Yellow, Green };

// Enumerators are scoped - Color::Red and TrafficLight::Red are different

// ----------------------------------------------------------------------------
// enum class with explicit values
// ----------------------------------------------------------------------------

enum class HttpStatus {
    Ok = 200,
    Created = 201,
    BadRequest = 400,
    NotFound = 404,
    InternalError = 500
};

// ----------------------------------------------------------------------------
// enum class with underlying type
// ----------------------------------------------------------------------------

enum class Byte : unsigned char {};  // Empty enum, just the type

enum class Priority : int { Low = -1, Normal = 0, High = 1, Critical = 2 };

// ----------------------------------------------------------------------------
// Plain enum (unscoped) - AVOID in new code
// ----------------------------------------------------------------------------

// WARNING: Enumerators pollute the enclosing scope
enum OldColor { RED, GREEN, BLUE };  // RED, GREEN, BLUE are now global!

// This would conflict: enum AnotherEnum { RED };  // Error: RED redefinition

// ----------------------------------------------------------------------------
// Using enum class in functions
// ----------------------------------------------------------------------------

std::string color_name(Color c) {
    switch (c) {
        case Color::Red:
            return "Red";
        case Color::Green:
            return "Green";
        case Color::Blue:
            return "Blue";
    }
    return "Unknown";  // Should be unreachable
}

std::string status_message(HttpStatus status) {
    switch (status) {
        case HttpStatus::Ok:
            return "OK";
        case HttpStatus::Created:
            return "Created";
        case HttpStatus::BadRequest:
            return "Bad Request";
        case HttpStatus::NotFound:
            return "Not Found";
        case HttpStatus::InternalError:
            return "Internal Server Error";
    }
    return "Unknown Status";
}

// ----------------------------------------------------------------------------
// Operators with enum class
// ----------------------------------------------------------------------------

// Enum class doesn't have built-in operators, must define them
TrafficLight& operator++(TrafficLight& t) {
    switch (t) {
        case TrafficLight::Red:
            return t = TrafficLight::Green;
        case TrafficLight::Green:
            return t = TrafficLight::Yellow;
        case TrafficLight::Yellow:
            return t = TrafficLight::Red;
    }
    return t;
}

std::ostream& operator<<(std::ostream& os, TrafficLight t) {
    switch (t) {
        case TrafficLight::Red:
            return os << "RED";
        case TrafficLight::Green:
            return os << "GREEN";
        case TrafficLight::Yellow:
            return os << "YELLOW";
    }
    return os;
}

// ----------------------------------------------------------------------------
// Bitwise operations with enum class
// ----------------------------------------------------------------------------

enum class Permissions : unsigned {
    None = 0,
    Read = 1 << 0,   // 1
    Write = 1 << 1,  // 2
    Execute = 1 << 2 // 4
};

// Enable bitwise operators
constexpr Permissions operator|(Permissions a, Permissions b) {
    return static_cast<Permissions>(static_cast<unsigned>(a) | static_cast<unsigned>(b));
}

constexpr Permissions operator&(Permissions a, Permissions b) {
    return static_cast<Permissions>(static_cast<unsigned>(a) & static_cast<unsigned>(b));
}

constexpr bool has_permission(Permissions p, Permissions check) {
    return (p & check) == check;
}

// ----------------------------------------------------------------------------
// Main demonstration
// ----------------------------------------------------------------------------

int main() {
    std::cout << "=== enum class Basics ===\n";

    Color c = Color::Red;
    std::cout << "Color: " << color_name(c) << "\n";

    // Type safety: cannot mix different enum types
    // TrafficLight t = Color::Red;  // ERROR: different types

    // No implicit conversion to int
    // int i = c;  // ERROR: no implicit conversion
    int i = static_cast<int>(c);  // Explicit conversion OK
    std::cout << "Color as int: " << i << "\n";

    std::cout << "\n=== enum class with values ===\n";

    HttpStatus status = HttpStatus::NotFound;
    std::cout << "HTTP " << static_cast<int>(status) << ": " << status_message(status) << "\n";

    status = HttpStatus::Ok;
    std::cout << "HTTP " << static_cast<int>(status) << ": " << status_message(status) << "\n";

    std::cout << "\n=== Custom operators ===\n";

    TrafficLight light = TrafficLight::Red;
    std::cout << "Light: " << light << "\n";

    ++light;
    std::cout << "After ++: " << light << "\n";

    ++light;
    std::cout << "After ++: " << light << "\n";

    ++light;
    std::cout << "After ++: " << light << "\n";

    std::cout << "\n=== Bitwise enum (flags) ===\n";

    Permissions user_perms = Permissions::Read | Permissions::Write;

    std::cout << "User permissions value: " << static_cast<unsigned>(user_perms) << "\n";
    std::cout << "Has Read: " << std::boolalpha << has_permission(user_perms, Permissions::Read)
              << "\n";
    std::cout << "Has Execute: " << has_permission(user_perms, Permissions::Execute) << "\n";

    // Add execute permission
    Permissions all_perms = user_perms | Permissions::Execute;
    std::cout << "All permissions: " << static_cast<unsigned>(all_perms) << "\n";

    std::cout << "\n=== Plain enum (for comparison) ===\n";

    // Plain enum - enumerators in enclosing scope
    OldColor old = RED;  // No scope qualifier needed (that's the problem!)
    std::cout << "Old enum value: " << old << "\n";  // Implicit conversion to int

    // Can accidentally use as int
    int sum = old + 10;  // Compiles! Probably a bug.
    std::cout << "old + 10 = " << sum << " (probably unintended)\n";

    std::cout << "\n=== Best Practices ===\n";
    std::cout << "1. Always use 'enum class' not plain 'enum'\n";
    std::cout << "2. Enumerators are scoped (Color::Red not just Red)\n";
    std::cout << "3. No implicit conversion to int (type safety)\n";
    std::cout << "4. Use explicit underlying type if needed\n";
    std::cout << "5. Define operators if you need them\n";

    return 0;
}
