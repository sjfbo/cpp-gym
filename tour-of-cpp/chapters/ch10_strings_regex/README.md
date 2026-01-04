# Chapter 10: Strings and Regular Expressions

## TL;DR

Use `std::string` for dynamic text handling and `std::string_view` for non-owning references that avoid copies. Regular expressions (`std::regex`) provide powerful pattern matching - use them for complex text processing, but prefer simple string operations when possible due to regex performance overhead.

## Key Concepts

| Concept | One-liner | Example |
|---------|-----------|---------|
| `std::string` | Owning, mutable string | `std::string s = "hello";` |
| `std::string_view` | Non-owning string reference | `std::string_view sv = s;` |
| `c_str()` | Get null-terminated C string | `s.c_str()` for C APIs |
| `substr()` | Extract substring | `s.substr(0, 5)` |
| `find()` | Search for substring | `s.find("llo")` returns 2 |
| `std::regex` | Regular expression pattern | `std::regex r{R"(\d+)"};` |
| `regex_match` | Match entire string | `regex_match(s, r)` |
| `regex_search` | Find pattern in string | `regex_search(s, match, r)` |
| `regex_replace` | Replace pattern | `regex_replace(s, r, "new")` |

## Things to Remember

- **string vs string_view**: Use `string` when you need ownership, `string_view` for read-only access without copying.
- **string_view dangers**: Ensure the underlying string outlives the view; dangling views are UB.
- **c_str() for C APIs**: Use when interfacing with C libraries expecting `const char*`.
- **npos for not found**: `find()` returns `std::string::npos` when substring not found.
- **Raw strings for regex**: Use `R"(...)"` to avoid double-escaping backslashes.
- **regex performance**: Compile regex once, use many times; regex is relatively slow.
- **ECMAScript by default**: C++ uses ECMAScript regex grammar (similar to JavaScript).

## Modern C++ Highlights

What's new/better in C++20 compared to older styles:

| Feature | Old Style | Modern C++20 |
|---------|-----------|--------------|
| String reference | `const std::string&` | `std::string_view` |
| String contains | `s.find(x) != npos` | `s.contains(x)` (C++23) |
| Starts/ends with | Manual comparison | `s.starts_with("hi")`, `s.ends_with("!")` |
| Interpolation | `"Value: " + std::to_string(x)` | `std::format("Value: {}", x)` |
| Raw strings | `"\\d+\\.\\d+"` | `R"(\d+\.\d+)"` |

## Common Pitfalls

- **Dangling string_view**: View outlives the string it references.
  ```cpp
  std::string_view bad() {
      std::string s = "hello";
      return s;  // BAD: s destroyed, view dangles!
  }
  ```

- **Forgetting npos check**: Assuming find() always succeeds.
  ```cpp
  auto pos = s.find("x");
  char c = s[pos];  // BAD: if not found, pos is npos (huge)!

  if (auto pos = s.find("x"); pos != std::string::npos) {
      char c = s[pos];  // GOOD: check first
  }
  ```

- **Implicit conversions losing data**: string_view from temporary.
  ```cpp
  std::string_view sv = std::string("temp");  // BAD: temp destroyed!
  std::string s = "keep";
  std::string_view sv = s;  // GOOD: s outlives sv
  ```

- **Regex escaping**: Forgetting to escape in regular strings.
  ```cpp
  std::regex r("\\d+");     // OK but ugly (double backslash)
  std::regex r{R"(\d+)"};   // BETTER: raw string literal
  ```

- **Regex performance**: Creating regex in a loop.
  ```cpp
  for (const auto& s : strings) {
      std::regex r{pattern};  // BAD: compiles regex each iteration
      regex_match(s, r);
  }

  std::regex r{pattern};  // GOOD: compile once
  for (const auto& s : strings) {
      regex_match(s, r);
  }
  ```

## Quick Reference

```cpp
// ============================================================================
// std::string
// ============================================================================
#include <string>

std::string s = "Hello, World!";
std::string s2{"Hello"};              // Uniform initialization
std::string s3(5, 'x');               // "xxxxx"

// Size and capacity
s.length();                           // 13 (same as size())
s.empty();                            // false
s.capacity();                         // >= length()
s.reserve(100);                       // Reserve capacity

// Access
s[0];                                 // 'H' (no bounds check)
s.at(0);                              // 'H' (throws if out of bounds)
s.front();                            // 'H'
s.back();                             // '!'
s.c_str();                            // const char* for C APIs
s.data();                             // char* (C++17: non-const)

// Modification
s += " Hi";                           // Append
s.append(" there");                   // Append
s.insert(5, "XYZ");                   // Insert at position
s.erase(5, 3);                        // Erase 3 chars at position 5
s.replace(0, 5, "Hi");                // Replace "Hello" with "Hi"
s.clear();                            // Empty the string

// Searching
s.find("World");                      // Position or npos
s.rfind("o");                         // Last occurrence
s.find_first_of("aeiou");             // First vowel
s.find_last_not_of(" \t\n");          // Trim position

// Substring
s.substr(0, 5);                       // First 5 chars
s.substr(7);                          // From position 7 to end

// Comparison
s == "Hello";                         // Equality
s.compare("Hello");                   // <0, 0, >0
s.starts_with("He");                  // C++20
s.ends_with("!");                     // C++20

// Conversion
std::to_string(42);                   // "42"
std::to_string(3.14);                 // "3.140000"
std::stoi("42");                      // 42
std::stod("3.14");                    // 3.14

// ============================================================================
// std::string_view (C++17)
// ============================================================================
#include <string_view>

std::string str = "Hello, World!";
std::string_view sv = str;            // Non-owning view
std::string_view sv2 = "literal";     // View of string literal (safe)
std::string_view sv3{str.data(), 5};  // First 5 chars

// All const operations from string work
sv.length();
sv.substr(0, 5);
sv.find("o");
sv.starts_with("He");

// Creating string from view
std::string s{sv};                    // Explicit conversion

// ============================================================================
// std::regex
// ============================================================================
#include <regex>

// Creating regex (use raw strings R"(...)" to avoid escape hell)
std::regex digits{R"(\d+)"};          // One or more digits
std::regex email{R"(\w+@\w+\.\w+)"};  // Simple email pattern
std::regex phone{R"(\(\d{3}\) \d{3}-\d{4})"};  // (123) 456-7890

// Match entire string
bool match = std::regex_match("12345", digits);  // true

// Search for pattern
std::smatch m;
std::string text = "Price: $42.99";
if (std::regex_search(text, m, std::regex{R"(\$(\d+)\.(\d+))"})) {
    // m[0] = "$42.99" (full match)
    // m[1] = "42" (first capture group)
    // m[2] = "99" (second capture group)
}

// Replace pattern
std::string result = std::regex_replace(
    "Hello World",
    std::regex{R"(\bWorld\b)"},
    "C++"
);  // "Hello C++"

// Iterate over matches
std::string numbers = "a1b22c333";
std::regex num{R"(\d+)"};
auto begin = std::sregex_iterator(numbers.begin(), numbers.end(), num);
auto end = std::sregex_iterator();
for (auto it = begin; it != end; ++it) {
    std::cout << it->str() << "\n";  // 1, 22, 333
}

// Common regex patterns
R"(\d)"          // Single digit
R"(\d+)"         // One or more digits
R"(\d*)"         // Zero or more digits
R"(\d{3})"       // Exactly 3 digits
R"(\d{2,4})"     // 2 to 4 digits
R"(\w)"          // Word character [a-zA-Z0-9_]
R"(\s)"          // Whitespace
R"(.)"           // Any character except newline
R"(^start)"      // Anchored at start
R"(end$)"        // Anchored at end
R"(\bword\b)"    // Word boundary
R"(a|b)"         // a or b
R"([aeiou])"     // Character class
R"([^aeiou])"    // Negated character class
R"((...)+)"      // Capture group, one or more
R"((?:...)+)"    // Non-capturing group
```

## Book Sections Covered

- **10.1** Introduction
- **10.2** Strings
- **10.3** String Views
- **10.4** Regular Expressions
- **10.5** Advice

## Exercises in This Chapter

| Exercise | Difficulty | Concepts Practiced |
|----------|------------|-------------------|
| ex01_strings | **       | String manipulation, searching, substr |
| ex02_regex | **         | Pattern matching, capturing groups, replacement |

## Running the Examples

```bash
# Build all chapter 10 examples
cmake --build build --target ch10_examples

# Run individual examples
./build/bin/ch10_strings
./build/bin/ch10_string_views
./build/bin/ch10_regex

# Run chapter 10 tests
ctest --test-dir build -R ch10
```
