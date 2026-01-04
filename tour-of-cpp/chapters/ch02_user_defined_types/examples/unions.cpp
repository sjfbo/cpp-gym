// Unions - Shared Storage (and std::variant)
// Book reference: 2.5 Unions
//
// A union holds one of several types in the same memory location.
// Raw unions are error-prone; prefer std::variant in modern C++.

#include <iostream>
#include <string>
#include <variant>

// ----------------------------------------------------------------------------
// Traditional union (dangerous!)
// ----------------------------------------------------------------------------

union Value {
    int i;
    double d;
    char c;
    // All members share the same memory location
    // Size is the size of the largest member
};

// ----------------------------------------------------------------------------
// Tagged union (manual type tracking)
// ----------------------------------------------------------------------------

enum class ValueType { Int, Double, String };

struct TaggedValue {
    ValueType type;
    union {
        int i;
        double d;
        // std::string s;  // Cannot use complex types in union!
    };
    std::string s;  // Has to be outside the union

    // Manual constructors for each type
    TaggedValue(int val) : type{ValueType::Int}, i{val} {}
    TaggedValue(double val) : type{ValueType::Double}, d{val} {}
    TaggedValue(const std::string& val) : type{ValueType::String}, s{val} {}

    void print() const {
        switch (type) {
            case ValueType::Int:
                std::cout << "Int: " << i;
                break;
            case ValueType::Double:
                std::cout << "Double: " << d;
                break;
            case ValueType::String:
                std::cout << "String: " << s;
                break;
        }
        std::cout << "\n";
    }
};

// ----------------------------------------------------------------------------
// std::variant (type-safe union) - PREFERRED
// ----------------------------------------------------------------------------

using SafeValue = std::variant<int, double, std::string>;

void print_variant(const SafeValue& v) {
    // Using std::visit with a lambda
    std::visit(
        [](const auto& value) {
            std::cout << "Value: " << value << "\n";
        },
        v);
}

// Overload pattern for type-specific handling
template <class... Ts>
struct overloaded : Ts... {
    using Ts::operator()...;
};

void print_variant_typed(const SafeValue& v) {
    std::visit(
        overloaded{[](int i) { std::cout << "Int: " << i << "\n"; },
                   [](double d) { std::cout << "Double: " << d << "\n"; },
                   [](const std::string& s) { std::cout << "String: " << s << "\n"; }},
        v);
}

// ----------------------------------------------------------------------------
// Main demonstration
// ----------------------------------------------------------------------------

int main() {
    std::cout << "=== Raw Union (Dangerous) ===\n";

    Value v;
    std::cout << "sizeof(Value) = " << sizeof(Value) << " bytes\n";
    std::cout << "sizeof(int) = " << sizeof(int) << ", sizeof(double) = " << sizeof(double)
              << "\n";

    v.i = 42;
    std::cout << "Set v.i = 42\n";
    std::cout << "v.i = " << v.i << "\n";

    // DANGER: Reading wrong member is undefined behavior!
    std::cout << "v.d = " << v.d << " (GARBAGE! UB!)\n";

    v.d = 3.14159;
    std::cout << "\nSet v.d = 3.14159\n";
    std::cout << "v.d = " << v.d << "\n";
    std::cout << "v.i = " << v.i << " (GARBAGE! UB!)\n";

    std::cout << "\n=== Tagged Union (Manual Safety) ===\n";

    TaggedValue tv1{42};
    TaggedValue tv2{3.14};
    TaggedValue tv3{"hello"};

    tv1.print();
    tv2.print();
    tv3.print();

    std::cout << "\n=== std::variant (Type-Safe) ===\n";

    SafeValue sv1 = 42;
    SafeValue sv2 = 3.14;
    SafeValue sv3 = std::string{"hello"};

    std::cout << "Generic print:\n";
    print_variant(sv1);
    print_variant(sv2);
    print_variant(sv3);

    std::cout << "\nTyped print:\n";
    print_variant_typed(sv1);
    print_variant_typed(sv2);
    print_variant_typed(sv3);

    std::cout << "\n=== std::variant Operations ===\n";

    // Check current type
    std::cout << "sv1 holds int: " << std::boolalpha << std::holds_alternative<int>(sv1) << "\n";
    std::cout << "sv1 holds string: " << std::holds_alternative<std::string>(sv1) << "\n";

    // Get value (throws if wrong type)
    try {
        int i = std::get<int>(sv1);
        std::cout << "Got int: " << i << "\n";

        // This will throw
        // int j = std::get<int>(sv2);  // sv2 holds double!
    } catch (const std::bad_variant_access& e) {
        std::cout << "Bad access: " << e.what() << "\n";
    }

    // Safe access with get_if (returns pointer)
    if (auto* pi = std::get_if<int>(&sv1)) {
        std::cout << "sv1 as int: " << *pi << "\n";
    }

    if (auto* pd = std::get_if<double>(&sv1)) {
        std::cout << "sv1 as double: " << *pd << "\n";
    } else {
        std::cout << "sv1 is not a double\n";
    }

    // Get by index
    std::cout << "Index of current type: " << sv1.index() << "\n";
    std::cout << "std::get<0>(sv1) = " << std::get<0>(sv1) << "\n";  // 0 = int

    // Modify variant
    sv1 = 100;  // Still int
    std::cout << "After sv1 = 100: ";
    print_variant_typed(sv1);

    sv1 = "now a string";  // Change type!
    std::cout << "After sv1 = \"now a string\": ";
    print_variant_typed(sv1);

    std::cout << "\n=== std::monostate for 'empty' variant ===\n";

    // If you need a variant that can be empty, use monostate
    std::variant<std::monostate, int, std::string> maybe_value;

    std::cout << "Empty variant holds monostate: " << std::holds_alternative<std::monostate>(maybe_value)
              << "\n";

    maybe_value = 42;
    std::cout << "After assignment, holds int: " << std::holds_alternative<int>(maybe_value)
              << "\n";

    std::cout << "\n=== Summary ===\n";
    std::cout << "1. Avoid raw unions - undefined behavior if you read wrong member\n";
    std::cout << "2. Tagged unions work but are verbose and error-prone\n";
    std::cout << "3. Use std::variant for type-safe discriminated unions\n";
    std::cout << "4. std::visit for processing variant values\n";
    std::cout << "5. std::monostate for 'empty' state in variant\n";

    return 0;
}
