// Exercise 1: Header/Implementation Separation
// Book reference: 3.2 Separate Compilation
//
// In this exercise, you'll practice separating a class into header and
// implementation files. For simplicity, we'll keep everything in this single
// file but structure it as if it were separate files.
//
// YOUR TASK:
// 1. Read through the "simulated header" section
// 2. Complete the implementation of the Stack class methods
// 3. All tests should pass when you run this file
//
// In a real project, you would split this into:
// - stack.h (the header section)
// - stack.cpp (the implementation section)
// - main.cpp (the test/usage section)

#include <iostream>
#include <stdexcept>
#include <cassert>

// ============================================================================
// SIMULATED HEADER FILE: stack.h
// ============================================================================
// This section would normally be in a separate .h file
// Notice: Only declarations, no definitions (except inline/constexpr)

#pragma once  // Would be at top of real header

namespace collections {

// A simple fixed-capacity stack of integers
// Invariant: top_ is always in range [-1, capacity_-1]
//            top_ == -1 means empty stack
class Stack {
public:
    // Constructor: creates a stack with given capacity
    explicit Stack(int capacity);

    // Destructor
    ~Stack();

    // Prevent copying (for simplicity)
    Stack(const Stack&) = delete;
    Stack& operator=(const Stack&) = delete;

    // Stack operations
    void push(int value);
    int pop();
    [[nodiscard]] int top() const;

    // Capacity queries
    [[nodiscard]] bool empty() const;
    [[nodiscard]] bool full() const;
    [[nodiscard]] int size() const;
    [[nodiscard]] int capacity() const;

private:
    int* data_;      // Array of elements
    int top_;        // Index of top element (-1 if empty)
    int capacity_;   // Maximum number of elements
};

}  // namespace collections

// ============================================================================
// SIMULATED IMPLEMENTATION FILE: stack.cpp
// ============================================================================
// This section would normally be in a separate .cpp file
// Complete the implementations below

// #include "stack.h"  // Would include the header

namespace collections {

// TODO: Implement the constructor
// - Allocate array of 'capacity' integers
// - Initialize top_ to -1 (empty)
// - Store the capacity
Stack::Stack(int capacity)
    : data_{nullptr}   // FIX THIS
    , top_{0}          // FIX THIS
    , capacity_{0}     // FIX THIS
{
    // Add validation: capacity must be positive
}

// TODO: Implement the destructor
// - Free the allocated array
Stack::~Stack() {
    // YOUR CODE HERE
}

// TODO: Implement push
// - Throw std::overflow_error if stack is full
// - Increment top_ and store value
void Stack::push(int value) {
    // YOUR CODE HERE
}

// TODO: Implement pop
// - Throw std::underflow_error if stack is empty
// - Return the top value and decrement top_
int Stack::pop() {
    // YOUR CODE HERE
    return 0;  // FIX THIS
}

// TODO: Implement top
// - Throw std::underflow_error if stack is empty
// - Return the top value without removing it
int Stack::top() const {
    // YOUR CODE HERE
    return 0;  // FIX THIS
}

// TODO: Implement empty
// - Return true if stack has no elements
bool Stack::empty() const {
    // YOUR CODE HERE
    return true;  // FIX THIS
}

// TODO: Implement full
// - Return true if stack has reached capacity
bool Stack::full() const {
    // YOUR CODE HERE
    return false;  // FIX THIS
}

// TODO: Implement size
// - Return the number of elements in the stack
int Stack::size() const {
    // YOUR CODE HERE
    return 0;  // FIX THIS
}

// TODO: Implement capacity
// - Return the maximum capacity of the stack
int Stack::capacity() const {
    // YOUR CODE HERE
    return 0;  // FIX THIS
}

}  // namespace collections

// ============================================================================
// TEST FILE: main.cpp
// ============================================================================
// This would normally be in a separate test file
// DO NOT MODIFY THIS SECTION

void run_tests() {
    using collections::Stack;

    std::cout << "Running Stack tests...\n";

    // Test 1: Construction
    {
        Stack s(5);
        assert(s.empty());
        assert(!s.full());
        assert(s.size() == 0);
        assert(s.capacity() == 5);
        std::cout << "  [PASS] Construction\n";
    }

    // Test 2: Push and top
    {
        Stack s(3);
        s.push(10);
        assert(!s.empty());
        assert(s.size() == 1);
        assert(s.top() == 10);

        s.push(20);
        s.push(30);
        assert(s.full());
        assert(s.size() == 3);
        assert(s.top() == 30);
        std::cout << "  [PASS] Push and top\n";
    }

    // Test 3: Pop
    {
        Stack s(3);
        s.push(1);
        s.push(2);
        s.push(3);

        assert(s.pop() == 3);
        assert(s.pop() == 2);
        assert(s.pop() == 1);
        assert(s.empty());
        std::cout << "  [PASS] Pop\n";
    }

    // Test 4: Overflow exception
    {
        Stack s(2);
        s.push(1);
        s.push(2);
        bool caught = false;
        try {
            s.push(3);  // Should throw
        } catch (const std::overflow_error&) {
            caught = true;
        }
        assert(caught);
        std::cout << "  [PASS] Overflow exception\n";
    }

    // Test 5: Underflow exception (pop)
    {
        Stack s(2);
        bool caught = false;
        try {
            s.pop();  // Should throw
        } catch (const std::underflow_error&) {
            caught = true;
        }
        assert(caught);
        std::cout << "  [PASS] Underflow exception (pop)\n";
    }

    // Test 6: Underflow exception (top)
    {
        Stack s(2);
        bool caught = false;
        try {
            s.top();  // Should throw
        } catch (const std::underflow_error&) {
            caught = true;
        }
        assert(caught);
        std::cout << "  [PASS] Underflow exception (top)\n";
    }

    std::cout << "\nAll tests passed!\n";
}

int main() {
    std::cout << "=== Exercise 1: Header/Implementation Separation ===\n\n";

    run_tests();

    std::cout << "\n";
    std::cout << "KEY TAKEAWAYS:\n";
    std::cout << "1. Headers (.h): declarations only\n";
    std::cout << "2. Implementation (.cpp): definitions\n";
    std::cout << "3. Use #pragma once or include guards\n";
    std::cout << "4. Include the header in both .cpp files\n";
    std::cout << "5. Compile: g++ -c stack.cpp && g++ -c main.cpp && g++ stack.o main.o\n";

    return 0;
}
