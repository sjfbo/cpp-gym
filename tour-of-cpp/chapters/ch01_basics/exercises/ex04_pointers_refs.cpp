// Exercise 1.4: Pointers and References
// Book reference: 1.7 Pointers, Arrays, and References
// Difficulty: **
//
// Instructions:
// Implement the following functions using pointers and references:
//
// 1. void swap_ptr(int* a, int* b) - swap values using pointers
// 2. void swap_ref(int& a, int& b) - swap values using references
// 3. int* find_max(int* arr, int size) - return pointer to max element
// 4. void double_elements(int* arr, int size) - double each element in place
//
// Expected output:
//   swap_ptr test: a=20, b=10
//   swap_ref test: a=200, b=100
//   max element: 42
//   doubled array: 2 4 6 8 10
//
// Hints:
// - For swap, use a temporary variable
// - For find_max, track both the max value and its pointer
// - Use pointer arithmetic or array indexing for double_elements

#include <iostream>

// TODO: Implement swap_ptr - swap two integers using pointers
void swap_ptr(int* a, int* b) {
    // Your code here
    (void)a; (void)b;  // Remove these lines when you implement the function
}

// TODO: Implement swap_ref - swap two integers using references
void swap_ref(int& a, int& b) {
    // Your code here
    (void)a; (void)b;  // Remove these lines when you implement the function
}

// TODO: Implement find_max - return pointer to the maximum element
// Return nullptr if array is empty (size <= 0)
int* find_max(int* arr, int size) {
    // Your code here
    (void)arr; (void)size;  // Remove these lines when you implement the function
    return nullptr;  // Replace this
}

// TODO: Implement double_elements - double each element in the array
void double_elements(int* arr, int size) {
    // Your code here
    (void)arr; (void)size;  // Remove these lines when you implement the function
}

int main() {
    std::cout << "=== Testing swap_ptr ===\n";
    int a = 10, b = 20;
    std::cout << "Before: a=" << a << ", b=" << b << "\n";
    swap_ptr(&a, &b);
    std::cout << "After swap_ptr: a=" << a << ", b=" << b << "\n";

    std::cout << "\n=== Testing swap_ref ===\n";
    int x = 100, y = 200;
    std::cout << "Before: x=" << x << ", y=" << y << "\n";
    swap_ref(x, y);
    std::cout << "After swap_ref: x=" << x << ", y=" << y << "\n";

    std::cout << "\n=== Testing find_max ===\n";
    int arr[] = {5, 12, 42, 7, 23};
    int* max_ptr = find_max(arr, 5);
    if (max_ptr != nullptr) {
        std::cout << "Max element: " << *max_ptr << "\n";
        std::cout << "Max is at index: " << (max_ptr - arr) << "\n";
    }

    std::cout << "\n=== Testing double_elements ===\n";
    int nums[] = {1, 2, 3, 4, 5};
    std::cout << "Before: ";
    for (int n : nums) std::cout << n << " ";
    std::cout << "\n";

    double_elements(nums, 5);

    std::cout << "After:  ";
    for (int n : nums) std::cout << n << " ";
    std::cout << "\n";

    std::cout << "\n=== Bonus Challenge ===\n";
    std::cout << "Try implementing:\n";
    std::cout << "- void reverse(int* arr, int size)\n";
    std::cout << "- int* find(int* arr, int size, int value)\n";

    return 0;
}
