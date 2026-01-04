// Chapter 6: Essential Operations - Resource Management (RAII)
//
// This example demonstrates:
// - RAII (Resource Acquisition Is Initialization) pattern
// - Smart pointers: unique_ptr, shared_ptr, weak_ptr
// - Custom RAII wrappers
// - Exception safety through RAII
// - Rule of Zero with smart pointers

#include <cstdio>
#include <fstream>
#include <iostream>
#include <memory>
#include <mutex>
#include <string>
#include <utility>
#include <vector>

// =============================================================================
// 1. The Problem: Manual Resource Management
// =============================================================================

namespace manual_example {

// BAD: Manual resource management is error-prone
void dangerous_function() {
    int* data = new int[100];

    // What if an exception is thrown here?
    // What if we add an early return?
    // The delete[] will never be called!

    // ... some operations that might throw ...

    delete[] data;  // Easy to forget or skip
}

// Also BAD: Multiple resources compound the problem
void multiple_resources() {
    int* data1 = new int[100];
    int* data2 = new int[200];  // If this throws, data1 leaks!

    // ... operations ...

    delete[] data2;
    delete[] data1;
}

}  // namespace manual_example

// =============================================================================
// 2. RAII Solution: Let Objects Manage Resources
// =============================================================================

namespace raii_example {

// Simple RAII wrapper for a dynamic array
class IntArray {
    int* data_;
    std::size_t size_;

public:
    explicit IntArray(std::size_t size)
        : data_{new int[size]{}}, size_{size} {
        std::cout << "  IntArray: allocated " << size << " ints\n";
    }

    ~IntArray() {
        std::cout << "  IntArray: deallocating " << size_ << " ints\n";
        delete[] data_;
    }

    // Disable copying (Rule of Five consideration)
    IntArray(const IntArray&) = delete;
    IntArray& operator=(const IntArray&) = delete;

    // Enable moving
    IntArray(IntArray&& other) noexcept
        : data_{other.data_}, size_{other.size_} {
        other.data_ = nullptr;
        other.size_ = 0;
    }

    IntArray& operator=(IntArray&& other) noexcept {
        if (this != &other) {
            delete[] data_;
            data_ = other.data_;
            size_ = other.size_;
            other.data_ = nullptr;
            other.size_ = 0;
        }
        return *this;
    }

    int& operator[](std::size_t i) { return data_[i]; }
    [[nodiscard]] std::size_t size() const { return size_; }
};

void safe_function() {
    IntArray arr{100};  // Resource acquired in constructor
    // arr automatically cleaned up when it goes out of scope
    // Even if an exception is thrown!
}

}  // namespace raii_example

// =============================================================================
// 3. Smart Pointers
// =============================================================================

namespace smart_pointers {

struct Resource {
    std::string name;

    explicit Resource(std::string n) : name{std::move(n)} {
        std::cout << "  Resource '" << name << "' created\n";
    }

    ~Resource() {
        std::cout << "  Resource '" << name << "' destroyed\n";
    }

    void use() const {
        std::cout << "  Using resource: " << name << "\n";
    }
};

// -----------------------------------------------------------------------------
// unique_ptr: Exclusive ownership
// -----------------------------------------------------------------------------

void unique_ptr_demo() {
    std::cout << "\n--- unique_ptr Demo ---\n";

    // Create with make_unique (preferred)
    auto ptr1 = std::make_unique<Resource>("exclusive");
    ptr1->use();

    // Transfer ownership with std::move
    auto ptr2 = std::move(ptr1);
    std::cout << "  After move, ptr1 is " << (ptr1 ? "valid" : "null") << "\n";
    ptr2->use();

    // unique_ptr for arrays
    auto arr = std::make_unique<int[]>(10);
    arr[0] = 42;
    std::cout << "  Array element: " << arr[0] << "\n";

    // Automatic cleanup when ptr2 goes out of scope
}

// -----------------------------------------------------------------------------
// shared_ptr: Shared ownership with reference counting
// -----------------------------------------------------------------------------

void shared_ptr_demo() {
    std::cout << "\n--- shared_ptr Demo ---\n";

    // Create with make_shared (efficient: single allocation)
    auto ptr1 = std::make_shared<Resource>("shared");
    std::cout << "  Reference count: " << ptr1.use_count() << "\n";

    {
        auto ptr2 = ptr1;  // Share ownership
        std::cout << "  Reference count after copy: " << ptr1.use_count() << "\n";
        ptr2->use();
    }  // ptr2 destroyed, but resource lives on

    std::cout << "  Reference count after ptr2 destroyed: " << ptr1.use_count() << "\n";
    ptr1->use();

    // Resource destroyed when last shared_ptr goes out of scope
}

// -----------------------------------------------------------------------------
// weak_ptr: Non-owning observer
// -----------------------------------------------------------------------------

void weak_ptr_demo() {
    std::cout << "\n--- weak_ptr Demo ---\n";

    std::weak_ptr<Resource> weak;

    {
        auto shared = std::make_shared<Resource>("observed");
        weak = shared;  // weak_ptr doesn't increase ref count

        std::cout << "  Inside scope - expired: " << weak.expired() << "\n";

        // Convert to shared_ptr to use (safe access)
        if (auto locked = weak.lock()) {
            locked->use();
            std::cout << "  Shared ref count: " << locked.use_count() << "\n";
        }
    }  // shared destroyed, resource destroyed

    std::cout << "  Outside scope - expired: " << weak.expired() << "\n";

    // Safe: lock() returns empty shared_ptr if expired
    if (auto locked = weak.lock()) {
        locked->use();  // Won't execute
    } else {
        std::cout << "  Resource no longer available\n";
    }
}

}  // namespace smart_pointers

// =============================================================================
// 4. Custom RAII Wrappers
// =============================================================================

namespace custom_raii {

// RAII wrapper for C-style FILE*
class FileHandle {
    std::FILE* file_;

public:
    explicit FileHandle(const char* filename, const char* mode)
        : file_{std::fopen(filename, mode)} {
        if (!file_) {
            throw std::runtime_error("Failed to open file");
        }
        std::cout << "  FileHandle: opened file\n";
    }

    ~FileHandle() {
        if (file_) {
            std::fclose(file_);
            std::cout << "  FileHandle: closed file\n";
        }
    }

    // Non-copyable
    FileHandle(const FileHandle&) = delete;
    FileHandle& operator=(const FileHandle&) = delete;

    // Movable
    FileHandle(FileHandle&& other) noexcept : file_{other.file_} {
        other.file_ = nullptr;
    }

    FileHandle& operator=(FileHandle&& other) noexcept {
        if (this != &other) {
            if (file_) std::fclose(file_);
            file_ = other.file_;
            other.file_ = nullptr;
        }
        return *this;
    }

    std::FILE* get() const { return file_; }

    void write(const std::string& text) {
        std::fputs(text.c_str(), file_);
    }
};

// Generic RAII wrapper using a custom deleter
template<typename T, typename Deleter>
class ScopedResource {
    T resource_;
    Deleter deleter_;
    bool valid_;

public:
    ScopedResource(T resource, Deleter deleter)
        : resource_{resource}, deleter_{std::move(deleter)}, valid_{true} {}

    ~ScopedResource() {
        if (valid_) {
            deleter_(resource_);
        }
    }

    // Non-copyable
    ScopedResource(const ScopedResource&) = delete;
    ScopedResource& operator=(const ScopedResource&) = delete;

    // Movable
    ScopedResource(ScopedResource&& other) noexcept
        : resource_{other.resource_},
          deleter_{std::move(other.deleter_)},
          valid_{other.valid_} {
        other.valid_ = false;
    }

    ScopedResource& operator=(ScopedResource&& other) noexcept {
        if (this != &other) {
            if (valid_) deleter_(resource_);
            resource_ = other.resource_;
            deleter_ = std::move(other.deleter_);
            valid_ = other.valid_;
            other.valid_ = false;
        }
        return *this;
    }

    T get() const { return resource_; }
    T operator*() const { return resource_; }
};

// Factory function for type deduction
template<typename T, typename Deleter>
auto make_scoped(T resource, Deleter deleter) {
    return ScopedResource<T, Deleter>{resource, std::move(deleter)};
}

}  // namespace custom_raii

// =============================================================================
// 5. Rule of Zero: Using RAII Members
// =============================================================================

namespace rule_of_zero {

// GOOD: No manual resource management needed
// All members manage their own resources
class Document {
    std::string title_;
    std::string content_;
    std::vector<std::string> tags_;
    std::unique_ptr<std::fstream> file_;  // Optional file handle

public:
    explicit Document(std::string title)
        : title_{std::move(title)} {
        std::cout << "  Document created: " << title_ << "\n";
    }

    // Rule of Zero: Compiler-generated special members work correctly!
    // No need for custom destructor, copy/move constructors, or assignment operators

    void set_content(std::string content) { content_ = std::move(content); }
    void add_tag(std::string tag) { tags_.push_back(std::move(tag)); }

    void print() const {
        std::cout << "  Title: " << title_ << "\n";
        std::cout << "  Content: " << content_ << "\n";
        std::cout << "  Tags: ";
        for (const auto& tag : tags_) {
            std::cout << "[" << tag << "] ";
        }
        std::cout << "\n";
    }
};

void demo() {
    std::cout << "\n--- Rule of Zero Demo ---\n";

    Document doc{"My Document"};
    doc.set_content("Hello, World!");
    doc.add_tag("example");
    doc.add_tag("cpp20");
    doc.print();

    // Move works automatically
    Document doc2 = std::move(doc);
    std::cout << "  After move:\n";
    doc2.print();
}

}  // namespace rule_of_zero

// =============================================================================
// 6. Exception Safety with RAII
// =============================================================================

namespace exception_safety {

class ThrowingOperation {
public:
    static void maybe_throw(bool should_throw) {
        if (should_throw) {
            throw std::runtime_error("Operation failed!");
        }
    }
};

void demo_exception_safety() {
    std::cout << "\n--- Exception Safety Demo ---\n";

    try {
        auto resource = std::make_unique<int[]>(100);
        std::cout << "  Resource allocated\n";

        // Even if this throws, resource is cleaned up
        ThrowingOperation::maybe_throw(true);

        std::cout << "  This line never executes\n";
    } catch (const std::exception& e) {
        std::cout << "  Caught exception: " << e.what() << "\n";
        std::cout << "  Resource was still properly cleaned up!\n";
    }
}

// Multiple resources are safe with RAII
void multiple_resources_safe() {
    auto r1 = std::make_unique<int>(1);
    auto r2 = std::make_unique<int>(2);  // If this throws, r1 is cleaned up
    auto r3 = std::make_unique<int>(3);  // If this throws, r1 and r2 are cleaned up

    // Use resources...
    // All automatically cleaned up in reverse order
}

}  // namespace exception_safety

// =============================================================================
// Main
// =============================================================================

int main() {
    std::cout << "=== Resource Management (RAII) Demo ===\n";

    // RAII wrapper demo
    std::cout << "\n--- RAII IntArray Demo ---\n";
    {
        raii_example::IntArray arr{10};
        arr[0] = 42;
        std::cout << "  arr[0] = " << arr[0] << "\n";
    }

    // Smart pointers
    smart_pointers::unique_ptr_demo();
    smart_pointers::shared_ptr_demo();
    smart_pointers::weak_ptr_demo();

    // Custom RAII with generic wrapper
    std::cout << "\n--- Custom Scoped Resource Demo ---\n";
    {
        int* raw = new int{42};
        auto scoped = custom_raii::make_scoped(raw, [](int* p) {
            std::cout << "  Deleting int with value: " << *p << "\n";
            delete p;
        });
        std::cout << "  Value: " << *scoped.get() << "\n";
    }

    // Rule of Zero
    rule_of_zero::demo();

    // Exception safety
    exception_safety::demo_exception_safety();

    std::cout << "\n=== End of Demo ===\n";
    return 0;
}
