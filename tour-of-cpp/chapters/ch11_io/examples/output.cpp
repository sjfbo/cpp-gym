// Output - Console Output with cout
// Book reference: 11.2 Output
//
// This example demonstrates basic console output using std::cout
// and traditional I/O manipulators for formatting.

#include <iomanip>
#include <iostream>

int main() {
    std::cout << "=== Console Output with cout ===\n\n";

    // ========================================================================
    // Basic Output
    // ========================================================================
    std::cout << "--- Basic Output ---\n";

    std::cout << "Hello, World!\n";
    std::cout << "Multiple " << "values " << "chained\n";
    std::cout << "Integer: " << 42 << '\n';
    std::cout << "Double: " << 3.14159 << '\n';
    std::cout << "Boolean: " << true << '\n';  // Prints 1
    std::cout << '\n';

    // ========================================================================
    // Boolean Formatting
    // ========================================================================
    std::cout << "--- Boolean Formatting ---\n";

    std::cout << "Default: " << true << ", " << false << '\n';
    std::cout << std::boolalpha;  // Enable textual bool output
    std::cout << "boolalpha: " << true << ", " << false << '\n';
    std::cout << std::noboolalpha;  // Reset to numeric
    std::cout << '\n';

    // ========================================================================
    // Integer Formatting
    // ========================================================================
    std::cout << "--- Integer Formatting ---\n";

    int value = 255;
    std::cout << "Decimal: " << value << '\n';
    std::cout << "Hex: " << std::hex << value << '\n';
    std::cout << "Octal: " << std::oct << value << '\n';
    std::cout << std::dec;  // Reset to decimal

    std::cout << "With base prefix:\n";
    std::cout << std::showbase;
    std::cout << "  Hex: " << std::hex << value << '\n';
    std::cout << "  Octal: " << std::oct << value << '\n';
    std::cout << std::dec << std::noshowbase;
    std::cout << '\n';

    // ========================================================================
    // Floating-Point Formatting
    // ========================================================================
    std::cout << "--- Floating-Point Formatting ---\n";

    double pi = 3.14159265358979;
    std::cout << "Default: " << pi << '\n';
    std::cout << "Fixed: " << std::fixed << pi << '\n';
    std::cout << "Scientific: " << std::scientific << pi << '\n';
    std::cout << std::defaultfloat;  // Reset

    // Precision
    std::cout << "Precision:\n";
    std::cout << "  2 digits: " << std::setprecision(2) << pi << '\n';
    std::cout << "  6 digits: " << std::setprecision(6) << pi << '\n';
    std::cout << "  10 digits: " << std::setprecision(10) << pi << '\n';
    std::cout << '\n';

    // ========================================================================
    // Width and Alignment
    // ========================================================================
    std::cout << "--- Width and Alignment ---\n";

    std::cout << "Default width:\n";
    std::cout << "|" << 42 << "|\n";

    std::cout << "Width 10, right-aligned (default):\n";
    std::cout << "|" << std::setw(10) << 42 << "|\n";

    std::cout << "Width 10, left-aligned:\n";
    std::cout << "|" << std::left << std::setw(10) << 42 << "|\n";

    std::cout << "Width 10, fill with zeros:\n";
    std::cout << "|" << std::right << std::setfill('0') << std::setw(10) << 42 << "|\n";
    std::cout << std::setfill(' ');  // Reset fill character

    std::cout << "Formatted table:\n";
    std::cout << std::left << std::setw(15) << "Name"
              << std::right << std::setw(10) << "Value" << '\n';
    std::cout << std::left << std::setw(15) << "Temperature"
              << std::right << std::setw(10) << 23.5 << '\n';
    std::cout << std::left << std::setw(15) << "Humidity"
              << std::right << std::setw(10) << 65 << '\n';
    std::cout << '\n';

    // ========================================================================
    // Flushing Output
    // ========================================================================
    std::cout << "--- Flushing Output ---\n";

    std::cout << "Using endl (adds newline and flushes): ";
    std::cout << "Hello" << std::endl;  // Flushes buffer

    std::cout << "Using flush (no newline): ";
    std::cout << "World" << std::flush;
    std::cout << " <- no newline before this\n";

    std::cout << "Prefer '\\n' over endl for performance\n";

    return 0;
}

// Key points:
// - std::cout is the standard output stream
// - Use << operator to chain outputs
// - Use '\n' for newlines (faster than std::endl)
// - std::endl flushes the buffer (performance cost)
// - iomanip provides formatting manipulators
// - Some manipulators are "sticky" (persist), others are temporary
