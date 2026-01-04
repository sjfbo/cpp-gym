// Strings - std::string Operations
// Book reference: 10.2 Strings
//
// This example demonstrates std::string: the standard dynamic string class.
// std::string manages its own memory and provides rich text manipulation.

#include <algorithm>
#include <cctype>
#include <format>
#include <iostream>
#include <string>

int main() {
    std::cout << "=== std::string Operations ===\n\n";

    // ========================================================================
    // Construction
    // ========================================================================
    std::cout << "--- Construction ---\n";

    std::string s1 = "Hello, World!";               // From C-string literal
    std::string s2{"Hello"};                         // Uniform initialization
    std::string s3(10, 'x');                         // 10 copies of 'x'
    std::string s4 = s1;                             // Copy
    std::string s5 = std::move(s4);                  // Move (s4 now empty)

    std::cout << std::format("s1: '{}'\n", s1);
    std::cout << std::format("s2: '{}'\n", s2);
    std::cout << std::format("s3: '{}'\n", s3);
    std::cout << std::format("s4 (moved from): '{}'\n", s4);
    std::cout << std::format("s5 (moved to): '{}'\n\n", s5);

    // ========================================================================
    // Size and Capacity
    // ========================================================================
    std::cout << "--- Size and Capacity ---\n";

    std::string str = "Hello";
    std::cout << std::format("str: '{}'\n", str);
    std::cout << std::format("length(): {}\n", str.length());
    std::cout << std::format("size(): {}\n", str.size());  // Same as length()
    std::cout << std::format("capacity(): {}\n", str.capacity());
    std::cout << std::format("empty(): {}\n\n", str.empty());

    // ========================================================================
    // Element Access
    // ========================================================================
    std::cout << "--- Element Access ---\n";

    std::string text = "Hello";
    std::cout << std::format("text: '{}'\n", text);
    std::cout << std::format("text[0]: '{}'\n", text[0]);       // No bounds check
    std::cout << std::format("text.at(1): '{}'\n", text.at(1)); // Bounds checked
    std::cout << std::format("text.front(): '{}'\n", text.front());
    std::cout << std::format("text.back(): '{}'\n", text.back());

    // c_str() - for C APIs expecting null-terminated string
    const char* cstr = text.c_str();
    std::cout << std::format("c_str(): {}\n\n", cstr);

    // ========================================================================
    // Modification
    // ========================================================================
    std::cout << "--- Modification ---\n";

    std::string base = "Hello";
    std::cout << std::format("base: '{}'\n", base);

    // Append
    base += ", World";
    std::cout << std::format("after +=: '{}'\n", base);

    base.append("!");
    std::cout << std::format("after append: '{}'\n", base);

    base.push_back('!');
    std::cout << std::format("after push_back: '{}'\n", base);

    // Insert
    base.insert(5, " there");
    std::cout << std::format("after insert: '{}'\n", base);

    // Erase
    base.erase(5, 6);  // Remove " there"
    std::cout << std::format("after erase: '{}'\n", base);

    // Replace
    base.replace(7, 5, "C++");  // Replace "World" with "C++"
    std::cout << std::format("after replace: '{}'\n", base);

    // Pop back
    base.pop_back();  // Remove trailing '!'
    std::cout << std::format("after pop_back: '{}'\n\n", base);

    // ========================================================================
    // Searching
    // ========================================================================
    std::cout << "--- Searching ---\n";

    std::string haystack = "Hello, World! Hello again!";
    std::cout << std::format("haystack: '{}'\n", haystack);

    // find - first occurrence
    auto pos = haystack.find("Hello");
    std::cout << std::format("find('Hello'): {}\n", pos);

    // find from position
    pos = haystack.find("Hello", 1);  // Start after first char
    std::cout << std::format("find('Hello', 1): {}\n", pos);

    // rfind - last occurrence
    pos = haystack.rfind("Hello");
    std::cout << std::format("rfind('Hello'): {}\n", pos);

    // find_first_of - any character from set
    pos = haystack.find_first_of("aeiou");
    std::cout << std::format("find_first_of('aeiou'): {} ('{}')\n",
                             pos, haystack[pos]);

    // Check for not found
    pos = haystack.find("xyz");
    if (pos == std::string::npos) {
        std::cout << "find('xyz'): not found (npos)\n";
    }

    // C++20: starts_with and ends_with
    std::cout << std::format("starts_with('Hello'): {}\n",
                             haystack.starts_with("Hello"));
    std::cout << std::format("ends_with('!'): {}\n\n",
                             haystack.ends_with("!"));

    // ========================================================================
    // Substring
    // ========================================================================
    std::cout << "--- Substring ---\n";

    std::string original = "Hello, World!";
    std::cout << std::format("original: '{}'\n", original);

    std::string sub1 = original.substr(0, 5);   // First 5 chars
    std::string sub2 = original.substr(7);       // From position 7 to end
    std::string sub3 = original.substr(7, 5);    // 5 chars starting at 7

    std::cout << std::format("substr(0, 5): '{}'\n", sub1);
    std::cout << std::format("substr(7): '{}'\n", sub2);
    std::cout << std::format("substr(7, 5): '{}'\n\n", sub3);

    // ========================================================================
    // Comparison
    // ========================================================================
    std::cout << "--- Comparison ---\n";

    std::string a = "apple";
    std::string b = "banana";
    std::string c = "apple";

    std::cout << std::format("'{}' == '{}': {}\n", a, c, a == c);
    std::cout << std::format("'{}' < '{}': {}\n", a, b, a < b);
    std::cout << std::format("'{}' > '{}': {}\n", a, b, a > b);

    // compare() returns <0, 0, or >0
    int cmp = a.compare(b);
    std::cout << std::format("'{}'.compare('{}'): {}\n\n", a, b, cmp);

    // ========================================================================
    // Conversions
    // ========================================================================
    std::cout << "--- Conversions ---\n";

    // Number to string
    std::string num_str = std::to_string(42);
    std::cout << std::format("to_string(42): '{}'\n", num_str);

    std::string pi_str = std::to_string(3.14159);
    std::cout << std::format("to_string(3.14159): '{}'\n", pi_str);

    // String to number
    int n = std::stoi("123");
    double d = std::stod("3.14");
    std::cout << std::format("stoi('123'): {}\n", n);
    std::cout << std::format("stod('3.14'): {}\n\n", d);

    // ========================================================================
    // Common Patterns
    // ========================================================================
    std::cout << "--- Common Patterns ---\n";

    // Trim whitespace (no built-in, but easy to do)
    std::string padded = "   hello world   ";
    std::cout << std::format("padded: '{}'\n", padded);

    auto ltrim = [](std::string& s) {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
            return !std::isspace(ch);
        }));
    };

    auto rtrim = [](std::string& s) {
        s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
            return !std::isspace(ch);
        }).base(), s.end());
    };

    std::string trimmed = padded;
    ltrim(trimmed);
    rtrim(trimmed);
    std::cout << std::format("trimmed: '{}'\n", trimmed);

    // Convert to uppercase
    std::string lower = "hello";
    std::string upper;
    std::transform(lower.begin(), lower.end(), std::back_inserter(upper),
                   [](unsigned char c) { return std::toupper(c); });
    std::cout << std::format("uppercase of '{}': '{}'\n", lower, upper);

    // Split string (basic implementation)
    std::string csv = "apple,banana,cherry";
    std::cout << std::format("csv: '{}'\n", csv);
    std::cout << "split by ',':\n";

    size_t start = 0;
    size_t end = csv.find(',');
    while (end != std::string::npos) {
        std::cout << std::format("  '{}'\n", csv.substr(start, end - start));
        start = end + 1;
        end = csv.find(',', start);
    }
    std::cout << std::format("  '{}'\n", csv.substr(start));

    return 0;
}

// Key points:
// - std::string owns its data and manages memory automatically
// - Use c_str() when C APIs need const char*
// - find() returns npos when not found - always check!
// - substr() extracts portions of a string
// - C++20 adds starts_with() and ends_with()
// - Use std::format for modern string formatting
