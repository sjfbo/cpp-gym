// unique_ptr - Exclusive Ownership Smart Pointer
// Book reference: 15.2.1 unique_ptr
//
// unique_ptr provides exclusive ownership semantics:
// - Only one unique_ptr can own an object at a time
// - Automatically deletes the object when the unique_ptr goes out of scope
// - Zero overhead compared to raw pointers
// - Move-only: cannot be copied, only moved

#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <fstream>
#include <cstdio>

// A simple class to demonstrate ownership
class Resource {
public:
    Resource(std::string name) : name_{std::move(name)} {
        std::cout << "Resource '" << name_ << "' constructed\n";
    }

    ~Resource() {
        std::cout << "Resource '" << name_ << "' destroyed\n";
    }

    void use() const {
        std::cout << "Using resource '" << name_ << "'\n";
    }

    const std::string& name() const { return name_; }

private:
    std::string name_;
};

// ============================================================================
// Basic unique_ptr usage
// ============================================================================

void basic_usage() {
    std::cout << "=== Basic unique_ptr Usage ===\n";

    // Preferred: use make_unique (exception-safe, more efficient)
    auto ptr = std::make_unique<Resource>("Alpha");
    ptr->use();

    // Access the raw pointer (don't delete it!)
    Resource* raw = ptr.get();
    std::cout << "Raw pointer to: " << raw->name() << "\n";

    // Check if unique_ptr owns an object
    if (ptr) {
        std::cout << "ptr owns a resource\n";
    }

    // unique_ptr automatically deletes when going out of scope
    std::cout << "Leaving scope...\n";
}

// ============================================================================
// Ownership transfer with move
// ============================================================================

std::unique_ptr<Resource> create_resource(const std::string& name) {
    // Factory function returning ownership
    return std::make_unique<Resource>(name);
}

void take_ownership(std::unique_ptr<Resource> res) {
    // Function taking ownership
    std::cout << "Took ownership of: " << res->name() << "\n";
    // res is destroyed when function returns
}

void ownership_transfer() {
    std::cout << "\n=== Ownership Transfer ===\n";

    // Create and receive ownership from factory
    auto ptr1 = create_resource("Beta");

    // Transfer ownership via move
    auto ptr2 = std::move(ptr1);

    // ptr1 is now nullptr
    if (!ptr1) {
        std::cout << "ptr1 is now empty\n";
    }

    // ptr2 owns the resource
    ptr2->use();

    // Transfer to function (function takes ownership)
    take_ownership(std::move(ptr2));

    // ptr2 is now empty
    if (!ptr2) {
        std::cout << "ptr2 is now empty\n";
    }
}

// ============================================================================
// unique_ptr for arrays
// ============================================================================

void array_unique_ptr() {
    std::cout << "\n=== unique_ptr for Arrays ===\n";

    // unique_ptr can manage arrays
    auto arr = std::make_unique<int[]>(5);

    // Initialize array elements
    for (int i = 0; i < 5; ++i) {
        arr[i] = i * 10;
    }

    // Access elements
    std::cout << "Array: ";
    for (int i = 0; i < 5; ++i) {
        std::cout << arr[i] << " ";
    }
    std::cout << "\n";

    // Note: Prefer std::vector for most cases
    // unique_ptr<T[]> is useful when interfacing with C APIs
}

// ============================================================================
// Custom deleters
// ============================================================================

void custom_deleters() {
    std::cout << "\n=== Custom Deleters ===\n";

    // Custom deleter for FILE*
    auto file_deleter = [](FILE* fp) {
        if (fp) {
            std::cout << "Closing file\n";
            std::fclose(fp);
        }
    };

    // unique_ptr with custom deleter
    {
        std::unique_ptr<FILE, decltype(file_deleter)> file(
            std::fopen("test.txt", "w"), file_deleter);

        if (file) {
            std::fputs("Hello from unique_ptr!\n", file.get());
            std::cout << "Wrote to file\n";
        }
        // file automatically closed when leaving scope
    }

    // Using function pointer as deleter
    auto file2 = std::unique_ptr<FILE, int(*)(FILE*)>(
        std::fopen("test.txt", "r"), &std::fclose);

    if (file2) {
        char buffer[100];
        if (std::fgets(buffer, sizeof(buffer), file2.get())) {
            std::cout << "Read from file: " << buffer;
        }
    }

    // Cleanup test file
    std::remove("test.txt");

    // Custom deleter for any resource
    struct Connection {
        int id;
    };

    auto connection_deleter = [](Connection* c) {
        std::cout << "Closing connection " << c->id << "\n";
        delete c;
    };

    using ConnectionPtr = std::unique_ptr<Connection, decltype(connection_deleter)>;
    ConnectionPtr conn(new Connection{42}, connection_deleter);
    std::cout << "Using connection " << conn->id << "\n";
}

// ============================================================================
// unique_ptr in containers
// ============================================================================

void unique_ptr_in_containers() {
    std::cout << "\n=== unique_ptr in Containers ===\n";

    std::vector<std::unique_ptr<Resource>> resources;

    // Add resources (must move into container)
    resources.push_back(std::make_unique<Resource>("One"));
    resources.push_back(std::make_unique<Resource>("Two"));
    resources.push_back(std::make_unique<Resource>("Three"));

    // Access through container
    for (const auto& res : resources) {
        res->use();
    }

    // Remove specific element
    std::cout << "Removing 'Two'...\n";
    resources.erase(resources.begin() + 1);

    std::cout << "After removal:\n";
    for (const auto& res : resources) {
        res->use();
    }

    // Clear container (all resources destroyed)
    std::cout << "Clearing container...\n";
}

// ============================================================================
// reset() and release()
// ============================================================================

void reset_and_release() {
    std::cout << "\n=== reset() and release() ===\n";

    auto ptr = std::make_unique<Resource>("Gamma");

    // reset() - delete current object and optionally take ownership of new one
    std::cout << "Calling reset() with new object...\n";
    ptr.reset(new Resource("Delta"));  // Gamma destroyed, now owns Delta

    // reset() to nullptr
    std::cout << "Calling reset() to nullptr...\n";
    ptr.reset();  // Delta destroyed, ptr is now nullptr

    // release() - give up ownership and return raw pointer
    ptr = std::make_unique<Resource>("Epsilon");
    Resource* raw = ptr.release();  // ptr is now nullptr, we own raw

    std::cout << "After release(), ptr is " << (ptr ? "not null" : "null") << "\n";
    std::cout << "raw points to: " << raw->name() << "\n";

    // We must manually delete raw now!
    delete raw;
    std::cout << "Manually deleted raw pointer\n";
}

// ============================================================================
// Polymorphism with unique_ptr
// ============================================================================

class Animal {
public:
    virtual ~Animal() = default;
    virtual void speak() const = 0;
};

class Dog : public Animal {
public:
    void speak() const override { std::cout << "Woof!\n"; }
};

class Cat : public Animal {
public:
    void speak() const override { std::cout << "Meow!\n"; }
};

void polymorphism() {
    std::cout << "\n=== Polymorphism with unique_ptr ===\n";

    std::vector<std::unique_ptr<Animal>> animals;

    animals.push_back(std::make_unique<Dog>());
    animals.push_back(std::make_unique<Cat>());
    animals.push_back(std::make_unique<Dog>());

    for (const auto& animal : animals) {
        animal->speak();
    }
}

int main() {
    basic_usage();
    ownership_transfer();
    array_unique_ptr();
    custom_deleters();
    unique_ptr_in_containers();
    reset_and_release();
    polymorphism();

    std::cout << "\n=== Program ending ===\n";
    return 0;
}

// Key points:
// - Use make_unique for exception safety and efficiency
// - unique_ptr is move-only; use std::move to transfer ownership
// - Zero overhead compared to raw pointers
// - Automatically deletes owned object when destroyed
// - Supports custom deleters for non-standard cleanup
// - Works with polymorphic types through base class pointers
