// Exercise 10.2: Pattern Matching with Regular Expressions
// Book reference: 10.4 Regular Expressions
// Difficulty: **
//
// Instructions:
// Implement validation and extraction functions using std::regex.
// Use raw string literals R"(...)" to avoid double-escaping.
//
// Hints:
// - Use std::regex_match for full string matching
// - Use std::regex_search with std::smatch to extract groups
// - Use std::regex_replace for substitutions
// - Remember to check if matches were found before accessing groups

#include <iostream>
#include <optional>
#include <regex>
#include <string>
#include <vector>

// ============================================================================
// Exercise 1: Email Validation
// ============================================================================

// Validate if a string is a valid email address
// Valid format: word@word.word (simplified)
// Examples:
//   "user@example.com" -> true
//   "test.user@domain.org" -> true
//   "invalid" -> false
//   "@missing.com" -> false
bool is_valid_email(const std::string& email) {
    // TODO: Create regex pattern and use regex_match
    // Pattern hint: one or more word characters, @, word chars, dot, word chars
    // Word characters include letters, digits, and underscore
    // For a more complete pattern, allow dots in local part: [\w.]+@[\w.]+\.\w+
    return false;
}

// ============================================================================
// Exercise 2: Phone Number Extraction
// ============================================================================

// Extract all phone numbers from text
// Phone format: (XXX) XXX-XXXX or XXX-XXX-XXXX
// Example: "Call (123) 456-7890 or 987-654-3210" -> {"(123) 456-7890", "987-654-3210"}
std::vector<std::string> extract_phone_numbers(const std::string& text) {
    std::vector<std::string> result;
    // TODO: Create regex for phone patterns and find all matches
    // Hint: Use sregex_iterator or sregex_token_iterator
    // Pattern hint: optional parentheses around area code
    return result;
}

// ============================================================================
// Exercise 3: URL Parsing
// ============================================================================

struct UrlParts {
    std::string scheme;   // http or https
    std::string host;     // domain name
    std::string path;     // path (may be empty)

    bool valid = false;
};

// Parse a URL into its components
// Example: "https://example.com/path/to/page" ->
//   scheme: "https", host: "example.com", path: "/path/to/page"
UrlParts parse_url(const std::string& url) {
    UrlParts result;
    // TODO: Create regex with capture groups for scheme, host, path
    // Pattern hint: (https?)://([\w.]+)(/.*)?
    // Use regex_match and smatch to extract groups
    return result;
}

// ============================================================================
// Exercise 4: Date Format Conversion
// ============================================================================

// Convert date from MM/DD/YYYY to YYYY-MM-DD (ISO format)
// Example: "12/25/2024" -> "2024-12-25"
// Returns empty optional if format doesn't match
std::optional<std::string> convert_date_format(const std::string& date) {
    // TODO: Match MM/DD/YYYY and reformat using capture groups
    // Hint: Use regex_replace with backreferences $1, $2, $3
    // Or use regex_search to capture and manually construct result
    return std::nullopt;
}

// ============================================================================
// Exercise 5: Sensitive Data Masking
// ============================================================================

// Mask credit card numbers, keeping only last 4 digits
// Example: "Card: 1234-5678-9012-3456" -> "Card: ****-****-****-3456"
std::string mask_credit_cards(const std::string& text) {
    // TODO: Find credit card patterns and mask all but last 4 digits
    // Pattern: XXXX-XXXX-XXXX-XXXX where X is digit
    // Hint: Capture last 4 digits and replace rest with ****
    return text;
}

// ============================================================================
// Exercise 6: Extract Hashtags
// ============================================================================

// Extract all hashtags from text
// Hashtag format: # followed by word characters
// Example: "Hello #world! Check out #cpp20 and #programming" ->
//   {"#world", "#cpp20", "#programming"}
std::vector<std::string> extract_hashtags(const std::string& text) {
    std::vector<std::string> result;
    // TODO: Find all hashtag patterns
    // Pattern: #\w+
    return result;
}

// ============================================================================
// Test your implementations
// ============================================================================

int main() {
    std::cout << "=== Regex Exercises ===\n\n";

    // Test email validation
    std::cout << "--- Email Validation ---\n";
    std::vector<std::string> emails = {
        "user@example.com",
        "test.user@domain.org",
        "invalid",
        "@missing.com",
        "no-at-sign.com"
    };
    for (const auto& email : emails) {
        std::cout << "'" << email << "': "
                  << (is_valid_email(email) ? "valid" : "invalid") << "\n";
    }
    std::cout << "\n";

    // Test phone extraction
    std::cout << "--- Phone Extraction ---\n";
    std::string phone_text = "Contact: (123) 456-7890 or 987-654-3210 for info.";
    std::cout << "Text: '" << phone_text << "'\n";
    std::cout << "Found: ";
    for (const auto& phone : extract_phone_numbers(phone_text)) {
        std::cout << "'" << phone << "' ";
    }
    std::cout << "\n\n";

    // Test URL parsing
    std::cout << "--- URL Parsing ---\n";
    std::vector<std::string> urls = {
        "https://example.com/path/to/page",
        "http://test.org",
        "not-a-url"
    };
    for (const auto& url : urls) {
        auto parts = parse_url(url);
        std::cout << "'" << url << "':\n";
        if (parts.valid) {
            std::cout << "  scheme: '" << parts.scheme << "'\n";
            std::cout << "  host: '" << parts.host << "'\n";
            std::cout << "  path: '" << parts.path << "'\n";
        } else {
            std::cout << "  Invalid URL\n";
        }
    }
    std::cout << "\n";

    // Test date conversion
    std::cout << "--- Date Conversion ---\n";
    std::vector<std::string> dates = {"12/25/2024", "01/01/2000", "invalid"};
    for (const auto& date : dates) {
        auto converted = convert_date_format(date);
        std::cout << "'" << date << "' -> ";
        if (converted) {
            std::cout << "'" << *converted << "'\n";
        } else {
            std::cout << "(invalid format)\n";
        }
    }
    std::cout << "\n";

    // Test credit card masking
    std::cout << "--- Credit Card Masking ---\n";
    std::string cc_text = "Cards: 1234-5678-9012-3456 and 1111-2222-3333-4444";
    std::cout << "Original: '" << cc_text << "'\n";
    std::cout << "Masked: '" << mask_credit_cards(cc_text) << "'\n\n";

    // Test hashtag extraction
    std::cout << "--- Hashtag Extraction ---\n";
    std::string hashtag_text = "Learning #cpp20 is fun! #programming #code";
    std::cout << "Text: '" << hashtag_text << "'\n";
    std::cout << "Hashtags: ";
    for (const auto& tag : extract_hashtags(hashtag_text)) {
        std::cout << "'" << tag << "' ";
    }
    std::cout << "\n\n";

    return 0;
}

// Expected output when correctly implemented:
// --- Email Validation ---
// 'user@example.com': valid
// 'test.user@domain.org': valid
// 'invalid': invalid
// '@missing.com': invalid
// 'no-at-sign.com': invalid
//
// --- Phone Extraction ---
// Text: 'Contact: (123) 456-7890 or 987-654-3210 for info.'
// Found: '(123) 456-7890' '987-654-3210'
//
// --- URL Parsing ---
// 'https://example.com/path/to/page':
//   scheme: 'https'
//   host: 'example.com'
//   path: '/path/to/page'
// 'http://test.org':
//   scheme: 'http'
//   host: 'test.org'
//   path: ''
// 'not-a-url':
//   Invalid URL
//
// --- Date Conversion ---
// '12/25/2024' -> '2024-12-25'
// '01/01/2000' -> '2000-01-01'
// 'invalid' -> (invalid format)
//
// --- Credit Card Masking ---
// Original: 'Cards: 1234-5678-9012-3456 and 1111-2222-3333-4444'
// Masked: 'Cards: ****-****-****-3456 and ****-****-****-4444'
//
// --- Hashtag Extraction ---
// Text: 'Learning #cpp20 is fun! #programming #code'
// Hashtags: '#cpp20' '#programming' '#code'
