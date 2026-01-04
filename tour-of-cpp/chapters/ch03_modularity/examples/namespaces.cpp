// Namespaces - Organizing Code and Avoiding Name Collisions
// Book reference: 3.3 Namespaces
//
// Namespaces group related declarations together and prevent name conflicts.
// They are essential for organizing large codebases and libraries.

#include <iostream>
#include <string>

// ============================================================================
// Basic Namespace Declaration - See 3.3
// ============================================================================

namespace graphics {

struct Point {
    double x = 0;
    double y = 0;
};

struct Color {
    int r = 0, g = 0, b = 0;
};

void draw_point(const Point& p, const Color& c) {
    std::cout << "Drawing point (" << p.x << ", " << p.y << ") "
              << "with color RGB(" << c.r << ", " << c.g << ", " << c.b << ")\n";
}

}  // namespace graphics

// ============================================================================
// Another Namespace with Same Name (Extends the namespace)
// ============================================================================

namespace graphics {

// Adding more to the graphics namespace
struct Line {
    Point start;
    Point end;
};

void draw_line(const Line& line, [[maybe_unused]] const Color& c) {
    std::cout << "Drawing line from (" << line.start.x << ", " << line.start.y
              << ") to (" << line.end.x << ", " << line.end.y << ")\n";
}

}  // namespace graphics

// ============================================================================
// Nested Namespaces (Traditional style)
// ============================================================================

namespace company {
namespace project {
namespace utils {

std::string format_name(const std::string& first, const std::string& last) {
    return first + " " + last;
}

}  // namespace utils
}  // namespace project
}  // namespace company

// ============================================================================
// Nested Namespaces (C++17 style - preferred)
// ============================================================================

namespace company::project::math {

double square(double x) { return x * x; }
double cube(double x) { return x * x * x; }

}  // namespace company::project::math

// ============================================================================
// Namespace Aliases
// ============================================================================

// Create shorter names for deeply nested namespaces
namespace cpm = company::project::math;
namespace cpu = company::project::utils;

// ============================================================================
// Anonymous Namespace (Internal Linkage) - See 3.3
// ============================================================================

namespace {

// Everything here is only visible in this translation unit (file)
// Similar to 'static' at file scope, but works for types too

constexpr int kInternalConstant = 42;

int internal_helper(int x) {
    return x * kInternalConstant;
}

struct InternalData {
    int value;
};

}  // anonymous namespace

// ============================================================================
// Inline Namespaces (C++11) - For API versioning
// ============================================================================

namespace api {

// Default version (inline makes it accessible as api::feature)
inline namespace v2 {

struct Feature {
    std::string name = "v2 feature";
    int version = 2;

    void describe() const {
        std::cout << name << " (version " << version << ")\n";
    }
};

}  // inline namespace v2

// Old version (still accessible as api::v1::Feature)
namespace v1 {

struct Feature {
    std::string name = "v1 feature";
    int version = 1;

    void describe() const {
        std::cout << name << " (version " << version << ")\n";
    }
};

}  // namespace v1

}  // namespace api

// ============================================================================
// Using Declarations and Directives
// ============================================================================

void demonstrate_using() {
    std::cout << "\n--- Using Declarations and Directives ---\n";

    // Using declaration: brings ONE specific name into scope
    using graphics::Point;
    using graphics::Color;

    Point p{10, 20};
    Color c{255, 0, 0};
    graphics::draw_point(p, c);

    // Using directive: brings ALL names from namespace into scope
    // AVOID in headers! OK in implementation files within limited scope
    {
        using namespace graphics;  // Only in this block
        Line line{{0, 0}, {100, 100}};
        draw_line(line, c);
    }

    // Prefer qualified names or using declarations over using directives
    std::cout << "Prefer: graphics::draw_point() or 'using graphics::Point;'\n";
    std::cout << "Avoid:  'using namespace graphics;' especially in headers\n";
}

// ============================================================================
// ADL (Argument-Dependent Lookup) / Koenig Lookup
// ============================================================================

namespace network {

struct Connection {
    std::string address;
    int port;
};

// This function will be found via ADL when called with Connection
void send(const Connection& conn, const std::string& data) {
    std::cout << "Sending '" << data << "' to " << conn.address
              << ":" << conn.port << "\n";
}

}  // namespace network

void demonstrate_adl() {
    std::cout << "\n--- Argument-Dependent Lookup (ADL) ---\n";

    network::Connection conn{"localhost", 8080};

    // ADL finds network::send because conn is from namespace network
    // No need to qualify: network::send(conn, "hello")
    send(conn, "Hello via ADL!");

    std::cout << "ADL automatically searches the namespace of argument types\n";
}

// ============================================================================
// Namespace Best Practices
// ============================================================================

namespace best_practices {

// 1. Use namespaces to organize related functionality
namespace math {
    double add(double a, double b) { return a + b; }
}

// 2. Don't pollute the global namespace
// BAD: int global_count = 0;
// GOOD: namespace my_project { int count = 0; }

// 3. Use anonymous namespaces for file-local helpers
// (See anonymous namespace above)

// 4. Prefer namespace aliases over 'using namespace' for long names
// namespace fs = std::filesystem;

// 5. In headers, NEVER use 'using namespace std;'

}  // namespace best_practices

// ============================================================================
// Main Demonstration
// ============================================================================

int main() {
    std::cout << "=== Namespace Demonstration ===\n";

    // --------------------------------------------------------------------
    // Basic namespace usage
    // --------------------------------------------------------------------
    std::cout << "\n--- Basic Namespace Usage ---\n";

    graphics::Point p{5.0, 10.0};
    graphics::Color red{255, 0, 0};
    graphics::draw_point(p, red);

    graphics::Line line{{0, 0}, {100, 50}};
    graphics::draw_line(line, red);

    // --------------------------------------------------------------------
    // Nested namespaces
    // --------------------------------------------------------------------
    std::cout << "\n--- Nested Namespaces ---\n";

    std::cout << "Traditional: company::project::utils::format_name\n";
    std::cout << "Result: " << company::project::utils::format_name("John", "Doe") << "\n";

    std::cout << "C++17 style: company::project::math::square\n";
    std::cout << "square(5) = " << company::project::math::square(5) << "\n";

    // Using aliases
    std::cout << "\nWith aliases:\n";
    std::cout << "cpm::cube(3) = " << cpm::cube(3) << "\n";
    std::cout << "cpu::format_name(\"Jane\", \"Doe\") = "
              << cpu::format_name("Jane", "Doe") << "\n";

    // --------------------------------------------------------------------
    // Anonymous namespace (internal linkage)
    // --------------------------------------------------------------------
    std::cout << "\n--- Anonymous Namespace ---\n";
    std::cout << "internal_helper(10) = " << internal_helper(10) << "\n";
    std::cout << "(Only visible in this file)\n";

    // --------------------------------------------------------------------
    // Inline namespaces (versioning)
    // --------------------------------------------------------------------
    std::cout << "\n--- Inline Namespaces (Versioning) ---\n";

    api::Feature current;  // Uses api::v2::Feature (inline)
    current.describe();

    api::v1::Feature old_version;  // Explicit v1
    old_version.describe();

    std::cout << "api::Feature uses v2 (inline namespace)\n";
    std::cout << "api::v1::Feature explicitly uses v1\n";

    // --------------------------------------------------------------------
    // Using declarations and directives
    // --------------------------------------------------------------------
    demonstrate_using();

    // --------------------------------------------------------------------
    // ADL demonstration
    // --------------------------------------------------------------------
    demonstrate_adl();

    // --------------------------------------------------------------------
    // Summary
    // --------------------------------------------------------------------
    std::cout << "\n=== Key Points ===\n";
    std::cout << "1. Use namespaces to organize and avoid name collisions\n";
    std::cout << "2. C++17: namespace A::B::C {} for nested namespaces\n";
    std::cout << "3. Anonymous namespace = internal linkage (file-local)\n";
    std::cout << "4. Inline namespace = default version for API versioning\n";
    std::cout << "5. Prefer 'using X::name;' over 'using namespace X;'\n";
    std::cout << "6. NEVER use 'using namespace std;' in headers!\n";

    return 0;
}
