// Exercise 2.1: Define a Point3D Structure
// Book reference: 2.2 Structures
// Difficulty: *
//
// Instructions:
// Define a Point3D struct with:
// 1. Three double members: x, y, z (with default values 0.0)
// 2. A member function distance_to(const Point3D& other) that returns
//    the Euclidean distance to another point
// 3. A member function translate(double dx, double dy, double dz) that
//    moves the point
//
// Expected output:
//   p1 = (0, 0, 0)
//   p2 = (1, 2, 3)
//   Distance from p1 to p2: 3.74166
//   After translate(1,1,1): p1 = (1, 1, 1)

#include <cmath>
#include <iostream>

// TODO: Define Point3D struct here

int main() {
    // Uncomment after implementing Point3D:

    // Point3D p1;  // Default: (0, 0, 0)
    // Point3D p2{1.0, 2.0, 3.0};

    // std::cout << "p1 = (" << p1.x << ", " << p1.y << ", " << p1.z << ")\n";
    // std::cout << "p2 = (" << p2.x << ", " << p2.y << ", " << p2.z << ")\n";

    // std::cout << "Distance from p1 to p2: " << p1.distance_to(p2) << "\n";

    // p1.translate(1, 1, 1);
    // std::cout << "After translate(1,1,1): p1 = ("
    //           << p1.x << ", " << p1.y << ", " << p1.z << ")\n";

    std::cout << "TODO: Implement Point3D struct\n";

    return 0;
}
