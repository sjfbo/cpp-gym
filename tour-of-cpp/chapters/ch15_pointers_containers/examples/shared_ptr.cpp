// shared_ptr - Shared Ownership Smart Pointer
// Book reference: 15.2.2 shared_ptr, 15.2.3 weak_ptr
//
// shared_ptr provides reference-counted shared ownership:
// - Multiple shared_ptrs can own the same object
// - Object is deleted when the last shared_ptr is destroyed
// - Can be copied (unlike unique_ptr)
// - Use weak_ptr to break reference cycles

#include <iostream>
#include <memory>
#include <string>
#include <vector>

// A simple class to demonstrate lifetime management
class Widget {
public:
    Widget(int id) : id_{id} {
        std::cout << "Widget " << id_ << " constructed\n";
    }

    ~Widget() {
        std::cout << "Widget " << id_ << " destroyed\n";
    }

    int id() const { return id_; }

private:
    int id_;
};

// ============================================================================
// Basic shared_ptr usage
// ============================================================================

void basic_usage() {
    std::cout << "=== Basic shared_ptr Usage ===\n";

    // Preferred: use make_shared (single allocation, more efficient)
    auto sp1 = std::make_shared<Widget>(1);
    std::cout << "Reference count: " << sp1.use_count() << "\n";

    // Copy creates shared ownership
    auto sp2 = sp1;
    std::cout << "After copy, reference count: " << sp1.use_count() << "\n";

    // Access the object
    std::cout << "sp1 points to Widget " << sp1->id() << "\n";
    std::cout << "sp2 points to Widget " << sp2->id() << "\n";

    // Check if same object
    if (sp1 == sp2) {
        std::cout << "sp1 and sp2 point to the same Widget\n";
    }

    // sp1 goes out of scope, but Widget survives (sp2 still owns it)
    sp1.reset();
    std::cout << "After sp1.reset(), sp2 count: " << sp2.use_count() << "\n";

    std::cout << "Leaving scope (sp2 destroyed, Widget deleted)...\n";
}

// ============================================================================
// shared_ptr in functions
// ============================================================================

std::shared_ptr<Widget> create_widget(int id) {
    return std::make_shared<Widget>(id);
}

void use_widget(std::shared_ptr<Widget> w) {
    // Incrementing reference count while function runs
    std::cout << "Using Widget " << w->id()
              << " (count: " << w.use_count() << ")\n";
}

void observe_widget(const std::shared_ptr<Widget>& w) {
    // No increment - just observing
    std::cout << "Observing Widget " << w->id()
              << " (count: " << w.use_count() << ")\n";
}

void shared_ptr_passing() {
    std::cout << "\n=== Passing shared_ptr ===\n";

    auto widget = create_widget(2);
    std::cout << "Initial count: " << widget.use_count() << "\n";

    // Pass by value - increments count
    use_widget(widget);
    std::cout << "After use_widget: " << widget.use_count() << "\n";

    // Pass by const reference - no increment
    observe_widget(widget);
    std::cout << "After observe_widget: " << widget.use_count() << "\n";
}

// ============================================================================
// weak_ptr - Non-owning Observer
// ============================================================================

void weak_ptr_usage() {
    std::cout << "\n=== weak_ptr Usage ===\n";

    std::weak_ptr<Widget> weak;

    {
        auto shared = std::make_shared<Widget>(3);
        weak = shared;  // weak_ptr observes but doesn't own

        std::cout << "Inside scope:\n";
        std::cout << "  shared.use_count(): " << shared.use_count() << "\n";
        std::cout << "  weak.expired(): " << std::boolalpha << weak.expired() << "\n";

        // Access through weak_ptr using lock()
        if (auto locked = weak.lock()) {
            std::cout << "  Accessed Widget " << locked->id()
                      << " through weak_ptr\n";
        }
    }

    std::cout << "Outside scope:\n";
    std::cout << "  weak.expired(): " << weak.expired() << "\n";

    if (auto locked = weak.lock()) {
        std::cout << "  Widget still alive\n";
    } else {
        std::cout << "  Widget has been destroyed\n";
    }
}

// ============================================================================
// Breaking Reference Cycles with weak_ptr
// ============================================================================

// BAD: Circular reference causes memory leak
struct BadNode {
    std::string name;
    std::shared_ptr<BadNode> next;
    std::shared_ptr<BadNode> prev;  // Creates cycle!

    BadNode(std::string n) : name{std::move(n)} {
        std::cout << "BadNode '" << name << "' constructed\n";
    }

    ~BadNode() {
        std::cout << "BadNode '" << name << "' destroyed\n";
    }
};

// GOOD: Use weak_ptr to break cycle
struct GoodNode {
    std::string name;
    std::shared_ptr<GoodNode> next;
    std::weak_ptr<GoodNode> prev;  // Doesn't own, breaks cycle

    GoodNode(std::string n) : name{std::move(n)} {
        std::cout << "GoodNode '" << name << "' constructed\n";
    }

    ~GoodNode() {
        std::cout << "GoodNode '" << name << "' destroyed\n";
    }
};

void circular_reference_demo() {
    std::cout << "\n=== Circular Reference Problem ===\n";

    // Demonstrate the leak with BadNode
    std::cout << "Creating BadNodes (will leak):\n";
    {
        auto node1 = std::make_shared<BadNode>("Bad1");
        auto node2 = std::make_shared<BadNode>("Bad2");

        node1->next = node2;
        node2->prev = node1;  // Circular reference!

        std::cout << "node1 use_count: " << node1.use_count() << "\n";
        std::cout << "node2 use_count: " << node2.use_count() << "\n";
        std::cout << "Leaving scope... (BadNodes will NOT be destroyed)\n";
    }
    std::cout << "Leaked! No destructors called.\n";

    // Demonstrate proper cleanup with GoodNode
    std::cout << "\nCreating GoodNodes (no leak):\n";
    {
        auto node1 = std::make_shared<GoodNode>("Good1");
        auto node2 = std::make_shared<GoodNode>("Good2");

        node1->next = node2;
        node2->prev = node1;  // weak_ptr, doesn't increase count

        std::cout << "node1 use_count: " << node1.use_count() << "\n";
        std::cout << "node2 use_count: " << node2.use_count() << "\n";
        std::cout << "Leaving scope...\n";
    }
    std::cout << "No leak! Destructors called properly.\n";
}

// ============================================================================
// enable_shared_from_this
// ============================================================================

class SharedFromThis : public std::enable_shared_from_this<SharedFromThis> {
public:
    SharedFromThis(int id) : id_{id} {
        std::cout << "SharedFromThis " << id_ << " constructed\n";
    }

    ~SharedFromThis() {
        std::cout << "SharedFromThis " << id_ << " destroyed\n";
    }

    std::shared_ptr<SharedFromThis> get_shared() {
        return shared_from_this();
    }

    void register_callback(std::vector<std::shared_ptr<SharedFromThis>>& registry) {
        // Safely get a shared_ptr to this object
        registry.push_back(shared_from_this());
    }

    int id() const { return id_; }

private:
    int id_;
};

void enable_shared_from_this_demo() {
    std::cout << "\n=== enable_shared_from_this ===\n";

    std::vector<std::shared_ptr<SharedFromThis>> registry;

    auto obj = std::make_shared<SharedFromThis>(42);
    std::cout << "Initial count: " << obj.use_count() << "\n";

    // Object can safely get a shared_ptr to itself
    obj->register_callback(registry);
    std::cout << "After register: " << obj.use_count() << "\n";

    // Get another shared_ptr from the object
    auto another = obj->get_shared();
    std::cout << "After get_shared: " << obj.use_count() << "\n";

    // All point to same object
    std::cout << "All IDs: " << obj->id() << ", "
              << registry[0]->id() << ", " << another->id() << "\n";
}

// ============================================================================
// Aliasing Constructor
// ============================================================================

struct Container {
    int data;
    std::string name;

    Container(int d, std::string n) : data{d}, name{std::move(n)} {
        std::cout << "Container '" << name << "' constructed\n";
    }

    ~Container() {
        std::cout << "Container '" << name << "' destroyed\n";
    }
};

void aliasing_constructor_demo() {
    std::cout << "\n=== Aliasing Constructor ===\n";

    auto container = std::make_shared<Container>(42, "MyContainer");

    // Create shared_ptr to member, sharing ownership with container
    std::shared_ptr<int> data_ptr(container, &container->data);

    std::cout << "container.use_count(): " << container.use_count() << "\n";
    std::cout << "*data_ptr: " << *data_ptr << "\n";

    // Even after container goes out of scope, object lives
    container.reset();
    std::cout << "After container.reset(), *data_ptr: " << *data_ptr << "\n";
    std::cout << "data_ptr.use_count(): " << data_ptr.use_count() << "\n";

    // Container destroyed when data_ptr is destroyed
    std::cout << "Releasing data_ptr...\n";
}

// ============================================================================
// make_shared vs new
// ============================================================================

void make_shared_benefits() {
    std::cout << "\n=== make_shared Benefits ===\n";

    // BAD: Two allocations (object + control block)
    // Also not exception-safe in complex expressions
    std::shared_ptr<Widget> bad(new Widget(100));

    // GOOD: Single allocation for object + control block
    // Exception-safe, more efficient
    auto good = std::make_shared<Widget>(200);

    std::cout << "Both work, but make_shared is preferred\n";
}

int main() {
    basic_usage();
    shared_ptr_passing();
    weak_ptr_usage();
    circular_reference_demo();
    enable_shared_from_this_demo();
    aliasing_constructor_demo();
    make_shared_benefits();

    std::cout << "\n=== Program ending ===\n";
    return 0;
}

// Key points:
// - Use make_shared for efficiency and exception safety
// - shared_ptr uses reference counting; object deleted when count reaches 0
// - Use weak_ptr to observe without owning (breaks reference cycles)
// - Call weak_ptr::lock() to safely access the object
// - Inherit from enable_shared_from_this to get shared_ptr from this
// - Prefer unique_ptr when shared ownership isn't needed
