// Chapter 6 Tests: Essential Operations
// These tests verify understanding of copy/move semantics, RAII, and operators.

#include <catch2/catch_test_macros.hpp>
#include <memory>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

// =============================================================================
// Section 6.2: Copy and Move Operations
// =============================================================================

namespace {

// A class that tracks construction, copying, and moving
class Tracker {
    static inline int construction_count = 0;
    static inline int copy_count = 0;
    static inline int move_count = 0;
    static inline int destruction_count = 0;

    int value_;
    bool moved_from_ = false;

public:
    explicit Tracker(int v = 0) : value_{v} {
        ++construction_count;
    }

    ~Tracker() {
        ++destruction_count;
    }

    Tracker(const Tracker& other) : value_{other.value_} {
        ++copy_count;
    }

    Tracker(Tracker&& other) noexcept : value_{other.value_} {
        other.moved_from_ = true;
        ++move_count;
    }

    Tracker& operator=(const Tracker& other) {
        if (this != &other) {
            value_ = other.value_;
            ++copy_count;
        }
        return *this;
    }

    Tracker& operator=(Tracker&& other) noexcept {
        if (this != &other) {
            value_ = other.value_;
            other.moved_from_ = true;
            ++move_count;
        }
        return *this;
    }

    [[nodiscard]] int value() const { return value_; }
    [[nodiscard]] bool moved_from() const { return moved_from_; }

    static void reset_counts() {
        construction_count = 0;
        copy_count = 0;
        move_count = 0;
        destruction_count = 0;
    }

    static int constructions() { return construction_count; }
    static int copies() { return copy_count; }
    static int moves() { return move_count; }
    static int destructions() { return destruction_count; }
};

}  // namespace

TEST_CASE("Copy: copy constructor creates independent copy", "[ch06][copy]") {
    Tracker::reset_counts();

    Tracker original{42};
    Tracker copy{original};

    REQUIRE(copy.value() == 42);
    REQUIRE(original.value() == 42);  // Original unchanged
    REQUIRE_FALSE(original.moved_from());
    REQUIRE(Tracker::copies() == 1);
}

TEST_CASE("Copy: copy assignment creates independent copy", "[ch06][copy]") {
    Tracker::reset_counts();

    Tracker original{42};
    Tracker target{0};
    target = original;

    REQUIRE(target.value() == 42);
    REQUIRE(original.value() == 42);
    REQUIRE_FALSE(original.moved_from());
    REQUIRE(Tracker::copies() == 1);
}

TEST_CASE("Move: move constructor transfers ownership", "[ch06][move]") {
    Tracker::reset_counts();

    Tracker original{42};
    Tracker moved{std::move(original)};

    REQUIRE(moved.value() == 42);
    REQUIRE(original.moved_from());
    REQUIRE(Tracker::moves() == 1);
    REQUIRE(Tracker::copies() == 0);
}

TEST_CASE("Move: move assignment transfers ownership", "[ch06][move]") {
    Tracker::reset_counts();

    Tracker original{42};
    Tracker target{0};
    target = std::move(original);

    REQUIRE(target.value() == 42);
    REQUIRE(original.moved_from());
    REQUIRE(Tracker::moves() == 1);
    REQUIRE(Tracker::copies() == 0);
}

TEST_CASE("Move: std::move is just a cast", "[ch06][move]") {
    int x = 42;
    int& lref = x;
    int&& rref = std::move(x);

    // std::move doesn't actually move - it just casts to rvalue reference
    REQUIRE(x == 42);  // x is unchanged!
    REQUIRE(&lref == &x);
    REQUIRE(&rref == &x);  // All refer to the same object
}

TEST_CASE("Move: passing temporary uses move", "[ch06][move]") {
    Tracker::reset_counts();

    auto process = [](Tracker t) { return t.value(); };

    // Passing a temporary should move, not copy
    int result = process(Tracker{42});
    REQUIRE(result == 42);
    // The temporary is moved (or elided), not copied
    REQUIRE(Tracker::copies() == 0);
}

TEST_CASE("Move: noexcept enables optimizations", "[ch06][move][noexcept]") {
    // Verify that move operations are noexcept
    static_assert(std::is_nothrow_move_constructible_v<Tracker>);
    static_assert(std::is_nothrow_move_assignable_v<Tracker>);

    // vector uses move when reallocation is needed (if noexcept)
    std::vector<Tracker> vec;
    vec.reserve(1);

    Tracker::reset_counts();
    vec.push_back(Tracker{1});
    vec.push_back(Tracker{2});  // Forces reallocation and move

    // With noexcept move, existing elements are moved, not copied
    REQUIRE(Tracker::copies() == 0);
}

// =============================================================================
// Section 6.3: Resource Management (RAII)
// =============================================================================

namespace {

class RAIIResource {
    static inline int active_resources = 0;
    bool active_ = true;

public:
    RAIIResource() { ++active_resources; }
    ~RAIIResource() {
        if (active_) --active_resources;
    }

    RAIIResource(const RAIIResource&) = delete;
    RAIIResource& operator=(const RAIIResource&) = delete;

    RAIIResource(RAIIResource&& other) noexcept : active_{other.active_} {
        other.active_ = false;
    }

    RAIIResource& operator=(RAIIResource&& other) noexcept {
        if (this != &other) {
            if (active_) --active_resources;
            active_ = other.active_;
            other.active_ = false;
        }
        return *this;
    }

    static int count() { return active_resources; }
    static void reset() { active_resources = 0; }
};

}  // namespace

TEST_CASE("RAII: resources are cleaned up automatically", "[ch06][raii]") {
    RAIIResource::reset();
    REQUIRE(RAIIResource::count() == 0);

    {
        RAIIResource r1;
        REQUIRE(RAIIResource::count() == 1);
        {
            RAIIResource r2;
            REQUIRE(RAIIResource::count() == 2);
        }
        REQUIRE(RAIIResource::count() == 1);  // r2 cleaned up
    }
    REQUIRE(RAIIResource::count() == 0);  // r1 cleaned up
}

TEST_CASE("RAII: resources survive moves", "[ch06][raii]") {
    RAIIResource::reset();

    {
        RAIIResource r1;
        REQUIRE(RAIIResource::count() == 1);

        RAIIResource r2 = std::move(r1);
        REQUIRE(RAIIResource::count() == 1);  // Still just one active resource
    }
    REQUIRE(RAIIResource::count() == 0);  // Cleaned up
}

TEST_CASE("unique_ptr: exclusive ownership", "[ch06][raii][smart_ptr]") {
    auto ptr = std::make_unique<int>(42);

    REQUIRE(*ptr == 42);

    // Cannot copy unique_ptr
    static_assert(!std::is_copy_constructible_v<std::unique_ptr<int>>);
    static_assert(!std::is_copy_assignable_v<std::unique_ptr<int>>);

    // Can move unique_ptr
    auto ptr2 = std::move(ptr);
    REQUIRE(ptr == nullptr);
    REQUIRE(*ptr2 == 42);
}

TEST_CASE("shared_ptr: shared ownership with reference counting", "[ch06][raii][smart_ptr]") {
    auto ptr1 = std::make_shared<int>(42);
    REQUIRE(ptr1.use_count() == 1);

    auto ptr2 = ptr1;  // Share ownership
    REQUIRE(ptr1.use_count() == 2);
    REQUIRE(ptr2.use_count() == 2);
    REQUIRE(*ptr1 == *ptr2);

    ptr1.reset();  // Release ptr1's ownership
    REQUIRE(ptr1 == nullptr);
    REQUIRE(ptr2.use_count() == 1);
    REQUIRE(*ptr2 == 42);  // Resource still alive
}

TEST_CASE("weak_ptr: non-owning observer", "[ch06][raii][smart_ptr]") {
    std::weak_ptr<int> weak;

    {
        auto shared = std::make_shared<int>(42);
        weak = shared;

        REQUIRE_FALSE(weak.expired());
        auto locked = weak.lock();
        REQUIRE(*locked == 42);
    }

    REQUIRE(weak.expired());
    REQUIRE(weak.lock() == nullptr);
}

// =============================================================================
// Section 6.4-6.5: Operator Overloading
// =============================================================================

namespace {

class Fraction {
    int num_, den_;

public:
    constexpr Fraction(int n = 0, int d = 1) : num_{n}, den_{d} {}

    [[nodiscard]] constexpr int numerator() const { return num_; }
    [[nodiscard]] constexpr int denominator() const { return den_; }

    constexpr Fraction& operator+=(const Fraction& rhs) {
        num_ = num_ * rhs.den_ + rhs.num_ * den_;
        den_ *= rhs.den_;
        return *this;
    }

    constexpr Fraction& operator*=(const Fraction& rhs) {
        num_ *= rhs.num_;
        den_ *= rhs.den_;
        return *this;
    }

    constexpr Fraction operator-() const {
        return Fraction{-num_, den_};
    }

    constexpr bool operator==(const Fraction& rhs) const {
        return num_ * rhs.den_ == rhs.num_ * den_;
    }

    constexpr auto operator<=>(const Fraction& rhs) const {
        return num_ * rhs.den_ <=> rhs.num_ * den_;
    }
};

constexpr Fraction operator+(Fraction lhs, const Fraction& rhs) {
    return lhs += rhs;
}

constexpr Fraction operator*(Fraction lhs, const Fraction& rhs) {
    return lhs *= rhs;
}

}  // namespace

TEST_CASE("Operators: arithmetic operators", "[ch06][operators]") {
    constexpr Fraction half{1, 2};
    constexpr Fraction third{1, 3};

    constexpr auto sum = half + third;
    REQUIRE(sum.numerator() == 5);
    REQUIRE(sum.denominator() == 6);

    constexpr auto product = half * third;
    REQUIRE(product.numerator() == 1);
    REQUIRE(product.denominator() == 6);

    constexpr auto neg = -half;
    REQUIRE(neg.numerator() == -1);
    REQUIRE(neg.denominator() == 2);
}

TEST_CASE("Operators: comparison operators", "[ch06][operators]") {
    constexpr Fraction half{1, 2};
    constexpr Fraction other_half{2, 4};
    constexpr Fraction third{1, 3};

    static_assert(half == other_half);
    static_assert(half > third);
    static_assert(third < half);
    static_assert(half >= other_half);
    static_assert(half <= other_half);
    static_assert(half != third);
}

TEST_CASE("Operators: spaceship operator generates all comparisons", "[ch06][operators][cpp20]") {
    struct Point {
        int x, y;
        auto operator<=>(const Point&) const = default;
    };

    constexpr Point p1{1, 2};
    constexpr Point p2{1, 2};
    constexpr Point p3{1, 3};

    static_assert(p1 == p2);
    static_assert(p1 != p3);
    static_assert(p1 < p3);
    static_assert(p3 > p1);
    static_assert(p1 <= p2);
    static_assert(p1 >= p2);
}

namespace {

class SafeArray {
    std::vector<int> data_;

public:
    SafeArray(std::initializer_list<int> init) : data_{init} {}

    int& operator[](std::size_t i) {
        if (i >= data_.size()) throw std::out_of_range{"index out of bounds"};
        return data_[i];
    }

    const int& operator[](std::size_t i) const {
        if (i >= data_.size()) throw std::out_of_range{"index out of bounds"};
        return data_[i];
    }

    [[nodiscard]] std::size_t size() const { return data_.size(); }
};

}  // namespace

TEST_CASE("Operators: subscript operator", "[ch06][operators]") {
    SafeArray arr{10, 20, 30};

    REQUIRE(arr[0] == 10);
    REQUIRE(arr[1] == 20);
    REQUIRE(arr[2] == 30);

    arr[1] = 25;
    REQUIRE(arr[1] == 25);

    REQUIRE_THROWS_AS(arr[10], std::out_of_range);
}

namespace {

class Adder {
    int base_;

public:
    explicit Adder(int b) : base_{b} {}

    int operator()(int x) const {
        return base_ + x;
    }

    int operator()(int x, int y) const {
        return base_ + x + y;
    }
};

}  // namespace

TEST_CASE("Operators: function call operator (functor)", "[ch06][operators]") {
    Adder add5{5};

    REQUIRE(add5(10) == 15);
    REQUIRE(add5(10, 20) == 35);
}

// =============================================================================
// Section 6.2: Special Member Functions and Rule of 5
// =============================================================================

namespace {

// Class that properly implements Rule of 5
class Buffer {
    int* data_;
    std::size_t size_;

public:
    explicit Buffer(std::size_t n) : data_{new int[n]{}}, size_{n} {}

    ~Buffer() { delete[] data_; }

    Buffer(const Buffer& other) : data_{new int[other.size_]}, size_{other.size_} {
        std::copy(other.data_, other.data_ + size_, data_);
    }

    Buffer(Buffer&& other) noexcept : data_{other.data_}, size_{other.size_} {
        other.data_ = nullptr;
        other.size_ = 0;
    }

    Buffer& operator=(const Buffer& other) {
        if (this != &other) {
            delete[] data_;
            size_ = other.size_;
            data_ = new int[size_];
            std::copy(other.data_, other.data_ + size_, data_);
        }
        return *this;
    }

    Buffer& operator=(Buffer&& other) noexcept {
        if (this != &other) {
            delete[] data_;
            data_ = other.data_;
            size_ = other.size_;
            other.data_ = nullptr;
            other.size_ = 0;
        }
        return *this;
    }

    [[nodiscard]] std::size_t size() const { return size_; }
    [[nodiscard]] bool valid() const { return data_ != nullptr; }

    int& operator[](std::size_t i) { return data_[i]; }
    const int& operator[](std::size_t i) const { return data_[i]; }
};

}  // namespace

TEST_CASE("Rule of 5: all special members work correctly", "[ch06][rule_of_5]") {
    // Constructor
    Buffer b1{10};
    REQUIRE(b1.size() == 10);
    b1[0] = 42;

    // Copy constructor
    Buffer b2{b1};
    REQUIRE(b2.size() == 10);
    REQUIRE(b2[0] == 42);
    REQUIRE(b1.valid());  // Original still valid

    // Move constructor
    Buffer b3{std::move(b1)};
    REQUIRE(b3.size() == 10);
    REQUIRE(b3[0] == 42);
    REQUIRE_FALSE(b1.valid());  // Original moved from

    // Copy assignment
    Buffer b4{5};
    b4 = b2;
    REQUIRE(b4.size() == 10);
    REQUIRE(b4[0] == 42);
    REQUIRE(b2.valid());

    // Move assignment
    Buffer b5{5};
    b5 = std::move(b2);
    REQUIRE(b5.size() == 10);
    REQUIRE(b5[0] == 42);
    REQUIRE_FALSE(b2.valid());
}

TEST_CASE("Self-assignment: copy assignment handles x = x", "[ch06][rule_of_5]") {
    Buffer b{10};
    b[0] = 42;

    // Use reference indirection to test self-assignment without compiler warning
    Buffer& ref = b;
    b = ref;  // Self-assignment through reference

    REQUIRE(b.size() == 10);
    REQUIRE(b[0] == 42);
    REQUIRE(b.valid());
}

TEST_CASE("Self-assignment: move assignment handles x = move(x)", "[ch06][rule_of_5]") {
    Buffer b{10};
    b[0] = 42;

    // Use reference indirection to test self-move without compiler warning
    Buffer& ref = b;
    b = std::move(ref);  // Self-move through reference

    // Note: The state after self-move is unspecified but valid
    // Our implementation preserves the object
    REQUIRE(b.valid());
}

// =============================================================================
// Rule of Zero
// =============================================================================

TEST_CASE("Rule of Zero: compiler generates correct operations", "[ch06][rule_of_zero]") {
    struct Person {
        std::string name;
        std::vector<int> scores;
    };

    Person p1;
    p1.name = "Alice";
    p1.scores = {90, 85, 92};

    // Copy
    Person p2 = p1;
    REQUIRE(p2.name == "Alice");
    REQUIRE(p2.scores.size() == 3);
    REQUIRE(p1.name == "Alice");  // Original unchanged

    // Move
    Person p3 = std::move(p1);
    REQUIRE(p3.name == "Alice");
    REQUIRE(p1.name.empty());  // Moved from

    // Assignment
    Person p4;
    p4 = p2;
    REQUIRE(p4.name == "Alice");
}

// =============================================================================
// Type Traits for Special Members
// =============================================================================

TEST_CASE("Type traits: check special member availability", "[ch06][type_traits]") {
    // Tracker is copyable and movable
    static_assert(std::is_copy_constructible_v<Tracker>);
    static_assert(std::is_copy_assignable_v<Tracker>);
    static_assert(std::is_move_constructible_v<Tracker>);
    static_assert(std::is_move_assignable_v<Tracker>);

    // RAIIResource is move-only
    static_assert(!std::is_copy_constructible_v<RAIIResource>);
    static_assert(!std::is_copy_assignable_v<RAIIResource>);
    static_assert(std::is_move_constructible_v<RAIIResource>);
    static_assert(std::is_move_assignable_v<RAIIResource>);

    // unique_ptr is move-only
    static_assert(!std::is_copy_constructible_v<std::unique_ptr<int>>);
    static_assert(std::is_move_constructible_v<std::unique_ptr<int>>);

    // shared_ptr is copyable
    static_assert(std::is_copy_constructible_v<std::shared_ptr<int>>);
    static_assert(std::is_move_constructible_v<std::shared_ptr<int>>);
}
