// Chapter 6: Exercise 1 - Implement a Simple String Class
//
// Objective: Implement a string class with proper copy/move semantics
//            following the Rule of Five.
//
// Requirements:
// 1. Implement constructor, destructor, copy constructor, move constructor,
//    copy assignment, and move assignment.
// 2. Support basic operations: length(), c_str(), empty(), clear()
// 3. Support operator[], operator+, operator+=, operator==
// 4. Mark move operations as noexcept
// 5. Handle self-assignment correctly
//
// Tips:
// - Store a null-terminated char array
// - Use the copy-and-swap idiom for assignment operators
// - Don't forget to handle the empty string case

#include <algorithm>
#include <cstddef>
#include <cstring>
#include <iostream>
#include <utility>

class SimpleString {
    char* data_;
    std::size_t size_;

    // Helper to allocate and copy
    void copy_from(const char* str, std::size_t len) {
        size_ = len;
        data_ = new char[size_ + 1];
        std::memcpy(data_, str, size_);
        data_[size_] = '\0';
    }

public:
    // ==========================================================================
    // TODO: Implement the following special member functions
    // ==========================================================================

    // Default constructor - creates empty string
    SimpleString() : data_{new char[1]{'\0'}}, size_{0} {
        // Provided for you
    }

    // Constructor from C-string
    explicit SimpleString(const char* str) {
        // TODO: Implement this
        // Hint: Use strlen to get length, then copy_from
        (void)str;  // Remove this line when implementing
    }

    // Destructor
    ~SimpleString() {
        // TODO: Implement this
        // Hint: Free the allocated memory
    }

    // Copy constructor
    SimpleString(const SimpleString& other) {
        // TODO: Implement this
        // Hint: Deep copy the data from other
        (void)other;  // Remove this line when implementing
    }

    // Move constructor (should be noexcept!)
    SimpleString(SimpleString&& other) noexcept {
        // TODO: Implement this
        // Hint: Take ownership of other's resources, leave other in valid state
        (void)other;  // Remove this line when implementing
    }

    // Copy assignment operator
    SimpleString& operator=(const SimpleString& other) {
        // TODO: Implement this
        // Hint: Check for self-assignment, then copy
        // Alternative: Use copy-and-swap idiom
        (void)other;  // Remove this line when implementing
        return *this;
    }

    // Move assignment operator (should be noexcept!)
    SimpleString& operator=(SimpleString&& other) noexcept {
        // TODO: Implement this
        // Hint: Check for self-assignment, transfer resources
        (void)other;  // Remove this line when implementing
        return *this;
    }

    // ==========================================================================
    // TODO: Implement these accessor/utility functions
    // ==========================================================================

    // Returns the length of the string (not including null terminator)
    [[nodiscard]] std::size_t length() const {
        // TODO: Implement this
        return 0;
    }

    // Returns the length of the string (alias for length)
    [[nodiscard]] std::size_t size() const {
        return length();
    }

    // Returns true if the string is empty
    [[nodiscard]] bool empty() const {
        // TODO: Implement this
        return true;
    }

    // Returns pointer to null-terminated string
    [[nodiscard]] const char* c_str() const {
        // TODO: Implement this
        return "";
    }

    // Clears the string, making it empty
    void clear() {
        // TODO: Implement this
    }

    // ==========================================================================
    // TODO: Implement these operators
    // ==========================================================================

    // Element access (no bounds checking)
    char& operator[](std::size_t index) {
        // TODO: Implement this
        (void)index;  // Remove this line when implementing
        static char dummy = '\0';
        return dummy;
    }

    const char& operator[](std::size_t index) const {
        // TODO: Implement this
        (void)index;  // Remove this line when implementing
        static char dummy = '\0';
        return dummy;
    }

    // Concatenation
    SimpleString& operator+=(const SimpleString& other) {
        // TODO: Implement this
        // Hint: Allocate new buffer, copy both strings, delete old buffer
        (void)other;  // Remove this line when implementing
        return *this;
    }

    SimpleString& operator+=(char c) {
        // TODO: Implement this
        // Hint: Allocate new buffer with room for one more char
        (void)c;  // Remove this line when implementing
        return *this;
    }

    // Comparison
    bool operator==(const SimpleString& other) const {
        // TODO: Implement this
        // Hint: Compare sizes first, then use strcmp or manual comparison
        (void)other;  // Remove this line when implementing
        return false;
    }

    bool operator!=(const SimpleString& other) const {
        return !(*this == other);
    }

    // ==========================================================================
    // Provided: Swap and stream operators
    // ==========================================================================

    friend void swap(SimpleString& a, SimpleString& b) noexcept {
        using std::swap;
        swap(a.data_, b.data_);
        swap(a.size_, b.size_);
    }

    friend std::ostream& operator<<(std::ostream& os, const SimpleString& str) {
        return os << str.c_str();
    }
};

// Binary concatenation operator
SimpleString operator+(SimpleString lhs, const SimpleString& rhs) {
    return lhs += rhs;
}

// =============================================================================
// Test your implementation
// =============================================================================

int main() {
    std::cout << "=== SimpleString Exercise ===\n\n";

    // Test 1: Default construction
    std::cout << "Test 1: Default construction\n";
    SimpleString s1;
    std::cout << "  Empty string length: " << s1.length() << " (expected: 0)\n";
    std::cout << "  Empty: " << (s1.empty() ? "true" : "false") << " (expected: true)\n";

    // Test 2: Construction from C-string
    std::cout << "\nTest 2: Construction from C-string\n";
    SimpleString s2{"Hello"};
    std::cout << "  s2 = \"" << s2 << "\" (expected: Hello)\n";
    std::cout << "  s2.length() = " << s2.length() << " (expected: 5)\n";

    // Test 3: Copy construction
    std::cout << "\nTest 3: Copy construction\n";
    SimpleString s3{s2};
    std::cout << "  s3 = \"" << s3 << "\" (expected: Hello)\n";
    std::cout << "  s2 == s3: " << (s2 == s3 ? "true" : "false") << " (expected: true)\n";

    // Test 4: Move construction
    std::cout << "\nTest 4: Move construction\n";
    SimpleString s4{std::move(s3)};
    std::cout << "  s4 = \"" << s4 << "\" (expected: Hello)\n";
    std::cout << "  s3 (moved-from) empty: " << (s3.empty() ? "true" : "false") << " (expected: true)\n";

    // Test 5: Copy assignment
    std::cout << "\nTest 5: Copy assignment\n";
    SimpleString s5;
    s5 = s2;
    std::cout << "  s5 = \"" << s5 << "\" (expected: Hello)\n";

    // Test 6: Move assignment
    std::cout << "\nTest 6: Move assignment\n";
    SimpleString s6{"Temporary"};
    SimpleString s7;
    s7 = std::move(s6);
    std::cout << "  s7 = \"" << s7 << "\" (expected: Temporary)\n";
    std::cout << "  s6 (moved-from) empty: " << (s6.empty() ? "true" : "false") << " (expected: true)\n";

    // Test 7: Self-assignment
    std::cout << "\nTest 7: Self-assignment\n";
    SimpleString s8{"Self"};
    s8 = s8;  // Should not crash or corrupt
    std::cout << "  s8 = \"" << s8 << "\" (expected: Self)\n";

    // Test 8: Subscript operator
    std::cout << "\nTest 8: Subscript operator\n";
    std::cout << "  s2[0] = '" << s2[0] << "' (expected: H)\n";
    std::cout << "  s2[4] = '" << s2[4] << "' (expected: o)\n";

    // Test 9: Concatenation
    std::cout << "\nTest 9: Concatenation\n";
    SimpleString s9{"Hello"};
    SimpleString s10{", World!"};
    SimpleString s11 = s9 + s10;
    std::cout << "  s9 + s10 = \"" << s11 << "\" (expected: Hello, World!)\n";

    s9 += s10;
    std::cout << "  s9 += s10: \"" << s9 << "\" (expected: Hello, World!)\n";

    // Test 10: Character append
    std::cout << "\nTest 10: Character append\n";
    SimpleString s12{"Test"};
    s12 += '!';
    std::cout << "  s12 += '!': \"" << s12 << "\" (expected: Test!)\n";

    // Test 11: Clear
    std::cout << "\nTest 11: Clear\n";
    SimpleString s13{"ClearMe"};
    s13.clear();
    std::cout << "  After clear, empty: " << (s13.empty() ? "true" : "false") << " (expected: true)\n";

    std::cout << "\n=== Exercise Complete ===\n";
    return 0;
}
