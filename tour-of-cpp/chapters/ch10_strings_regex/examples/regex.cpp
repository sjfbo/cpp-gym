// Regular Expressions - Pattern Matching with std::regex
// Book reference: 10.4 Regular Expressions
//
// This example demonstrates regex matching, searching, and replacing.
// C++ uses ECMAScript regex grammar by default.

#include <format>
#include <iostream>
#include <regex>
#include <string>
#include <vector>

// ============================================================================
// Basic Matching
// ============================================================================
void demonstrate_matching() {
    std::cout << "=== Basic Matching ===\n\n";

    // regex_match: matches the ENTIRE string
    std::regex digits{R"(\d+)"};  // One or more digits

    std::cout << "Pattern: \\d+ (one or more digits)\n";
    std::cout << std::format("  '12345' matches: {}\n",
                             std::regex_match("12345", digits));
    std::cout << std::format("  'abc' matches: {}\n",
                             std::regex_match("abc", digits));
    std::cout << std::format("  '123abc' matches: {}\n",
                             std::regex_match("123abc", digits));  // Partial != full match!

    // Email validation (simplified)
    std::regex email{R"(\w+@\w+\.\w+)"};
    std::cout << "\nPattern: \\w+@\\w+\\.\\w+ (simple email)\n";
    std::cout << std::format("  'user@example.com' matches: {}\n",
                             std::regex_match("user@example.com", email));
    std::cout << std::format("  'invalid.email' matches: {}\n",
                             std::regex_match("invalid.email", email));
    std::cout << "\n";
}

// ============================================================================
// Searching
// ============================================================================
void demonstrate_searching() {
    std::cout << "=== Searching ===\n\n";

    std::string text = "The price is $42.99 and the quantity is 5 items.";
    std::cout << std::format("Text: '{}'\n\n", text);

    // regex_search: finds pattern ANYWHERE in string
    std::regex price_pattern{R"(\$\d+\.\d{2})"};  // $XX.XX format
    std::smatch match;

    if (std::regex_search(text, match, price_pattern)) {
        std::cout << "Found price pattern:\n";
        std::cout << std::format("  match[0] (full): '{}'\n", match[0].str());
        std::cout << std::format("  position: {}\n", match.position());
        std::cout << std::format("  length: {}\n", match.length());
    }

    // Finding with capture groups
    std::regex capture_pattern{R"(\$(\d+)\.(\d{2}))"};  // Capture dollars and cents

    if (std::regex_search(text, match, capture_pattern)) {
        std::cout << "\nWith capture groups:\n";
        std::cout << std::format("  match[0] (full): '{}'\n", match[0].str());
        std::cout << std::format("  match[1] (dollars): '{}'\n", match[1].str());
        std::cout << std::format("  match[2] (cents): '{}'\n", match[2].str());
    }
    std::cout << "\n";
}

// ============================================================================
// Finding All Matches
// ============================================================================
void demonstrate_find_all() {
    std::cout << "=== Finding All Matches ===\n\n";

    std::string text = "Contact: john@example.com, jane@test.org, bob@company.net";
    std::cout << std::format("Text: '{}'\n\n", text);

    std::regex email{R"(\w+@\w+\.\w+)"};

    // Using sregex_iterator to find all matches
    std::cout << "All email addresses:\n";
    auto begin = std::sregex_iterator(text.begin(), text.end(), email);
    auto end = std::sregex_iterator();

    int count = 0;
    for (auto it = begin; it != end; ++it) {
        std::cout << std::format("  {}: '{}' at position {}\n",
                                 ++count, it->str(), it->position());
    }

    // Using sregex_token_iterator for just the matches
    std::cout << "\nUsing token iterator:\n";
    std::sregex_token_iterator token_begin(text.begin(), text.end(), email);
    std::sregex_token_iterator token_end;

    std::vector<std::string> emails(token_begin, token_end);
    for (const auto& e : emails) {
        std::cout << std::format("  '{}'\n", e);
    }
    std::cout << "\n";
}

// ============================================================================
// Replacing
// ============================================================================
void demonstrate_replacing() {
    std::cout << "=== Replacing ===\n\n";

    // Simple replacement
    std::string text1 = "Hello World, Hello Universe!";
    std::cout << std::format("Original: '{}'\n", text1);

    std::regex hello{R"(Hello)"};
    std::string result1 = std::regex_replace(text1, hello, "Hi");
    std::cout << std::format("Replace 'Hello' with 'Hi': '{}'\n\n", result1);

    // Replacement with backreferences
    std::string text2 = "John Smith, Jane Doe, Bob Jones";
    std::cout << std::format("Original: '{}'\n", text2);

    std::regex name{R"((\w+) (\w+))"};  // First Last
    std::string result2 = std::regex_replace(text2, name, "$2, $1");  // Last, First
    std::cout << std::format("Swap first/last names: '{}'\n\n", result2);

    // Phone number formatting
    std::string phones = "1234567890 and 9876543210";
    std::cout << std::format("Original: '{}'\n", phones);

    std::regex phone{R"((\d{3})(\d{3})(\d{4}))"};
    std::string formatted = std::regex_replace(phones, phone, "($1) $2-$3");
    std::cout << std::format("Formatted: '{}'\n\n", formatted);

    // Removing matches
    std::string noisy = "Hello123World456!";
    std::cout << std::format("Original: '{}'\n", noisy);

    std::regex digits{R"(\d+)"};
    std::string clean = std::regex_replace(noisy, digits, "");
    std::cout << std::format("Remove digits: '{}'\n\n", clean);
}

// ============================================================================
// Common Patterns
// ============================================================================
void demonstrate_patterns() {
    std::cout << "=== Common Regex Patterns ===\n\n";

    // Test strings
    std::vector<std::pair<std::string, std::string>> tests = {
        {"US Phone", "(123) 456-7890"},
        {"Date", "2024-01-15"},
        {"Time", "14:30:00"},
        {"IPv4", "192.168.1.1"},
        {"Hex Color", "#FF5733"},
        {"URL", "https://example.com/path"},
    };

    // Patterns
    std::regex phone{R"(\(\d{3}\) \d{3}-\d{4})"};
    std::regex date{R"(\d{4}-\d{2}-\d{2})"};
    std::regex time24{R"(\d{2}:\d{2}:\d{2})"};
    std::regex ipv4{R"(\d{1,3}\.\d{1,3}\.\d{1,3}\.\d{1,3})"};
    std::regex hex_color{R"(#[0-9A-Fa-f]{6})"};
    std::regex url{R"(https?://\w+\.\w+(/\w+)*)"};

    std::vector<std::pair<std::string, std::regex*>> patterns = {
        {"Phone", &phone},
        {"Date", &date},
        {"Time", &time24},
        {"IPv4", &ipv4},
        {"Hex Color", &hex_color},
        {"URL", &url},
    };

    std::cout << "Testing various patterns:\n";
    for (const auto& [name, value] : tests) {
        std::cout << std::format("\n'{}' ({}):\n", value, name);
        for (const auto& [pattern_name, pattern] : patterns) {
            bool matches = std::regex_match(value, *pattern);
            if (matches) {
                std::cout << std::format("  [MATCH] {}\n", pattern_name);
            }
        }
    }
    std::cout << "\n";
}

// ============================================================================
// Regex Flags
// ============================================================================
void demonstrate_flags() {
    std::cout << "=== Regex Flags ===\n\n";

    std::string text = "Hello WORLD hello world";
    std::cout << std::format("Text: '{}'\n\n", text);

    // Default: case-sensitive
    std::regex case_sensitive{R"(hello)"};
    std::cout << "Pattern 'hello' (case-sensitive):\n";
    auto begin1 = std::sregex_iterator(text.begin(), text.end(), case_sensitive);
    auto end1 = std::sregex_iterator();
    int count1 = std::distance(begin1, end1);
    std::cout << std::format("  Found {} matches\n", count1);

    // Case-insensitive
    std::regex case_insensitive{R"(hello)", std::regex::icase};
    std::cout << "\nPattern 'hello' (case-insensitive):\n";
    auto begin2 = std::sregex_iterator(text.begin(), text.end(), case_insensitive);
    auto end2 = std::sregex_iterator();
    int count2 = std::distance(begin2, end2);
    std::cout << std::format("  Found {} matches\n\n", count2);
}

// ============================================================================
// Performance Considerations
// ============================================================================
void demonstrate_performance() {
    std::cout << "=== Performance Tips ===\n\n";

    // BAD: Compiling regex in loop
    std::cout << "DON'T: Compile regex inside loops\n";
    std::cout << R"(  for (auto& s : strings) {
      std::regex r{pattern};  // Slow!
      regex_match(s, r);
  }
)";

    // GOOD: Compile once, use many times
    std::cout << "\nDO: Compile regex once, reuse\n";
    std::cout << R"(  std::regex r{pattern};  // Once
  for (auto& s : strings) {
      regex_match(s, r);  // Fast!
  }
)";

    std::cout << "\nOther tips:\n";
    std::cout << "  - Prefer simple string operations when regex isn't needed\n";
    std::cout << "  - Use non-capturing groups (?:...) when captures aren't needed\n";
    std::cout << "  - Anchor patterns with ^ and $ when matching whole strings\n";
    std::cout << "  - Be specific: \\d{4} is faster than \\d+\n\n";
}

int main() {
    std::cout << "========================================\n";
    std::cout << "    Regular Expressions in C++\n";
    std::cout << "========================================\n\n";

    demonstrate_matching();
    demonstrate_searching();
    demonstrate_find_all();
    demonstrate_replacing();
    demonstrate_patterns();
    demonstrate_flags();
    demonstrate_performance();

    return 0;
}

// Key points:
// - Use raw strings R"(...)" to avoid double-escaping backslashes
// - regex_match matches entire string; regex_search finds pattern anywhere
// - Use std::smatch to access match results and capture groups
// - sregex_iterator finds all matches in a string
// - regex_replace supports backreferences ($1, $2, etc.)
// - Compile regex once, use many times for performance
// - C++ uses ECMAScript regex grammar by default
