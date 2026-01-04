// Chapter 10 Tests: Strings and Regular Expressions
// These tests verify understanding of string handling and regex.

#include <catch2/catch_test_macros.hpp>
#include <regex>
#include <string>
#include <string_view>
#include <vector>

// ============================================================================
// Section 10.2: Strings
// ============================================================================

TEST_CASE("Strings: construction", "[ch10][strings]") {
    std::string s1 = "Hello";
    std::string s2{"World"};
    std::string s3(5, 'x');
    std::string s4 = s1;

    REQUIRE(s1 == "Hello");
    REQUIRE(s2 == "World");
    REQUIRE(s3 == "xxxxx");
    REQUIRE(s4 == s1);
}

TEST_CASE("Strings: size operations", "[ch10][strings]") {
    std::string s = "Hello";

    REQUIRE(s.length() == 5);
    REQUIRE(s.size() == 5);
    REQUIRE_FALSE(s.empty());

    std::string empty;
    REQUIRE(empty.empty());
    REQUIRE(empty.length() == 0);
}

TEST_CASE("Strings: element access", "[ch10][strings]") {
    std::string s = "Hello";

    REQUIRE(s[0] == 'H');
    REQUIRE(s[4] == 'o');
    REQUIRE(s.at(0) == 'H');
    REQUIRE(s.front() == 'H');
    REQUIRE(s.back() == 'o');

    // at() throws on out of bounds
    REQUIRE_THROWS_AS(s.at(100), std::out_of_range);
}

TEST_CASE("Strings: modification", "[ch10][strings]") {
    std::string s = "Hello";

    s += " World";
    REQUIRE(s == "Hello World");

    s.append("!");
    REQUIRE(s == "Hello World!");

    s.push_back('!');
    REQUIRE(s == "Hello World!!");

    s.pop_back();
    REQUIRE(s == "Hello World!");

    s.insert(5, ",");
    REQUIRE(s == "Hello, World!");

    s.erase(5, 1);
    REQUIRE(s == "Hello World!");

    s.replace(6, 5, "C++");
    REQUIRE(s == "Hello C++!");
}

TEST_CASE("Strings: searching", "[ch10][strings]") {
    std::string s = "Hello, World! Hello!";

    // find - first occurrence
    REQUIRE(s.find("Hello") == 0);
    REQUIRE(s.find("World") == 7);
    REQUIRE(s.find("xyz") == std::string::npos);

    // find from position
    REQUIRE(s.find("Hello", 1) == 14);

    // rfind - last occurrence
    REQUIRE(s.rfind("Hello") == 14);
    REQUIRE(s.rfind("!") == 19);

    // find_first_of / find_last_of
    REQUIRE(s.find_first_of("aeiou") == 1);  // 'e' in Hello
    REQUIRE(s.find_last_of("!") == 19);
}

TEST_CASE("Strings: substring", "[ch10][strings]") {
    std::string s = "Hello, World!";

    REQUIRE(s.substr(0, 5) == "Hello");
    REQUIRE(s.substr(7, 5) == "World");
    REQUIRE(s.substr(7) == "World!");
}

TEST_CASE("Strings: comparison", "[ch10][strings]") {
    std::string a = "apple";
    std::string b = "banana";
    std::string c = "apple";

    REQUIRE(a == c);
    REQUIRE(a != b);
    REQUIRE(a < b);  // Lexicographic
    REQUIRE(b > a);

    // compare() returns <0, 0, or >0
    REQUIRE(a.compare(c) == 0);
    REQUIRE(a.compare(b) < 0);
    REQUIRE(b.compare(a) > 0);
}

TEST_CASE("Strings: C++20 starts_with/ends_with", "[ch10][strings]") {
    std::string s = "Hello, World!";

    REQUIRE(s.starts_with("Hello"));
    REQUIRE(s.starts_with("He"));
    REQUIRE_FALSE(s.starts_with("World"));

    REQUIRE(s.ends_with("!"));
    REQUIRE(s.ends_with("World!"));
    REQUIRE_FALSE(s.ends_with("Hello"));
}

TEST_CASE("Strings: conversions", "[ch10][strings]") {
    // Number to string
    REQUIRE(std::to_string(42) == "42");
    REQUIRE(std::to_string(-17) == "-17");

    // String to number
    REQUIRE(std::stoi("42") == 42);
    REQUIRE(std::stoi("-17") == -17);
    REQUIRE(std::stod("3.14") == 3.14);
    REQUIRE(std::stoll("9999999999") == 9999999999LL);

    // Invalid conversion throws
    REQUIRE_THROWS_AS(std::stoi("not a number"), std::invalid_argument);
}

// ============================================================================
// Section 10.3: String Views
// ============================================================================

TEST_CASE("String views: construction", "[ch10][string_view]") {
    std::string str = "Hello, World!";
    std::string_view sv1 = str;
    std::string_view sv2 = "literal";
    std::string_view sv3{str.data(), 5};

    REQUIRE(sv1 == "Hello, World!");
    REQUIRE(sv2 == "literal");
    REQUIRE(sv3 == "Hello");
}

TEST_CASE("String views: operations", "[ch10][string_view]") {
    std::string_view sv = "Hello, World!";

    REQUIRE(sv.length() == 13);
    REQUIRE(sv[0] == 'H');
    REQUIRE(sv.front() == 'H');
    REQUIRE(sv.back() == '!');
    REQUIRE(sv.substr(0, 5) == "Hello");
    REQUIRE(sv.find("World") == 7);
}

TEST_CASE("String views: remove_prefix/suffix", "[ch10][string_view]") {
    std::string_view sv = "   Hello   ";

    sv.remove_prefix(3);
    REQUIRE(sv == "Hello   ");

    sv.remove_suffix(3);
    REQUIRE(sv == "Hello");
}

TEST_CASE("String views: zero-copy substring", "[ch10][string_view]") {
    std::string str = "Hello, World!";
    std::string_view full = str;
    std::string_view hello = full.substr(0, 5);
    std::string_view world = full.substr(7, 5);

    // All point to the same underlying data
    REQUIRE(full.data() == str.data());
    REQUIRE(hello.data() == str.data());
    REQUIRE(world.data() == str.data() + 7);
}

TEST_CASE("String views: conversion to string", "[ch10][string_view]") {
    std::string_view sv = "Hello";

    // Explicit conversion to string
    std::string str{sv};
    REQUIRE(str == "Hello");

    // Modification only affects the copy
    str[0] = 'J';
    REQUIRE(str == "Jello");
    REQUIRE(sv == "Hello");
}

// ============================================================================
// Section 10.4: Regular Expressions
// ============================================================================

TEST_CASE("Regex: basic matching", "[ch10][regex]") {
    std::regex digits{R"(\d+)"};

    REQUIRE(std::regex_match("12345", digits));
    REQUIRE_FALSE(std::regex_match("abc", digits));
    REQUIRE_FALSE(std::regex_match("123abc", digits));  // Must match entire string
}

TEST_CASE("Regex: search", "[ch10][regex]") {
    std::string text = "The price is $42.99";
    std::regex price{R"(\$(\d+)\.(\d{2}))"};
    std::smatch match;

    REQUIRE(std::regex_search(text, match, price));
    REQUIRE(match[0] == "$42.99");  // Full match
    REQUIRE(match[1] == "42");      // First capture group
    REQUIRE(match[2] == "99");      // Second capture group
}

TEST_CASE("Regex: find all matches", "[ch10][regex]") {
    std::string text = "a1 b22 c333";
    std::regex digits{R"(\d+)"};

    std::vector<std::string> matches;
    auto begin = std::sregex_iterator(text.begin(), text.end(), digits);
    auto end = std::sregex_iterator();

    for (auto it = begin; it != end; ++it) {
        matches.push_back(it->str());
    }

    REQUIRE(matches.size() == 3);
    REQUIRE(matches[0] == "1");
    REQUIRE(matches[1] == "22");
    REQUIRE(matches[2] == "333");
}

TEST_CASE("Regex: replace", "[ch10][regex]") {
    std::string text = "Hello World";
    std::regex word{R"(\bWorld\b)"};

    std::string result = std::regex_replace(text, word, "C++");
    REQUIRE(result == "Hello C++");
}

TEST_CASE("Regex: replace with backreferences", "[ch10][regex]") {
    std::string text = "John Smith";
    std::regex name{R"((\w+) (\w+))"};

    std::string result = std::regex_replace(text, name, "$2, $1");
    REQUIRE(result == "Smith, John");
}

TEST_CASE("Regex: case insensitive", "[ch10][regex]") {
    std::string text = "Hello HELLO hello";
    std::regex pattern{R"(hello)", std::regex::icase};

    int count = 0;
    auto begin = std::sregex_iterator(text.begin(), text.end(), pattern);
    auto end = std::sregex_iterator();
    for (auto it = begin; it != end; ++it) {
        ++count;
    }

    REQUIRE(count == 3);
}

TEST_CASE("Regex: common patterns", "[ch10][regex]") {
    // Email (simplified)
    std::regex email{R"(\w+@\w+\.\w+)"};
    REQUIRE(std::regex_match("user@example.com", email));
    REQUIRE_FALSE(std::regex_match("invalid", email));

    // Phone number
    std::regex phone{R"(\(\d{3}\) \d{3}-\d{4})"};
    REQUIRE(std::regex_match("(123) 456-7890", phone));
    REQUIRE_FALSE(std::regex_match("1234567890", phone));

    // Date (YYYY-MM-DD)
    std::regex date{R"(\d{4}-\d{2}-\d{2})"};
    REQUIRE(std::regex_match("2024-01-15", date));
    REQUIRE_FALSE(std::regex_match("01/15/2024", date));

    // IPv4 address (simplified, doesn't validate range)
    std::regex ipv4{R"(\d{1,3}\.\d{1,3}\.\d{1,3}\.\d{1,3})"};
    REQUIRE(std::regex_match("192.168.1.1", ipv4));
    REQUIRE_FALSE(std::regex_match("192.168.1", ipv4));
}

TEST_CASE("Regex: character classes", "[ch10][regex]") {
    // Word characters
    std::regex word_chars{R"(\w+)"};
    REQUIRE(std::regex_match("hello123", word_chars));
    REQUIRE_FALSE(std::regex_match("hello world", word_chars));  // Space not word char

    // Digits
    std::regex digit{R"(\d)"};
    std::smatch m;
    std::string digit_str = "abc5def";
    REQUIRE(std::regex_search(digit_str, m, digit));
    REQUIRE(m[0] == "5");

    // Whitespace
    std::regex ws{R"(\s+)"};
    std::string ws_str = "hello world";
    REQUIRE(std::regex_search(ws_str, m, ws));
}

TEST_CASE("Regex: quantifiers", "[ch10][regex]") {
    std::regex zero_or_more{R"(a*)"};
    std::regex one_or_more{R"(a+)"};
    std::regex optional{R"(colou?r)"};
    std::regex exact{R"(\d{3})"};
    std::regex range{R"(\d{2,4})"};

    REQUIRE(std::regex_match("", zero_or_more));
    REQUIRE(std::regex_match("aaa", zero_or_more));

    REQUIRE_FALSE(std::regex_match("", one_or_more));
    REQUIRE(std::regex_match("aaa", one_or_more));

    REQUIRE(std::regex_match("color", optional));
    REQUIRE(std::regex_match("colour", optional));

    REQUIRE(std::regex_match("123", exact));
    REQUIRE_FALSE(std::regex_match("12", exact));
    REQUIRE_FALSE(std::regex_match("1234", exact));

    REQUIRE(std::regex_match("12", range));
    REQUIRE(std::regex_match("1234", range));
    REQUIRE_FALSE(std::regex_match("1", range));
    REQUIRE_FALSE(std::regex_match("12345", range));
}

TEST_CASE("Regex: anchors", "[ch10][regex]") {
    std::regex start_anchor{R"(^Hello)"};
    std::regex end_anchor{R"(World!$)"};
    std::regex both{R"(^Hello, World!$)"};

    // Use cmatch for const char* (string literals)
    std::cmatch m;
    REQUIRE(std::regex_search("Hello, World!", m, start_anchor));
    REQUIRE_FALSE(std::regex_search("Say Hello", m, start_anchor));

    REQUIRE(std::regex_search("Hello, World!", m, end_anchor));
    REQUIRE_FALSE(std::regex_search("World! indeed", m, end_anchor));

    REQUIRE(std::regex_match("Hello, World!", both));
}
