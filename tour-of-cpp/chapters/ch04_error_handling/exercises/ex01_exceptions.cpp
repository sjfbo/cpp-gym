/**
 * Exercise 01: Exception Handling
 *
 * Implement a configuration file parser with proper exception handling.
 *
 * Requirements:
 * 1. Create custom exception classes that inherit from std::exception
 * 2. Implement a ConfigParser class that:
 *    - Parses key=value pairs from a string
 *    - Throws appropriate exceptions for various error conditions
 *    - Uses RAII for resource management
 * 3. Demonstrate proper exception handling patterns
 *
 * Learning objectives:
 * - Creating custom exception hierarchies
 * - Throwing and catching exceptions appropriately
 * - Using RAII for cleanup
 * - Understanding when to use exceptions vs other error handling
 */

#include <iostream>
#include <string>
#include <string_view>
#include <map>
#include <sstream>
#include <stdexcept>
#include <optional>
#include <vector>

// =============================================================================
// TODO: Implement custom exception classes
// =============================================================================

/**
 * Base exception class for all configuration errors.
 * Should inherit from std::runtime_error.
 *
 * TODO: Implement this class with:
 * - A constructor that takes a message
 * - Proper inheritance from std::runtime_error
 */
class ConfigError : public std::runtime_error {
public:
    // TODO: Implement constructor
    explicit ConfigError(const std::string& msg)
        : std::runtime_error(msg) {}
};

/**
 * Exception for syntax errors in config format.
 *
 * TODO: Implement with:
 * - Line number where error occurred
 * - The problematic line content
 * - A descriptive message
 */
class SyntaxError : public ConfigError {
public:
    // TODO: Implement constructor and accessors
    SyntaxError(int line_number, const std::string& line, const std::string& reason)
        : ConfigError("Syntax error on line " + std::to_string(line_number) +
                      ": " + reason + " (line: '" + line + "')")
        , line_number_(line_number)
        , line_(line)
    {}

    int line_number() const noexcept { return line_number_; }
    const std::string& line() const noexcept { return line_; }

private:
    int line_number_;
    std::string line_;
};

/**
 * Exception for missing required configuration keys.
 *
 * TODO: Implement with:
 * - The name of the missing key
 * - A descriptive message
 */
class MissingKeyError : public ConfigError {
public:
    // TODO: Implement constructor and accessor
    explicit MissingKeyError(const std::string& key)
        : ConfigError("Missing required configuration key: " + key)
        , key_(key)
    {}

    const std::string& key() const noexcept { return key_; }

private:
    std::string key_;
};

/**
 * Exception for invalid configuration values.
 *
 * TODO: Implement with:
 * - The key name
 * - The invalid value
 * - Why it's invalid
 */
class InvalidValueError : public ConfigError {
public:
    // TODO: Implement constructor and accessors
    InvalidValueError(const std::string& key, const std::string& value,
                      const std::string& reason)
        : ConfigError("Invalid value for key '" + key + "': " + reason +
                      " (got: '" + value + "')")
        , key_(key)
        , value_(value)
    {}

    const std::string& key() const noexcept { return key_; }
    const std::string& value() const noexcept { return value_; }

private:
    std::string key_;
    std::string value_;
};

// =============================================================================
// TODO: Implement ConfigParser class
// =============================================================================

/**
 * A simple configuration parser that reads key=value pairs.
 *
 * Format:
 * - Each line is either empty, a comment (# ...), or a key=value pair
 * - Keys are alphanumeric (plus underscore)
 * - Values are everything after the = until end of line
 * - Whitespace around = is trimmed
 *
 * TODO: Implement the following methods:
 * - parse(std::string_view content) - parse config content
 * - get(std::string_view key) - get required value (throws if missing)
 * - get_optional(std::string_view key) - get optional value
 * - get_int(std::string_view key) - get value as integer
 * - get_bool(std::string_view key) - get value as boolean
 */
class ConfigParser {
public:
    /**
     * Parse configuration content.
     * @throws SyntaxError if the format is invalid
     */
    void parse(std::string_view content) {
        // TODO: Implement parsing logic
        // - Split content into lines
        // - Skip empty lines and comments
        // - Parse key=value pairs
        // - Throw SyntaxError for malformed lines

        std::istringstream stream{std::string(content)};
        std::string line;
        int line_number = 0;

        while (std::getline(stream, line)) {
            ++line_number;

            // Trim leading whitespace
            size_t start = line.find_first_not_of(" \t");
            if (start == std::string::npos) {
                continue;  // Empty line
            }

            // Skip comments
            if (line[start] == '#') {
                continue;
            }

            // Find the = separator
            size_t eq_pos = line.find('=');
            if (eq_pos == std::string::npos) {
                throw SyntaxError(line_number, line, "missing '=' separator");
            }

            // Extract key (trim whitespace)
            std::string key = line.substr(start, eq_pos - start);
            size_t key_end = key.find_last_not_of(" \t");
            if (key_end == std::string::npos) {
                throw SyntaxError(line_number, line, "empty key");
            }
            key = key.substr(0, key_end + 1);

            // Validate key (alphanumeric + underscore)
            for (char c : key) {
                if (!std::isalnum(static_cast<unsigned char>(c)) && c != '_') {
                    throw SyntaxError(line_number, line,
                                      "invalid character in key: '" +
                                      std::string(1, c) + "'");
                }
            }

            // Extract value (trim leading whitespace only)
            std::string value;
            if (eq_pos + 1 < line.size()) {
                size_t value_start = line.find_first_not_of(" \t", eq_pos + 1);
                if (value_start != std::string::npos) {
                    value = line.substr(value_start);
                }
            }

            config_[key] = value;
        }
    }

    /**
     * Get a required configuration value.
     * @throws MissingKeyError if key doesn't exist
     */
    std::string get(std::string_view key) const {
        // TODO: Implement
        auto it = config_.find(std::string(key));
        if (it == config_.end()) {
            throw MissingKeyError(std::string(key));
        }
        return it->second;
    }

    /**
     * Get an optional configuration value.
     */
    std::optional<std::string> get_optional(std::string_view key) const {
        // TODO: Implement
        auto it = config_.find(std::string(key));
        if (it == config_.end()) {
            return std::nullopt;
        }
        return it->second;
    }

    /**
     * Get a configuration value as an integer.
     * @throws MissingKeyError if key doesn't exist
     * @throws InvalidValueError if value is not a valid integer
     */
    int get_int(std::string_view key) const {
        // TODO: Implement
        std::string value = get(key);

        try {
            size_t pos = 0;
            int result = std::stoi(value, &pos);
            if (pos != value.size()) {
                throw InvalidValueError(std::string(key), value,
                                        "not a valid integer");
            }
            return result;
        }
        catch (const std::invalid_argument&) {
            throw InvalidValueError(std::string(key), value,
                                    "not a valid integer");
        }
        catch (const std::out_of_range&) {
            throw InvalidValueError(std::string(key), value,
                                    "integer out of range");
        }
    }

    /**
     * Get a configuration value as a boolean.
     * Accepts: true, false, yes, no, 1, 0 (case-insensitive)
     * @throws MissingKeyError if key doesn't exist
     * @throws InvalidValueError if value is not a valid boolean
     */
    bool get_bool(std::string_view key) const {
        // TODO: Implement
        std::string value = get(key);

        // Convert to lowercase for comparison
        std::string lower_value;
        for (char c : value) {
            lower_value += static_cast<char>(
                std::tolower(static_cast<unsigned char>(c)));
        }

        if (lower_value == "true" || lower_value == "yes" || lower_value == "1") {
            return true;
        }
        if (lower_value == "false" || lower_value == "no" || lower_value == "0") {
            return false;
        }

        throw InvalidValueError(std::string(key), value,
                                "not a valid boolean (expected true/false/yes/no/1/0)");
    }

    /**
     * Get all configuration keys.
     */
    std::vector<std::string> keys() const {
        std::vector<std::string> result;
        for (const auto& [key, value] : config_) {
            result.push_back(key);
        }
        return result;
    }

private:
    std::map<std::string, std::string, std::less<>> config_;
};

// =============================================================================
// Test the implementation
// =============================================================================

void test_valid_config() {
    std::cout << "=== Testing Valid Configuration ===\n\n";

    std::string config_content = R"(
# Database configuration
db_host = localhost
db_port = 5432
db_name = myapp

# Application settings
debug = true
max_connections = 100
)";

    ConfigParser parser;
    parser.parse(config_content);

    std::cout << "Parsed configuration:\n";
    for (const auto& key : parser.keys()) {
        std::cout << "  " << key << " = " << parser.get(key) << '\n';
    }

    std::cout << "\nTyped access:\n";
    std::cout << "  db_port (int): " << parser.get_int("db_port") << '\n';
    std::cout << "  debug (bool): " << std::boolalpha << parser.get_bool("debug") << '\n';

    std::cout << '\n';
}

void test_missing_key() {
    std::cout << "=== Testing Missing Key ===\n\n";

    std::string config_content = "name = test\n";

    ConfigParser parser;
    parser.parse(config_content);

    try {
        std::string value = parser.get("nonexistent");
        std::cout << "ERROR: Should have thrown MissingKeyError\n";
    }
    catch (const MissingKeyError& e) {
        std::cout << "Caught MissingKeyError: " << e.what() << '\n';
        std::cout << "Missing key: " << e.key() << '\n';
    }

    // Test optional access
    auto opt = parser.get_optional("nonexistent");
    std::cout << "Optional access returned: "
              << (opt.has_value() ? *opt : "(none)") << '\n';

    std::cout << '\n';
}

void test_syntax_error() {
    std::cout << "=== Testing Syntax Errors ===\n\n";

    // Missing equals sign
    try {
        ConfigParser parser;
        parser.parse("valid = good\nno_equals_here\n");
    }
    catch (const SyntaxError& e) {
        std::cout << "Caught SyntaxError: " << e.what() << '\n';
        std::cout << "Line number: " << e.line_number() << '\n';
    }

    // Invalid key character
    try {
        ConfigParser parser;
        parser.parse("valid-key = value\n");  // hyphen not allowed
    }
    catch (const SyntaxError& e) {
        std::cout << "Caught SyntaxError: " << e.what() << '\n';
    }

    std::cout << '\n';
}

void test_invalid_values() {
    std::cout << "=== Testing Invalid Values ===\n\n";

    std::string config_content = R"(
port = not_a_number
enabled = maybe
count = 42
)";

    ConfigParser parser;
    parser.parse(config_content);

    // Invalid integer
    try {
        parser.get_int("port");
    }
    catch (const InvalidValueError& e) {
        std::cout << "Caught InvalidValueError: " << e.what() << '\n';
    }

    // Invalid boolean
    try {
        parser.get_bool("enabled");
    }
    catch (const InvalidValueError& e) {
        std::cout << "Caught InvalidValueError: " << e.what() << '\n';
    }

    // Valid integer
    std::cout << "count (valid): " << parser.get_int("count") << '\n';

    std::cout << '\n';
}

void test_exception_hierarchy() {
    std::cout << "=== Testing Exception Hierarchy ===\n\n";

    std::vector<std::function<void()>> tests = {
        []() {
            ConfigParser p;
            p.parse("bad line no equals");
        },
        []() {
            ConfigParser p;
            p.parse("key = value");
            p.get("missing");
        },
        []() {
            ConfigParser p;
            p.parse("num = abc");
            p.get_int("num");
        }
    };

    for (size_t i = 0; i < tests.size(); ++i) {
        std::cout << "Test " << (i + 1) << ": ";
        try {
            tests[i]();
            std::cout << "No exception (unexpected)\n";
        }
        catch (const SyntaxError& e) {
            std::cout << "SyntaxError\n";
        }
        catch (const MissingKeyError& e) {
            std::cout << "MissingKeyError\n";
        }
        catch (const InvalidValueError& e) {
            std::cout << "InvalidValueError\n";
        }
        catch (const ConfigError& e) {
            std::cout << "ConfigError (base)\n";
        }
        catch (const std::exception& e) {
            std::cout << "std::exception\n";
        }
    }

    std::cout << '\n';
}

int main() {
    std::cout << "Exercise 01: Exception Handling\n";
    std::cout << "===============================\n\n";

    test_valid_config();
    test_missing_key();
    test_syntax_error();
    test_invalid_values();
    test_exception_hierarchy();

    std::cout << "All tests completed.\n";
    return 0;
}
