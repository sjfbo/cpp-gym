// Exercise 11.2: String Formatting
// Book reference: 11.6 Formatting
// Difficulty: **
//
// Instructions:
// Practice using std::format for string formatting.
// Implement each function to produce the expected output.

#include <format>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

// ============================================================================
// Part 1: Basic Formatting
// ============================================================================

// TODO 1: Format a greeting message.
// Example: format_greeting("Alice") -> "Hello, Alice!"

std::string format_greeting(const std::string& name) {
    // TODO: Use std::format
    (void)name;
    return "";
}


// TODO 2: Format a number with specified width (right-aligned).
// Example: format_padded(42, 6) -> "    42"

std::string format_padded(int value, int width) {
    // TODO: Use std::format with width specifier
    (void)value;
    (void)width;
    return "";
}


// TODO 3: Format a floating-point number with specified precision.
// Example: format_decimal(3.14159, 2) -> "3.14"

std::string format_decimal(double value, int precision) {
    // TODO: Use std::format with precision specifier
    (void)value;
    (void)precision;
    return "";
}


// ============================================================================
// Part 2: Number Formatting
// ============================================================================

// TODO 4: Format an integer in hexadecimal.
// Example: format_hex(255) -> "ff"

std::string format_hex(int value) {
    // TODO: Use std::format with hex specifier
    (void)value;
    return "";
}


// TODO 5: Format an integer in binary.
// Example: format_binary(10) -> "1010"

std::string format_binary(int value) {
    // TODO: Use std::format with binary specifier
    (void)value;
    return "";
}


// TODO 6: Format a number with sign (+ for positive, - for negative).
// Example: format_signed(42) -> "+42", format_signed(-42) -> "-42"

std::string format_signed(int value) {
    // TODO: Use std::format with sign specifier
    (void)value;
    return "";
}


// ============================================================================
// Part 3: Alignment and Fill
// ============================================================================

// TODO 7: Left-align a string with padding.
// Example: format_left("Hi", 10, '*') -> "Hi********"

std::string format_left(const std::string& text, int width, char fill) {
    // TODO: Use std::format with fill and align
    (void)text;
    (void)width;
    (void)fill;
    return "";
}


// TODO 8: Center a string with padding.
// Example: format_center("Hi", 10, '-') -> "----Hi----"

std::string format_center(const std::string& text, int width, char fill) {
    // TODO: Use std::format with center alignment
    (void)text;
    (void)width;
    (void)fill;
    return "";
}


// ============================================================================
// Part 4: Practical Applications
// ============================================================================

// TODO 9: Format a table row with fixed-width columns.
// Example: format_row("Alice", 95, 88.5) -> "Alice          95    88.50"
// Name: 15 chars left-aligned, Score: 5 chars right-aligned, Average: 8 chars with 2 decimal

std::string format_row(const std::string& name, int score, double average) {
    // TODO: Use std::format
    (void)name;
    (void)score;
    (void)average;
    return "";
}


// TODO 10: Format currency.
// Example: format_currency(1234.5) -> "$1,234.50"
// Note: C++20 doesn't have built-in thousands separator, so implement manually

std::string format_currency(double amount) {
    // TODO: Format as currency with 2 decimal places
    // Bonus: Add thousands separator
    (void)amount;
    return "";
}


// TODO 11: Format a date as YYYY-MM-DD.
// Example: format_date(2024, 3, 15) -> "2024-03-15"

std::string format_date(int year, int month, int day) {
    // TODO: Use std::format with zero-padding
    (void)year;
    (void)month;
    (void)day;
    return "";
}


// TODO 12: Format a time as HH:MM:SS.
// Example: format_time(9, 5, 3) -> "09:05:03"

std::string format_time(int hours, int minutes, int seconds) {
    // TODO: Use std::format with zero-padding
    (void)hours;
    (void)minutes;
    (void)seconds;
    return "";
}


// ============================================================================
// Part 5: Complex Formatting
// ============================================================================

// TODO 13: Format a progress bar.
// Example: format_progress(7, 10, 20) -> "[==============      ] 70%"
// Parameters: current, total, bar_width

std::string format_progress(int current, int total, int bar_width) {
    // TODO: Create a visual progress bar
    (void)current;
    (void)total;
    (void)bar_width;
    return "";
}


// TODO 14: Format a memory size with appropriate unit.
// Example: format_size(1536) -> "1.5 KB"
// Example: format_size(1048576) -> "1.0 MB"

std::string format_size(std::size_t bytes) {
    // TODO: Choose appropriate unit (B, KB, MB, GB)
    (void)bytes;
    return "";
}


// ============================================================================
// Tests
// ============================================================================

int main() {
    std::cout << "=== Exercise 11.2: String Formatting ===\n\n";

    // Test 1: format_greeting
    std::cout << "--- Test 1: format_greeting ---\n";
    std::cout << "format_greeting(\"Alice\"): " << format_greeting("Alice") << '\n';
    std::cout << "Expected: Hello, Alice!\n\n";

    // Test 2: format_padded
    std::cout << "--- Test 2: format_padded ---\n";
    std::cout << "format_padded(42, 6): '" << format_padded(42, 6) << "'\n";
    std::cout << "Expected: '    42'\n\n";

    // Test 3: format_decimal
    std::cout << "--- Test 3: format_decimal ---\n";
    std::cout << "format_decimal(3.14159, 2): " << format_decimal(3.14159, 2) << '\n';
    std::cout << "Expected: 3.14\n\n";

    // Test 4: format_hex
    std::cout << "--- Test 4: format_hex ---\n";
    std::cout << "format_hex(255): " << format_hex(255) << '\n';
    std::cout << "Expected: ff\n\n";

    // Test 5: format_binary
    std::cout << "--- Test 5: format_binary ---\n";
    std::cout << "format_binary(10): " << format_binary(10) << '\n';
    std::cout << "Expected: 1010\n\n";

    // Test 6: format_signed
    std::cout << "--- Test 6: format_signed ---\n";
    std::cout << "format_signed(42): " << format_signed(42) << '\n';
    std::cout << "format_signed(-42): " << format_signed(-42) << '\n';
    std::cout << "Expected: +42, -42\n\n";

    // Test 7: format_left
    std::cout << "--- Test 7: format_left ---\n";
    std::cout << "format_left(\"Hi\", 10, '*'): " << format_left("Hi", 10, '*') << '\n';
    std::cout << "Expected: Hi********\n\n";

    // Test 8: format_center
    std::cout << "--- Test 8: format_center ---\n";
    std::cout << "format_center(\"Hi\", 10, '-'): " << format_center("Hi", 10, '-') << '\n';
    std::cout << "Expected: ----Hi----\n\n";

    // Test 9: format_row
    std::cout << "--- Test 9: format_row ---\n";
    std::cout << "format_row(\"Alice\", 95, 88.5):\n";
    std::cout << "  '" << format_row("Alice", 95, 88.5) << "'\n";
    std::cout << "Expected (approx): 'Alice          95    88.50'\n\n";

    // Test 10: format_currency
    std::cout << "--- Test 10: format_currency ---\n";
    std::cout << "format_currency(1234.5): " << format_currency(1234.5) << '\n';
    std::cout << "Expected: $1,234.50 (or $1234.50)\n\n";

    // Test 11: format_date
    std::cout << "--- Test 11: format_date ---\n";
    std::cout << "format_date(2024, 3, 15): " << format_date(2024, 3, 15) << '\n';
    std::cout << "Expected: 2024-03-15\n\n";

    // Test 12: format_time
    std::cout << "--- Test 12: format_time ---\n";
    std::cout << "format_time(9, 5, 3): " << format_time(9, 5, 3) << '\n';
    std::cout << "Expected: 09:05:03\n\n";

    // Test 13: format_progress
    std::cout << "--- Test 13: format_progress ---\n";
    std::cout << "format_progress(7, 10, 20): " << format_progress(7, 10, 20) << '\n';
    std::cout << "Expected: [==============      ] 70%\n\n";

    // Test 14: format_size
    std::cout << "--- Test 14: format_size ---\n";
    std::cout << "format_size(1536): " << format_size(1536) << '\n';
    std::cout << "format_size(1048576): " << format_size(1048576) << '\n';
    std::cout << "Expected: 1.5 KB, 1.0 MB\n\n";

    return 0;
}
