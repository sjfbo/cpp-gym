// Function Arguments and Return Values
// Book reference: 3.4 Function Arguments and Return Values
//
// C++ offers multiple ways to pass arguments to functions. Choosing the right
// method affects performance, safety, and expressiveness.

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <utility>  // For std::move

// ============================================================================
// Pass by Value - See 3.4.1
// ============================================================================

// Use for: small types (int, double, pointers, small structs)
// The function gets a COPY of the argument

void by_value(int x) {
    x = x * 2;  // Modifies the copy, not the original
    std::cout << "Inside by_value: x = " << x << "\n";
}

// For small types, pass by value is efficient (no indirection)
struct SmallStruct {
    int a, b;
};

void by_value_struct(SmallStruct s) {
    s.a = 999;  // Modifies copy
    std::cout << "Inside by_value_struct: a = " << s.a << "\n";
}

// ============================================================================
// Pass by Reference - See 3.4.2
// ============================================================================

// Use for: when you need to MODIFY the caller's value
// The function operates on the ORIGINAL object

void by_reference(int& x) {
    x = x * 2;  // Modifies the original!
    std::cout << "Inside by_reference: x = " << x << "\n";
}

// Common use case: output parameters
bool divide(int a, int b, int& quotient, int& remainder) {
    if (b == 0) return false;
    quotient = a / b;
    remainder = a % b;
    return true;
}

// ============================================================================
// Pass by Const Reference - See 3.4.3
// ============================================================================

// Use for: large types when you only need to READ
// Avoids copying, prevents modification

void by_const_ref(const std::string& s) {
    std::cout << "String length: " << s.length() << "\n";
    // s += "x";  // ERROR: cannot modify const reference
}

// Prefer const ref for containers and strings
void process_vector(const std::vector<int>& v) {
    int sum = 0;
    for (int x : v) {
        sum += x;
    }
    std::cout << "Sum of vector: " << sum << "\n";
}

// ============================================================================
// Pass by Pointer - See 3.4.4
// ============================================================================

// Use for:
// 1. Optional parameters (can be nullptr)
// 2. C API compatibility
// 3. When you need to reseat (point to something else)

void by_pointer(int* p) {
    if (p == nullptr) {
        std::cout << "Pointer is null\n";
        return;
    }
    *p = *p * 2;  // Modifies the pointed-to value
    std::cout << "Inside by_pointer: *p = " << *p << "\n";
}

// Optional parameter pattern
void greet(const std::string& name, const std::string* title = nullptr) {
    if (title) {
        std::cout << "Hello, " << *title << " " << name << "!\n";
    } else {
        std::cout << "Hello, " << name << "!\n";
    }
}

// ============================================================================
// Pass by Rvalue Reference (Move Semantics) - See 3.4.5
// ============================================================================

// Use for: taking ownership of temporary/moveable objects
// Enables efficient transfer of resources

class Buffer {
    std::vector<int> data;
    std::string name;

public:
    Buffer(std::string n, std::vector<int> d)
        : data{std::move(d)}, name{std::move(n)} {}

    // Take ownership via rvalue reference
    void set_data(std::vector<int>&& new_data) {
        data = std::move(new_data);  // Move, don't copy
        std::cout << "Data moved into buffer (size: " << data.size() << ")\n";
    }

    // Const ref for reading
    const std::vector<int>& get_data() const { return data; }
    const std::string& get_name() const { return name; }
};

// ============================================================================
// Return Values - See 3.4.6
// ============================================================================

// Return by value - modern compilers optimize (RVO/NRVO)
std::vector<int> create_sequence(int n) {
    std::vector<int> result;
    for (int i = 0; i < n; ++i) {
        result.push_back(i);
    }
    return result;  // Compiler will likely elide the copy (RVO)
}

// Return by reference - returning member or static data
class Container {
    std::vector<int> items;

public:
    void add(int x) { items.push_back(x); }

    // Return reference to allow modification
    std::vector<int>& data() { return items; }

    // Const version for read-only access
    const std::vector<int>& data() const { return items; }
};

// Return by move (explicit) - when RVO can't apply
std::string build_message(const std::string& prefix, int value) {
    std::string result = prefix + ": " + std::to_string(value);
    // Could return result (RVO might apply) or:
    return result;  // Compiler will move if RVO doesn't apply
}

// ============================================================================
// Guidelines Summary
// ============================================================================

/*
 * PARAMETER PASSING GUIDELINES - See 3.4
 *
 * Type/Intent                    | How to Pass
 * -------------------------------|------------------
 * Small type, read              | by value: f(int x)
 * Small type, modify            | by reference: f(int& x)
 * Large type, read              | by const ref: f(const T& x)
 * Large type, modify            | by reference: f(T& x)
 * Large type, take ownership    | by rvalue ref: f(T&& x) or by value + move
 * Optional parameter            | by pointer: f(T* x)
 * Array (C-style)               | by pointer: f(T* arr, size_t n)
 *
 * "Cheap to copy" types (pass by value):
 * - int, double, char, bool, pointers
 * - Small structs (< 2-3 words)
 * - std::string_view, std::span
 *
 * "Expensive to copy" types (pass by const ref):
 * - std::string
 * - std::vector, std::map, containers
 * - Large structs/classes
 */

// ============================================================================
// Main Demonstration
// ============================================================================

int main() {
    std::cout << "=== Function Arguments Demo ===\n";

    // --------------------------------------------------------------------
    // Pass by value
    // --------------------------------------------------------------------
    std::cout << "\n--- Pass by Value ---\n";

    int a = 10;
    std::cout << "Before by_value: a = " << a << "\n";
    by_value(a);
    std::cout << "After by_value: a = " << a << " (unchanged)\n";

    SmallStruct ss{1, 2};
    std::cout << "\nBefore by_value_struct: a = " << ss.a << "\n";
    by_value_struct(ss);
    std::cout << "After by_value_struct: a = " << ss.a << " (unchanged)\n";

    // --------------------------------------------------------------------
    // Pass by reference
    // --------------------------------------------------------------------
    std::cout << "\n--- Pass by Reference ---\n";

    int b = 10;
    std::cout << "Before by_reference: b = " << b << "\n";
    by_reference(b);
    std::cout << "After by_reference: b = " << b << " (MODIFIED)\n";

    int quotient, remainder;
    if (divide(17, 5, quotient, remainder)) {
        std::cout << "17 / 5 = " << quotient << " remainder " << remainder << "\n";
    }

    // --------------------------------------------------------------------
    // Pass by const reference
    // --------------------------------------------------------------------
    std::cout << "\n--- Pass by Const Reference ---\n";

    std::string message = "Hello, World!";
    by_const_ref(message);  // No copy made, can't modify

    std::vector<int> nums{1, 2, 3, 4, 5};
    process_vector(nums);  // Efficient: no copy of vector

    // --------------------------------------------------------------------
    // Pass by pointer
    // --------------------------------------------------------------------
    std::cout << "\n--- Pass by Pointer ---\n";

    int c = 10;
    std::cout << "Before by_pointer: c = " << c << "\n";
    by_pointer(&c);
    std::cout << "After by_pointer: c = " << c << " (MODIFIED)\n";

    by_pointer(nullptr);  // Handles null case

    // Optional parameter
    std::string title = "Dr.";
    greet("Smith");
    greet("Smith", &title);

    // --------------------------------------------------------------------
    // Rvalue reference (move semantics)
    // --------------------------------------------------------------------
    std::cout << "\n--- Rvalue Reference (Move) ---\n";

    Buffer buf("MyBuffer", {1, 2, 3});

    std::vector<int> new_data{10, 20, 30, 40, 50};
    std::cout << "new_data size before move: " << new_data.size() << "\n";
    buf.set_data(std::move(new_data));  // Transfer ownership
    std::cout << "new_data size after move: " << new_data.size() << " (moved-from)\n";

    // Also works with temporaries
    buf.set_data({100, 200, 300});  // Temporary, automatically moved

    // --------------------------------------------------------------------
    // Return values
    // --------------------------------------------------------------------
    std::cout << "\n--- Return Values ---\n";

    auto seq = create_sequence(5);  // RVO likely applies
    std::cout << "Sequence: ";
    for (int x : seq) std::cout << x << " ";
    std::cout << "\n";

    Container cont;
    cont.add(1);
    cont.add(2);
    cont.data().push_back(3);  // Modify via returned reference
    std::cout << "Container size: " << cont.data().size() << "\n";

    // --------------------------------------------------------------------
    // Summary
    // --------------------------------------------------------------------
    std::cout << "\n=== Key Points ===\n";
    std::cout << "1. Small types: pass by value\n";
    std::cout << "2. Large types, read-only: pass by const reference\n";
    std::cout << "3. Need to modify: pass by reference\n";
    std::cout << "4. Optional/nullable: pass by pointer\n";
    std::cout << "5. Taking ownership: pass by rvalue reference or value+move\n";
    std::cout << "6. Return values: compiler optimizes (RVO), don't over-think\n";

    return 0;
}
