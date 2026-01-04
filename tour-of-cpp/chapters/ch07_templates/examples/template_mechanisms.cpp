// Template Mechanisms - Specialization, SFINAE, and if constexpr
// Book reference: 7.4 Template Mechanisms
//
// Advanced template features that enable compile-time customization:
// - Template specialization (full and partial)
// - SFINAE (Substitution Failure Is Not An Error)
// - if constexpr for compile-time branching
// - Type traits for compile-time type inspection

#include <array>
#include <iostream>
#include <string>
#include <type_traits>
#include <vector>

// ----------------------------------------------------------------------------
// Full Template Specialization
// ----------------------------------------------------------------------------

// Primary template - general case
template<typename T>
struct TypeInfo {
    static constexpr const char* name = "unknown";
    static constexpr size_t size = sizeof(T);
    static void print() {
        std::cout << "TypeInfo<" << name << ">: " << size << " bytes\n";
    }
};

// Full specialization for int
template<>
struct TypeInfo<int> {
    static constexpr const char* name = "int";
    static constexpr size_t size = sizeof(int);
    static void print() {
        std::cout << "TypeInfo<int>: " << size << " bytes, signed integer\n";
    }
};

// Full specialization for double
template<>
struct TypeInfo<double> {
    static constexpr const char* name = "double";
    static constexpr size_t size = sizeof(double);
    static void print() {
        std::cout << "TypeInfo<double>: " << size << " bytes, IEEE 754\n";
    }
};

// Full specialization for bool
template<>
struct TypeInfo<bool> {
    static constexpr const char* name = "bool";
    static constexpr size_t size = sizeof(bool);
    static void print() {
        std::cout << "TypeInfo<bool>: " << size << " bytes, true/false only\n";
    }
};

// Full specialization for std::string
template<>
struct TypeInfo<std::string> {
    static constexpr const char* name = "std::string";
    static constexpr size_t size = sizeof(std::string);
    static void print() {
        std::cout << "TypeInfo<std::string>: " << size << " bytes (object size, not content)\n";
    }
};

// ----------------------------------------------------------------------------
// Partial Template Specialization
// ----------------------------------------------------------------------------

// Primary template for a pair
template<typename T, typename U>
struct PairTraits {
    static constexpr bool same_type = false;
    static std::string description() {
        return "heterogeneous pair";
    }
};

// Partial specialization when both types are the same
template<typename T>
struct PairTraits<T, T> {
    static constexpr bool same_type = true;
    static std::string description() {
        return "homogeneous pair";
    }
};

// Partial specialization for pointer types
template<typename T>
struct TypeInfo<T*> {
    static constexpr const char* name = "pointer";
    static constexpr size_t size = sizeof(T*);
    static void print() {
        std::cout << "TypeInfo<pointer>: " << size << " bytes, points to "
                  << sizeof(T) << "-byte type\n";
    }
};

// Partial specialization for arrays
template<typename T, size_t N>
struct TypeInfo<T[N]> {
    static constexpr const char* name = "array";
    static constexpr size_t size = sizeof(T[N]);
    static void print() {
        std::cout << "TypeInfo<array>: " << N << " elements of "
                  << sizeof(T) << " bytes each = " << size << " bytes total\n";
    }
};

// Partial specialization for std::vector
template<typename T>
struct TypeInfo<std::vector<T>> {
    static constexpr const char* name = "std::vector";
    static constexpr size_t size = sizeof(std::vector<T>);
    static void print() {
        std::cout << "TypeInfo<std::vector>: object is " << size
                  << " bytes, elements are " << sizeof(T) << " bytes each\n";
    }
};

// ----------------------------------------------------------------------------
// SFINAE (Substitution Failure Is Not An Error)
// ----------------------------------------------------------------------------

// SFINAE helper: check if type has a size() method
template<typename T, typename = void>
struct has_size : std::false_type {};

template<typename T>
struct has_size<T, std::void_t<decltype(std::declval<T>().size())>> : std::true_type {};

// Helper variable template (C++17)
template<typename T>
inline constexpr bool has_size_v = has_size<T>::value;

// SFINAE-based function overload (pre-C++17 style)
template<typename T>
auto get_size_sfinae(const T& container)
    -> decltype(container.size()) {
    return container.size();
}

// Fallback when .size() is not available
template<typename T>
size_t get_size_sfinae(const T (&arr)[sizeof(T)]) {
    return sizeof(arr) / sizeof(arr[0]);
}

// Using enable_if for SFINAE
template<typename T>
std::enable_if_t<std::is_integral_v<T>, T>
double_value(T value) {
    std::cout << "(integral version) ";
    return value * 2;
}

template<typename T>
std::enable_if_t<std::is_floating_point_v<T>, T>
double_value(T value) {
    std::cout << "(floating-point version) ";
    return value * 2.0;
}

// ----------------------------------------------------------------------------
// if constexpr (C++17) - Compile-time Branching
// ----------------------------------------------------------------------------

// Much cleaner than SFINAE for conditional behavior
template<typename T>
auto process_value(T value) {
    if constexpr (std::is_integral_v<T>) {
        std::cout << "Processing integer: " << value << "\n";
        return value * 2;
    } else if constexpr (std::is_floating_point_v<T>) {
        std::cout << "Processing float: " << value << "\n";
        return value * 2.0;
    } else if constexpr (std::is_same_v<T, std::string>) {
        std::cout << "Processing string: " << value << "\n";
        return value + value;  // Concatenate
    } else {
        std::cout << "Processing unknown type\n";
        return value;
    }
}

// Compile-time container size
template<typename T>
constexpr size_t get_size(const T& container) {
    if constexpr (requires { container.size(); }) {
        return container.size();
    } else if constexpr (std::is_array_v<T>) {
        return std::extent_v<T>;
    } else {
        return 0;
    }
}

// if constexpr with type traits
template<typename T>
void describe_type() {
    std::cout << "Type analysis:\n";

    if constexpr (std::is_pointer_v<T>) {
        std::cout << "  - Is a pointer\n";
        using PointeeType = std::remove_pointer_t<T>;
        std::cout << "  - Points to type of size " << sizeof(PointeeType) << "\n";
    }

    if constexpr (std::is_reference_v<T>) {
        std::cout << "  - Is a reference\n";
    }

    if constexpr (std::is_const_v<std::remove_reference_t<T>>) {
        std::cout << "  - Is const\n";
    }

    if constexpr (std::is_arithmetic_v<T>) {
        std::cout << "  - Is arithmetic\n";
        if constexpr (std::is_signed_v<T>) {
            std::cout << "  - Is signed\n";
        } else {
            std::cout << "  - Is unsigned\n";
        }
    }

    if constexpr (std::is_class_v<T>) {
        std::cout << "  - Is a class/struct\n";
    }
}

// ----------------------------------------------------------------------------
// Type Traits
// ----------------------------------------------------------------------------

template<typename T>
void print_type_traits() {
    std::cout << "Type traits for " << TypeInfo<T>::name << ":\n";
    std::cout << "  is_integral: " << std::boolalpha << std::is_integral_v<T> << "\n";
    std::cout << "  is_floating_point: " << std::is_floating_point_v<T> << "\n";
    std::cout << "  is_arithmetic: " << std::is_arithmetic_v<T> << "\n";
    std::cout << "  is_signed: " << std::is_signed_v<T> << "\n";
    std::cout << "  is_const: " << std::is_const_v<T> << "\n";
    std::cout << "  is_pointer: " << std::is_pointer_v<T> << "\n";
    std::cout << "  is_reference: " << std::is_reference_v<T> << "\n";
}

// ----------------------------------------------------------------------------
// Compile-time Computation with Templates
// ----------------------------------------------------------------------------

// Compile-time factorial using template recursion
template<int N>
struct Factorial {
    static constexpr int value = N * Factorial<N - 1>::value;
};

template<>
struct Factorial<0> {
    static constexpr int value = 1;
};

// Modern approach: constexpr function (preferred)
constexpr int factorial(int n) {
    return (n <= 1) ? 1 : n * factorial(n - 1);
}

// Compile-time Fibonacci
template<int N>
struct Fibonacci {
    static constexpr int value = Fibonacci<N - 1>::value + Fibonacci<N - 2>::value;
};

template<>
struct Fibonacci<0> {
    static constexpr int value = 0;
};

template<>
struct Fibonacci<1> {
    static constexpr int value = 1;
};

// ----------------------------------------------------------------------------
// Variable Templates (C++14)
// ----------------------------------------------------------------------------

// A constant that depends on a type
template<typename T>
constexpr T default_value = T{};

template<>
constexpr int default_value<int> = -1;

template<>
constexpr double default_value<double> = 0.0;

// Pi with appropriate precision for each type
template<typename T>
constexpr T pi = T(3.14159265358979323846);

// ----------------------------------------------------------------------------
// Main function demonstrating all concepts
// ----------------------------------------------------------------------------

int main() {
    std::cout << "=== Full Template Specialization ===\n";
    TypeInfo<int>::print();
    TypeInfo<double>::print();
    TypeInfo<bool>::print();
    TypeInfo<std::string>::print();
    TypeInfo<char>::print();  // Uses primary template ("unknown")

    std::cout << "\n=== Partial Template Specialization ===\n";
    TypeInfo<int*>::print();
    TypeInfo<double*>::print();
    TypeInfo<int[10]>::print();
    TypeInfo<std::vector<int>>::print();

    std::cout << "\nPairTraits<int, double>: "
              << PairTraits<int, double>::description() << "\n";
    std::cout << "PairTraits<int, int>: "
              << PairTraits<int, int>::description() << "\n";

    std::cout << "\n=== SFINAE: has_size trait ===\n";
    std::cout << "vector has size(): " << std::boolalpha << has_size_v<std::vector<int>> << "\n";
    std::cout << "string has size(): " << has_size_v<std::string> << "\n";
    std::cout << "int has size(): " << has_size_v<int> << "\n";

    std::cout << "\n=== enable_if SFINAE ===\n";
    std::cout << "double_value(5) = " << double_value(5) << "\n";
    std::cout << "double_value(3.14) = " << double_value(3.14) << "\n";

    std::cout << "\n=== if constexpr (C++17) ===\n";
    process_value(42);
    process_value(3.14);
    process_value(std::string{"hello"});

    std::cout << "\n=== Type Analysis with if constexpr ===\n";
    std::cout << "int:\n";
    describe_type<int>();
    std::cout << "\nconst int*:\n";
    describe_type<const int*>();
    std::cout << "\nstd::string:\n";
    describe_type<std::string>();

    std::cout << "\n=== Type Traits ===\n";
    print_type_traits<int>();
    std::cout << "\n";
    print_type_traits<double>();

    std::cout << "\n=== Compile-time Computation ===\n";
    std::cout << "Factorial<5>::value = " << Factorial<5>::value << "\n";
    std::cout << "factorial(5) = " << factorial(5) << "\n";
    std::cout << "Fibonacci<10>::value = " << Fibonacci<10>::value << "\n";

    // Compile-time verification
    static_assert(Factorial<5>::value == 120);
    static_assert(factorial(6) == 720);
    static_assert(Fibonacci<10>::value == 55);

    std::cout << "\n=== Variable Templates ===\n";
    std::cout << "default_value<int> = " << default_value<int> << "\n";
    std::cout << "default_value<double> = " << default_value<double> << "\n";
    std::cout << "default_value<char> = '" << default_value<char> << "'\n";

    std::cout << "pi<float> = " << pi<float> << "\n";
    std::cout << "pi<double> = " << pi<double> << "\n";

    std::cout << "\n=== Key Takeaways ===\n";
    std::cout << "1. Specialization: customize templates for specific types\n";
    std::cout << "2. SFINAE: enable/disable overloads based on type properties\n";
    std::cout << "3. if constexpr: cleaner compile-time branching (C++17)\n";
    std::cout << "4. Type traits: query type properties at compile time\n";
    std::cout << "5. Variable templates: type-dependent constants\n";

    return 0;
}
