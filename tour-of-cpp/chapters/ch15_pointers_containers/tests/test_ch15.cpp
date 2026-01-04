// Chapter 15 Tests: Pointers and Containers
// These tests verify understanding of smart pointers and utility containers.

#include <any>
#include <catch2/catch_test_macros.hpp>
#include <memory>
#include <optional>
#include <span>
#include <string>
#include <variant>
#include <vector>

// ============================================================================
// Section 15.2.1: unique_ptr
// ============================================================================

TEST_CASE("unique_ptr: basic creation and access", "[ch15][unique_ptr]") {
    auto ptr = std::make_unique<int>(42);

    REQUIRE(ptr != nullptr);
    REQUIRE(*ptr == 42);

    *ptr = 100;
    REQUIRE(*ptr == 100);
}

TEST_CASE("unique_ptr: ownership transfer", "[ch15][unique_ptr]") {
    auto ptr1 = std::make_unique<std::string>("hello");
    REQUIRE(ptr1 != nullptr);

    auto ptr2 = std::move(ptr1);
    REQUIRE(ptr1 == nullptr);
    REQUIRE(ptr2 != nullptr);
    REQUIRE(*ptr2 == "hello");
}

TEST_CASE("unique_ptr: reset and release", "[ch15][unique_ptr]") {
    auto ptr = std::make_unique<int>(42);

    int* raw = ptr.release();
    REQUIRE(ptr == nullptr);
    REQUIRE(*raw == 42);
    delete raw;

    ptr = std::make_unique<int>(100);
    ptr.reset();
    REQUIRE(ptr == nullptr);
}

TEST_CASE("unique_ptr: array support", "[ch15][unique_ptr]") {
    auto arr = std::make_unique<int[]>(5);

    for (int i = 0; i < 5; ++i) {
        arr[i] = i * 10;
    }

    REQUIRE(arr[0] == 0);
    REQUIRE(arr[2] == 20);
    REQUIRE(arr[4] == 40);
}

TEST_CASE("unique_ptr: custom deleter", "[ch15][unique_ptr]") {
    bool deleted = false;
    auto deleter = [&deleted](int* p) {
        deleted = true;
        delete p;
    };

    {
        std::unique_ptr<int, decltype(deleter)> ptr(new int{42}, deleter);
        REQUIRE(*ptr == 42);
        REQUIRE_FALSE(deleted);
    }

    REQUIRE(deleted);
}

// ============================================================================
// Section 15.2.2: shared_ptr
// ============================================================================

TEST_CASE("shared_ptr: reference counting", "[ch15][shared_ptr]") {
    auto sp1 = std::make_shared<int>(42);
    REQUIRE(sp1.use_count() == 1);

    auto sp2 = sp1;
    REQUIRE(sp1.use_count() == 2);
    REQUIRE(sp2.use_count() == 2);

    sp1.reset();
    REQUIRE(sp1 == nullptr);
    REQUIRE(sp2.use_count() == 1);
    REQUIRE(*sp2 == 42);
}

TEST_CASE("shared_ptr: make_shared efficiency", "[ch15][shared_ptr]") {
    // make_shared allocates object and control block together
    auto ptr = std::make_shared<std::vector<int>>(10, 42);

    REQUIRE(ptr->size() == 10);
    REQUIRE((*ptr)[0] == 42);
}

// ============================================================================
// Section 15.2.3: weak_ptr
// ============================================================================

TEST_CASE("weak_ptr: observing without ownership", "[ch15][weak_ptr]") {
    std::weak_ptr<int> weak;

    {
        auto shared = std::make_shared<int>(42);
        weak = shared;

        REQUIRE_FALSE(weak.expired());
        REQUIRE(weak.use_count() == 1);

        if (auto locked = weak.lock()) {
            REQUIRE(*locked == 42);
        }
    }

    REQUIRE(weak.expired());
    REQUIRE(weak.lock() == nullptr);
}

// ============================================================================
// Section 15.3.1: span
// ============================================================================

TEST_CASE("span: from vector", "[ch15][span]") {
    std::vector<int> vec{1, 2, 3, 4, 5};
    std::span<int> s{vec};

    REQUIRE(s.size() == 5);
    REQUIRE(s[0] == 1);
    REQUIRE(s[4] == 5);
}

TEST_CASE("span: from C array", "[ch15][span]") {
    int arr[] = {10, 20, 30};
    std::span<int> s{arr};

    REQUIRE(s.size() == 3);
    REQUIRE(s.front() == 10);
    REQUIRE(s.back() == 30);
}

TEST_CASE("span: subspans", "[ch15][span]") {
    std::vector<int> vec{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    std::span<int> s{vec};

    auto first3 = s.first(3);
    REQUIRE(first3.size() == 3);
    REQUIRE(first3[0] == 0);
    REQUIRE(first3[2] == 2);

    auto last3 = s.last(3);
    REQUIRE(last3.size() == 3);
    REQUIRE(last3[0] == 7);

    auto middle = s.subspan(3, 4);
    REQUIRE(middle.size() == 4);
    REQUIRE(middle[0] == 3);
    REQUIRE(middle[3] == 6);
}

TEST_CASE("span: modification through span", "[ch15][span]") {
    std::vector<int> vec{1, 2, 3};
    std::span<int> s{vec};

    s[1] = 20;
    REQUIRE(vec[1] == 20);

    for (int& x : s) {
        x *= 10;
    }

    REQUIRE(vec[0] == 10);
    REQUIRE(vec[1] == 200);
    REQUIRE(vec[2] == 30);
}

// ============================================================================
// Section 15.4.1: optional
// ============================================================================

TEST_CASE("optional: empty and with value", "[ch15][optional]") {
    std::optional<int> empty;
    std::optional<int> with_value = 42;

    REQUIRE_FALSE(empty.has_value());
    REQUIRE(with_value.has_value());
    REQUIRE(*with_value == 42);
}

TEST_CASE("optional: value_or", "[ch15][optional]") {
    std::optional<std::string> empty;
    std::optional<std::string> with_value = "hello";

    REQUIRE(empty.value_or("default") == "default");
    REQUIRE(with_value.value_or("default") == "hello");
}

TEST_CASE("optional: emplace and reset", "[ch15][optional]") {
    std::optional<std::string> opt;

    opt.emplace("hello");
    REQUIRE(opt.has_value());
    REQUIRE(*opt == "hello");

    opt.reset();
    REQUIRE_FALSE(opt.has_value());
}

TEST_CASE("optional: nullopt assignment", "[ch15][optional]") {
    std::optional<int> opt = 42;
    REQUIRE(opt.has_value());

    opt = std::nullopt;
    REQUIRE_FALSE(opt.has_value());
}

// ============================================================================
// Section 15.4.2: variant
// ============================================================================

TEST_CASE("variant: holds_alternative", "[ch15][variant]") {
    std::variant<int, double, std::string> v = 42;

    REQUIRE(std::holds_alternative<int>(v));
    REQUIRE_FALSE(std::holds_alternative<double>(v));
    REQUIRE_FALSE(std::holds_alternative<std::string>(v));
}

TEST_CASE("variant: get and get_if", "[ch15][variant]") {
    std::variant<int, std::string> v = 42;

    REQUIRE(std::get<int>(v) == 42);
    REQUIRE(std::get<0>(v) == 42);

    int* p = std::get_if<int>(&v);
    REQUIRE(p != nullptr);
    REQUIRE(*p == 42);

    std::string* sp = std::get_if<std::string>(&v);
    REQUIRE(sp == nullptr);
}

TEST_CASE("variant: visit", "[ch15][variant]") {
    std::variant<int, double, std::string> v = 3.14;

    std::string result = std::visit([](const auto& val) -> std::string {
        using T = std::decay_t<decltype(val)>;
        if constexpr (std::is_same_v<T, int>) {
            return "int";
        } else if constexpr (std::is_same_v<T, double>) {
            return "double";
        } else {
            return "string";
        }
    }, v);

    REQUIRE(result == "double");
}

TEST_CASE("variant: index", "[ch15][variant]") {
    std::variant<int, double, std::string> v;

    v = 42;
    REQUIRE(v.index() == 0);

    v = 3.14;
    REQUIRE(v.index() == 1);

    v = std::string{"hello"};
    REQUIRE(v.index() == 2);
}

// ============================================================================
// Section 15.4.3: any
// ============================================================================

TEST_CASE("any: type erasure", "[ch15][any]") {
    std::any a = 42;

    REQUIRE(a.has_value());
    REQUIRE(std::any_cast<int>(a) == 42);

    a = std::string{"hello"};
    REQUIRE(std::any_cast<std::string>(a) == "hello");
}

TEST_CASE("any: safe access with pointer cast", "[ch15][any]") {
    std::any a = 42;

    int* p = std::any_cast<int>(&a);
    REQUIRE(p != nullptr);
    REQUIRE(*p == 42);

    std::string* sp = std::any_cast<std::string>(&a);
    REQUIRE(sp == nullptr);
}

TEST_CASE("any: reset and type", "[ch15][any]") {
    std::any a = 42;
    REQUIRE(a.type() == typeid(int));

    a.reset();
    REQUIRE_FALSE(a.has_value());
}

TEST_CASE("any: bad_any_cast exception", "[ch15][any]") {
    std::any a = 42;

    REQUIRE_THROWS_AS(std::any_cast<std::string>(a), std::bad_any_cast);
}

// ============================================================================
// Practical scenarios
// ============================================================================

TEST_CASE("smart pointers: polymorphism", "[ch15][practical]") {
    struct Base {
        virtual ~Base() = default;
        virtual int value() const = 0;
    };

    struct Derived : Base {
        int value() const override { return 42; }
    };

    std::unique_ptr<Base> ptr = std::make_unique<Derived>();
    REQUIRE(ptr->value() == 42);
}

TEST_CASE("optional: function returning optional", "[ch15][practical]") {
    auto find_first_even = [](const std::vector<int>& vec) -> std::optional<int> {
        for (int x : vec) {
            if (x % 2 == 0) return x;
        }
        return std::nullopt;
    };

    std::vector<int> v1{1, 3, 5, 6, 7};
    std::vector<int> v2{1, 3, 5, 7};

    REQUIRE(find_first_even(v1) == 6);
    REQUIRE_FALSE(find_first_even(v2).has_value());
}

TEST_CASE("variant: type-safe error handling", "[ch15][practical]") {
    using Result = std::variant<int, std::string>;

    auto divide = [](int a, int b) -> Result {
        if (b == 0) {
            return std::string{"division by zero"};
        }
        return a / b;
    };

    auto r1 = divide(10, 2);
    REQUIRE(std::holds_alternative<int>(r1));
    REQUIRE(std::get<int>(r1) == 5);

    auto r2 = divide(10, 0);
    REQUIRE(std::holds_alternative<std::string>(r2));
    REQUIRE(std::get<std::string>(r2) == "division by zero");
}
