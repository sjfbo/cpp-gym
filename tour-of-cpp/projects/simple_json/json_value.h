#ifndef JSON_VALUE_H
#define JSON_VALUE_H

#include <variant>
#include <string>
#include <vector>
#include <map>
#include <stdexcept>
#include <type_traits>

namespace json {

// Forward declaration for recursive types
struct JsonValue;

// Type aliases for JSON compound types
using JsonArray = std::vector<JsonValue>;
using JsonObject = std::map<std::string, JsonValue>;

/**
 * Represents a JSON value using std::variant.
 *
 * Supports all JSON types:
 * - null (std::monostate)
 * - boolean (bool)
 * - number (double)
 * - string (std::string)
 * - array (std::vector<JsonValue>)
 * - object (std::map<std::string, JsonValue>)
 */
struct JsonValue {
    // The variant holding the actual value
    using Variant = std::variant<
        std::monostate,  // null
        bool,            // boolean
        double,          // number
        std::string,     // string
        JsonArray,       // array
        JsonObject       // object
    >;

    Variant data;

    // =========================================================================
    // Constructors
    // =========================================================================

    // Default constructs null
    JsonValue() : data(std::monostate{}) {}

    // Construct from null
    JsonValue(std::nullptr_t) : data(std::monostate{}) {}

    // Construct from boolean
    JsonValue(bool value) : data(value) {}

    // Construct from number types
    JsonValue(int value) : data(static_cast<double>(value)) {}
    JsonValue(double value) : data(value) {}

    // Construct from string
    JsonValue(const std::string& value) : data(value) {}
    JsonValue(std::string&& value) : data(std::move(value)) {}
    JsonValue(const char* value) : data(std::string(value)) {}

    // Construct from array
    JsonValue(const JsonArray& value) : data(value) {}
    JsonValue(JsonArray&& value) : data(std::move(value)) {}

    // Construct from object
    JsonValue(const JsonObject& value) : data(value) {}
    JsonValue(JsonObject&& value) : data(std::move(value)) {}

    // Construct from initializer list (for arrays)
    JsonValue(std::initializer_list<JsonValue> init) : data(JsonArray(init)) {}

    // =========================================================================
    // Type Checking
    // =========================================================================

    [[nodiscard]] bool is_null() const {
        return std::holds_alternative<std::monostate>(data);
    }

    [[nodiscard]] bool is_bool() const {
        return std::holds_alternative<bool>(data);
    }

    [[nodiscard]] bool is_number() const {
        return std::holds_alternative<double>(data);
    }

    [[nodiscard]] bool is_string() const {
        return std::holds_alternative<std::string>(data);
    }

    [[nodiscard]] bool is_array() const {
        return std::holds_alternative<JsonArray>(data);
    }

    [[nodiscard]] bool is_object() const {
        return std::holds_alternative<JsonObject>(data);
    }

    // =========================================================================
    // Value Access (throwing)
    // =========================================================================

    [[nodiscard]] bool as_bool() const {
        if (auto* p = std::get_if<bool>(&data)) {
            return *p;
        }
        throw std::runtime_error("JSON value is not a boolean");
    }

    [[nodiscard]] double as_number() const {
        if (auto* p = std::get_if<double>(&data)) {
            return *p;
        }
        throw std::runtime_error("JSON value is not a number");
    }

    [[nodiscard]] const std::string& as_string() const {
        if (auto* p = std::get_if<std::string>(&data)) {
            return *p;
        }
        throw std::runtime_error("JSON value is not a string");
    }

    [[nodiscard]] std::string& as_string() {
        if (auto* p = std::get_if<std::string>(&data)) {
            return *p;
        }
        throw std::runtime_error("JSON value is not a string");
    }

    [[nodiscard]] const JsonArray& as_array() const {
        if (auto* p = std::get_if<JsonArray>(&data)) {
            return *p;
        }
        throw std::runtime_error("JSON value is not an array");
    }

    [[nodiscard]] JsonArray& as_array() {
        if (auto* p = std::get_if<JsonArray>(&data)) {
            return *p;
        }
        throw std::runtime_error("JSON value is not an array");
    }

    [[nodiscard]] const JsonObject& as_object() const {
        if (auto* p = std::get_if<JsonObject>(&data)) {
            return *p;
        }
        throw std::runtime_error("JSON value is not an object");
    }

    [[nodiscard]] JsonObject& as_object() {
        if (auto* p = std::get_if<JsonObject>(&data)) {
            return *p;
        }
        throw std::runtime_error("JSON value is not an object");
    }

    // =========================================================================
    // Convenience Operators
    // =========================================================================

    /**
     * Array subscript access.
     */
    [[nodiscard]] const JsonValue& operator[](size_t index) const {
        return as_array().at(index);
    }

    [[nodiscard]] JsonValue& operator[](size_t index) {
        return as_array().at(index);
    }

    /**
     * Object key access.
     */
    [[nodiscard]] const JsonValue& operator[](const std::string& key) const {
        const auto& obj = as_object();
        auto it = obj.find(key);
        if (it == obj.end()) {
            throw std::runtime_error("Key not found: " + key);
        }
        return it->second;
    }

    [[nodiscard]] JsonValue& operator[](const std::string& key) {
        auto& obj = as_object();
        auto it = obj.find(key);
        if (it == obj.end()) {
            throw std::runtime_error("Key not found: " + key);
        }
        return it->second;
    }

    /**
     * Check if object contains key.
     */
    [[nodiscard]] bool contains(const std::string& key) const {
        if (!is_object()) return false;
        const auto& obj = std::get<JsonObject>(data);
        return obj.find(key) != obj.end();
    }

    /**
     * Get array size or object size.
     */
    [[nodiscard]] size_t size() const {
        if (is_array()) {
            return std::get<JsonArray>(data).size();
        }
        if (is_object()) {
            return std::get<JsonObject>(data).size();
        }
        throw std::runtime_error("JSON value is not an array or object");
    }

    // =========================================================================
    // Comparison
    // =========================================================================

    bool operator==(const JsonValue& other) const {
        return data == other.data;
    }

    bool operator!=(const JsonValue& other) const {
        return !(*this == other);
    }

    // =========================================================================
    // Type Name (for debugging)
    // =========================================================================

    [[nodiscard]] std::string type_name() const {
        return std::visit([](auto&& arg) -> std::string {
            using T = std::decay_t<decltype(arg)>;
            if constexpr (std::is_same_v<T, std::monostate>) {
                return "null";
            } else if constexpr (std::is_same_v<T, bool>) {
                return "boolean";
            } else if constexpr (std::is_same_v<T, double>) {
                return "number";
            } else if constexpr (std::is_same_v<T, std::string>) {
                return "string";
            } else if constexpr (std::is_same_v<T, JsonArray>) {
                return "array";
            } else if constexpr (std::is_same_v<T, JsonObject>) {
                return "object";
            }
            return "unknown";
        }, data);
    }
};

} // namespace json

#endif // JSON_VALUE_H
