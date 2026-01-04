// Input - Console Input with cin and getline
// Book reference: 11.3 Input
//
// This example demonstrates reading input from the console using
// std::cin and std::getline.

#include <iostream>
#include <limits>
#include <sstream>
#include <string>

int main() {
    std::cout << "=== Console Input with cin ===\n\n";

    // ========================================================================
    // Basic Input with cin
    // ========================================================================
    std::cout << "--- Basic Input (simulated) ---\n";

    // Note: In an interactive program, you would use:
    // std::cin >> value;
    // Here we simulate input with stringstream for demonstration

    std::istringstream int_input("42");
    int number = 0;
    int_input >> number;
    std::cout << "Read integer: " << number << '\n';

    std::istringstream double_input("3.14");
    double decimal = 0.0;
    double_input >> decimal;
    std::cout << "Read double: " << decimal << '\n';

    std::istringstream word_input("Hello");
    std::string word;
    word_input >> word;
    std::cout << "Read word: " << word << '\n';
    std::cout << '\n';

    // ========================================================================
    // Whitespace Handling
    // ========================================================================
    std::cout << "--- Whitespace Handling ---\n";

    // >> operator skips leading whitespace and stops at whitespace
    std::istringstream words("Hello World");
    std::string first;
    std::string second;
    words >> first >> second;
    std::cout << "Two words: '" << first << "' and '" << second << "'\n";

    // getline reads entire line including spaces
    std::istringstream line_input("Hello World with spaces");
    std::string full_line;
    std::getline(line_input, full_line);
    std::cout << "Full line: '" << full_line << "'\n";
    std::cout << '\n';

    // ========================================================================
    // Reading Multiple Values
    // ========================================================================
    std::cout << "--- Reading Multiple Values ---\n";

    std::istringstream multi("10 20 30 40 50");
    int a = 0;
    int b = 0;
    int c = 0;
    multi >> a >> b >> c;
    std::cout << "Read three values: " << a << ", " << b << ", " << c << '\n';

    // Read remaining values in a loop
    std::cout << "Remaining: ";
    int val = 0;
    while (multi >> val) {
        std::cout << val << " ";
    }
    std::cout << '\n' << '\n';

    // ========================================================================
    // getline with Custom Delimiter
    // ========================================================================
    std::cout << "--- getline with Custom Delimiter ---\n";

    std::istringstream csv("apple,banana,cherry");
    std::string item;
    std::cout << "CSV items: ";
    while (std::getline(csv, item, ',')) {
        std::cout << "'" << item << "' ";
    }
    std::cout << '\n' << '\n';

    // ========================================================================
    // Error Handling
    // ========================================================================
    std::cout << "--- Error Handling ---\n";

    // Trying to read invalid input
    std::istringstream bad_input("not_a_number");
    int bad_num = -1;
    bad_input >> bad_num;

    if (bad_input.fail()) {
        std::cout << "Failed to read number (as expected)\n";
        std::cout << "Stream state: fail=" << bad_input.fail()
                  << ", bad=" << bad_input.bad()
                  << ", eof=" << bad_input.eof() << '\n';
    }

    // Clearing error state
    bad_input.clear();  // Reset error flags
    std::cout << "After clear(): fail=" << bad_input.fail() << '\n';
    std::cout << '\n';

    // ========================================================================
    // Mixing >> and getline
    // ========================================================================
    std::cout << "--- Mixing >> and getline ---\n";

    // Common pitfall: >> leaves newline in buffer
    std::istringstream mixed("42\nHello World");
    int n = 0;
    std::string rest;

    mixed >> n;
    // Need to skip the newline left by >>
    mixed.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::getline(mixed, rest);

    std::cout << "Number: " << n << '\n';
    std::cout << "Rest of line: '" << rest << "'\n";
    std::cout << '\n';

    // ========================================================================
    // Parsing Structured Input
    // ========================================================================
    std::cout << "--- Parsing Structured Input ---\n";

    // Parse "name:value" pairs
    std::istringstream pairs("temperature:23.5\nhumidity:65");
    std::string line;

    while (std::getline(pairs, line)) {
        auto colon_pos = line.find(':');
        if (colon_pos != std::string::npos) {
            std::string name = line.substr(0, colon_pos);
            std::string value = line.substr(colon_pos + 1);
            std::cout << "  " << name << " = " << value << '\n';
        }
    }

    return 0;
}

// Key points:
// - std::cin >> skips whitespace, reads until whitespace
// - std::getline reads entire line including spaces
// - Check stream state with fail(), bad(), eof()
// - Use clear() to reset error state
// - Use ignore() when mixing >> and getline
// - getline accepts custom delimiters
