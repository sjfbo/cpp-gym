// Exercise 2.2: Implement a Stack Class
// Book reference: 2.3 Classes
// Difficulty: **
//
// Instructions:
// Implement a simple Stack class that:
// 1. Stores integers in a fixed-size array (capacity = 10)
// 2. Has push(int) to add element (throws if full)
// 3. Has pop() to remove and return top element (throws if empty)
// 4. Has top() const to peek at top element (throws if empty)
// 5. Has size() const and empty() const
// 6. Has is_full() const
//
// The class should maintain the invariant that elements are stored
// from index 0 to (count-1).
//
// Expected output:
//   Initial: empty=true, size=0
//   After pushing 1,2,3: size=3, top=3
//   Pop: 3, top=2
//   Overflow test: caught exception

#include <iostream>
#include <stdexcept>

class Stack {
    static constexpr int CAPACITY = 10;
    int data[CAPACITY];
    int count = 0;

public:
    // TODO: Implement push(int value)
    // Should throw std::overflow_error if full

    // TODO: Implement pop()
    // Should throw std::underflow_error if empty

    // TODO: Implement top() const
    // Should throw std::underflow_error if empty

    // TODO: Implement size() const

    // TODO: Implement empty() const

    // TODO: Implement is_full() const
};

int main() {
    // Uncomment after implementing Stack:

    // Stack s;
    // std::cout << "Initial: empty=" << std::boolalpha << s.empty()
    //           << ", size=" << s.size() << "\n";

    // s.push(1);
    // s.push(2);
    // s.push(3);
    // std::cout << "After pushing 1,2,3: size=" << s.size()
    //           << ", top=" << s.top() << "\n";

    // std::cout << "Pop: " << s.pop() << ", top=" << s.top() << "\n";

    // // Test overflow
    // Stack full;
    // try {
    //     for (int i = 0; i < 15; ++i) {
    //         full.push(i);
    //     }
    // } catch (const std::overflow_error& e) {
    //     std::cout << "Overflow test: caught exception\n";
    // }

    std::cout << "TODO: Implement Stack class\n";

    return 0;
}
