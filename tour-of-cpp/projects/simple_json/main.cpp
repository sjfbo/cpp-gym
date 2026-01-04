#include "json_parser.h"
#include <iostream>
#include <iomanip>

/**
 * Demonstrates the simple_json library.
 */

// Forward declaration for recursive printing
void print_json(const json::JsonValue& value, int indent = 0);

void print_indent(int level) {
    for (int i = 0; i < level; ++i) {
        std::cout << "  ";
    }
}

void print_json(const json::JsonValue& value, int indent) {
    std::visit([indent](auto&& arg) {
        using T = std::decay_t<decltype(arg)>;

        if constexpr (std::is_same_v<T, std::monostate>) {
            std::cout << "null";
        } else if constexpr (std::is_same_v<T, bool>) {
            std::cout << (arg ? "true" : "false");
        } else if constexpr (std::is_same_v<T, double>) {
            std::cout << arg;
        } else if constexpr (std::is_same_v<T, std::string>) {
            std::cout << '"' << arg << '"';
        } else if constexpr (std::is_same_v<T, json::JsonArray>) {
            if (arg.empty()) {
                std::cout << "[]";
            } else {
                std::cout << "[\n";
                for (size_t i = 0; i < arg.size(); ++i) {
                    print_indent(indent + 1);
                    print_json(arg[i], indent + 1);
                    if (i < arg.size() - 1) std::cout << ",";
                    std::cout << "\n";
                }
                print_indent(indent);
                std::cout << "]";
            }
        } else if constexpr (std::is_same_v<T, json::JsonObject>) {
            if (arg.empty()) {
                std::cout << "{}";
            } else {
                std::cout << "{\n";
                size_t i = 0;
                for (const auto& [key, val] : arg) {
                    print_indent(indent + 1);
                    std::cout << '"' << key << "\": ";
                    print_json(val, indent + 1);
                    if (i < arg.size() - 1) std::cout << ",";
                    std::cout << "\n";
                    ++i;
                }
                print_indent(indent);
                std::cout << "}";
            }
        }
    }, value.data);
}

int main() {
    std::cout << "=== Simple JSON Demo ===\n\n";

    // 1. Parse simple values
    std::cout << "1. Parsing simple values:\n";

    auto null_val = json::parse("null");
    std::cout << "   null -> type: " << null_val.type_name()
              << ", is_null: " << std::boolalpha << null_val.is_null() << "\n";

    auto bool_val = json::parse("true");
    std::cout << "   true -> type: " << bool_val.type_name()
              << ", value: " << bool_val.as_bool() << "\n";

    auto num_val = json::parse("42.5");
    std::cout << "   42.5 -> type: " << num_val.type_name()
              << ", value: " << num_val.as_number() << "\n";

    auto str_val = json::parse("\"hello world\"");
    std::cout << "   \"hello world\" -> type: " << str_val.type_name()
              << ", value: " << str_val.as_string() << "\n";

    // 2. Parse arrays
    std::cout << "\n2. Parsing arrays:\n";

    auto array_val = json::parse("[1, 2, 3, \"four\", true, null]");
    std::cout << "   ";
    print_json(array_val);
    std::cout << "\n";
    std::cout << "   Array size: " << array_val.size() << "\n";
    std::cout << "   array[3]: " << array_val[3].as_string() << "\n";

    // 3. Parse objects
    std::cout << "\n3. Parsing objects:\n";

    auto obj_val = json::parse(R"({
        "name": "Alice",
        "age": 30,
        "active": true,
        "email": null
    })");
    std::cout << "   ";
    print_json(obj_val);
    std::cout << "\n";
    std::cout << "   Object size: " << obj_val.size() << "\n";
    std::cout << "   obj[\"name\"]: " << obj_val["name"].as_string() << "\n";
    std::cout << "   obj[\"age\"]: " << obj_val["age"].as_number() << "\n";

    // 4. Parse nested structures
    std::cout << "\n4. Parsing nested structures:\n";

    const char* complex_json = R"({
        "users": [
            {"id": 1, "name": "Alice", "roles": ["admin", "user"]},
            {"id": 2, "name": "Bob", "roles": ["user"]}
        ],
        "metadata": {
            "version": "1.0",
            "count": 2
        }
    })";

    auto complex_val = json::parse(complex_json);
    print_json(complex_val);
    std::cout << "\n\n";

    // Access nested values
    std::cout << "   Accessing nested values:\n";
    std::cout << "   users[0].name: " << complex_val["users"][0]["name"].as_string() << "\n";
    std::cout << "   users[1].roles[0]: " << complex_val["users"][1]["roles"][0].as_string() << "\n";
    std::cout << "   metadata.version: " << complex_val["metadata"]["version"].as_string() << "\n";

    // 5. Construct JSON programmatically
    std::cout << "\n5. Constructing JSON programmatically:\n";

    json::JsonValue constructed = json::JsonObject{
        {"title", "A Tour of C++"},
        {"author", "Bjarne Stroustrup"},
        {"year", 2022},
        {"topics", json::JsonArray{"classes", "templates", "concepts", "modules"}},
        {"available", true}
    };
    print_json(constructed);
    std::cout << "\n";

    // 6. Type checking and safe access
    std::cout << "\n6. Type checking:\n";

    json::JsonValue mixed = json::parse("[1, \"two\", true, null, {\"key\": \"value\"}]");
    for (size_t i = 0; i < mixed.size(); ++i) {
        const auto& elem = mixed[i];
        std::cout << "   [" << i << "] type: " << elem.type_name() << "\n";
    }

    // 7. Error handling
    std::cout << "\n7. Error handling:\n";

    try {
        json::parse("{invalid json}");
    } catch (const json::ParseError& e) {
        std::cout << "   Caught error: " << e.what() << "\n";
    }

    // Using try_parse for safer parsing
    auto safe_val = json::Parser::try_parse("not valid", json::JsonValue("default"));
    std::cout << "   try_parse with invalid input: " << safe_val.as_string() << "\n";

    // 8. Using std::visit
    std::cout << "\n8. Using std::visit for type dispatch:\n";

    json::JsonValue value = json::parse("[42, \"hello\", true]");
    for (size_t i = 0; i < value.size(); ++i) {
        std::cout << "   [" << i << "]: ";
        std::visit([](auto&& arg) {
            using T = std::decay_t<decltype(arg)>;
            if constexpr (std::is_same_v<T, std::monostate>) {
                std::cout << "null value";
            } else if constexpr (std::is_same_v<T, bool>) {
                std::cout << "boolean = " << std::boolalpha << arg;
            } else if constexpr (std::is_same_v<T, double>) {
                std::cout << "number = " << arg;
            } else if constexpr (std::is_same_v<T, std::string>) {
                std::cout << "string = \"" << arg << "\"";
            } else if constexpr (std::is_same_v<T, json::JsonArray>) {
                std::cout << "array with " << arg.size() << " elements";
            } else if constexpr (std::is_same_v<T, json::JsonObject>) {
                std::cout << "object with " << arg.size() << " keys";
            }
        }, value[i].data);
        std::cout << "\n";
    }

    // 9. Number parsing edge cases
    std::cout << "\n9. Number formats:\n";
    std::cout << "   -42: " << json::parse("-42").as_number() << "\n";
    std::cout << "   3.14159: " << json::parse("3.14159").as_number() << "\n";
    std::cout << "   1e10: " << json::parse("1e10").as_number() << "\n";
    std::cout << "   2.5e-3: " << json::parse("2.5e-3").as_number() << "\n";

    // 10. String escapes
    std::cout << "\n10. String escapes:\n";
    auto escaped = json::parse(R"("line1\nline2\ttabbed")");
    std::cout << "   Parsed: " << escaped.as_string() << "\n";

    std::cout << "\n=== Demo Complete ===\n";
    return 0;
}
