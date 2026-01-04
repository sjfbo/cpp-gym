// Exercise 15.2: Optional and Variant Practice
// Book reference: 15.4 Alternatives
// Difficulty: **
//
// Instructions:
// Implement the TODOs below to practice optional, variant, and visitor usage.
//
// Part 1: Implement safe division returning optional
// Part 2: Implement a simple expression evaluator using variant
// Part 3: Implement a Result type using variant (like Rust's Result<T, E>)
//
// Compile and run to verify your solutions.

#include <cmath>
#include <iostream>
#include <optional>
#include <string>
#include <variant>
#include <vector>
#include <cassert>

// ============================================================================
// Part 1: Safe Division with Optional
// ============================================================================

// TODO 1: Implement safe_divide
// Return nullopt if divisor is zero, otherwise return the quotient
std::optional<double> safe_divide(double dividend, double divisor) {
    // Your implementation here
    return std::nullopt;  // Replace this
}

// TODO 2: Implement safe_sqrt
// Return nullopt if value is negative, otherwise return the square root
std::optional<double> safe_sqrt(double value) {
    // Your implementation here
    return std::nullopt;  // Replace this
}

// TODO 3: Implement chain_operations
// Compute: sqrt(b^2 - 4ac) / (2a) using the safe functions above
// Return nullopt if any operation fails (division by zero or negative sqrt)
std::optional<double> quadratic_discriminant(double a, double b, double c) {
    // Your implementation here
    // Hint: Use safe_sqrt and safe_divide, checking for nullopt at each step
    return std::nullopt;  // Replace this
}

void test_part1() {
    std::cout << "=== Part 1: Optional Operations ===\n";

    // Test safe_divide
    auto result1 = safe_divide(10.0, 2.0);
    assert(result1.has_value());
    assert(*result1 == 5.0);

    auto result2 = safe_divide(10.0, 0.0);
    assert(!result2.has_value());

    // Test safe_sqrt
    auto result3 = safe_sqrt(16.0);
    assert(result3.has_value());
    assert(*result3 == 4.0);

    auto result4 = safe_sqrt(-1.0);
    assert(!result4.has_value());

    // Test quadratic_discriminant
    // For x^2 - 5x + 6 = 0: a=1, b=-5, c=6
    // discriminant = sqrt(25 - 24) / 2 = 0.5
    auto result5 = quadratic_discriminant(1.0, -5.0, 6.0);
    assert(result5.has_value());
    assert(std::abs(*result5 - 0.5) < 0.001);

    // For x^2 + x + 1 = 0: discriminant is negative
    auto result6 = quadratic_discriminant(1.0, 1.0, 1.0);
    assert(!result6.has_value());

    // Division by zero case (a = 0)
    auto result7 = quadratic_discriminant(0.0, 2.0, 1.0);
    assert(!result7.has_value());

    std::cout << "Part 1 passed!\n\n";
}

// ============================================================================
// Part 2: Expression Evaluator with Variant
// ============================================================================

// Expression types
struct Number { double value; };
struct Add { };
struct Subtract { };
struct Multiply { };
struct Divide { };

using Operator = std::variant<Add, Subtract, Multiply, Divide>;
using Token = std::variant<Number, Operator>;

// TODO 4: Implement apply_operator
// Apply the operator to left and right operands
// Return nullopt for division by zero
std::optional<double> apply_operator(const Operator& op, double left, double right) {
    // Your implementation here
    // Hint: Use std::visit with the overloaded pattern
    return std::nullopt;  // Replace this
}

// TODO 5: Implement evaluate_expression
// Evaluate a simple expression: [Number, Operator, Number]
// Return nullopt if evaluation fails
std::optional<double> evaluate_expression(
    const std::vector<Token>& tokens) {
    // Your implementation here
    // Expect exactly 3 tokens: Number, Operator, Number
    // Hint: Use std::get_if to extract values safely
    return std::nullopt;  // Replace this
}

void test_part2() {
    std::cout << "=== Part 2: Expression Evaluator ===\n";

    // Test apply_operator
    assert(apply_operator(Add{}, 3.0, 4.0) == 7.0);
    assert(apply_operator(Subtract{}, 10.0, 3.0) == 7.0);
    assert(apply_operator(Multiply{}, 3.0, 4.0) == 12.0);
    assert(apply_operator(Divide{}, 10.0, 2.0) == 5.0);
    assert(!apply_operator(Divide{}, 10.0, 0.0).has_value());

    // Test evaluate_expression
    std::vector<Token> expr1 = {Number{5.0}, Operator{Add{}}, Number{3.0}};
    assert(evaluate_expression(expr1) == 8.0);

    std::vector<Token> expr2 = {Number{10.0}, Operator{Divide{}}, Number{0.0}};
    assert(!evaluate_expression(expr2).has_value());

    std::vector<Token> expr3 = {Number{6.0}, Operator{Multiply{}}, Number{7.0}};
    assert(evaluate_expression(expr3) == 42.0);

    std::cout << "Part 2 passed!\n\n";
}

// ============================================================================
// Part 3: Result Type (like Rust's Result<T, E>)
// ============================================================================

// Error type
struct Error {
    std::string message;
};

// Result is either a value or an error
template<typename T>
using Result = std::variant<T, Error>;

// Helper functions
template<typename T>
bool is_ok(const Result<T>& result) {
    return std::holds_alternative<T>(result);
}

template<typename T>
bool is_err(const Result<T>& result) {
    return std::holds_alternative<Error>(result);
}

// TODO 6: Implement unwrap_or
// Return the value if ok, otherwise return the default value
template<typename T>
T unwrap_or(const Result<T>& result, T default_value) {
    // Your implementation here
    return default_value;  // Replace this
}

// TODO 7: Implement parse_int
// Parse a string to int, returning Error if parsing fails
// Use std::stoi and catch exceptions
Result<int> parse_int(const std::string& str) {
    // Your implementation here
    return Error{"Not implemented"};  // Replace this
}

// TODO 8: Implement map_result
// Apply function f to the value if ok, pass through error otherwise
template<typename T, typename U, typename F>
Result<U> map_result(const Result<T>& result, F f) {
    // Your implementation here
    return Error{"Not implemented"};  // Replace this
}

void test_part3() {
    std::cout << "=== Part 3: Result Type ===\n";

    // Test is_ok and is_err
    Result<int> ok_result = 42;
    Result<int> err_result = Error{"something went wrong"};

    assert(is_ok(ok_result));
    assert(!is_err(ok_result));
    assert(!is_ok(err_result));
    assert(is_err(err_result));

    // Test unwrap_or
    assert(unwrap_or(ok_result, 0) == 42);
    assert(unwrap_or(err_result, 0) == 0);

    // Test parse_int
    auto parsed1 = parse_int("42");
    assert(is_ok(parsed1));
    assert(std::get<int>(parsed1) == 42);

    auto parsed2 = parse_int("not a number");
    assert(is_err(parsed2));

    auto parsed3 = parse_int("-123");
    assert(is_ok(parsed3));
    assert(std::get<int>(parsed3) == -123);

    // Test map_result
    auto doubled = map_result<int, int>(ok_result, [](int x) { return x * 2; });
    assert(is_ok(doubled));
    assert(std::get<int>(doubled) == 84);

    auto doubled_err = map_result<int, int>(err_result, [](int x) { return x * 2; });
    assert(is_err(doubled_err));

    auto stringified = map_result<int, std::string>(ok_result,
        [](int x) { return "value: " + std::to_string(x); });
    assert(is_ok(stringified));
    assert(std::get<std::string>(stringified) == "value: 42");

    std::cout << "Part 3 passed!\n\n";
}

int main() {
    test_part1();
    test_part2();
    test_part3();

    std::cout << "=== All exercises completed! ===\n";
    return 0;
}
