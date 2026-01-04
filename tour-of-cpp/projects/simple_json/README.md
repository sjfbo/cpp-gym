# Simple JSON

A basic JSON parser implementation demonstrating `std::variant`, type-safe unions, and recursive data structures in modern C++.

## Learning Objectives

After completing this project, you will understand:

1. **std::variant**
   - Type-safe union for holding one of several types
   - Using `std::visit` for type-based dispatch
   - `std::holds_alternative` for type checking
   - `std::get` and `std::get_if` for value access

2. **Recursive Data Structures**
   - Forward declarations and incomplete types
   - Using smart pointers for recursive types
   - `std::unique_ptr` with `std::vector` for JSON arrays
   - `std::map` for JSON objects

3. **String Parsing**
   - Character-by-character parsing techniques
   - Handling escape sequences
   - Number parsing with edge cases

4. **Error Handling**
   - Using exceptions for parse errors
   - Providing meaningful error messages

5. **Modern C++ Type Design**
   - Designing type hierarchies without inheritance
   - Value semantics with variant types

## Project Structure

```
simple_json/
├── CMakeLists.txt          # Build configuration
├── README.md               # This file
├── json_value.h            # JSON value type definition
├── json_parser.h           # Parser interface
├── json_parser.cpp         # Parser implementation
├── main.cpp                # Demo program
└── tests/
    └── test_json.cpp       # Catch2 unit tests
```

## JSON Value Types

The implementation supports all JSON types:
- `null` - represented as `std::monostate`
- `boolean` - `true` or `false`
- `number` - floating-point numbers
- `string` - UTF-8 strings
- `array` - ordered list of values
- `object` - key-value pairs

## Implementation Notes

### The JsonValue Type

```cpp
// Forward declaration for recursive type
struct JsonValue;

// JSON object is a map of string to JsonValue
using JsonObject = std::map<std::string, JsonValue>;

// JSON array is a vector of JsonValue
using JsonArray = std::vector<JsonValue>;

// The variant holding all possible JSON types
using JsonVariant = std::variant<
    std::monostate,  // null
    bool,            // boolean
    double,          // number
    std::string,     // string
    JsonArray,       // array
    JsonObject       // object
>;
```

### Accessing Values

Use `std::visit` with a visitor pattern:

```cpp
std::visit([](auto&& arg) {
    using T = std::decay_t<decltype(arg)>;
    if constexpr (std::is_same_v<T, std::string>) {
        std::cout << "It's a string: " << arg << "\n";
    }
    // ... handle other types
}, json_value);
```

Or use typed accessors:

```cpp
if (value.is_string()) {
    std::cout << value.as_string() << "\n";
}
```

## Building

```bash
mkdir build && cd build
cmake ..
cmake --build .
```

## Running

```bash
# Run the demo
./simple_json_demo

# Run tests
ctest --output-on-failure
```

## Key Concepts from "A Tour of C++"

This project applies concepts from:
- **Chapter 15**: Utilities (variant, optional, any)
- **Chapter 10**: Strings (string handling, parsing)
- **Chapter 4**: Error Handling (exceptions)

## Limitations

This is a learning project with intentional simplifications:
- No support for Unicode escape sequences (`\uXXXX`)
- Limited number precision
- No streaming/incremental parsing
- No JSON serialization (output)

## Extension Ideas

- Add JSON serialization (pretty-print output)
- Support for comments (non-standard but useful)
- Add JSON Pointer (RFC 6901) support
- Implement JSON Schema validation
- Add streaming parser for large files
