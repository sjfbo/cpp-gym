// Exercise 10.1: String Manipulation
// Book reference: 10.2 Strings
// Difficulty: **
//
// Instructions:
// Implement the following string manipulation functions.
// Each function has TODO comments explaining what to do.
//
// Hints:
// - Use find(), substr(), replace() for searching and modifying
// - Use starts_with(), ends_with() for prefix/suffix checks (C++20)
// - Remember: find() returns std::string::npos when not found
// - Use std::transform for case conversion

#include <algorithm>
#include <cctype>
#include <iostream>
#include <string>
#include <vector>

// ============================================================================
// Exercise 1: Implement trim functions
// ============================================================================

// Remove leading whitespace from string
// Example: "  hello  " -> "hello  "
std::string trim_left(std::string s) {
    // TODO: Remove leading whitespace characters
    // Hint: Use find_if and erase, or a loop
    return s;
}

// Remove trailing whitespace from string
// Example: "  hello  " -> "  hello"
std::string trim_right(std::string s) {
    // TODO: Remove trailing whitespace characters
    // Hint: Use find_if with reverse iterators and erase
    return s;
}

// Remove both leading and trailing whitespace
// Example: "  hello  " -> "hello"
std::string trim(std::string s) {
    // TODO: Combine trim_left and trim_right
    return s;
}

// ============================================================================
// Exercise 2: Case conversion
// ============================================================================

// Convert string to uppercase
// Example: "Hello World" -> "HELLO WORLD"
std::string to_upper(std::string s) {
    // TODO: Convert all characters to uppercase
    // Hint: Use std::transform with std::toupper
    return s;
}

// Convert string to lowercase
// Example: "Hello World" -> "hello world"
std::string to_lower(std::string s) {
    // TODO: Convert all characters to lowercase
    // Hint: Use std::transform with std::tolower
    return s;
}

// ============================================================================
// Exercise 3: String splitting
// ============================================================================

// Split a string by a delimiter character
// Example: split("a,b,c", ',') -> {"a", "b", "c"}
std::vector<std::string> split(const std::string& s, char delimiter) {
    std::vector<std::string> result;
    // TODO: Split the string by delimiter
    // Hint: Use find() and substr() in a loop
    return result;
}

// ============================================================================
// Exercise 4: String joining
// ============================================================================

// Join a vector of strings with a delimiter
// Example: join({"a", "b", "c"}, ", ") -> "a, b, c"
std::string join(const std::vector<std::string>& parts, const std::string& delimiter) {
    // TODO: Join strings with delimiter between them
    // Hint: Be careful not to add delimiter after last element
    return "";
}

// ============================================================================
// Exercise 5: Replace all occurrences
// ============================================================================

// Replace all occurrences of 'from' with 'to' in string
// Example: replace_all("hello hello", "hello", "hi") -> "hi hi"
std::string replace_all(std::string s, const std::string& from, const std::string& to) {
    // TODO: Replace all occurrences of 'from' with 'to'
    // Hint: Use find() and replace() in a loop
    // Be careful with replacement position when 'to' contains 'from'
    return s;
}

// ============================================================================
// Exercise 6: Word counting
// ============================================================================

// Count the number of words in a string (words are separated by whitespace)
// Example: count_words("  hello   world  ") -> 2
int count_words(const std::string& s) {
    // TODO: Count words separated by whitespace
    // Hint: You can use istringstream, or manual iteration
    return 0;
}

// ============================================================================
// Test your implementations
// ============================================================================

int main() {
    std::cout << "=== String Manipulation Exercises ===\n\n";

    // Test trim functions
    std::cout << "--- Trim Functions ---\n";
    std::string padded = "  hello world  ";
    std::cout << "Original: '" << padded << "'\n";
    std::cout << "trim_left: '" << trim_left(padded) << "'\n";
    std::cout << "trim_right: '" << trim_right(padded) << "'\n";
    std::cout << "trim: '" << trim(padded) << "'\n\n";

    // Test case conversion
    std::cout << "--- Case Conversion ---\n";
    std::string mixed = "Hello World";
    std::cout << "Original: '" << mixed << "'\n";
    std::cout << "to_upper: '" << to_upper(mixed) << "'\n";
    std::cout << "to_lower: '" << to_lower(mixed) << "'\n\n";

    // Test split
    std::cout << "--- Split ---\n";
    std::string csv = "apple,banana,cherry,date";
    std::cout << "Original: '" << csv << "'\n";
    std::cout << "split by ',': ";
    auto parts = split(csv, ',');
    for (const auto& p : parts) {
        std::cout << "'" << p << "' ";
    }
    std::cout << "\n\n";

    // Test join
    std::cout << "--- Join ---\n";
    std::vector<std::string> words = {"one", "two", "three"};
    std::cout << "Parts: ";
    for (const auto& w : words) std::cout << "'" << w << "' ";
    std::cout << "\n";
    std::cout << "join with ' - ': '" << join(words, " - ") << "'\n\n";

    // Test replace_all
    std::cout << "--- Replace All ---\n";
    std::string text = "the cat sat on the mat";
    std::cout << "Original: '" << text << "'\n";
    std::cout << "replace 'the' with 'a': '" << replace_all(text, "the", "a") << "'\n\n";

    // Test count_words
    std::cout << "--- Count Words ---\n";
    std::string sentence = "  The quick brown fox  jumps  ";
    std::cout << "Sentence: '" << sentence << "'\n";
    std::cout << "Word count: " << count_words(sentence) << "\n\n";

    return 0;
}

// Expected output when correctly implemented:
// --- Trim Functions ---
// Original: '  hello world  '
// trim_left: 'hello world  '
// trim_right: '  hello world'
// trim: 'hello world'
//
// --- Case Conversion ---
// Original: 'Hello World'
// to_upper: 'HELLO WORLD'
// to_lower: 'hello world'
//
// --- Split ---
// Original: 'apple,banana,cherry,date'
// split by ',': 'apple' 'banana' 'cherry' 'date'
//
// --- Join ---
// Parts: 'one' 'two' 'three'
// join with ' - ': 'one - two - three'
//
// --- Replace All ---
// Original: 'the cat sat on the mat'
// replace 'the' with 'a': 'a cat sat on a mat'
//
// --- Count Words ---
// Sentence: '  The quick brown fox  jumps  '
// Word count: 5
