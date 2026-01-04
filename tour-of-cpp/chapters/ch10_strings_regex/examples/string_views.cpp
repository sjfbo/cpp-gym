// String Views - Non-owning String References
// Book reference: 10.3 String Views
//
// std::string_view provides a lightweight, non-owning reference to a string.
// It avoids copying and is ideal for read-only string parameters.

#include <format>
#include <iostream>
#include <string>
#include <string_view>

// ============================================================================
// Function parameters: string_view vs const string&
// ============================================================================

// Old style: requires std::string, forces copy from C-strings
void process_string_old(const std::string& s) {
    std::cout << std::format("  [const string&] length: {}\n", s.length());
}

// Modern style: accepts any string-like type without copying
void process_string_new(std::string_view sv) {
    std::cout << std::format("  [string_view] length: {}\n", sv.length());
}

// ============================================================================
// Demonstrating substring views
// ============================================================================
void demonstrate_substrings() {
    std::cout << "\n--- Substring Views ---\n";

    std::string str = "Hello, World!";
    std::string_view full = str;
    std::string_view hello = full.substr(0, 5);
    std::string_view world = full.substr(7, 5);

    std::cout << std::format("full: '{}'\n", full);
    std::cout << std::format("hello: '{}'\n", hello);
    std::cout << std::format("world: '{}'\n", world);

    // Important: substrings are views into the SAME memory
    std::cout << std::format("full.data(): {}\n", static_cast<const void*>(full.data()));
    std::cout << std::format("hello.data(): {}\n", static_cast<const void*>(hello.data()));
    std::cout << std::format("world.data(): {}\n", static_cast<const void*>(world.data()));
}

// ============================================================================
// Safe string parsing without allocations
// ============================================================================
struct ParsedUrl {
    std::string_view scheme;
    std::string_view host;
    std::string_view path;
};

ParsedUrl parse_url(std::string_view url) {
    ParsedUrl result;

    // Find scheme (e.g., "https")
    if (auto pos = url.find("://"); pos != std::string_view::npos) {
        result.scheme = url.substr(0, pos);
        url.remove_prefix(pos + 3);  // Skip past "://"
    }

    // Find host (everything before first '/')
    if (auto pos = url.find('/'); pos != std::string_view::npos) {
        result.host = url.substr(0, pos);
        result.path = url.substr(pos);
    } else {
        result.host = url;
        result.path = "/";
    }

    return result;
}

void demonstrate_parsing() {
    std::cout << "\n--- URL Parsing with string_view ---\n";

    std::string url = "https://example.com/path/to/resource";
    std::cout << std::format("URL: '{}'\n", url);

    auto parsed = parse_url(url);
    std::cout << std::format("scheme: '{}'\n", parsed.scheme);
    std::cout << std::format("host: '{}'\n", parsed.host);
    std::cout << std::format("path: '{}'\n", parsed.path);

    // Note: All views point into the original string - no allocations!
}

// ============================================================================
// View modification (viewing different portions)
// ============================================================================
void demonstrate_view_modification() {
    std::cout << "\n--- View Modification ---\n";

    std::string_view sv = "   Hello, World!   ";
    std::cout << std::format("original: '{}'\n", sv);

    // remove_prefix - remove characters from the front
    sv.remove_prefix(3);  // Remove leading spaces
    std::cout << std::format("after remove_prefix(3): '{}'\n", sv);

    // remove_suffix - remove characters from the end
    sv.remove_suffix(3);  // Remove trailing spaces
    std::cout << std::format("after remove_suffix(3): '{}'\n", sv);

    // Note: These don't modify the underlying string - just the view!
}

// ============================================================================
// String literal views (always safe)
// ============================================================================
void demonstrate_literal_views() {
    std::cout << "\n--- Literal Views ---\n";

    // String literals have static storage duration - safe for string_view
    std::string_view sv1 = "Hello, World!";  // Safe: literal lives forever
    std::cout << std::format("literal view: '{}'\n", sv1);

    // Using constexpr for compile-time string handling
    constexpr std::string_view greeting = "Compile-time greeting";
    std::cout << std::format("constexpr view: '{}'\n", greeting);
    static_assert(greeting.length() == 21);
}

// ============================================================================
// Common operations (same interface as std::string for reading)
// ============================================================================
void demonstrate_operations() {
    std::cout << "\n--- Common Operations ---\n";

    std::string_view sv = "Hello, World!";

    // Size operations
    std::cout << std::format("sv: '{}'\n", sv);
    std::cout << std::format("length(): {}\n", sv.length());
    std::cout << std::format("empty(): {}\n", sv.empty());

    // Element access
    std::cout << std::format("sv[0]: '{}'\n", sv[0]);
    std::cout << std::format("sv.at(7): '{}'\n", sv.at(7));
    std::cout << std::format("front(): '{}'\n", sv.front());
    std::cout << std::format("back(): '{}'\n", sv.back());

    // Searching
    std::cout << std::format("find('o'): {}\n", sv.find('o'));
    std::cout << std::format("rfind('o'): {}\n", sv.rfind('o'));

    // Prefix/suffix checking (C++20)
    std::cout << std::format("starts_with('Hello'): {}\n", sv.starts_with("Hello"));
    std::cout << std::format("ends_with('!'): {}\n", sv.ends_with("!"));

    // Comparison
    std::cout << std::format("compare('Hello, World!'): {}\n",
                             sv.compare("Hello, World!"));
}

// ============================================================================
// Danger zone: views can dangle!
// ============================================================================
void demonstrate_dangers() {
    std::cout << "\n--- DANGER: Dangling Views ---\n";

    // SAFE: view of a string that outlives the view
    std::string s = "I'm safe!";
    std::string_view safe_view = s;
    std::cout << std::format("safe_view: '{}'\n", safe_view);

    // DANGEROUS patterns (commented out to avoid UB):

    // 1. View of temporary
    // std::string_view bad1 = std::string("temporary");  // BAD!

    // 2. View outlives source
    // std::string_view bad2;
    // {
    //     std::string temp = "temporary";
    //     bad2 = temp;
    // }  // temp destroyed, bad2 dangles!

    // 3. Returning view of local
    // auto bad_func = []() -> std::string_view {
    //     std::string local = "local";
    //     return local;  // BAD: returns dangling view
    // };

    std::cout << "Remember: the underlying string must outlive the view!\n";
}

// ============================================================================
// Converting between string and string_view
// ============================================================================
void demonstrate_conversion() {
    std::cout << "\n--- Conversion ---\n";

    // string -> string_view: implicit
    std::string str = "Hello";
    std::string_view sv = str;
    std::cout << std::format("string -> string_view: '{}'\n", sv);

    // string_view -> string: explicit (requires copy)
    std::string str2{sv};  // Explicit construction
    std::cout << std::format("string_view -> string: '{}'\n", str2);

    // C-string literal -> string_view: implicit (and safe)
    std::string_view sv2 = "literal";
    std::cout << std::format("literal -> string_view: '{}'\n", sv2);
}

int main() {
    std::cout << "=== std::string_view Examples ===\n";

    // Demonstrate efficiency advantage
    std::cout << "\n--- Parameter Passing ---\n";
    std::string s = "A std::string";
    const char* c = "A C-string";

    std::cout << "Calling with std::string:\n";
    process_string_old(s);  // OK
    process_string_new(s);  // OK, no copy

    std::cout << "Calling with C-string:\n";
    process_string_old(c);  // Creates temporary std::string!
    process_string_new(c);  // No copy, just a view

    std::cout << "Calling with literal:\n";
    process_string_old("literal");  // Creates temporary std::string!
    process_string_new("literal");  // No copy, just a view

    demonstrate_substrings();
    demonstrate_parsing();
    demonstrate_view_modification();
    demonstrate_literal_views();
    demonstrate_operations();
    demonstrate_dangers();
    demonstrate_conversion();

    return 0;
}

// Key points:
// - string_view is a non-owning reference to a string
// - Use it for function parameters that only need to read strings
// - Avoids copies when passing C-strings or string literals
// - DANGER: Ensure the underlying string outlives the view
// - Most read-only std::string operations work on string_view
// - Converting view to string requires explicit copy
