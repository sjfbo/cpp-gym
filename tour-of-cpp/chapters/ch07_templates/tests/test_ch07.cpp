// Chapter 7 Tests: Templates
// These tests verify understanding of C++ template concepts.

#include <algorithm>
#include <array>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <memory>
#include <string>
#include <type_traits>
#include <vector>

// ============================================================================
// Section 7.2: Parameterized Types (Class Templates)
// ============================================================================

// Simple class template for testing
template<typename T>
class Box {
    T value_;
public:
    explicit Box(T val) : value_{std::move(val)} {}
    T& get() { return value_; }
    const T& get() const { return value_; }
    void set(T val) { value_ = std::move(val); }
};

// Template with multiple parameters
template<typename T, typename U>
struct Pair {
    T first;
    U second;

    bool operator==(const Pair& other) const {
        return first == other.first && second == other.second;
    }
};

// Deduction guide
template<typename T, typename U>
Pair(T, U) -> Pair<T, U>;

// Template with non-type parameter
template<typename T, size_t N>
class FixedArray {
    T data_[N]{};
public:
    T& operator[](size_t i) { return data_[i]; }
    const T& operator[](size_t i) const { return data_[i]; }
    constexpr size_t size() const { return N; }
};

TEST_CASE("Class templates: basic instantiation", "[ch07][class_templates]") {
    Box<int> int_box{42};
    REQUIRE(int_box.get() == 42);

    Box<std::string> str_box{"hello"};
    REQUIRE(str_box.get() == "hello");

    Box<double> dbl_box{3.14};
    REQUIRE_THAT(dbl_box.get(), Catch::Matchers::WithinRel(3.14, 0.001));
}

TEST_CASE("Class templates: modification", "[ch07][class_templates]") {
    Box<int> box{10};
    box.set(20);
    REQUIRE(box.get() == 20);

    box.get() = 30;  // Modify through reference
    REQUIRE(box.get() == 30);
}

TEST_CASE("Class templates: multiple type parameters", "[ch07][class_templates]") {
    Pair<int, std::string> p{42, "answer"};
    REQUIRE(p.first == 42);
    REQUIRE(p.second == "answer");

    Pair<double, bool> p2{3.14, true};
    REQUIRE_THAT(p2.first, Catch::Matchers::WithinRel(3.14, 0.001));
    REQUIRE(p2.second == true);
}

TEST_CASE("Class templates: CTAD (C++17)", "[ch07][class_templates][ctad]") {
    Pair p{1, 2.0};  // Deduces Pair<int, double>
    static_assert(std::is_same_v<decltype(p.first), int>);
    static_assert(std::is_same_v<decltype(p.second), double>);

    REQUIRE(p.first == 1);
    REQUIRE_THAT(p.second, Catch::Matchers::WithinRel(2.0, 0.001));
}

TEST_CASE("Class templates: non-type parameters", "[ch07][class_templates]") {
    FixedArray<int, 5> arr;
    arr[0] = 10;
    arr[4] = 50;

    REQUIRE(arr.size() == 5);
    REQUIRE(arr[0] == 10);
    REQUIRE(arr[4] == 50);

    // Compile-time size check
    static_assert(FixedArray<double, 10>{}.size() == 10);
}

// ============================================================================
// Section 7.3: Function Templates
// ============================================================================

// Basic function template
template<typename T>
T max_of(T a, T b) {
    return (a > b) ? a : b;
}

template<typename T>
T min_of(T a, T b) {
    return (a < b) ? a : b;
}

// Function template with multiple parameters
template<typename T, typename U>
auto add(T a, U b) {
    return a + b;
}

// Function template with container
template<typename Container>
auto sum(const Container& c) {
    typename Container::value_type result{};
    for (const auto& elem : c) {
        result += elem;
    }
    return result;
}

// Iterator-based algorithm
template<typename Iterator, typename T>
Iterator my_find(Iterator begin, Iterator end, const T& value) {
    while (begin != end) {
        if (*begin == value) {
            return begin;
        }
        ++begin;
    }
    return end;
}

template<typename Iterator, typename T>
size_t my_count(Iterator begin, Iterator end, const T& value) {
    size_t result = 0;
    while (begin != end) {
        if (*begin == value) {
            ++result;
        }
        ++begin;
    }
    return result;
}

template<typename Iterator, typename Predicate>
Iterator my_find_if(Iterator begin, Iterator end, Predicate pred) {
    while (begin != end) {
        if (pred(*begin)) {
            return begin;
        }
        ++begin;
    }
    return end;
}

TEST_CASE("Function templates: basic usage", "[ch07][function_templates]") {
    REQUIRE(max_of(3, 7) == 7);
    REQUIRE(max_of(10, 5) == 10);
    REQUIRE_THAT(max_of(3.14, 2.71), Catch::Matchers::WithinRel(3.14, 0.001));

    REQUIRE(min_of(3, 7) == 3);
    REQUIRE(min_of(10, 5) == 5);
}

TEST_CASE("Function templates: type deduction", "[ch07][function_templates]") {
    auto result = max_of(5, 10);  // T deduced as int
    static_assert(std::is_same_v<decltype(result), int>);
    REQUIRE(result == 10);

    auto dbl_result = max_of(1.5, 2.5);  // T deduced as double
    static_assert(std::is_same_v<decltype(dbl_result), double>);
}

TEST_CASE("Function templates: multiple type parameters", "[ch07][function_templates]") {
    auto result1 = add(1, 2.5);  // int + double = double
    static_assert(std::is_same_v<decltype(result1), double>);
    REQUIRE_THAT(result1, Catch::Matchers::WithinRel(3.5, 0.001));

    auto result2 = add(std::string{"hello"}, std::string{" world"});
    REQUIRE(result2 == "hello world");
}

TEST_CASE("Function templates: container algorithms", "[ch07][function_templates]") {
    std::vector<int> vec = {1, 2, 3, 4, 5};
    REQUIRE(sum(vec) == 15);

    std::array<double, 3> arr = {1.1, 2.2, 3.3};
    REQUIRE_THAT(sum(arr), Catch::Matchers::WithinRel(6.6, 0.001));
}

TEST_CASE("Function templates: iterator-based find", "[ch07][function_templates]") {
    std::vector<int> vec = {1, 2, 3, 4, 5};

    auto it = my_find(vec.begin(), vec.end(), 3);
    REQUIRE(it != vec.end());
    REQUIRE(*it == 3);

    it = my_find(vec.begin(), vec.end(), 99);
    REQUIRE(it == vec.end());
}

TEST_CASE("Function templates: iterator-based count", "[ch07][function_templates]") {
    std::vector<int> vec = {1, 2, 2, 3, 2, 4};

    REQUIRE(my_count(vec.begin(), vec.end(), 2) == 3);
    REQUIRE(my_count(vec.begin(), vec.end(), 1) == 1);
    REQUIRE(my_count(vec.begin(), vec.end(), 99) == 0);
}

TEST_CASE("Function templates: predicate-based find_if", "[ch07][function_templates]") {
    std::vector<int> vec = {1, 3, 5, 8, 9, 11};

    auto is_even = [](int x) { return x % 2 == 0; };
    auto it = my_find_if(vec.begin(), vec.end(), is_even);

    REQUIRE(it != vec.end());
    REQUIRE(*it == 8);

    auto gt_100 = [](int x) { return x > 100; };
    it = my_find_if(vec.begin(), vec.end(), gt_100);
    REQUIRE(it == vec.end());
}

// ============================================================================
// Section 7.3.3: Lambda Templates
// ============================================================================

TEST_CASE("Generic lambdas: auto parameters", "[ch07][lambda_templates]") {
    auto add_lambda = [](auto a, auto b) { return a + b; };

    REQUIRE(add_lambda(1, 2) == 3);
    REQUIRE_THAT(add_lambda(1.5, 2.5), Catch::Matchers::WithinRel(4.0, 0.001));
    REQUIRE(add_lambda(std::string{"a"}, std::string{"b"}) == "ab");
}

TEST_CASE("Generic lambdas: with algorithms", "[ch07][lambda_templates]") {
    std::vector<int> vec = {3, 1, 4, 1, 5, 9};

    auto less_than = [](auto a, auto b) { return a < b; };
    std::sort(vec.begin(), vec.end(), less_than);

    REQUIRE(vec[0] == 1);
    REQUIRE(vec[vec.size() - 1] == 9);
}

TEST_CASE("Template lambdas: explicit template parameters (C++20)", "[ch07][lambda_templates]") {
    auto typed_size = []<typename T>(const T&) { return sizeof(T); };

    REQUIRE(typed_size(42) == sizeof(int));
    REQUIRE(typed_size(3.14) == sizeof(double));
    REQUIRE(typed_size('x') == sizeof(char));
}

TEST_CASE("Template lambdas: type constraints with if constexpr", "[ch07][lambda_templates]") {
    auto process = []<typename T>(T value) {
        if constexpr (std::is_integral_v<T>) {
            return value * 2;
        } else if constexpr (std::is_floating_point_v<T>) {
            return value * 2.0;
        } else {
            return value;
        }
    };

    REQUIRE(process(5) == 10);
    REQUIRE_THAT(process(2.5), Catch::Matchers::WithinRel(5.0, 0.001));
}

// ============================================================================
// Section 7.4: Template Mechanisms
// ============================================================================

// Specialization example
template<typename T>
struct TypeName {
    static constexpr const char* value = "unknown";
};

template<>
struct TypeName<int> {
    static constexpr const char* value = "int";
};

template<>
struct TypeName<double> {
    static constexpr const char* value = "double";
};

template<>
struct TypeName<std::string> {
    static constexpr const char* value = "std::string";
};

// Partial specialization for pointers
template<typename T>
struct TypeName<T*> {
    static constexpr const char* value = "pointer";
};

// Partial specialization for vectors
template<typename T>
struct TypeName<std::vector<T>> {
    static constexpr const char* value = "std::vector";
};

// SFINAE trait: has_size
template<typename T, typename = void>
struct has_size : std::false_type {};

template<typename T>
struct has_size<T, std::void_t<decltype(std::declval<T>().size())>> : std::true_type {};

template<typename T>
inline constexpr bool has_size_v = has_size<T>::value;

// if constexpr example
template<typename T>
auto double_value(T value) {
    if constexpr (std::is_arithmetic_v<T>) {
        return value * 2;
    } else {
        return value;
    }
}

TEST_CASE("Template specialization: full specialization", "[ch07][template_mechanisms]") {
    REQUIRE(std::string_view{TypeName<int>::value} == "int");
    REQUIRE(std::string_view{TypeName<double>::value} == "double");
    REQUIRE(std::string_view{TypeName<std::string>::value} == "std::string");
    REQUIRE(std::string_view{TypeName<char>::value} == "unknown");
}

TEST_CASE("Template specialization: partial specialization", "[ch07][template_mechanisms]") {
    REQUIRE(std::string_view{TypeName<int*>::value} == "pointer");
    REQUIRE(std::string_view{TypeName<double*>::value} == "pointer");
    REQUIRE(std::string_view{TypeName<std::vector<int>>::value} == "std::vector");
    REQUIRE(std::string_view{TypeName<std::vector<std::string>>::value} == "std::vector");
}

TEST_CASE("SFINAE: has_size trait", "[ch07][template_mechanisms][sfinae]") {
    REQUIRE(has_size_v<std::vector<int>> == true);
    REQUIRE(has_size_v<std::string> == true);
    REQUIRE(has_size_v<std::array<int, 5>> == true);
    REQUIRE(has_size_v<int> == false);
    REQUIRE(has_size_v<double*> == false);
}

TEST_CASE("if constexpr: compile-time branching", "[ch07][template_mechanisms]") {
    REQUIRE(double_value(5) == 10);
    REQUIRE_THAT(double_value(2.5), Catch::Matchers::WithinRel(5.0, 0.001));

    // Non-arithmetic type just returns the value
    std::string str = "hello";
    REQUIRE(double_value(str) == "hello");
}

// Type traits tests
TEST_CASE("Type traits: basic queries", "[ch07][template_mechanisms][type_traits]") {
    REQUIRE(std::is_integral_v<int> == true);
    REQUIRE(std::is_integral_v<double> == false);

    REQUIRE(std::is_floating_point_v<double> == true);
    REQUIRE(std::is_floating_point_v<int> == false);

    REQUIRE(std::is_arithmetic_v<int> == true);
    REQUIRE(std::is_arithmetic_v<double> == true);
    REQUIRE(std::is_arithmetic_v<std::string> == false);

    REQUIRE(std::is_pointer_v<int*> == true);
    REQUIRE(std::is_pointer_v<int> == false);

    REQUIRE(std::is_same_v<int, int> == true);
    REQUIRE(std::is_same_v<int, long> == false);
}

TEST_CASE("Type traits: type transformations", "[ch07][template_mechanisms][type_traits]") {
    static_assert(std::is_same_v<std::remove_const_t<const int>, int>);
    static_assert(std::is_same_v<std::remove_pointer_t<int*>, int>);
    static_assert(std::is_same_v<std::remove_reference_t<int&>, int>);
    static_assert(std::is_same_v<std::add_pointer_t<int>, int*>);
    static_assert(std::is_same_v<std::add_const_t<int>, const int>);

    REQUIRE(true);  // Static asserts passed
}

// ============================================================================
// Compile-time computation
// ============================================================================

template<int N>
struct Factorial {
    static constexpr int value = N * Factorial<N - 1>::value;
};

template<>
struct Factorial<0> {
    static constexpr int value = 1;
};

constexpr int factorial(int n) {
    return (n <= 1) ? 1 : n * factorial(n - 1);
}

TEST_CASE("Compile-time computation: factorial", "[ch07][template_mechanisms]") {
    static_assert(Factorial<5>::value == 120);
    static_assert(factorial(5) == 120);
    static_assert(factorial(0) == 1);
    static_assert(factorial(1) == 1);

    REQUIRE(Factorial<6>::value == 720);
    REQUIRE(factorial(6) == 720);
}

// Variable template
template<typename T>
constexpr T pi = T(3.14159265358979323846);

TEST_CASE("Variable templates", "[ch07][template_mechanisms]") {
    // Use double comparison to avoid float-to-double promotion warning in Catch2
    REQUIRE_THAT(static_cast<double>(pi<float>), Catch::Matchers::WithinRel(3.14159, 0.0001));
    REQUIRE_THAT(pi<double>, Catch::Matchers::WithinRel(3.14159265358979, 0.0000001));
}

// ============================================================================
// Edge cases and interesting scenarios
// ============================================================================

TEST_CASE("Templates: with smart pointers", "[ch07][class_templates]") {
    Box<std::unique_ptr<int>> box{std::make_unique<int>(42)};
    REQUIRE(*box.get() == 42);

    *box.get() = 100;
    REQUIRE(*box.get() == 100);
}

TEST_CASE("Templates: nested templates", "[ch07][class_templates]") {
    Box<Box<int>> nested{Box<int>{42}};
    REQUIRE(nested.get().get() == 42);

    std::vector<Box<int>> vec_of_boxes;
    vec_of_boxes.push_back(Box<int>{1});
    vec_of_boxes.push_back(Box<int>{2});
    REQUIRE(vec_of_boxes[0].get() == 1);
    REQUIRE(vec_of_boxes[1].get() == 2);
}

TEST_CASE("Templates: with const types", "[ch07][class_templates]") {
    const Box<int> const_box{42};
    REQUIRE(const_box.get() == 42);
    // const_box.set(10);  // Would not compile

    Box<const int*> ptr_to_const{nullptr};
    static_assert(std::is_same_v<decltype(ptr_to_const.get()), const int*&>);
}
