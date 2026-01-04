// Function Adaption - std::function, std::bind, and Lambdas
// Book reference: 16.3 Function Adaption
//
// C++ provides several mechanisms for working with callables:
// - std::function: type-erased callable wrapper
// - std::bind: partial application and argument reordering
// - Lambdas: inline function objects (preferred in modern C++)
// - std::invoke: uniform call syntax for all callables

#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>

// ============================================================================
// std::function Basics
// ============================================================================

int add(int a, int b) {
    return a + b;
}

struct Multiplier {
    int factor;

    int operator()(int x) const {
        return x * factor;
    }
};

void function_basics() {
    std::cout << "=== std::function Basics ===\n";

    // Wrap a regular function
    std::function<int(int, int)> f1 = add;
    std::cout << "add(3, 4) = " << f1(3, 4) << "\n";

    // Wrap a lambda
    std::function<int(int)> f2 = [](int x) { return x * x; };
    std::cout << "square(5) = " << f2(5) << "\n";

    // Wrap a functor (function object)
    std::function<int(int)> f3 = Multiplier{3};
    std::cout << "multiply by 3: " << f3(7) << "\n";

    // Empty function (callable check)
    std::function<void()> empty;
    std::cout << "Empty function: " << std::boolalpha << static_cast<bool>(empty) << "\n";

    // Assigning different callables
    std::function<int(int)> flexible;
    flexible = [](int x) { return x + 1; };
    std::cout << "x + 1: " << flexible(10) << "\n";

    flexible = Multiplier{2};
    std::cout << "x * 2: " << flexible(10) << "\n";
}

// ============================================================================
// std::function for Callbacks
// ============================================================================

class Button {
public:
    using Callback = std::function<void()>;

    void set_on_click(Callback cb) {
        on_click_ = std::move(cb);
    }

    void click() {
        if (on_click_) {
            on_click_();
        }
    }

private:
    Callback on_click_;
};

void callback_demo() {
    std::cout << "\n=== Callbacks with std::function ===\n";

    Button button;

    // Lambda callback
    button.set_on_click([]() {
        std::cout << "Button clicked!\n";
    });
    button.click();

    // Callback with captured state
    int click_count = 0;
    button.set_on_click([&click_count]() {
        ++click_count;
        std::cout << "Clicked " << click_count << " times\n";
    });

    button.click();
    button.click();
    button.click();
}

// ============================================================================
// std::bind - Partial Application
// ============================================================================

int divide(int a, int b) {
    return a / b;
}

void print_values(int x, int y, int z) {
    std::cout << "x=" << x << ", y=" << y << ", z=" << z << "\n";
}

void bind_demo() {
    std::cout << "\n=== std::bind (Prefer Lambdas) ===\n";

    using namespace std::placeholders;

    // Bind first argument
    auto divide_by_2 = std::bind(divide, _1, 2);
    std::cout << "10 / 2 = " << divide_by_2(10) << "\n";

    // Bind second argument
    auto divide_100_by = std::bind(divide, 100, _1);
    std::cout << "100 / 4 = " << divide_100_by(4) << "\n";

    // Reorder arguments
    auto reverse_divide = std::bind(divide, _2, _1);
    std::cout << "reverse(20, 4) = 4/20 = " << reverse_divide(20, 4) << "\n";

    // Multiple placeholders
    auto reorder = std::bind(print_values, _3, _1, _2);
    std::cout << "Reordered: ";
    reorder(1, 2, 3);  // Prints x=3, y=1, z=2

    // Nested binds
    auto add_then_double = std::bind(std::multiplies<int>(),
                                      std::bind(add, _1, _2), 2);
    std::cout << "(3 + 4) * 2 = " << add_then_double(3, 4) << "\n";
}

// ============================================================================
// Lambda Equivalents (Preferred)
// ============================================================================

void lambda_equivalents() {
    std::cout << "\n=== Lambda Equivalents (Preferred) ===\n";

    // Instead of bind(divide, _1, 2)
    auto divide_by_2 = [](int x) { return divide(x, 2); };
    std::cout << "10 / 2 = " << divide_by_2(10) << "\n";

    // Instead of bind(divide, 100, _1)
    auto divide_100_by = [](int x) { return divide(100, x); };
    std::cout << "100 / 4 = " << divide_100_by(4) << "\n";

    // Argument reordering
    auto reverse_divide = [](int a, int b) { return divide(b, a); };
    std::cout << "reverse(20, 4) = " << reverse_divide(20, 4) << "\n";

    // Capturing by value
    int multiplier = 3;
    auto times_n = [multiplier](int x) { return x * multiplier; };
    std::cout << "7 * 3 = " << times_n(7) << "\n";

    // Capturing by reference
    int counter = 0;
    auto increment = [&counter]() { return ++counter; };
    std::cout << "Counter: " << increment() << ", " << increment() << "\n";

    // Generic lambdas (C++14+)
    auto generic_add = [](auto a, auto b) { return a + b; };
    std::cout << "Generic add: " << generic_add(1, 2) << ", "
              << generic_add(1.5, 2.5) << "\n";
}

// ============================================================================
// std::ref and std::cref
// ============================================================================

void modify(int& x) {
    x *= 2;
}

void ref_demo() {
    std::cout << "\n=== std::ref and std::cref ===\n";

    int value = 10;
    std::cout << "Original: " << value << "\n";

    // bind copies by default
    auto bad_modifier = std::bind(modify, value);  // Copies value!
    bad_modifier();
    std::cout << "After bad_modifier: " << value << "\n";  // Still 10

    // Use std::ref for reference
    auto good_modifier = std::bind(modify, std::ref(value));
    good_modifier();
    std::cout << "After good_modifier: " << value << "\n";  // Now 20

    // std::cref for const reference
    auto print_ref = [](const int& x) { std::cout << "Value: " << x << "\n"; };
    std::function<void()> printer = std::bind(print_ref, std::cref(value));
    value = 100;
    printer();  // Prints 100 (sees updated value)
}

// ============================================================================
// std::invoke - Uniform Call Syntax
// ============================================================================

struct Widget {
    int value = 42;

    void print() const {
        std::cout << "Widget value: " << value << "\n";
    }

    int get_value() const {
        return value;
    }
};

void invoke_demo() {
    std::cout << "\n=== std::invoke ===\n";

    // Invoke regular function
    auto result1 = std::invoke(add, 3, 4);
    std::cout << "invoke(add, 3, 4) = " << result1 << "\n";

    // Invoke lambda
    auto lambda = [](int x) { return x * x; };
    auto result2 = std::invoke(lambda, 5);
    std::cout << "invoke(lambda, 5) = " << result2 << "\n";

    // Invoke member function
    Widget w;
    std::invoke(&Widget::print, w);

    // Invoke member function with smart pointer
    auto wp = std::make_unique<Widget>();
    wp->value = 100;
    std::invoke(&Widget::print, wp);

    // Access member variable
    auto val = std::invoke(&Widget::value, w);
    std::cout << "invoke(&Widget::value, w) = " << val << "\n";

    // invoke_r (C++23) for result type conversion
    // auto str = std::invoke_r<std::string>(...);
}

// ============================================================================
// std::mem_fn - Member Function Wrapper
// ============================================================================

void mem_fn_demo() {
    std::cout << "\n=== std::mem_fn ===\n";

    std::vector<Widget> widgets(3);
    widgets[0].value = 10;
    widgets[1].value = 20;
    widgets[2].value = 30;

    // Create callable from member function
    auto get_val = std::mem_fn(&Widget::get_value);

    for (const auto& w : widgets) {
        std::cout << "Value: " << get_val(w) << "\n";
    }

    // Works with pointers too
    Widget* ptr = &widgets[0];
    std::cout << "Via pointer: " << get_val(ptr) << "\n";

    // Works with smart pointers
    auto sp = std::make_shared<Widget>();
    sp->value = 42;
    std::cout << "Via shared_ptr: " << get_val(sp) << "\n";
}

// ============================================================================
// Practical: Event System
// ============================================================================

class EventEmitter {
public:
    using Handler = std::function<void(const std::string&)>;

    void on(const std::string& event, Handler handler) {
        handlers_[event].push_back(std::move(handler));
    }

    void emit(const std::string& event, const std::string& data = "") {
        if (auto it = handlers_.find(event); it != handlers_.end()) {
            for (const auto& handler : it->second) {
                handler(data);
            }
        }
    }

private:
    std::map<std::string, std::vector<Handler>> handlers_;
};

void event_system_demo() {
    std::cout << "\n=== Event System Example ===\n";

    EventEmitter events;

    // Register handlers
    events.on("click", [](const std::string& data) {
        std::cout << "Click handler 1: " << data << "\n";
    });

    events.on("click", [](const std::string& data) {
        std::cout << "Click handler 2: " << data << "\n";
    });

    events.on("load", [](const std::string& data) {
        std::cout << "Load handler: " << data << "\n";
    });

    // Emit events
    events.emit("click", "button1");
    events.emit("load", "page ready");
    events.emit("unknown");  // No handlers
}

int main() {
    function_basics();
    callback_demo();
    bind_demo();
    lambda_equivalents();
    ref_demo();
    invoke_demo();
    mem_fn_demo();
    event_system_demo();

    return 0;
}

// Key points:
// - std::function provides type erasure for callable objects
// - Prefer lambdas over std::bind for clarity and efficiency
// - Use std::ref/std::cref when bind needs references
// - std::invoke provides uniform syntax for all callables
// - std::mem_fn wraps member functions for use with algorithms
// - std::function has overhead; use templates for hot paths
