// Chapter 6: Essential Operations - Copy and Move Semantics
//
// This example demonstrates:
// - Copy constructor vs move constructor
// - Copy assignment vs move assignment
// - When each operation is called
// - std::move and its behavior
// - Rule of Five implementation

#include <algorithm>
#include <cstddef>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

// =============================================================================
// A class that tracks all special member function calls
// =============================================================================

class TrackedResource {
    int* data_;
    std::size_t size_;
    std::string name_;

public:
    // Constructor
    explicit TrackedResource(std::size_t size, std::string name = "unnamed")
        : data_{new int[size]{}}, size_{size}, name_{std::move(name)} {
        std::cout << "  [" << name_ << "] Constructor (size=" << size_ << ")\n";
    }

    // Destructor
    ~TrackedResource() {
        std::cout << "  [" << name_ << "] Destructor";
        if (data_) {
            std::cout << " (freeing " << size_ << " ints)";
            delete[] data_;
        } else {
            std::cout << " (moved-from, nothing to free)";
        }
        std::cout << "\n";
    }

    // Copy constructor
    TrackedResource(const TrackedResource& other)
        : data_{new int[other.size_]},
          size_{other.size_},
          name_{other.name_ + "_copy"} {
        std::copy(other.data_, other.data_ + size_, data_);
        std::cout << "  [" << name_ << "] Copy constructor (from " << other.name_ << ")\n";
    }

    // Move constructor
    TrackedResource(TrackedResource&& other) noexcept
        : data_{other.data_},
          size_{other.size_},
          name_{std::move(other.name_) + "_moved"} {
        std::cout << "  [" << name_ << "] Move constructor\n";
        other.data_ = nullptr;
        other.size_ = 0;
        other.name_ = "moved-from";
    }

    // Copy assignment
    TrackedResource& operator=(const TrackedResource& other) {
        std::cout << "  [" << name_ << "] Copy assignment (from " << other.name_ << ")\n";
        if (this != &other) {
            delete[] data_;
            size_ = other.size_;
            data_ = new int[size_];
            std::copy(other.data_, other.data_ + size_, data_);
        }
        return *this;
    }

    // Move assignment
    TrackedResource& operator=(TrackedResource&& other) noexcept {
        std::cout << "  [" << name_ << "] Move assignment (from " << other.name_ << ")\n";
        if (this != &other) {
            delete[] data_;
            data_ = other.data_;
            size_ = other.size_;
            other.data_ = nullptr;
            other.size_ = 0;
        }
        return *this;
    }

    // Accessors
    [[nodiscard]] std::size_t size() const { return size_; }
    [[nodiscard]] const std::string& name() const { return name_; }
    [[nodiscard]] bool valid() const { return data_ != nullptr; }
};

// =============================================================================
// Helper functions to demonstrate copy/move scenarios
// =============================================================================

// Return by value - triggers move (or copy elision)
TrackedResource create_resource(std::size_t size, const std::string& name) {
    TrackedResource r{size, name};
    return r;  // Move (or NRVO eliminates the move entirely)
}

// Pass by value - triggers copy or move depending on argument
void consume_resource(TrackedResource r) {
    std::cout << "  consume_resource: got resource '" << r.name()
              << "' with size " << r.size() << "\n";
}

// Pass by const reference - no copy, no move
void inspect_resource(const TrackedResource& r) {
    std::cout << "  inspect_resource: inspecting '" << r.name()
              << "' with size " << r.size() << "\n";
}

// =============================================================================
// Main demonstration
// =============================================================================

int main() {
    std::cout << "=== Copy and Move Semantics Demo ===\n\n";

    // -------------------------------------------------------------------------
    std::cout << "1. Basic construction:\n";
    {
        TrackedResource r1{10, "r1"};
    }  // Destructor called here
    std::cout << "\n";

    // -------------------------------------------------------------------------
    std::cout << "2. Copy construction:\n";
    {
        TrackedResource original{5, "original"};
        TrackedResource copied{original};  // Copy constructor
        std::cout << "  original valid: " << original.valid()
                  << ", copied valid: " << copied.valid() << "\n";
    }
    std::cout << "\n";

    // -------------------------------------------------------------------------
    std::cout << "3. Move construction (using std::move):\n";
    {
        TrackedResource original{5, "original"};
        TrackedResource moved{std::move(original)};  // Move constructor
        std::cout << "  original valid: " << original.valid()
                  << ", moved valid: " << moved.valid() << "\n";
    }
    std::cout << "\n";

    // -------------------------------------------------------------------------
    std::cout << "4. Copy assignment:\n";
    {
        TrackedResource a{3, "a"};
        TrackedResource b{7, "b"};
        b = a;  // Copy assignment
    }
    std::cout << "\n";

    // -------------------------------------------------------------------------
    std::cout << "5. Move assignment (using std::move):\n";
    {
        TrackedResource a{3, "a"};
        TrackedResource b{7, "b"};
        b = std::move(a);  // Move assignment
        std::cout << "  a valid: " << a.valid() << ", b valid: " << b.valid() << "\n";
    }
    std::cout << "\n";

    // -------------------------------------------------------------------------
    std::cout << "6. Return value optimization (RVO/NRVO):\n";
    {
        // Compiler may elide the move entirely (Named Return Value Optimization)
        auto r = create_resource(8, "created");
        std::cout << "  Got: " << r.name() << "\n";
    }
    std::cout << "\n";

    // -------------------------------------------------------------------------
    std::cout << "7. Passing to function by value (lvalue - copies):\n";
    {
        TrackedResource r{4, "resource"};
        consume_resource(r);  // Copies because r is an lvalue
        std::cout << "  r still valid: " << r.valid() << "\n";
    }
    std::cout << "\n";

    // -------------------------------------------------------------------------
    std::cout << "8. Passing to function by value (rvalue - moves):\n";
    {
        TrackedResource r{4, "resource"};
        consume_resource(std::move(r));  // Moves because std::move makes it rvalue
        std::cout << "  r still valid: " << r.valid() << "\n";
    }
    std::cout << "\n";

    // -------------------------------------------------------------------------
    std::cout << "9. Passing by const reference (no copy/move):\n";
    {
        TrackedResource r{4, "resource"};
        inspect_resource(r);  // No copy, no move - just a reference
    }
    std::cout << "\n";

    // -------------------------------------------------------------------------
    std::cout << "10. std::vector and move semantics:\n";
    {
        std::vector<TrackedResource> vec;
        vec.reserve(3);  // Pre-allocate to avoid reallocation moves

        std::cout << "  Adding r1...\n";
        vec.push_back(TrackedResource{2, "r1"});  // Move from temporary

        std::cout << "  Adding r2...\n";
        TrackedResource r2{3, "r2"};
        vec.push_back(std::move(r2));  // Explicit move

        std::cout << "  Adding r3 (copy)...\n";
        TrackedResource r3{4, "r3"};
        vec.push_back(r3);  // Copy (r3 is lvalue)
    }
    std::cout << "\n";

    // -------------------------------------------------------------------------
    std::cout << "11. Self-assignment safety:\n";
    {
        TrackedResource r{5, "self"};
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wself-assign-overloaded"
        r = r;  // Self-assignment - should be handled safely
#pragma clang diagnostic pop
    }
    std::cout << "\n";

    std::cout << "=== End of Demo ===\n";
    return 0;
}
