// Hello World - The Classic First Program
// Book reference: 1.2 Programs
//
// Every C++ program has a main() function that returns an int.
// The return value indicates success (0) or failure (non-zero).
// The << operator is used for output to streams.

#include <iostream>

int main() {
    // Output "Hello, World!" followed by a newline
    // std::cout is the standard output stream
    // std::endl flushes the buffer and adds newline
    std::cout << "Hello, World!" << std::endl;

    // Alternative: '\n' is more efficient (no flush)
    std::cout << "Hello again!\n";

    // Return 0 to indicate successful execution
    // If omitted, main() implicitly returns 0
    return 0;
}

// Key points:
// - #include <iostream> brings in I/O facilities
// - std:: is the standard library namespace
// - main() is the program entry point
// - Return 0 for success, non-zero for failure
