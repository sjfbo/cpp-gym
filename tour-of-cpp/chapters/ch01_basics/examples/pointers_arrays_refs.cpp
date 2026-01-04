// Pointers, Arrays, and References
// Book reference: 1.7 Pointers, Arrays, and References
//
// These are fundamental mechanisms for accessing memory:
// - Pointers: hold memory addresses, can be null, can be reseated
// - References: aliases to existing objects, cannot be null, cannot be reseated
// - Arrays: contiguous sequences of elements

#include <array>
#include <iostream>

int main() {
    std::cout << "=== Pointers ===\n";

    int x = 42;
    int* p = &x;  // p points to x (& is address-of operator)

    std::cout << "x = " << x << "\n";
    std::cout << "&x (address of x) = " << &x << "\n";
    std::cout << "p (pointer value) = " << p << "\n";
    std::cout << "*p (dereference) = " << *p << "\n";

    // Modify through pointer
    *p = 100;
    std::cout << "After *p = 100: x = " << x << "\n";

    // Null pointer
    int* null_ptr = nullptr;  // Modern C++ way (not NULL or 0)
    std::cout << "null_ptr = " << null_ptr << "\n";

    // Check before dereferencing
    if (null_ptr != nullptr) {
        std::cout << "*null_ptr = " << *null_ptr << "\n";  // Would crash!
    } else {
        std::cout << "null_ptr is null, cannot dereference\n";
    }

    std::cout << "\n=== References ===\n";

    int y = 10;
    int& ref = y;  // ref is an alias for y

    std::cout << "y = " << y << "\n";
    std::cout << "ref = " << ref << "\n";
    std::cout << "&y = " << &y << "\n";
    std::cout << "&ref = " << &ref << " (same address!)\n";

    // Modify through reference
    ref = 20;
    std::cout << "After ref = 20: y = " << y << "\n";

    // Reference must be initialized
    // int& bad_ref;  // ERROR: reference must be initialized

    // Reference cannot be reseated
    int z = 30;
    ref = z;  // This assigns z's value to y, doesn't make ref refer to z
    std::cout << "After ref = z: y = " << y << ", ref = " << ref << "\n";

    std::cout << "\n=== Pointers vs References ===\n";
    std::cout << "Pointer: can be null, can be reseated, uses * to dereference\n";
    std::cout << "Reference: cannot be null, cannot be reseated, auto-dereferences\n";

    std::cout << "\n=== C-style Arrays ===\n";

    int arr[5] = {1, 2, 3, 4, 5};  // C-style array

    std::cout << "arr[0] = " << arr[0] << "\n";
    std::cout << "arr[4] = " << arr[4] << "\n";
    std::cout << "sizeof(arr) = " << sizeof(arr) << " bytes\n";
    std::cout << "Number of elements: " << sizeof(arr) / sizeof(arr[0]) << "\n";

    // Array name decays to pointer
    int* arr_ptr = arr;  // arr decays to &arr[0]
    std::cout << "arr_ptr[2] = " << arr_ptr[2] << "\n";
    std::cout << "*(arr_ptr + 2) = " << *(arr_ptr + 2) << " (pointer arithmetic)\n";

    // Iterate with range-based for
    std::cout << "Elements: ";
    for (int elem : arr) {
        std::cout << elem << " ";
    }
    std::cout << "\n";

    std::cout << "\n=== std::array (Modern C++) ===\n";

    std::array<int, 5> modern_arr = {10, 20, 30, 40, 50};

    std::cout << "modern_arr[0] = " << modern_arr[0] << "\n";
    std::cout << "modern_arr.at(4) = " << modern_arr.at(4) << " (bounds-checked)\n";
    std::cout << "modern_arr.size() = " << modern_arr.size() << "\n";
    std::cout << "modern_arr.front() = " << modern_arr.front() << "\n";
    std::cout << "modern_arr.back() = " << modern_arr.back() << "\n";

    // at() throws if out of bounds
    try {
        std::cout << modern_arr.at(10) << "\n";  // Throws!
    } catch (const std::out_of_range& e) {
        std::cout << "Caught out_of_range: " << e.what() << "\n";
    }

    std::cout << "\n=== Pointer Arithmetic ===\n";

    int nums[] = {100, 200, 300, 400};
    int* ptr = nums;

    std::cout << "*ptr = " << *ptr << "\n";
    std::cout << "*(ptr + 1) = " << *(ptr + 1) << "\n";
    std::cout << "*(ptr + 2) = " << *(ptr + 2) << "\n";

    ptr++;  // Move to next element
    std::cout << "After ptr++: *ptr = " << *ptr << "\n";

    // Pointer difference
    int* start = nums;
    int* end = nums + 4;
    std::cout << "end - start = " << (end - start) << " elements\n";

    std::cout << "\n=== const with Pointers ===\n";

    int value = 42;

    // Pointer to const: cannot modify value through pointer
    const int* ptr_to_const = &value;
    // *ptr_to_const = 10;  // ERROR: cannot modify
    std::cout << "pointer to const: *ptr_to_const = " << *ptr_to_const << "\n";

    // const pointer: cannot change what pointer points to
    int* const const_ptr = &value;
    *const_ptr = 50;  // OK: can modify value
    // const_ptr = &x;  // ERROR: cannot reseat
    std::cout << "const pointer: value = " << value << "\n";

    // const pointer to const: neither can change
    const int* const fully_const = &value;
    std::cout << "const pointer to const: " << *fully_const << "\n";

    // Read right-to-left: "const int* const" = const pointer to const int

    return 0;
}
