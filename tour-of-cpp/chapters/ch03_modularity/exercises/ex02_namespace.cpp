// Exercise 2: Organizing Code with Namespaces
// Book reference: 3.3 Namespaces
//
// In this exercise, you'll practice organizing code into namespaces
// to avoid naming conflicts and create logical groupings.
//
// YOUR TASK:
// 1. Read through the existing code and understand the naming conflicts
// 2. Organize the code into appropriate namespaces
// 3. Fix the main() function to use the namespaced versions
// 4. All tests should pass when you run this file

#include <iostream>
#include <string>
#include <cmath>
#include <cassert>

// ============================================================================
// PROBLEM: These types and functions have naming conflicts!
// YOUR TASK: Organize them into namespaces
// ============================================================================

// TODO: Put this Point in a "graphics2d" namespace
struct Point {
    double x = 0;
    double y = 0;
};

// TODO: Put this Point in a "graphics3d" namespace
// (Currently commented out due to conflict)
/*
struct Point {
    double x = 0;
    double y = 0;
    double z = 0;
};
*/

// TODO: Put in "graphics2d" namespace
double distance(const Point& a, const Point& b) {
    double dx = a.x - b.x;
    double dy = a.y - b.y;
    return std::sqrt(dx * dx + dy * dy);
}

// TODO: Put in "graphics3d" namespace
// (Currently commented out due to conflict)
/*
double distance(const Point& a, const Point& b) {
    double dx = a.x - b.x;
    double dy = a.y - b.y;
    double dz = a.z - b.z;
    return std::sqrt(dx * dx + dy * dy + dz * dz);
}
*/

// ============================================================================
// TODO: Create a nested namespace "game::physics" with:
// - struct Vector { double x, y; }
// - Vector add(const Vector& a, const Vector& b)
// - double magnitude(const Vector& v)
// ============================================================================

// YOUR NAMESPACE CODE HERE

// ============================================================================
// TODO: Create an anonymous namespace with a helper function
// This should only be visible in this file
// ============================================================================

// YOUR ANONYMOUS NAMESPACE CODE HERE
// Suggestion: double square(double x) { return x * x; }

// ============================================================================
// TODO: Create a namespace alias 'gfx2d' for graphics2d
// ============================================================================

// YOUR ALIAS HERE

// ============================================================================
// TEST CODE - FIX THIS AFTER ADDING NAMESPACES
// ============================================================================

void run_tests() {
    std::cout << "Running namespace tests...\n";

    // Test 1: 2D Point and distance
    {
        // TODO: Use graphics2d::Point
        Point p1{0, 0};
        Point p2{3, 4};

        // TODO: Use graphics2d::distance
        double d = distance(p1, p2);
        assert(std::abs(d - 5.0) < 0.001);
        std::cout << "  [PASS] 2D distance\n";
    }

    // Test 2: 3D Point and distance
    {
        // TODO: Uncomment and fix after adding graphics3d namespace
        /*
        graphics3d::Point p1{0, 0, 0};
        graphics3d::Point p2{1, 2, 2};

        double d = graphics3d::distance(p1, p2);
        assert(std::abs(d - 3.0) < 0.001);
        std::cout << "  [PASS] 3D distance\n";
        */
        std::cout << "  [SKIP] 3D distance (implement graphics3d namespace)\n";
    }

    // Test 3: game::physics namespace
    {
        // TODO: Uncomment after implementing game::physics
        /*
        game::physics::Vector v1{3, 0};
        game::physics::Vector v2{0, 4};

        auto sum = game::physics::add(v1, v2);
        assert(sum.x == 3 && sum.y == 4);

        double mag = game::physics::magnitude(sum);
        assert(std::abs(mag - 5.0) < 0.001);
        std::cout << "  [PASS] game::physics\n";
        */
        std::cout << "  [SKIP] game::physics (implement the namespace)\n";
    }

    // Test 4: Namespace alias
    {
        // TODO: Uncomment after creating gfx2d alias
        /*
        gfx2d::Point p{1, 1};
        assert(p.x == 1 && p.y == 1);
        std::cout << "  [PASS] Namespace alias\n";
        */
        std::cout << "  [SKIP] Namespace alias (create gfx2d alias)\n";
    }

    // Test 5: Using declarations
    {
        // TODO: Add a using declaration and use it
        // using graphics2d::Point;
        // Point p{5, 5};
        std::cout << "  [SKIP] Using declarations (add using declaration)\n";
    }

    std::cout << "\nTests completed!\n";
}

// ============================================================================
// MAIN
// ============================================================================

int main() {
    std::cout << "=== Exercise 2: Organizing Code with Namespaces ===\n\n";

    run_tests();

    std::cout << "\n";
    std::cout << "EXERCISE GOALS:\n";
    std::cout << "1. Create graphics2d and graphics3d namespaces\n";
    std::cout << "2. Create nested game::physics namespace\n";
    std::cout << "3. Create anonymous namespace for helpers\n";
    std::cout << "4. Create namespace alias gfx2d\n";
    std::cout << "5. Use 'using' declarations appropriately\n";
    std::cout << "\n";
    std::cout << "TIPS:\n";
    std::cout << "- Use 'namespace A::B { }' for nested namespaces (C++17)\n";
    std::cout << "- Anonymous namespace: 'namespace { }'\n";
    std::cout << "- Alias: 'namespace gfx2d = graphics2d;'\n";
    std::cout << "- Never use 'using namespace X;' in headers!\n";

    return 0;
}
