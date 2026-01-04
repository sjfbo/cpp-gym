#ifndef JSON_PARSER_H
#define JSON_PARSER_H

#include "json_value.h"
#include <string>
#include <string_view>
#include <stdexcept>

namespace json {

/**
 * Exception thrown when JSON parsing fails.
 */
class ParseError : public std::runtime_error {
public:
    ParseError(const std::string& message, size_t position)
        : std::runtime_error("JSON parse error at position " +
                             std::to_string(position) + ": " + message)
        , position_(position)
    {}

    [[nodiscard]] size_t position() const noexcept { return position_; }

private:
    size_t position_;
};

/**
 * Simple recursive-descent JSON parser.
 */
class Parser {
public:
    /**
     * Parse a JSON string into a JsonValue.
     * @param input The JSON string to parse
     * @return The parsed JsonValue
     * @throws ParseError if parsing fails
     */
    [[nodiscard]] static JsonValue parse(std::string_view input);

    /**
     * Try to parse a JSON string, returning a default value on failure.
     * @param input The JSON string to parse
     * @param default_value Value to return on parse failure
     * @return The parsed JsonValue or default_value
     */
    [[nodiscard]] static JsonValue try_parse(std::string_view input,
                                              JsonValue default_value = JsonValue());

private:
    std::string_view input_;
    size_t pos_;

    explicit Parser(std::string_view input);

    // Parsing methods
    [[nodiscard]] JsonValue parse_value();
    [[nodiscard]] JsonValue parse_null();
    [[nodiscard]] JsonValue parse_bool();
    [[nodiscard]] JsonValue parse_number();
    [[nodiscard]] JsonValue parse_string();
    [[nodiscard]] JsonValue parse_array();
    [[nodiscard]] JsonValue parse_object();

    // Helper methods
    void skip_whitespace();
    [[nodiscard]] char peek() const;
    [[nodiscard]] char advance();
    [[nodiscard]] bool at_end() const;
    void expect(char c);
    [[nodiscard]] bool match(std::string_view str);

    // Error reporting
    [[noreturn]] void error(const std::string& message);
};

/**
 * Convenience function to parse JSON.
 */
[[nodiscard]] inline JsonValue parse(std::string_view input) {
    return Parser::parse(input);
}

} // namespace json

#endif // JSON_PARSER_H
