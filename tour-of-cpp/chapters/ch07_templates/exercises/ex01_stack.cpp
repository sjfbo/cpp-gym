// Exercise 7.1: Implement a Generic Stack<T>
// Book reference: 7.2 Parameterized Types
// Difficulty: **
//
// Instructions:
// Implement a generic Stack<T> class template with the following operations:
// 1. push(const T& value) - add element to top
// 2. pop() - remove top element
// 3. top() / top() const - access top element (reference)
// 4. empty() const - check if stack is empty
// 5. size() const - return number of elements
//
// Requirements:
// - Use std::vector<T> as the underlying container
// - Throw std::out_of_range for top() or pop() on empty stack
// - Support move semantics with push(T&& value)
// - Make the class const-correct
//
// Bonus challenges:
// - Add a clear() method
// - Add emplace() for in-place construction
// - Add swap() method
//
// Example usage:
//   Stack<int> s;
//   s.push(1);
//   s.push(2);
//   s.push(3);
//   std::cout << s.top();  // prints 3
//   s.pop();
//   std::cout << s.top();  // prints 2
//   std::cout << s.size(); // prints 2
//
// Hints:
// - std::vector has push_back(), pop_back(), back(), empty(), size()
// - Use vector's clear() for your clear()
// - Consider what happens if you call top() on an empty stack

#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

// TODO: Implement the Stack<T> class template
template<typename T>
class Stack {
private:
    // TODO: Add private member - use std::vector<T> as the underlying container
    std::vector<T> data_;  // Stub container

public:
    // TODO: Implement push(const T& value)
    // Add an element to the top of the stack
    void push(const T& value) {
        (void)value;  // TODO: Implement - use data_.push_back(value)
    }

    // TODO: Implement push(T&& value) for move semantics
    // Add an element to the top using move semantics
    void push(T&& value) {
        (void)value;  // TODO: Implement - use data_.push_back(std::move(value))
    }

    // TODO: Implement pop()
    // Remove the top element
    // Should throw std::out_of_range if stack is empty
    void pop() {
        // TODO: Check if empty, throw if so, otherwise data_.pop_back()
    }

    // TODO: Implement top() - non-const version
    // Return a reference to the top element
    // Should throw std::out_of_range if stack is empty
    T& top() {
        // TODO: Check if empty, throw if so, otherwise return data_.back()
        throw std::out_of_range("Stack is empty");
    }

    // TODO: Implement top() const - const version
    // Return a const reference to the top element
    // Should throw std::out_of_range if stack is empty
    const T& top() const {
        // TODO: Check if empty, throw if so, otherwise return data_.back()
        throw std::out_of_range("Stack is empty");
    }

    // TODO: Implement empty() const
    // Return true if stack has no elements
    [[nodiscard]] bool empty() const {
        return true;  // TODO: return data_.empty()
    }

    // TODO: Implement size() const
    // Return the number of elements in the stack
    [[nodiscard]] std::size_t size() const {
        return 0;  // TODO: return data_.size()
    }

    // BONUS: Implement clear()
    // Remove all elements from the stack
    void clear() {
        // TODO: data_.clear()
    }

    // BONUS: Implement emplace(Args&&... args)
    // Construct element in-place at top of stack
    // template<typename... Args>
    // void emplace(Args&&... args);

    // BONUS: Implement swap(Stack& other)
    // Exchange contents with another stack
    void swap(Stack& other) {
        (void)other;  // TODO: data_.swap(other.data_)
    }
};

// ----------------------------------------------------------------------------
// Test your implementation
// ----------------------------------------------------------------------------

void test_basic_operations() {
    std::cout << "=== Testing Basic Operations ===\n";

    Stack<int> s;

    // Test empty stack
    std::cout << "Empty stack - empty(): " << std::boolalpha << s.empty() << "\n";
    std::cout << "Empty stack - size(): " << s.size() << "\n";

    // Test push and top
    s.push(10);
    s.push(20);
    s.push(30);
    std::cout << "After pushing 10, 20, 30:\n";
    std::cout << "  top(): " << s.top() << "\n";
    std::cout << "  size(): " << s.size() << "\n";
    std::cout << "  empty(): " << s.empty() << "\n";

    // Test pop
    s.pop();
    std::cout << "After pop():\n";
    std::cout << "  top(): " << s.top() << "\n";
    std::cout << "  size(): " << s.size() << "\n";

    // Test modifying through top()
    s.top() = 99;
    std::cout << "After s.top() = 99:\n";
    std::cout << "  top(): " << s.top() << "\n";
}

void test_different_types() {
    std::cout << "\n=== Testing Different Types ===\n";

    // Stack of strings
    Stack<std::string> str_stack;
    str_stack.push("hello");
    str_stack.push("world");
    std::cout << "String stack top: " << str_stack.top() << "\n";
    str_stack.pop();
    std::cout << "After pop, top: " << str_stack.top() << "\n";

    // Stack of doubles
    Stack<double> dbl_stack;
    dbl_stack.push(3.14);
    dbl_stack.push(2.71);
    std::cout << "Double stack top: " << dbl_stack.top() << "\n";
}

void test_exception_handling() {
    std::cout << "\n=== Testing Exception Handling ===\n";

    Stack<int> s;

    // Test pop on empty stack
    try {
        s.pop();
        std::cout << "ERROR: pop() should have thrown\n";
    } catch (const std::out_of_range& e) {
        std::cout << "Caught expected exception on pop(): " << e.what() << "\n";
    }

    // Test top on empty stack
    try {
        [[maybe_unused]] auto val = s.top();
        std::cout << "ERROR: top() should have thrown\n";
    } catch (const std::out_of_range& e) {
        std::cout << "Caught expected exception on top(): " << e.what() << "\n";
    }
}

void test_move_semantics() {
    std::cout << "\n=== Testing Move Semantics ===\n";

    Stack<std::string> s;

    // Push rvalue (should use move)
    s.push(std::string{"moved string"});
    std::cout << "After push(rvalue): " << s.top() << "\n";

    // Push lvalue (should use copy)
    std::string str = "copied string";
    s.push(str);
    std::cout << "After push(lvalue): " << s.top() << "\n";
    std::cout << "Original string still valid: " << str << "\n";
}

void test_const_correctness() {
    std::cout << "\n=== Testing Const Correctness ===\n";

    Stack<int> s;
    s.push(42);

    const Stack<int>& cs = s;
    std::cout << "Const stack top: " << cs.top() << "\n";
    std::cout << "Const stack size: " << cs.size() << "\n";
    std::cout << "Const stack empty: " << cs.empty() << "\n";

    // cs.push(10);  // Should not compile - push on const
    // cs.pop();     // Should not compile - pop on const
    // cs.top() = 5; // Should not compile - modifying through const top
}

int main() {
    test_basic_operations();
    test_different_types();
    test_exception_handling();
    test_move_semantics();
    test_const_correctness();

    std::cout << "\n=== All Tests Completed ===\n";
    std::cout << "If you see this without errors, your Stack<T> is working!\n";

    return 0;
}
