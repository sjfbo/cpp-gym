// std::optional, std::variant, std::any - Type-Safe Alternatives
// Book reference: 15.4 Alternatives
//
// These types provide type-safe alternatives to traditional C patterns:
// - optional: may or may not contain a value (vs nullable pointers)
// - variant: type-safe discriminated union (vs C unions)
// - any: type-safe container for any type (vs void*)

#include <any>
#include <iostream>
#include <map>
#include <optional>
#include <string>
#include <variant>
#include <vector>

// ============================================================================
// std::optional - Maybe a Value
// ============================================================================

// Function that might not return a value
std::optional<int> find_first_positive(const std::vector<int>& vec) {
    for (int x : vec) {
        if (x > 0) {
            return x;  // Return the value wrapped in optional
        }
    }
    return std::nullopt;  // No value found
}

std::optional<std::string> get_env_var(const std::string& name) {
    if (const char* value = std::getenv(name.c_str())) {
        return std::string{value};
    }
    return std::nullopt;
}

void optional_demo() {
    std::cout << "=== std::optional ===\n";

    // Creating optionals
    std::optional<int> empty;                    // No value
    std::optional<int> with_value = 42;          // Has value
    std::optional<std::string> str = "hello";    // Has value

    // Checking for value
    std::cout << "empty has value: " << std::boolalpha << empty.has_value() << "\n";
    std::cout << "with_value has value: " << with_value.has_value() << "\n";

    // Boolean conversion
    if (with_value) {
        std::cout << "with_value is truthy\n";
    }

    // Accessing value
    std::cout << "with_value: " << *with_value << "\n";        // Unchecked
    std::cout << "with_value: " << with_value.value() << "\n"; // Checked (throws)

    // Safe access with default
    std::cout << "empty.value_or(0): " << empty.value_or(0) << "\n";

    // Using optional-returning function
    std::vector<int> numbers{-5, -3, 0, 4, 7};
    if (auto result = find_first_positive(numbers)) {
        std::cout << "First positive: " << *result << "\n";
    }

    std::vector<int> all_negative{-5, -3, -1};
    if (auto result = find_first_positive(all_negative)) {
        std::cout << "First positive: " << *result << "\n";
    } else {
        std::cout << "No positive numbers found\n";
    }

    // Modifying optional
    std::optional<int> opt = 10;
    opt.reset();              // Clear the value
    std::cout << "After reset: " << opt.has_value() << "\n";

    opt.emplace(20);          // Construct value in place
    std::cout << "After emplace: " << *opt << "\n";

    // Optional with complex types
    struct Person {
        std::string name;
        int age;
    };

    std::optional<Person> maybe_person;
    maybe_person.emplace("Alice", 30);  // Construct in place

    if (maybe_person) {
        std::cout << "Person: " << maybe_person->name
                  << ", age " << maybe_person->age << "\n";
    }
}

// ============================================================================
// std::variant - Type-Safe Union
// ============================================================================

// Define a variant that can hold different types
using JsonValue = std::variant<
    std::nullptr_t,
    bool,
    int,
    double,
    std::string
>;

void print_json_value(const JsonValue& v) {
    // Using std::visit with a generic lambda
    std::visit([](const auto& val) {
        using T = std::decay_t<decltype(val)>;
        if constexpr (std::is_same_v<T, std::nullptr_t>) {
            std::cout << "null";
        } else if constexpr (std::is_same_v<T, bool>) {
            std::cout << (val ? "true" : "false");
        } else if constexpr (std::is_same_v<T, std::string>) {
            std::cout << '"' << val << '"';
        } else {
            std::cout << val;
        }
    }, v);
    std::cout << "\n";
}

void variant_demo() {
    std::cout << "\n=== std::variant ===\n";

    // Creating variants
    std::variant<int, double, std::string> v1 = 42;        // int
    std::variant<int, double, std::string> v2 = 3.14;      // double
    std::variant<int, double, std::string> v3 = "hello";   // string

    // Checking which type is held
    std::cout << "v1 holds int: " << std::holds_alternative<int>(v1) << "\n";
    std::cout << "v1 holds double: " << std::holds_alternative<double>(v1) << "\n";

    // Getting the type index
    std::cout << "v1.index(): " << v1.index() << "\n";  // 0 for int
    std::cout << "v2.index(): " << v2.index() << "\n";  // 1 for double
    std::cout << "v3.index(): " << v3.index() << "\n";  // 2 for string

    // Accessing value with std::get
    std::cout << "v1 value: " << std::get<int>(v1) << "\n";
    std::cout << "v2 value: " << std::get<double>(v2) << "\n";
    std::cout << "v3 value: " << std::get<std::string>(v3) << "\n";

    // Safe access with std::get_if
    if (auto* p = std::get_if<int>(&v1)) {
        std::cout << "v1 is int: " << *p << "\n";
    }
    if (auto* p = std::get_if<std::string>(&v1)) {
        std::cout << "v1 is string: " << *p << "\n";
    } else {
        std::cout << "v1 is not a string\n";
    }

    // Changing the held type
    v1 = "now a string";
    std::cout << "v1 now holds: " << std::get<std::string>(v1) << "\n";

    // Using visit for pattern matching
    std::cout << "\nVisiting variants:\n";
    for (const auto& v : {v1, v2, v3}) {
        std::visit([](const auto& val) {
            std::cout << "  Value: " << val << "\n";
        }, v);
    }

    // JSON-like values demo
    std::cout << "\nJSON values:\n";
    std::vector<JsonValue> json_array{
        nullptr,
        true,
        42,
        3.14159,
        std::string{"hello"}
    };

    for (const auto& val : json_array) {
        std::cout << "  ";
        print_json_value(val);
    }
}

// ============================================================================
// Visitor Pattern with std::variant
// ============================================================================

// Overload helper for creating visitors
template<class... Ts>
struct overloaded : Ts... { using Ts::operator()...; };

void visitor_pattern_demo() {
    std::cout << "\n=== Visitor Pattern ===\n";

    struct Circle { double radius; };
    struct Rectangle { double width, height; };
    struct Triangle { double base, height; };

    using Shape = std::variant<Circle, Rectangle, Triangle>;

    auto area_visitor = overloaded{
        [](const Circle& c) { return 3.14159 * c.radius * c.radius; },
        [](const Rectangle& r) { return r.width * r.height; },
        [](const Triangle& t) { return 0.5 * t.base * t.height; }
    };

    std::vector<Shape> shapes{
        Circle{5.0},
        Rectangle{3.0, 4.0},
        Triangle{6.0, 4.0}
    };

    for (const auto& shape : shapes) {
        double area = std::visit(area_visitor, shape);
        std::cout << "Area: " << area << "\n";
    }
}

// ============================================================================
// std::any - Type-Safe Any
// ============================================================================

void any_demo() {
    std::cout << "\n=== std::any ===\n";

    // Creating any
    std::any empty;
    std::any a_int = 42;
    std::any a_double = 3.14;
    std::any a_string = std::string{"hello"};

    // Check if has value
    std::cout << "empty has value: " << empty.has_value() << "\n";
    std::cout << "a_int has value: " << a_int.has_value() << "\n";

    // Get type info
    std::cout << "a_int type: " << a_int.type().name() << "\n";
    std::cout << "a_string type: " << a_string.type().name() << "\n";

    // Access with any_cast (throws if wrong type)
    std::cout << "a_int value: " << std::any_cast<int>(a_int) << "\n";
    std::cout << "a_string value: " << std::any_cast<std::string>(a_string) << "\n";

    // Safe access with pointer any_cast
    if (auto* p = std::any_cast<int>(&a_int)) {
        std::cout << "a_int (safe): " << *p << "\n";
    }
    if ([[maybe_unused]] auto* p = std::any_cast<double>(&a_int)) {
        std::cout << "a_int is double\n";
    } else {
        std::cout << "a_int is not double\n";
    }

    // Change type
    a_int = std::string{"now a string"};
    std::cout << "a_int now: " << std::any_cast<std::string>(a_int) << "\n";

    // Reset
    a_int.reset();
    std::cout << "After reset, has value: " << a_int.has_value() << "\n";

    // Emplace
    a_int.emplace<std::vector<int>>(std::initializer_list<int>{1, 2, 3});
    auto& vec = std::any_cast<std::vector<int>&>(a_int);
    std::cout << "Vector size: " << vec.size() << "\n";
}

// ============================================================================
// Practical Example: Configuration System
// ============================================================================

class Config {
public:
    using Value = std::variant<bool, int, double, std::string>;

    void set(const std::string& key, Value value) {
        settings_[key] = std::move(value);
    }

    template<typename T>
    std::optional<T> get(const std::string& key) const {
        auto it = settings_.find(key);
        if (it == settings_.end()) {
            return std::nullopt;
        }
        if (auto* p = std::get_if<T>(&it->second)) {
            return *p;
        }
        return std::nullopt;
    }

    void print() const {
        for (const auto& [key, value] : settings_) {
            std::cout << key << " = ";
            std::visit([](const auto& v) { std::cout << v; }, value);
            std::cout << "\n";
        }
    }

private:
    std::map<std::string, Value> settings_;
};

void config_demo() {
    std::cout << "\n=== Configuration System Example ===\n";

    Config config;

    config.set("debug", true);
    config.set("max_connections", 100);
    config.set("timeout", 30.5);
    config.set("hostname", std::string{"localhost"});

    std::cout << "All settings:\n";
    config.print();

    // Type-safe retrieval
    if (auto debug = config.get<bool>("debug")) {
        std::cout << "\nDebug mode: " << (*debug ? "on" : "off") << "\n";
    }

    if (auto max_conn = config.get<int>("max_connections")) {
        std::cout << "Max connections: " << *max_conn << "\n";
    }

    // Wrong type returns nullopt
    if (auto wrong = config.get<std::string>("max_connections")) {
        std::cout << "max_connections as string: " << *wrong << "\n";
    } else {
        std::cout << "max_connections is not a string\n";
    }

    // Missing key returns nullopt
    if (auto missing = config.get<int>("nonexistent")) {
        std::cout << "Found: " << *missing << "\n";
    } else {
        std::cout << "Key 'nonexistent' not found\n";
    }
}

// ============================================================================
// Comparison: When to Use What
// ============================================================================

void comparison_demo() {
    std::cout << "\n=== When to Use What ===\n";

    std::cout << R"(
std::optional<T>:
  - Function might not return a value
  - Nullable object that isn't a pointer
  - Lazy initialization

std::variant<Ts...>:
  - Type-safe union of known types
  - State machines with different data per state
  - Result type (variant<Value, Error>)
  - Visitor pattern / pattern matching

std::any:
  - Type erasure when types are unknown at compile time
  - Plugin systems
  - Avoid if possible (prefer variant when types are known)
)";
}

int main() {
    optional_demo();
    variant_demo();
    visitor_pattern_demo();
    any_demo();
    config_demo();
    comparison_demo();

    return 0;
}

// Key points:
// - optional: "maybe a value" - safer than nullable pointers
// - variant: type-safe discriminated union - safer than C unions
// - any: type-safe container for any type - safer than void*
// - Use value_or() for safe optional access with default
// - Use std::visit() for pattern matching on variants
// - Use std::get_if / any_cast with pointer for safe access
// - Prefer variant over any when types are known at compile time
