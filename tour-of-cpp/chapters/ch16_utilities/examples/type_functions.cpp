// Type Functions - Type Traits, Conditional, and Enable_if
// Book reference: 16.4 Type Functions
//
// Type traits allow compile-time introspection and transformation of types:
// - Query type properties (is_integral, is_pointer, etc.)
// - Transform types (remove_reference, add_const, etc.)
// - SFINAE-based overloading with enable_if
// - C++20 concepts provide cleaner syntax for constraints

#include <concepts>
#include <iostream>
#include <string>
#include <type_traits>
#include <vector>

// ============================================================================
// Basic Type Traits - Type Properties
// ============================================================================

void type_property_traits() {
    std::cout << "=== Type Property Traits ===\n";

    // Primary type categories
    std::cout << "is_void<void>: " << std::is_void_v<void> << "\n";
    std::cout << "is_integral<int>: " << std::is_integral_v<int> << "\n";
    std::cout << "is_integral<double>: " << std::is_integral_v<double> << "\n";
    std::cout << "is_floating_point<double>: " << std::is_floating_point_v<double> << "\n";
    std::cout << "is_array<int[5]>: " << std::is_array_v<int[5]> << "\n";
    std::cout << "is_pointer<int*>: " << std::is_pointer_v<int*> << "\n";
    std::cout << "is_reference<int&>: " << std::is_reference_v<int&> << "\n";

    std::cout << "\n";

    // Composite type categories
    std::cout << "is_arithmetic<int>: " << std::is_arithmetic_v<int> << "\n";
    std::cout << "is_arithmetic<std::string>: " << std::is_arithmetic_v<std::string> << "\n";
    std::cout << "is_fundamental<int>: " << std::is_fundamental_v<int> << "\n";
    std::cout << "is_object<int>: " << std::is_object_v<int> << "\n";
    std::cout << "is_scalar<int*>: " << std::is_scalar_v<int*> << "\n";

    std::cout << "\n";

    // Type properties
    std::cout << "is_const<const int>: " << std::is_const_v<const int> << "\n";
    std::cout << "is_volatile<volatile int>: " << std::is_volatile_v<volatile int> << "\n";
    std::cout << "is_signed<int>: " << std::is_signed_v<int> << "\n";
    std::cout << "is_unsigned<unsigned int>: " << std::is_unsigned_v<unsigned int> << "\n";
}

// ============================================================================
// Type Traits - Class Properties
// ============================================================================

struct Empty {};

struct NonEmpty {
    int x;
};

class Abstract {
public:
    virtual void foo() = 0;
    virtual ~Abstract() = default;
};

class Final final {};

class Polymorphic {
public:
    virtual ~Polymorphic() = default;
};

void class_property_traits() {
    std::cout << "\n=== Class Property Traits ===\n";

    std::cout << "is_empty<Empty>: " << std::is_empty_v<Empty> << "\n";
    std::cout << "is_empty<NonEmpty>: " << std::is_empty_v<NonEmpty> << "\n";
    std::cout << "is_abstract<Abstract>: " << std::is_abstract_v<Abstract> << "\n";
    std::cout << "is_final<Final>: " << std::is_final_v<Final> << "\n";
    std::cout << "is_polymorphic<Polymorphic>: " << std::is_polymorphic_v<Polymorphic> << "\n";

    std::cout << "\n";

    // Constructor/destructor traits
    std::cout << "is_default_constructible<std::string>: "
              << std::is_default_constructible_v<std::string> << "\n";
    std::cout << "is_copy_constructible<std::string>: "
              << std::is_copy_constructible_v<std::string> << "\n";
    std::cout << "is_move_constructible<std::string>: "
              << std::is_move_constructible_v<std::string> << "\n";
    std::cout << "is_trivially_copyable<int>: "
              << std::is_trivially_copyable_v<int> << "\n";
}

// ============================================================================
// Type Transformations
// ============================================================================

void type_transformations() {
    std::cout << "\n=== Type Transformations ===\n";

    // Remove qualifiers
    using NoRef = std::remove_reference_t<int&>;
    using NoConst = std::remove_const_t<const int>;
    using NoCV = std::remove_cv_t<const volatile int>;

    static_assert(std::is_same_v<NoRef, int>);
    static_assert(std::is_same_v<NoConst, int>);
    static_assert(std::is_same_v<NoCV, int>);

    std::cout << "remove_reference_t<int&> == int: "
              << std::is_same_v<NoRef, int> << "\n";

    // Add qualifiers
    using WithConst = std::add_const_t<int>;
    using WithPtr = std::add_pointer_t<int>;
    using WithRef = std::add_lvalue_reference_t<int>;

    static_assert(std::is_same_v<WithConst, const int>);
    static_assert(std::is_same_v<WithPtr, int*>);
    static_assert(std::is_same_v<WithRef, int&>);

    std::cout << "add_pointer_t<int> == int*: "
              << std::is_same_v<WithPtr, int*> << "\n";

    // Decay (removes refs, cv, array-to-pointer, function-to-pointer)
    using Decayed = std::decay_t<const int&>;
    static_assert(std::is_same_v<Decayed, int>);

    using ArrayDecayed = std::decay_t<int[5]>;
    static_assert(std::is_same_v<ArrayDecayed, int*>);

    std::cout << "decay_t<int[5]> == int*: "
              << std::is_same_v<ArrayDecayed, int*> << "\n";
}

// ============================================================================
// std::conditional - Compile-time Type Selection
// ============================================================================

template<bool Condition>
struct TypeSelector {
    using Type = std::conditional_t<Condition, int, double>;
};

// Nested conditionals - template alias must be at namespace scope
template<typename T>
using NumericType = std::conditional_t<
    std::is_integral_v<T>,
    long long,
    std::conditional_t<std::is_floating_point_v<T>, long double, void>>;

void conditional_demo() {
    std::cout << "\n=== std::conditional ===\n";

    // Select type based on condition
    using T1 = std::conditional_t<true, int, double>;   // int
    using T2 = std::conditional_t<false, int, double>;  // double

    static_assert(std::is_same_v<T1, int>);
    static_assert(std::is_same_v<T2, double>);

    std::cout << "conditional_t<true, int, double> == int: "
              << std::is_same_v<T1, int> << "\n";
    std::cout << "conditional_t<false, int, double> == double: "
              << std::is_same_v<T2, double> << "\n";

    // Practical example: choose type based on size
    using SmallInt = std::conditional_t<sizeof(void*) == 4, int32_t, int64_t>;
    std::cout << "SmallInt size: " << sizeof(SmallInt) << " bytes\n";

    // Nested conditionals (NumericType defined at namespace scope above)
    static_assert(std::is_same_v<NumericType<int>, long long>);
    static_assert(std::is_same_v<NumericType<float>, long double>);
    std::cout << "NumericType<int> == long long: "
              << std::is_same_v<NumericType<int>, long long> << "\n";
}

// ============================================================================
// std::enable_if - SFINAE-based Overloading
// ============================================================================

// Enable only for integral types
template<typename T, std::enable_if_t<std::is_integral_v<T>, int> = 0>
T double_value(T x) {
    std::cout << "  (integral version)\n";
    return x * 2;
}

// Enable only for floating-point types
template<typename T, std::enable_if_t<std::is_floating_point_v<T>, int> = 0>
T double_value(T x) {
    std::cout << "  (floating-point version)\n";
    return x * 2.0;
}

// Return type SFINAE
template<typename T>
std::enable_if_t<std::is_integral_v<T>, bool>
is_even(T x) {
    return x % 2 == 0;
}

void enable_if_demo() {
    std::cout << "\n=== std::enable_if (SFINAE) ===\n";

    std::cout << "double_value(5): " << double_value(5);
    std::cout << "double_value(3.14): " << double_value(3.14);

    std::cout << "is_even(4): " << std::boolalpha << is_even(4) << "\n";
    std::cout << "is_even(5): " << is_even(5) << "\n";
    // is_even(3.14);  // Would not compile - not integral
}

// ============================================================================
// C++20 Concepts - Modern Alternative to enable_if
// ============================================================================

// Define a concept
template<typename T>
concept Numeric = std::is_arithmetic_v<T>;

template<typename T>
concept Addable = requires(T a, T b) {
    { a + b } -> std::same_as<T>;
};

// Use concept in template parameter
template<Numeric T>
T triple_value(T x) {
    return x * 3;
}

// Use concept with requires clause
template<typename T>
    requires Addable<T>
T add_values(T a, T b) {
    return a + b;
}

// Abbreviated function template with concept
void print_integral(std::integral auto value) {
    std::cout << "Integral: " << value << "\n";
}

void concepts_demo() {
    std::cout << "\n=== C++20 Concepts ===\n";

    std::cout << "triple_value(5): " << triple_value(5) << "\n";
    std::cout << "triple_value(3.14): " << triple_value(3.14) << "\n";
    // triple_value("hello");  // Would not compile - not Numeric

    std::cout << "add_values(1, 2): " << add_values(1, 2) << "\n";
    std::cout << "add_values(1.5, 2.5): " << add_values(1.5, 2.5) << "\n";

    print_integral(42);
    print_integral(100L);
    // print_integral(3.14);  // Would not compile
}

// ============================================================================
// Standard Concepts
// ============================================================================

void standard_concepts_demo() {
    std::cout << "\n=== Standard Concepts ===\n";

    // Core concepts
    std::cout << "same_as<int, int>: " << std::same_as<int, int> << "\n";
    std::cout << "derived_from<std::string, std::string>: "
              << std::derived_from<std::string, std::string> << "\n";
    std::cout << "convertible_to<int, double>: "
              << std::convertible_to<int, double> << "\n";

    // Type concepts
    std::cout << "integral<int>: " << std::integral<int> << "\n";
    std::cout << "floating_point<double>: " << std::floating_point<double> << "\n";
    std::cout << "signed_integral<int>: " << std::signed_integral<int> << "\n";
    std::cout << "unsigned_integral<size_t>: " << std::unsigned_integral<size_t> << "\n";

    // Object concepts
    std::cout << "movable<std::string>: " << std::movable<std::string> << "\n";
    std::cout << "copyable<std::string>: " << std::copyable<std::string> << "\n";
    std::cout << "regular<int>: " << std::regular<int> << "\n";
}

// ============================================================================
// Practical: Type-Safe Serialization
// ============================================================================

template<typename T>
concept Serializable = std::is_trivially_copyable_v<T> && !std::is_pointer_v<T>;

template<Serializable T>
std::vector<std::byte> serialize(const T& value) {
    std::vector<std::byte> bytes(sizeof(T));
    std::memcpy(bytes.data(), &value, sizeof(T));
    return bytes;
}

template<Serializable T>
T deserialize(const std::vector<std::byte>& bytes) {
    T value;
    std::memcpy(&value, bytes.data(), sizeof(T));
    return value;
}

void serialization_demo() {
    std::cout << "\n=== Type-Safe Serialization ===\n";

    int x = 42;
    auto bytes = serialize(x);
    std::cout << "Serialized " << x << " to " << bytes.size() << " bytes\n";

    auto y = deserialize<int>(bytes);
    std::cout << "Deserialized to " << y << "\n";

    double d = 3.14159;
    auto dbytes = serialize(d);
    auto d2 = deserialize<double>(dbytes);
    std::cout << "Double roundtrip: " << d << " -> " << d2 << "\n";

    // serialize("hello");  // Would not compile - not trivially copyable
    // serialize(new int);  // Would not compile - is pointer
}

int main() {
    type_property_traits();
    class_property_traits();
    type_transformations();
    conditional_demo();
    enable_if_demo();
    concepts_demo();
    standard_concepts_demo();
    serialization_demo();

    return 0;
}

// Key points:
// - Type traits query and transform types at compile time
// - Use _v suffix for value traits (is_integral_v<T>)
// - Use _t suffix for type traits (remove_reference_t<T>)
// - std::conditional selects types based on compile-time conditions
// - std::enable_if enables SFINAE-based overloading
// - C++20 concepts provide cleaner syntax for type constraints
// - Prefer concepts over enable_if in modern C++ code
