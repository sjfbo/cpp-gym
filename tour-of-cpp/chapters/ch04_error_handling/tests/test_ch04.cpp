/**
 * Chapter 4: Error Handling - Catch2 Tests
 *
 * Tests for exception handling concepts including:
 * - Exception throwing and catching
 * - Custom exception types
 * - noexcept specifications
 * - Exception safety with RAII
 */

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_exception.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>

#include <stdexcept>
#include <string>
#include <memory>
#include <vector>

using namespace Catch::Matchers;

// =============================================================================
// Sample Code Under Test
// =============================================================================

// Custom exception hierarchy for testing
class AppError : public std::runtime_error {
public:
    explicit AppError(const std::string& msg) : std::runtime_error(msg) {}
};

class NetworkError : public AppError {
public:
    explicit NetworkError(const std::string& host)
        : AppError("Network error: " + host), host_(host) {}
    const std::string& host() const noexcept { return host_; }
private:
    std::string host_;
};

class TimeoutError : public NetworkError {
public:
    TimeoutError(const std::string& host, int timeout_ms)
        : NetworkError(host + " (timeout: " + std::to_string(timeout_ms) + "ms)")
        , timeout_ms_(timeout_ms) {}
    int timeout_ms() const noexcept { return timeout_ms_; }
private:
    int timeout_ms_;
};

// Functions that throw
int safe_divide(int a, int b) {
    if (b == 0) {
        throw std::invalid_argument("Division by zero");
    }
    return a / b;
}

int safe_sqrt_approx(int n) {
    if (n < 0) {
        throw std::domain_error("Cannot compute square root of negative number");
    }
    int result = 0;
    while (result * result <= n) ++result;
    return result - 1;
}

void simulate_network(int scenario) {
    switch (scenario) {
        case 0: throw NetworkError("api.example.com");
        case 1: throw TimeoutError("db.example.com", 5000);
        default: break;
    }
}

// noexcept functions
int no_throw_add(int a, int b) noexcept {
    return a + b;
}

void may_throw(bool should_throw) {
    if (should_throw) {
        throw std::runtime_error("Thrown!");
    }
}

// RAII helper for testing
class ResourceTracker {
public:
    explicit ResourceTracker(bool& flag) : flag_(flag) { flag_ = true; }
    ~ResourceTracker() { flag_ = false; }
    ResourceTracker(const ResourceTracker&) = delete;
    ResourceTracker& operator=(const ResourceTracker&) = delete;
private:
    bool& flag_;
};

// =============================================================================
// Tests: Basic Exception Throwing and Catching
// =============================================================================

TEST_CASE("REQUIRE_THROWS catches thrown exceptions", "[exceptions]") {
    REQUIRE_THROWS(safe_divide(10, 0));
    REQUIRE_THROWS(safe_sqrt_approx(-5));
}

TEST_CASE("REQUIRE_NOTHROW verifies no exception", "[exceptions]") {
    REQUIRE_NOTHROW(safe_divide(10, 2));
    REQUIRE_NOTHROW(safe_sqrt_approx(16));
}

TEST_CASE("REQUIRE_THROWS_AS checks exception type", "[exceptions]") {
    REQUIRE_THROWS_AS(safe_divide(10, 0), std::invalid_argument);
    REQUIRE_THROWS_AS(safe_sqrt_approx(-5), std::domain_error);

    // Both are std::exception subclasses
    REQUIRE_THROWS_AS(safe_divide(10, 0), std::exception);
    REQUIRE_THROWS_AS(safe_sqrt_approx(-5), std::exception);
}

TEST_CASE("REQUIRE_THROWS_WITH checks exception message", "[exceptions]") {
    REQUIRE_THROWS_WITH(safe_divide(10, 0), "Division by zero");
    REQUIRE_THROWS_WITH(safe_sqrt_approx(-5),
                        ContainsSubstring("negative"));
}

TEST_CASE("REQUIRE_THROWS_MATCHES uses matchers", "[exceptions]") {
    REQUIRE_THROWS_MATCHES(
        safe_divide(10, 0),
        std::invalid_argument,
        MessageMatches(ContainsSubstring("zero"))
    );
}

// =============================================================================
// Tests: Exception Hierarchy
// =============================================================================

TEST_CASE("Custom exception hierarchy works correctly", "[exceptions][hierarchy]") {
    SECTION("NetworkError is caught as AppError") {
        REQUIRE_THROWS_AS(simulate_network(0), AppError);
    }

    SECTION("TimeoutError is caught as NetworkError") {
        REQUIRE_THROWS_AS(simulate_network(1), NetworkError);
    }

    SECTION("TimeoutError is caught as AppError") {
        REQUIRE_THROWS_AS(simulate_network(1), AppError);
    }

    SECTION("TimeoutError is caught as std::runtime_error") {
        REQUIRE_THROWS_AS(simulate_network(1), std::runtime_error);
    }

    SECTION("Normal scenario doesn't throw") {
        REQUIRE_NOTHROW(simulate_network(99));
    }
}

TEST_CASE("Exception contains correct information", "[exceptions][hierarchy]") {
    try {
        throw TimeoutError("test.example.com", 3000);
        FAIL("Expected exception");
    }
    catch (const TimeoutError& e) {
        REQUIRE(e.timeout_ms() == 3000);
        REQUIRE_THAT(e.host(), ContainsSubstring("test.example.com"));
    }
}

TEST_CASE("Catch by reference preserves type", "[exceptions][best-practices]") {
    bool caught_as_derived = false;

    try {
        throw NetworkError("server.test");
    }
    catch (const NetworkError& e) {
        caught_as_derived = true;
        REQUIRE_THAT(e.host(), Equals("server.test"));
    }
    catch (const AppError&) {
        FAIL("Should have caught as NetworkError, not AppError");
    }

    REQUIRE(caught_as_derived);
}

// =============================================================================
// Tests: noexcept Specification
// =============================================================================

TEST_CASE("noexcept functions are detectable at compile time", "[noexcept]") {
    STATIC_REQUIRE(noexcept(no_throw_add(1, 2)));
    STATIC_REQUIRE_FALSE(noexcept(may_throw(true)));
    STATIC_REQUIRE_FALSE(noexcept(safe_divide(1, 1)));
}

TEST_CASE("noexcept function behavior", "[noexcept]") {
    // noexcept function works normally
    REQUIRE(no_throw_add(5, 3) == 8);
    REQUIRE(no_throw_add(-10, 10) == 0);
}

// =============================================================================
// Tests: RAII and Exception Safety
// =============================================================================

TEST_CASE("RAII cleanup occurs during stack unwinding", "[raii]") {
    bool resource_active = false;

    try {
        ResourceTracker tracker(resource_active);
        REQUIRE(resource_active);  // Resource is active

        throw std::runtime_error("Test exception");

        FAIL("Should not reach here");
    }
    catch (const std::runtime_error&) {
        // After exception, resource should be cleaned up
        REQUIRE_FALSE(resource_active);
    }
}

TEST_CASE("RAII cleanup occurs on normal exit", "[raii]") {
    bool resource_active = false;

    {
        ResourceTracker tracker(resource_active);
        REQUIRE(resource_active);
    }

    REQUIRE_FALSE(resource_active);
}

TEST_CASE("Smart pointers provide RAII for dynamic memory", "[raii]") {
    static int destruction_count = 0;

    struct CountedType {
        ~CountedType() { ++destruction_count; }
    };

    destruction_count = 0;

    try {
        auto ptr = std::make_unique<CountedType>();
        throw std::runtime_error("Test");
    }
    catch (...) {
        // Smart pointer should have cleaned up
    }

    REQUIRE(destruction_count == 1);
}

// =============================================================================
// Tests: Multiple Exception Scenarios
// =============================================================================

TEST_CASE("Different inputs produce different exceptions", "[exceptions]") {
    std::vector<std::pair<int, const char*>> test_cases = {
        {0, "Division by zero"},
        // We test both the original function and verify different scenarios
    };

    SECTION("Division by zero") {
        REQUIRE_THROWS_AS(safe_divide(100, 0), std::invalid_argument);
    }

    SECTION("Negative square root") {
        REQUIRE_THROWS_AS(safe_sqrt_approx(-1), std::domain_error);
        REQUIRE_THROWS_AS(safe_sqrt_approx(-100), std::domain_error);
    }

    SECTION("Valid operations succeed") {
        REQUIRE(safe_divide(100, 10) == 10);
        REQUIRE(safe_sqrt_approx(0) == 0);
        REQUIRE(safe_sqrt_approx(1) == 1);
        REQUIRE(safe_sqrt_approx(4) == 2);
        REQUIRE(safe_sqrt_approx(100) == 10);
    }
}

// =============================================================================
// Tests: Exception Rethrowing
// =============================================================================

void rethrow_wrapper(bool should_throw) {
    try {
        may_throw(should_throw);
    }
    catch (...) {
        // Log or do cleanup, then rethrow
        throw;
    }
}

TEST_CASE("Rethrowing preserves exception type", "[exceptions][rethrow]") {
    REQUIRE_THROWS_AS(rethrow_wrapper(true), std::runtime_error);
    REQUIRE_NOTHROW(rethrow_wrapper(false));
}

// =============================================================================
// Tests: static_assert (Compile-Time)
// =============================================================================

// These are compile-time checks - if they fail, the code won't compile
static_assert(sizeof(int) >= 4, "int must be at least 32 bits");
static_assert(std::is_nothrow_move_constructible_v<std::string>,
              "string should have noexcept move constructor");

TEST_CASE("static_assert passes at compile time", "[static_assert]") {
    // These are validated at compile time
    // This test just documents that the static_asserts above passed
    SUCCEED("All static_asserts passed");
}

// =============================================================================
// Tests: Exception-Safe Container Operations
// =============================================================================

TEST_CASE("Vector operations are exception-safe", "[raii][vector]") {
    std::vector<int> vec = {1, 2, 3, 4, 5};

    try {
        // at() throws on out-of-bounds
        [[maybe_unused]] int val = vec.at(100);
        FAIL("Should have thrown");
    }
    catch (const std::out_of_range&) {
        // Vector should still be intact
        REQUIRE(vec.size() == 5);
        REQUIRE(vec[0] == 1);
        REQUIRE(vec[4] == 5);
    }
}

// =============================================================================
// Tests: Function Try Block (Advanced)
// =============================================================================

class FailingInit {
public:
    FailingInit(bool should_fail) {
        if (should_fail) {
            throw std::runtime_error("Initialization failed");
        }
    }
};

class Container {
public:
    Container(bool fail) try : member_(fail) {
        // Constructor body
    }
    catch (...) {
        // Handle or rethrow
        throw;  // Must rethrow or the exception is implicitly rethrown
    }

private:
    FailingInit member_;
};

TEST_CASE("Constructor exceptions are catchable", "[exceptions][constructor]") {
    REQUIRE_THROWS_AS(Container(true), std::runtime_error);
    REQUIRE_NOTHROW(Container(false));
}

// =============================================================================
// Tests: Exception Guarantees Documentation
// =============================================================================

TEST_CASE("Document exception guarantee levels", "[documentation]") {
    // This test serves as documentation for exception guarantee levels

    SECTION("No-throw guarantee") {
        // Functions that never throw
        // Marked noexcept, will terminate if exception escapes
        REQUIRE(noexcept(no_throw_add(1, 2)));
    }

    SECTION("Strong exception guarantee") {
        // If operation fails, state is unchanged
        std::vector<int> original = {1, 2, 3};
        std::vector<int> copy = original;

        try {
            copy.at(100);  // Throws, but vector unchanged
        }
        catch (...) {
            REQUIRE(copy == original);
        }
    }

    SECTION("Basic exception guarantee") {
        // No resources leaked, object in valid state
        // (but possibly modified)
        // Most standard library operations provide this
        SUCCEED("Basic guarantee: no leaks, valid state");
    }
}
