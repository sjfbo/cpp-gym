#include "json_parser.h"
#include <cctype>
#include <charconv>
#include <cmath>

namespace json {

// =============================================================================
// Static Methods
// =============================================================================

JsonValue Parser::parse(std::string_view input) {
    Parser parser(input);
    parser.skip_whitespace();

    if (parser.at_end()) {
        parser.error("Empty input");
    }

    JsonValue result = parser.parse_value();

    parser.skip_whitespace();
    if (!parser.at_end()) {
        parser.error("Unexpected characters after JSON value");
    }

    return result;
}

JsonValue Parser::try_parse(std::string_view input, JsonValue default_value) {
    try {
        return parse(input);
    } catch (const ParseError&) {
        return default_value;
    }
}

// =============================================================================
// Constructor
// =============================================================================

Parser::Parser(std::string_view input)
    : input_(input)
    , pos_(0)
{}

// =============================================================================
// Value Parsing
// =============================================================================

JsonValue Parser::parse_value() {
    skip_whitespace();

    if (at_end()) {
        error("Unexpected end of input");
    }

    char c = peek();

    switch (c) {
        case 'n':
            return parse_null();
        case 't':
        case 'f':
            return parse_bool();
        case '"':
            return parse_string();
        case '[':
            return parse_array();
        case '{':
            return parse_object();
        case '-':
        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9':
            return parse_number();
        default:
            error("Unexpected character '" + std::string(1, c) + "'");
    }
}

JsonValue Parser::parse_null() {
    if (match("null")) {
        return JsonValue(nullptr);
    }
    error("Expected 'null'");
}

JsonValue Parser::parse_bool() {
    if (match("true")) {
        return JsonValue(true);
    }
    if (match("false")) {
        return JsonValue(false);
    }
    error("Expected 'true' or 'false'");
}

JsonValue Parser::parse_number() {
    size_t start = pos_;

    // Optional minus
    if (peek() == '-') {
        (void)advance();
    }

    // Integer part
    if (peek() == '0') {
        (void)advance();
    } else if (std::isdigit(peek())) {
        while (!at_end() && std::isdigit(peek())) {
            (void)advance();
        }
    } else {
        error("Invalid number");
    }

    // Fractional part
    if (!at_end() && peek() == '.') {
        (void)advance();
        if (at_end() || !std::isdigit(peek())) {
            error("Expected digit after decimal point");
        }
        while (!at_end() && std::isdigit(peek())) {
            (void)advance();
        }
    }

    // Exponent part
    if (!at_end() && (peek() == 'e' || peek() == 'E')) {
        (void)advance();
        if (!at_end() && (peek() == '+' || peek() == '-')) {
            (void)advance();
        }
        if (at_end() || !std::isdigit(peek())) {
            error("Expected digit in exponent");
        }
        while (!at_end() && std::isdigit(peek())) {
            (void)advance();
        }
    }

    std::string_view num_str = input_.substr(start, pos_ - start);

    // Parse the number
    double value = 0.0;
    auto result = std::from_chars(num_str.data(), num_str.data() + num_str.size(), value);

    if (result.ec != std::errc()) {
        error("Invalid number format");
    }

    return JsonValue(value);
}

JsonValue Parser::parse_string() {
    expect('"');

    std::string result;

    while (!at_end() && peek() != '"') {
        char c = advance();

        if (c == '\\') {
            if (at_end()) {
                error("Unexpected end of string");
            }

            char escaped = advance();
            switch (escaped) {
                case '"':  result += '"';  break;
                case '\\': result += '\\'; break;
                case '/':  result += '/';  break;
                case 'b':  result += '\b'; break;
                case 'f':  result += '\f'; break;
                case 'n':  result += '\n'; break;
                case 'r':  result += '\r'; break;
                case 't':  result += '\t'; break;
                case 'u':
                    // Basic Unicode escape (simplified - just skip 4 hex digits)
                    // A full implementation would convert to UTF-8
                    for (int i = 0; i < 4; ++i) {
                        if (at_end() || !std::isxdigit(peek())) {
                            error("Invalid Unicode escape sequence");
                        }
                        (void)advance();
                    }
                    result += '?';  // Placeholder for unsupported Unicode
                    break;
                default:
                    error("Invalid escape sequence '\\" + std::string(1, escaped) + "'");
            }
        } else if (static_cast<unsigned char>(c) < 0x20) {
            error("Control character in string");
        } else {
            result += c;
        }
    }

    if (at_end()) {
        error("Unterminated string");
    }

    expect('"');

    return JsonValue(std::move(result));
}

JsonValue Parser::parse_array() {
    expect('[');
    skip_whitespace();

    JsonArray result;

    if (peek() != ']') {
        // Parse first element
        result.push_back(parse_value());

        // Parse remaining elements
        skip_whitespace();
        while (peek() == ',') {
            (void)advance();
            result.push_back(parse_value());
            skip_whitespace();
        }
    }

    expect(']');

    return JsonValue(std::move(result));
}

JsonValue Parser::parse_object() {
    expect('{');
    skip_whitespace();

    JsonObject result;

    if (peek() != '}') {
        // Parse first key-value pair
        if (peek() != '"') {
            error("Expected string key");
        }
        std::string key = parse_string().as_string();

        skip_whitespace();
        expect(':');

        result[key] = parse_value();

        // Parse remaining pairs
        skip_whitespace();
        while (peek() == ',') {
            (void)advance();
            skip_whitespace();

            if (peek() != '"') {
                error("Expected string key");
            }
            key = parse_string().as_string();

            skip_whitespace();
            expect(':');

            result[key] = parse_value();
            skip_whitespace();
        }
    }

    expect('}');

    return JsonValue(std::move(result));
}

// =============================================================================
// Helper Methods
// =============================================================================

void Parser::skip_whitespace() {
    while (!at_end() && std::isspace(static_cast<unsigned char>(peek()))) {
        (void)advance();
    }
}

char Parser::peek() const {
    if (at_end()) {
        return '\0';
    }
    return input_[pos_];
}

char Parser::advance() {
    if (at_end()) {
        error("Unexpected end of input");
    }
    return input_[pos_++];
}

bool Parser::at_end() const {
    return pos_ >= input_.size();
}

void Parser::expect(char c) {
    if (at_end() || peek() != c) {
        error("Expected '" + std::string(1, c) + "'");
    }
    (void)advance();
}

bool Parser::match(std::string_view str) {
    if (pos_ + str.size() > input_.size()) {
        return false;
    }

    if (input_.substr(pos_, str.size()) == str) {
        pos_ += str.size();
        return true;
    }

    return false;
}

void Parser::error(const std::string& message) {
    throw ParseError(message, pos_);
}

} // namespace json
