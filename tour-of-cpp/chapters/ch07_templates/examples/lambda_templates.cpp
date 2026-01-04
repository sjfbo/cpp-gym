// Lambda Templates - Generic Lambdas and Template Lambdas
// Book reference: 7.3.3 Lambda Templates
//
// Lambdas evolved significantly from C++11 to C++20:
// - C++11: Lambdas with explicit types
// - C++14: Generic lambdas with 'auto' parameters
// - C++20: Template lambdas with explicit template parameters

#include <algorithm>
#include <functional>
#include <iostream>
#include <string>
#include <type_traits>
#include <vector>

// ----------------------------------------------------------------------------
// C++11 Style: Lambdas with Explicit Types
// ----------------------------------------------------------------------------

void demonstrate_cpp11_lambdas() {
    std::cout << "=== C++11 Style Lambdas (Explicit Types) ===\n";

    // Basic lambda with capture
    int multiplier = 3;
    auto times_three = [multiplier](int x) { return x * multiplier; };
    std::cout << "times_three(4) = " << times_three(4) << "\n";

    // Mutable lambda (can modify captured copies)
    int counter = 0;
    auto increment = [counter]() mutable { return ++counter; };
    std::cout << "increment(): " << increment() << ", " << increment() << ", " << increment() << "\n";
    std::cout << "Original counter still: " << counter << "\n";

    // Capture by reference
    auto increment_ref = [&counter]() { return ++counter; };
    increment_ref();
    increment_ref();
    std::cout << "After increment_ref() twice: " << counter << "\n";
}

// ----------------------------------------------------------------------------
// C++14 Style: Generic Lambdas with 'auto'
// ----------------------------------------------------------------------------

void demonstrate_generic_lambdas() {
    std::cout << "\n=== C++14 Generic Lambdas (auto parameters) ===\n";

    // Generic lambda - works with any type
    auto print = [](const auto& value) {
        std::cout << value << "\n";
    };

    std::cout << "print with different types:\n";
    print(42);
    print(3.14);
    print("hello");
    print(std::string{"world"});

    // Generic binary operation
    auto add = [](auto a, auto b) {
        return a + b;
    };

    std::cout << "add(1, 2) = " << add(1, 2) << "\n";
    std::cout << "add(1.5, 2.5) = " << add(1.5, 2.5) << "\n";
    std::cout << "add(string, string) = " << add(std::string{"hello "}, std::string{"world"}) << "\n";

    // Generic comparison
    auto less_than = [](const auto& a, const auto& b) {
        return a < b;
    };

    std::cout << "less_than(3, 5) = " << std::boolalpha << less_than(3, 5) << "\n";
    std::cout << "less_than(\"apple\", \"banana\") = " << less_than("apple", "banana") << "\n";

    // Using generic lambdas with algorithms
    std::vector<int> nums = {5, 2, 8, 1, 9, 3};
    std::sort(nums.begin(), nums.end(), [](auto a, auto b) { return a > b; });
    std::cout << "Sorted descending: ";
    for (auto n : nums) std::cout << n << " ";
    std::cout << "\n";
}

// ----------------------------------------------------------------------------
// C++20 Style: Template Lambdas
// ----------------------------------------------------------------------------

void demonstrate_template_lambdas() {
    std::cout << "\n=== C++20 Template Lambdas ===\n";

    // Template lambda with explicit template parameter
    auto typed_print = []<typename T>(const T& value) {
        std::cout << "Type size: " << sizeof(T) << ", value: " << value << "\n";
    };

    typed_print(42);
    typed_print(3.14);
    typed_print('x');

    // Template lambda with type constraints using if constexpr
    auto process = []<typename T>(T value) {
        if constexpr (std::is_integral_v<T>) {
            std::cout << value << " is integral, doubled: " << value * 2 << "\n";
        } else if constexpr (std::is_floating_point_v<T>) {
            std::cout << value << " is floating-point, squared: " << value * value << "\n";
        } else {
            std::cout << value << " is other type\n";
        }
    };

    process(10);
    process(3.14);
    process(std::string{"hello"});

    // Template lambda with multiple type parameters
    auto make_pair = []<typename T, typename U>(T first, U second) {
        struct Pair {
            T a;
            U b;
        };
        return Pair{first, second};
    };

    auto p = make_pair(42, "answer");
    std::cout << "Pair: (" << p.a << ", " << p.b << ")\n";

    // Template lambda with non-type template parameter
    auto repeat = []<int N>(auto value) {
        for (int i = 0; i < N; ++i) {
            std::cout << value << " ";
        }
        std::cout << "\n";
    };

    repeat.template operator()<3>("hello");
    repeat.template operator()<5>(42);
}

// ----------------------------------------------------------------------------
// Perfect Forwarding in Lambdas
// ----------------------------------------------------------------------------

void demonstrate_forwarding_lambdas() {
    std::cout << "\n=== Perfect Forwarding in Lambdas ===\n";

    // C++20 perfect forwarding lambda
    auto forwarder = []<typename... Args>(Args&&... args) {
        return std::make_tuple(std::forward<Args>(args)...);
    };

    [[maybe_unused]] auto tuple1 = forwarder(1, 2.0, "three");
    std::cout << "Created tuple with 3 elements\n";

    // Generic factory pattern
    auto make = []<typename T, typename... Args>(Args&&... args) {
        return T{std::forward<Args>(args)...};
    };

    auto str = make.template operator()<std::string>("hello");
    std::cout << "Made string: " << str << "\n";

    auto vec = make.template operator()<std::vector<int>>(std::initializer_list<int>{1, 2, 3});
    std::cout << "Made vector with " << vec.size() << " elements\n";
}

// ----------------------------------------------------------------------------
// Lambdas with Captures and Templates
// ----------------------------------------------------------------------------

void demonstrate_capturing_template_lambdas() {
    std::cout << "\n=== Template Lambdas with Captures ===\n";

    int scale = 10;
    std::string prefix = "Result: ";

    // Template lambda with value capture
    auto scaled_print = [scale, prefix]<typename T>(T value) {
        if constexpr (std::is_arithmetic_v<T>) {
            std::cout << prefix << (value * scale) << "\n";
        } else {
            std::cout << prefix << value << "\n";
        }
    };

    scaled_print(5);
    scaled_print(3.14);
    scaled_print("hello");

    // Init-capture with template lambda (C++14 init-capture + C++20 template)
    auto counter = [count = 0]<typename T>(T value) mutable {
        ++count;
        std::cout << "Call #" << count << ": " << value << "\n";
        return count;
    };

    counter(42);
    counter("hello");
    counter(3.14);
}

// ----------------------------------------------------------------------------
// Practical Use Cases
// ----------------------------------------------------------------------------

void demonstrate_practical_uses() {
    std::cout << "\n=== Practical Template Lambda Use Cases ===\n";

    // Type-safe event handler
    auto on_event = []<typename EventType>([[maybe_unused]] EventType event) {
        std::cout << "Handling event of size " << sizeof(EventType) << "\n";
    };

    struct ClickEvent { int x, y; };
    struct KeyEvent { char key; };

    on_event(ClickEvent{100, 200});
    on_event(KeyEvent{'a'});

    // Generic container printer
    auto print_all = []<typename Container>(const Container& c, std::string_view name) {
        std::cout << name << ": [";
        bool first = true;
        for (const auto& elem : c) {
            if (!first) std::cout << ", ";
            std::cout << elem;
            first = false;
        }
        std::cout << "]\n";
    };

    std::vector<int> ints = {1, 2, 3};
    std::vector<std::string> strs = {"a", "b", "c"};
    print_all(ints, "ints");
    print_all(strs, "strs");

    // Compile-time dispatch
    auto type_name = []<typename T>() -> std::string_view {
        if constexpr (std::is_same_v<T, int>) return "int";
        else if constexpr (std::is_same_v<T, double>) return "double";
        else if constexpr (std::is_same_v<T, std::string>) return "string";
        else return "unknown";
    };

    std::cout << "type_name<int>() = " << type_name.template operator()<int>() << "\n";
    std::cout << "type_name<double>() = " << type_name.template operator()<double>() << "\n";
    std::cout << "type_name<std::string>() = " << type_name.template operator()<std::string>() << "\n";
}

// ----------------------------------------------------------------------------
// Comparison: Generic Lambda vs Template Lambda
// ----------------------------------------------------------------------------

void demonstrate_comparison() {
    std::cout << "\n=== Generic Lambda vs Template Lambda ===\n";

    // Generic lambda (C++14) - can't access T directly
    auto generic = [](auto x, auto y) {
        // Can't easily check if x and y are the same type
        return x + y;
    };

    // Template lambda (C++20) - full control over types
    auto templated = []<typename T>(T x, T y) {
        // We know both are the same type T
        static_assert(std::is_same_v<decltype(x), decltype(y)>);
        return x + y;
    };

    std::cout << "generic(1, 2) = " << generic(1, 2) << "\n";
    std::cout << "templated(1, 2) = " << templated(1, 2) << "\n";

    // generic(1, 2.0);  // Works - different types allowed
    // templated(1, 2.0);  // Would not compile - requires same type

    // When to use which:
    std::cout << "\nGeneric lambda: flexible, accepts any types\n";
    std::cout << "Template lambda: explicit type control, enables constraints\n";
}

// ----------------------------------------------------------------------------
// Main function
// ----------------------------------------------------------------------------

int main() {
    demonstrate_cpp11_lambdas();
    demonstrate_generic_lambdas();
    demonstrate_template_lambdas();
    demonstrate_forwarding_lambdas();
    demonstrate_capturing_template_lambdas();
    demonstrate_practical_uses();
    demonstrate_comparison();

    std::cout << "\n=== Summary ===\n";
    std::cout << "C++11: [](int x) { ... }         - explicit types\n";
    std::cout << "C++14: [](auto x) { ... }        - generic, type deduced\n";
    std::cout << "C++20: []<typename T>(T x) { ... } - template, explicit type parameter\n";

    return 0;
}
