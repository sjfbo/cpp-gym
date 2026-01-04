// Exercise 2.3: Traffic Light State Machine
// Book reference: 2.4 Enumerations
// Difficulty: *
//
// Instructions:
// 1. Define an enum class TrafficLight with states: Red, Yellow, Green
// 2. Implement next_state(TrafficLight) that returns the next state
//    (Red -> Green -> Yellow -> Red)
// 3. Implement duration_seconds(TrafficLight) that returns how long
//    each light should be on (Red: 30, Green: 25, Yellow: 5)
// 4. Implement to_string(TrafficLight) for display
//
// Expected output:
//   Current: Red (30 seconds)
//   Next: Green (25 seconds)
//   Next: Yellow (5 seconds)
//   Next: Red (30 seconds)

#include <iostream>
#include <string>

// TODO: Define enum class TrafficLight

// TODO: Implement next_state(TrafficLight current)

// TODO: Implement duration_seconds(TrafficLight light)

// TODO: Implement to_string(TrafficLight light)

int main() {
    // Uncomment after implementing:

    // TrafficLight light = TrafficLight::Red;

    // for (int i = 0; i < 4; ++i) {
    //     std::cout << "Current: " << to_string(light)
    //               << " (" << duration_seconds(light) << " seconds)\n";
    //     light = next_state(light);
    // }

    std::cout << "TODO: Implement TrafficLight enum and functions\n";

    return 0;
}
