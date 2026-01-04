// Move and Forward - Perfect Forwarding and Move Semantics
// Book reference: 16.5 move() and forward()
//
// Understanding move semantics and perfect forwarding is crucial for
// writing efficient generic code:
// - std::move: casts to rvalue reference, enabling move semantics
// - std::forward: preserves value category in perfect forwarding
// - Forwarding references (T&&): can bind to both lvalues and rvalues

#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <vector>

// ============================================================================
// Value Categories Review
// ============================================================================

void value_categories() {
    std::cout << "=== Value Categories ===\n";

    // lvalue - has identity, can take address
    int x = 42;                  // x is an lvalue
    std::string s = "hello";     // s is an lvalue

    // rvalue - temporary, cannot take address
    // 42 is an rvalue
    // std::string{"hello"} is an rvalue

    // lvalue reference - binds to lvalues
    int& ref = x;                // OK
    // int& bad = 42;            // Error: can't bind lvalue ref to rvalue

    // const lvalue reference - binds to both
    const int& cref1 = x;        // OK: lvalue
    const int& cref2 = 42;       // OK: rvalue (lifetime extended)

    // rvalue reference - binds to rvalues
    int&& rref = 42;             // OK: rvalue
    // int&& bad = x;            // Error: can't bind rvalue ref to lvalue
    int&& moved = std::move(x);  // OK: move casts to rvalue

    std::cout << "x = " << x << ", ref = " << ref << "\n";
    std::cout << "rref = " << rref << ", moved = " << moved << "\n";

    (void)cref1;
    (void)cref2;
}

// ============================================================================
// std::move - Enabling Move Semantics
// ============================================================================

class HeavyObject {
public:
    HeavyObject() : data_(new int[1000]) {
        std::cout << "Default constructed\n";
    }

    ~HeavyObject() {
        delete[] data_;
    }

    // Copy constructor (expensive)
    HeavyObject(const HeavyObject& other) : data_(new int[1000]) {
        std::copy(other.data_, other.data_ + 1000, data_);
        std::cout << "Copy constructed (expensive)\n";
    }

    // Move constructor (cheap)
    HeavyObject(HeavyObject&& other) noexcept : data_(other.data_) {
        other.data_ = nullptr;
        std::cout << "Move constructed (cheap)\n";
    }

    // Copy assignment
    HeavyObject& operator=(const HeavyObject& other) {
        if (this != &other) {
            delete[] data_;
            data_ = new int[1000];
            std::copy(other.data_, other.data_ + 1000, data_);
            std::cout << "Copy assigned (expensive)\n";
        }
        return *this;
    }

    // Move assignment
    HeavyObject& operator=(HeavyObject&& other) noexcept {
        if (this != &other) {
            delete[] data_;
            data_ = other.data_;
            other.data_ = nullptr;
            std::cout << "Move assigned (cheap)\n";
        }
        return *this;
    }

private:
    int* data_;
};

void move_demo() {
    std::cout << "\n=== std::move Demo ===\n";

    std::cout << "\nCopy vs Move:\n";
    HeavyObject obj1;

    std::cout << "Copying...\n";
    HeavyObject obj2 = obj1;  // Copy

    std::cout << "Moving...\n";
    HeavyObject obj3 = std::move(obj1);  // Move

    std::cout << "\nWith strings:\n";
    std::string s1 = "Hello, World! This is a long string.";
    std::cout << "Before move: s1 = '" << s1 << "'\n";

    std::string s2 = std::move(s1);
    std::cout << "After move: s1 = '" << s1 << "' (empty or unspecified)\n";
    std::cout << "After move: s2 = '" << s2 << "'\n";

    std::cout << "\nWith vector:\n";
    std::vector<int> v1{1, 2, 3, 4, 5};
    std::cout << "Before move: v1.size() = " << v1.size() << "\n";

    std::vector<int> v2 = std::move(v1);
    std::cout << "After move: v1.size() = " << v1.size() << " (empty)\n";
    std::cout << "After move: v2.size() = " << v2.size() << "\n";
}

// ============================================================================
// Move in Containers
// ============================================================================

void move_in_containers() {
    std::cout << "\n=== Move in Containers ===\n";

    std::vector<std::string> vec;

    // Copy into vector
    std::string s1 = "copy me";
    vec.push_back(s1);  // Copies
    std::cout << "After push_back(copy): s1 = '" << s1 << "'\n";

    // Move into vector
    std::string s2 = "move me";
    vec.push_back(std::move(s2));  // Moves
    std::cout << "After push_back(move): s2 = '" << s2 << "'\n";

    // emplace_back constructs in place (no copy or move of element)
    vec.emplace_back("constructed in place");

    std::cout << "Vector contents:\n";
    for (const auto& s : vec) {
        std::cout << "  '" << s << "'\n";
    }
}

// ============================================================================
// Forwarding References (Universal References)
// ============================================================================

// T&& in a deduced context is a forwarding reference
template<typename T>
void examine(T&& arg) {
    if constexpr (std::is_lvalue_reference_v<T>) {
        std::cout << "Received lvalue reference\n";
    } else {
        std::cout << "Received rvalue reference\n";
    }
}

void forwarding_reference_demo() {
    std::cout << "\n=== Forwarding References ===\n";

    int x = 42;

    std::cout << "examine(x): ";
    examine(x);  // T deduced as int&, T&& is int& (lvalue)

    std::cout << "examine(42): ";
    examine(42);  // T deduced as int, T&& is int&& (rvalue)

    std::cout << "examine(std::move(x)): ";
    examine(std::move(x));  // T deduced as int, T&& is int&& (rvalue)

    // Note: auto&& is also a forwarding reference
    auto&& fref1 = x;   // int&
    auto&& fref2 = 42;  // int&&

    (void)fref1;
    (void)fref2;
}

// ============================================================================
// std::forward - Perfect Forwarding
// ============================================================================

void process(int& x) {
    std::cout << "process(lvalue): " << x << "\n";
}

void process(int&& x) {
    std::cout << "process(rvalue): " << x << "\n";
}

// Without forward - always calls lvalue version
template<typename T>
void bad_wrapper(T&& arg) {
    process(arg);  // arg is always an lvalue (it has a name!)
}

// With forward - preserves value category
template<typename T>
void good_wrapper(T&& arg) {
    process(std::forward<T>(arg));  // Forwards as lvalue or rvalue
}

void forward_demo() {
    std::cout << "\n=== std::forward Demo ===\n";

    int x = 42;

    std::cout << "Without forward (bad):\n";
    bad_wrapper(x);      // lvalue -> lvalue (correct)
    bad_wrapper(100);    // rvalue -> lvalue (wrong!)

    std::cout << "\nWith forward (good):\n";
    good_wrapper(x);     // lvalue -> lvalue (correct)
    good_wrapper(100);   // rvalue -> rvalue (correct!)
}

// ============================================================================
// Perfect Forwarding Factory
// ============================================================================

class Widget {
public:
    Widget(int id, std::string name)
        : id_{id}, name_{std::move(name)} {
        std::cout << "Widget(" << id_ << ", '" << name_ << "') constructed\n";
    }

    int id() const { return id_; }
    const std::string& name() const { return name_; }

private:
    int id_;
    std::string name_;
};

// Factory with perfect forwarding
template<typename... Args>
std::unique_ptr<Widget> make_widget(Args&&... args) {
    return std::make_unique<Widget>(std::forward<Args>(args)...);
}

void factory_demo() {
    std::cout << "\n=== Perfect Forwarding Factory ===\n";

    // Forwarding lvalues
    int id = 42;
    std::string name = "example";
    auto w1 = make_widget(id, name);
    std::cout << "w1: id=" << w1->id() << ", name still valid: '" << name << "'\n";

    // Forwarding rvalues
    auto w2 = make_widget(100, std::string{"moved"});
    std::cout << "w2: id=" << w2->id() << "\n";

    // Mixed
    auto w3 = make_widget(200, std::move(name));
    std::cout << "w3: id=" << w3->id() << ", name after move: '" << name << "'\n";
}

// ============================================================================
// Move-Only Types
// ============================================================================

class MoveOnly {
public:
    MoveOnly() = default;
    MoveOnly(const MoveOnly&) = delete;
    MoveOnly& operator=(const MoveOnly&) = delete;
    MoveOnly(MoveOnly&&) = default;
    MoveOnly& operator=(MoveOnly&&) = default;

    void use() const { std::cout << "Using MoveOnly\n"; }
};

void move_only_demo() {
    std::cout << "\n=== Move-Only Types ===\n";

    MoveOnly m1;
    // MoveOnly m2 = m1;           // Error: copy deleted
    MoveOnly m2 = std::move(m1);   // OK: move

    m2.use();

    // Common move-only types:
    // - std::unique_ptr
    // - std::thread
    // - std::promise/std::future (kinda)
    // - File handles, socket handles, etc.

    std::unique_ptr<int> up1 = std::make_unique<int>(42);
    // std::unique_ptr<int> up2 = up1;          // Error
    std::unique_ptr<int> up2 = std::move(up1);  // OK
    std::cout << "up1 is " << (up1 ? "valid" : "null") << "\n";
    std::cout << "up2 is " << (up2 ? "valid" : "null") << "\n";
}

// ============================================================================
// Common Patterns and Gotchas
// ============================================================================

void gotchas() {
    std::cout << "\n=== Common Gotchas ===\n";

    // 1. Move from const - silently copies!
    const std::string cs = "constant";
    std::string s1 = std::move(cs);  // Actually copies!
    std::cout << "Move from const: original still '" << cs << "'\n";

    // 2. Use after move - valid but unspecified state
    std::string s2 = "hello";
    std::string s3 = std::move(s2);
    // s2 is valid but in unspecified state
    // Can assign to it or call clear()
    s2 = "reassigned";
    std::cout << "After reassign: '" << s2 << "'\n";

    // 3. Returning local by std::move - prevents RVO!
    // BAD: return std::move(local);
    // GOOD: return local;  (compiler applies RVO or implicit move)

    // 4. std::move doesn't move - it just casts
    int x = 42;
    int y = std::move(x);  // Actually copies (int is trivially copyable)
    std::cout << "After 'moving' int: x=" << x << ", y=" << y << "\n";
}

// ============================================================================
// Sink and Source Patterns
// ============================================================================

class DataProcessor {
public:
    // Sink: take ownership via value parameter
    void set_data(std::vector<int> data) {
        data_ = std::move(data);  // Always move from parameter
    }

    // Source: return by value (RVO/move handles it)
    std::vector<int> get_data() const {
        return data_;  // Copy
    }

    std::vector<int> extract_data() {
        return std::move(data_);  // Move out
    }

private:
    std::vector<int> data_;
};

void sink_source_demo() {
    std::cout << "\n=== Sink and Source Patterns ===\n";

    DataProcessor dp;

    // Caller decides: copy or move
    std::vector<int> v1{1, 2, 3};
    dp.set_data(v1);  // Copies v1, then moves into data_
    std::cout << "After set_data(copy): v1.size() = " << v1.size() << "\n";

    std::vector<int> v2{4, 5, 6};
    dp.set_data(std::move(v2));  // Moves v2, then moves into data_
    std::cout << "After set_data(move): v2.size() = " << v2.size() << "\n";

    auto extracted = dp.extract_data();
    std::cout << "Extracted: " << extracted.size() << " elements\n";
}

int main() {
    value_categories();
    move_demo();
    move_in_containers();
    forwarding_reference_demo();
    forward_demo();
    factory_demo();
    move_only_demo();
    gotchas();
    sink_source_demo();

    return 0;
}

// Key points:
// - std::move casts to rvalue reference; actual moving is done by move ctor/assign
// - std::forward preserves value category in perfect forwarding
// - T&& in template context is a forwarding reference, not an rvalue reference
// - Moving from const silently copies (const blocks the move)
// - After move, object is in valid but unspecified state
// - Don't return std::move(local) - it prevents RVO
// - Use sink pattern (pass by value, then move) for taking ownership
