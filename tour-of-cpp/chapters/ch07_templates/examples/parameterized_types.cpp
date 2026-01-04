// Parameterized Types - Class Templates
// Book reference: 7.2 Parameterized Types
//
// Class templates allow you to define generic types that work with any
// element type. The compiler generates specialized code for each type
// you use, providing both flexibility and performance.

#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>

// ----------------------------------------------------------------------------
// Basic Class Template: Vector<T>
// ----------------------------------------------------------------------------

// A simple dynamic array that can hold elements of any type T
template<typename T>
class Vector {
private:
    std::unique_ptr<T[]> elements_;  // Use smart pointer for automatic cleanup
    size_t size_;
    size_t capacity_;

public:
    // Constructor with initial capacity
    explicit Vector(size_t capacity = 0)
        : elements_{capacity > 0 ? std::make_unique<T[]>(capacity) : nullptr}
        , size_{0}
        , capacity_{capacity} {}

    // Initializer list constructor
    Vector(std::initializer_list<T> init)
        : elements_{std::make_unique<T[]>(init.size())}
        , size_{init.size()}
        , capacity_{init.size()} {
        size_t i = 0;
        for (const auto& elem : init) {
            elements_[i++] = elem;
        }
    }

    // Copy constructor
    Vector(const Vector& other)
        : elements_{std::make_unique<T[]>(other.capacity_)}
        , size_{other.size_}
        , capacity_{other.capacity_} {
        for (size_t i = 0; i < size_; ++i) {
            elements_[i] = other.elements_[i];
        }
    }

    // Move constructor
    Vector(Vector&& other) noexcept
        : elements_{std::move(other.elements_)}
        , size_{other.size_}
        , capacity_{other.capacity_} {
        other.size_ = 0;
        other.capacity_ = 0;
    }

    // Copy assignment
    Vector& operator=(const Vector& other) {
        if (this != &other) {
            auto new_elements = std::make_unique<T[]>(other.capacity_);
            for (size_t i = 0; i < other.size_; ++i) {
                new_elements[i] = other.elements_[i];
            }
            elements_ = std::move(new_elements);
            size_ = other.size_;
            capacity_ = other.capacity_;
        }
        return *this;
    }

    // Move assignment
    Vector& operator=(Vector&& other) noexcept {
        if (this != &other) {
            elements_ = std::move(other.elements_);
            size_ = other.size_;
            capacity_ = other.capacity_;
            other.size_ = 0;
            other.capacity_ = 0;
        }
        return *this;
    }

    // Element access with bounds checking
    T& at(size_t index) {
        if (index >= size_) {
            throw std::out_of_range{"Vector index out of range"};
        }
        return elements_[index];
    }

    const T& at(size_t index) const {
        if (index >= size_) {
            throw std::out_of_range{"Vector index out of range"};
        }
        return elements_[index];
    }

    // Element access without bounds checking (faster)
    T& operator[](size_t index) { return elements_[index]; }
    const T& operator[](size_t index) const { return elements_[index]; }

    // Add element to the end
    void push_back(const T& value) {
        if (size_ >= capacity_) {
            grow();
        }
        elements_[size_++] = value;
    }

    // Emplace element at the end (move semantics)
    void push_back(T&& value) {
        if (size_ >= capacity_) {
            grow();
        }
        elements_[size_++] = std::move(value);
    }

    // Remove last element
    void pop_back() {
        if (size_ > 0) {
            --size_;
        }
    }

    // Accessors
    [[nodiscard]] size_t size() const { return size_; }
    [[nodiscard]] size_t capacity() const { return capacity_; }
    [[nodiscard]] bool empty() const { return size_ == 0; }

    // Iterator support (basic)
    T* begin() { return elements_.get(); }
    T* end() { return elements_.get() + size_; }
    const T* begin() const { return elements_.get(); }
    const T* end() const { return elements_.get() + size_; }

private:
    void grow() {
        size_t new_capacity = (capacity_ == 0) ? 1 : capacity_ * 2;
        auto new_elements = std::make_unique<T[]>(new_capacity);
        for (size_t i = 0; i < size_; ++i) {
            new_elements[i] = std::move(elements_[i]);
        }
        elements_ = std::move(new_elements);
        capacity_ = new_capacity;
    }
};

// ----------------------------------------------------------------------------
// Multiple Template Parameters: Pair<T, U>
// ----------------------------------------------------------------------------

// A pair of values of potentially different types
template<typename T, typename U>
struct Pair {
    T first;
    U second;

    // Default constructor
    Pair() : first{}, second{} {}

    // Parameterized constructor
    Pair(const T& f, const U& s) : first{f}, second{s} {}

    // Move-aware constructor
    Pair(T&& f, U&& s) : first{std::move(f)}, second{std::move(s)} {}

    // Comparison operators
    bool operator==(const Pair& other) const {
        return first == other.first && second == other.second;
    }

    bool operator!=(const Pair& other) const {
        return !(*this == other);
    }

    // Less-than for ordering (lexicographic)
    bool operator<(const Pair& other) const {
        if (first < other.first) return true;
        if (other.first < first) return false;
        return second < other.second;
    }
};

// Deduction guide (C++17) - allows Pair p{1, 2.0} without specifying types
template<typename T, typename U>
Pair(T, U) -> Pair<T, U>;

// Helper function to create pairs (pre-C++17 style, still useful)
// Note: Named my_make_pair to avoid ambiguity with std::make_pair
template<typename T, typename U>
Pair<T, U> my_make_pair(T&& first, U&& second) {
    return Pair<T, U>{std::forward<T>(first), std::forward<U>(second)};
}

// ----------------------------------------------------------------------------
// Non-type Template Parameters: FixedBuffer<T, N>
// ----------------------------------------------------------------------------

// A fixed-size buffer allocated on the stack
template<typename T, size_t N>
class FixedBuffer {
private:
    T data_[N];
    size_t size_ = 0;

public:
    // Compile-time capacity
    static constexpr size_t max_size = N;

    void push_back(const T& value) {
        if (size_ >= N) {
            throw std::overflow_error{"FixedBuffer is full"};
        }
        data_[size_++] = value;
    }

    void pop_back() {
        if (size_ > 0) {
            --size_;
        }
    }

    T& operator[](size_t index) { return data_[index]; }
    const T& operator[](size_t index) const { return data_[index]; }

    [[nodiscard]] size_t size() const { return size_; }
    [[nodiscard]] constexpr size_t capacity() const { return N; }
    [[nodiscard]] bool empty() const { return size_ == 0; }
    [[nodiscard]] bool full() const { return size_ == N; }

    T* begin() { return data_; }
    T* end() { return data_ + size_; }
    const T* begin() const { return data_; }
    const T* end() const { return data_ + size_; }
};

// ----------------------------------------------------------------------------
// Default Template Arguments
// ----------------------------------------------------------------------------

// Container with customizable allocator (simplified)
template<typename T, typename Container = Vector<T>>
class Stack {
private:
    Container container_;

public:
    void push(const T& value) {
        container_.push_back(value);
    }

    void pop() {
        container_.pop_back();
    }

    T& top() {
        return container_[container_.size() - 1];
    }

    const T& top() const {
        return container_[container_.size() - 1];
    }

    [[nodiscard]] bool empty() const {
        return container_.empty();
    }

    [[nodiscard]] size_t size() const {
        return container_.size();
    }
};

// ----------------------------------------------------------------------------
// Main function demonstrating all concepts
// ----------------------------------------------------------------------------

int main() {
    std::cout << "=== Vector<T> Class Template ===\n";

    // Vector with integers
    Vector<int> int_vec{1, 2, 3, 4, 5};
    std::cout << "int_vec: ";
    for (const auto& val : int_vec) {
        std::cout << val << " ";
    }
    std::cout << "(size: " << int_vec.size() << ")\n";

    // Vector with strings
    Vector<std::string> str_vec;
    str_vec.push_back("hello");
    str_vec.push_back("world");
    std::cout << "str_vec: ";
    for (const auto& val : str_vec) {
        std::cout << val << " ";
    }
    std::cout << "(size: " << str_vec.size() << ")\n";

    // Vector with doubles
    Vector<double> dbl_vec{1.1, 2.2, 3.3};
    dbl_vec.push_back(4.4);
    std::cout << "dbl_vec: ";
    for (const auto& val : dbl_vec) {
        std::cout << val << " ";
    }
    std::cout << "\n";

    std::cout << "\n=== Pair<T, U> with Multiple Type Parameters ===\n";

    // Explicit type specification
    Pair<int, std::string> p1{42, "answer"};
    std::cout << "p1: (" << p1.first << ", " << p1.second << ")\n";

    // CTAD (C++17) - types deduced automatically
    Pair p2{3.14, 100};  // Pair<double, int>
    std::cout << "p2 (CTAD): (" << p2.first << ", " << p2.second << ")\n";

    // Using my_make_pair helper
    auto p3 = my_make_pair(std::string{"key"}, 999);
    std::cout << "p3: (" << p3.first << ", " << p3.second << ")\n";

    std::cout << "\n=== FixedBuffer<T, N> with Non-type Parameter ===\n";

    FixedBuffer<int, 5> buffer;
    buffer.push_back(10);
    buffer.push_back(20);
    buffer.push_back(30);
    std::cout << "buffer (capacity " << buffer.capacity() << "): ";
    for (const auto& val : buffer) {
        std::cout << val << " ";
    }
    std::cout << "(size: " << buffer.size() << ")\n";

    // Compile-time size check
    static_assert(FixedBuffer<double, 10>::max_size == 10);
    std::cout << "FixedBuffer<double, 10>::max_size = "
              << FixedBuffer<double, 10>::max_size << "\n";

    std::cout << "\n=== Stack<T> with Default Template Argument ===\n";

    Stack<int> stack;  // Uses Vector<int> as default container
    stack.push(1);
    stack.push(2);
    stack.push(3);
    std::cout << "Stack top: " << stack.top() << "\n";
    stack.pop();
    std::cout << "After pop, top: " << stack.top() << "\n";

    std::cout << "\n=== Template Type Requirements ===\n";
    // Our Vector<T> requires T to be:
    // - Default constructible (for the array)
    // - Copy/move assignable (for push_back)
    // These are implicit requirements; C++20 concepts make them explicit.

    std::cout << "Vector works with any type meeting its requirements.\n";
    std::cout << "Each instantiation (Vector<int>, Vector<string>) is a distinct type.\n";

    return 0;
}
