#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include "json_parser.h"

using namespace json;
using Catch::Matchers::WithinRel;

// =============================================================================
// JsonValue Tests
// =============================================================================

TEST_CASE("JsonValue default construction", "[json][value]") {
    JsonValue v;
    REQUIRE(v.is_null());
    REQUIRE(v.type_name() == "null");
}

TEST_CASE("JsonValue null construction", "[json][value]") {
    JsonValue v(nullptr);
    REQUIRE(v.is_null());
}

TEST_CASE("JsonValue boolean construction", "[json][value]") {
    JsonValue t(true);
    JsonValue f(false);

    REQUIRE(t.is_bool());
    REQUIRE(t.as_bool() == true);
    REQUIRE(t.type_name() == "boolean");

    REQUIRE(f.is_bool());
    REQUIRE(f.as_bool() == false);
}

TEST_CASE("JsonValue number construction", "[json][value]") {
    JsonValue d(42.5);
    JsonValue i(42);

    REQUIRE(d.is_number());
    REQUIRE_THAT(d.as_number(), WithinRel(42.5, 1e-10));

    REQUIRE(i.is_number());
    REQUIRE_THAT(i.as_number(), WithinRel(42.0, 1e-10));
}

TEST_CASE("JsonValue string construction", "[json][value]") {
    JsonValue s1("hello");
    JsonValue s2(std::string("world"));

    REQUIRE(s1.is_string());
    REQUIRE(s1.as_string() == "hello");
    REQUIRE(s1.type_name() == "string");

    REQUIRE(s2.is_string());
    REQUIRE(s2.as_string() == "world");
}

TEST_CASE("JsonValue array construction", "[json][value]") {
    JsonArray arr{JsonValue(1), JsonValue(2), JsonValue(3)};
    JsonValue v(arr);

    REQUIRE(v.is_array());
    REQUIRE(v.size() == 3);
    REQUIRE(v.type_name() == "array");
}

TEST_CASE("JsonValue object construction", "[json][value]") {
    JsonObject obj{{"key", JsonValue("value")}};
    JsonValue v(obj);

    REQUIRE(v.is_object());
    REQUIRE(v.size() == 1);
    REQUIRE(v.type_name() == "object");
}

TEST_CASE("JsonValue type checking throws on wrong type", "[json][value]") {
    JsonValue str("hello");

    REQUIRE_THROWS_AS(str.as_bool(), std::runtime_error);
    REQUIRE_THROWS_AS(str.as_number(), std::runtime_error);
    REQUIRE_THROWS_AS(str.as_array(), std::runtime_error);
    REQUIRE_THROWS_AS(str.as_object(), std::runtime_error);
}

TEST_CASE("JsonValue array access", "[json][value]") {
    JsonValue arr = JsonArray{JsonValue(10), JsonValue(20), JsonValue(30)};

    REQUIRE_THAT(arr[0].as_number(), WithinRel(10.0, 1e-10));
    REQUIRE_THAT(arr[1].as_number(), WithinRel(20.0, 1e-10));
    REQUIRE_THAT(arr[2].as_number(), WithinRel(30.0, 1e-10));

    REQUIRE_THROWS_AS(arr[3], std::out_of_range);
}

TEST_CASE("JsonValue object access", "[json][value]") {
    JsonValue obj = JsonObject{
        {"name", JsonValue("Alice")},
        {"age", JsonValue(30)}
    };

    REQUIRE(obj["name"].as_string() == "Alice");
    REQUIRE_THAT(obj["age"].as_number(), WithinRel(30.0, 1e-10));

    REQUIRE(obj.contains("name"));
    REQUIRE_FALSE(obj.contains("missing"));

    REQUIRE_THROWS(obj["missing"]);
}

TEST_CASE("JsonValue comparison", "[json][value]") {
    REQUIRE(JsonValue(42) == JsonValue(42));
    REQUIRE(JsonValue("hello") == JsonValue("hello"));
    REQUIRE(JsonValue(true) == JsonValue(true));
    REQUIRE(JsonValue(nullptr) == JsonValue(nullptr));

    REQUIRE(JsonValue(42) != JsonValue(43));
    REQUIRE(JsonValue("hello") != JsonValue("world"));
    REQUIRE(JsonValue(true) != JsonValue(false));
}

// =============================================================================
// Parser Tests - Simple Values
// =============================================================================

TEST_CASE("Parse null", "[json][parser]") {
    auto v = parse("null");
    REQUIRE(v.is_null());
}

TEST_CASE("Parse boolean", "[json][parser]") {
    REQUIRE(parse("true").as_bool() == true);
    REQUIRE(parse("false").as_bool() == false);
}

TEST_CASE("Parse integers", "[json][parser]") {
    REQUIRE_THAT(parse("0").as_number(), WithinRel(0.0, 1e-10));
    REQUIRE_THAT(parse("42").as_number(), WithinRel(42.0, 1e-10));
    REQUIRE_THAT(parse("-42").as_number(), WithinRel(-42.0, 1e-10));
    REQUIRE_THAT(parse("123456789").as_number(), WithinRel(123456789.0, 1e-10));
}

TEST_CASE("Parse floating point", "[json][parser]") {
    REQUIRE_THAT(parse("3.14").as_number(), WithinRel(3.14, 1e-10));
    REQUIRE_THAT(parse("-0.5").as_number(), WithinRel(-0.5, 1e-10));
    REQUIRE_THAT(parse("0.123").as_number(), WithinRel(0.123, 1e-10));
}

TEST_CASE("Parse scientific notation", "[json][parser]") {
    REQUIRE_THAT(parse("1e10").as_number(), WithinRel(1e10, 1e-5));
    REQUIRE_THAT(parse("1E10").as_number(), WithinRel(1e10, 1e-5));
    REQUIRE_THAT(parse("1e+10").as_number(), WithinRel(1e10, 1e-5));
    REQUIRE_THAT(parse("1e-10").as_number(), WithinRel(1e-10, 1e-20));
    REQUIRE_THAT(parse("2.5e3").as_number(), WithinRel(2500.0, 1e-10));
}

TEST_CASE("Parse strings", "[json][parser]") {
    REQUIRE(parse("\"\"").as_string() == "");
    REQUIRE(parse("\"hello\"").as_string() == "hello");
    REQUIRE(parse("\"hello world\"").as_string() == "hello world");
}

TEST_CASE("Parse string escapes", "[json][parser]") {
    REQUIRE(parse("\"\\\"\"").as_string() == "\"");
    REQUIRE(parse("\"\\\\\"").as_string() == "\\");
    REQUIRE(parse("\"\\/\"").as_string() == "/");
    REQUIRE(parse("\"\\b\"").as_string() == "\b");
    REQUIRE(parse("\"\\f\"").as_string() == "\f");
    REQUIRE(parse("\"\\n\"").as_string() == "\n");
    REQUIRE(parse("\"\\r\"").as_string() == "\r");
    REQUIRE(parse("\"\\t\"").as_string() == "\t");
}

TEST_CASE("Parse with whitespace", "[json][parser]") {
    REQUIRE(parse("  null  ").is_null());
    REQUIRE(parse("\n\ttrue\n").as_bool() == true);
    REQUIRE_THAT(parse("  42  ").as_number(), WithinRel(42.0, 1e-10));
}

// =============================================================================
// Parser Tests - Arrays
// =============================================================================

TEST_CASE("Parse empty array", "[json][parser]") {
    auto v = parse("[]");
    REQUIRE(v.is_array());
    REQUIRE(v.size() == 0);
}

TEST_CASE("Parse simple array", "[json][parser]") {
    auto v = parse("[1, 2, 3]");
    REQUIRE(v.is_array());
    REQUIRE(v.size() == 3);
    REQUIRE_THAT(v[0].as_number(), WithinRel(1.0, 1e-10));
    REQUIRE_THAT(v[1].as_number(), WithinRel(2.0, 1e-10));
    REQUIRE_THAT(v[2].as_number(), WithinRel(3.0, 1e-10));
}

TEST_CASE("Parse mixed array", "[json][parser]") {
    auto v = parse("[null, true, 42, \"hello\"]");
    REQUIRE(v.size() == 4);
    REQUIRE(v[0].is_null());
    REQUIRE(v[1].as_bool() == true);
    REQUIRE_THAT(v[2].as_number(), WithinRel(42.0, 1e-10));
    REQUIRE(v[3].as_string() == "hello");
}

TEST_CASE("Parse nested arrays", "[json][parser]") {
    auto v = parse("[[1, 2], [3, 4]]");
    REQUIRE(v.size() == 2);
    REQUIRE(v[0].size() == 2);
    REQUIRE_THAT(v[0][0].as_number(), WithinRel(1.0, 1e-10));
    REQUIRE_THAT(v[1][1].as_number(), WithinRel(4.0, 1e-10));
}

// =============================================================================
// Parser Tests - Objects
// =============================================================================

TEST_CASE("Parse empty object", "[json][parser]") {
    auto v = parse("{}");
    REQUIRE(v.is_object());
    REQUIRE(v.size() == 0);
}

TEST_CASE("Parse simple object", "[json][parser]") {
    auto v = parse(R"({"key": "value"})");
    REQUIRE(v.is_object());
    REQUIRE(v.size() == 1);
    REQUIRE(v["key"].as_string() == "value");
}

TEST_CASE("Parse object with multiple keys", "[json][parser]") {
    auto v = parse(R"({"a": 1, "b": 2, "c": 3})");
    REQUIRE(v.size() == 3);
    REQUIRE_THAT(v["a"].as_number(), WithinRel(1.0, 1e-10));
    REQUIRE_THAT(v["b"].as_number(), WithinRel(2.0, 1e-10));
    REQUIRE_THAT(v["c"].as_number(), WithinRel(3.0, 1e-10));
}

TEST_CASE("Parse object with mixed values", "[json][parser]") {
    auto v = parse(R"({
        "null": null,
        "bool": true,
        "number": 42,
        "string": "hello"
    })");

    REQUIRE(v["null"].is_null());
    REQUIRE(v["bool"].as_bool() == true);
    REQUIRE_THAT(v["number"].as_number(), WithinRel(42.0, 1e-10));
    REQUIRE(v["string"].as_string() == "hello");
}

TEST_CASE("Parse nested objects", "[json][parser]") {
    auto v = parse(R"({
        "outer": {
            "inner": {
                "value": 42
            }
        }
    })");

    REQUIRE_THAT(v["outer"]["inner"]["value"].as_number(), WithinRel(42.0, 1e-10));
}

TEST_CASE("Parse object with array value", "[json][parser]") {
    auto v = parse(R"({"items": [1, 2, 3]})");
    REQUIRE(v["items"].is_array());
    REQUIRE(v["items"].size() == 3);
}

// =============================================================================
// Parser Tests - Complex Structures
// =============================================================================

TEST_CASE("Parse complex JSON", "[json][parser]") {
    const char* json = R"({
        "users": [
            {"id": 1, "name": "Alice", "active": true},
            {"id": 2, "name": "Bob", "active": false}
        ],
        "count": 2,
        "metadata": null
    })";

    auto v = parse(json);

    REQUIRE(v["users"].size() == 2);
    REQUIRE(v["users"][0]["name"].as_string() == "Alice");
    REQUIRE(v["users"][0]["active"].as_bool() == true);
    REQUIRE(v["users"][1]["name"].as_string() == "Bob");
    REQUIRE_THAT(v["count"].as_number(), WithinRel(2.0, 1e-10));
    REQUIRE(v["metadata"].is_null());
}

// =============================================================================
// Parser Tests - Error Cases
// =============================================================================

TEST_CASE("Parse error - empty input", "[json][parser][error]") {
    REQUIRE_THROWS_AS(parse(""), ParseError);
    REQUIRE_THROWS_AS(parse("   "), ParseError);
}

TEST_CASE("Parse error - invalid value", "[json][parser][error]") {
    REQUIRE_THROWS_AS(parse("invalid"), ParseError);
    REQUIRE_THROWS_AS(parse("undefined"), ParseError);
    REQUIRE_THROWS_AS(parse("NULL"), ParseError);  // Case sensitive
    REQUIRE_THROWS_AS(parse("True"), ParseError);  // Case sensitive
}

TEST_CASE("Parse error - unterminated string", "[json][parser][error]") {
    REQUIRE_THROWS_AS(parse("\"hello"), ParseError);
    REQUIRE_THROWS_AS(parse("\""), ParseError);
}

TEST_CASE("Parse error - invalid escape", "[json][parser][error]") {
    REQUIRE_THROWS_AS(parse("\"\\x\""), ParseError);
}

TEST_CASE("Parse error - invalid number", "[json][parser][error]") {
    REQUIRE_THROWS_AS(parse("1."), ParseError);
    REQUIRE_THROWS_AS(parse("1e"), ParseError);
    REQUIRE_THROWS_AS(parse("-"), ParseError);
}

TEST_CASE("Parse error - unterminated array", "[json][parser][error]") {
    REQUIRE_THROWS_AS(parse("[1, 2, 3"), ParseError);
    REQUIRE_THROWS_AS(parse("["), ParseError);
}

TEST_CASE("Parse error - unterminated object", "[json][parser][error]") {
    REQUIRE_THROWS_AS(parse("{\"key\": 1"), ParseError);
    REQUIRE_THROWS_AS(parse("{"), ParseError);
}

TEST_CASE("Parse error - missing colon in object", "[json][parser][error]") {
    REQUIRE_THROWS_AS(parse("{\"key\" 1}"), ParseError);
}

TEST_CASE("Parse error - trailing content", "[json][parser][error]") {
    REQUIRE_THROWS_AS(parse("null null"), ParseError);
    REQUIRE_THROWS_AS(parse("42 extra"), ParseError);
}

// =============================================================================
// try_parse Tests
// =============================================================================

TEST_CASE("try_parse returns default on error", "[json][parser]") {
    auto result = Parser::try_parse("invalid", JsonValue("default"));
    REQUIRE(result.as_string() == "default");
}

TEST_CASE("try_parse returns parsed value on success", "[json][parser]") {
    auto result = Parser::try_parse("42", JsonValue(0));
    REQUIRE_THAT(result.as_number(), WithinRel(42.0, 1e-10));
}
