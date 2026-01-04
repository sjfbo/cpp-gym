// Exercise 17.1: Random Number Generation
// Book reference: 17.5 Random Numbers
// Difficulty: **
//
// Instructions:
// Practice using the <random> library for various scenarios.
// Implement each function using appropriate engines and distributions.

#include <algorithm>
#include <iostream>
#include <map>
#include <random>
#include <string>
#include <vector>

// Global random engine for convenience (in real code, consider thread safety)
static std::mt19937& get_generator() {
    static std::mt19937 gen{std::random_device{}()};
    return gen;
}

// ============================================================================
// Part 1: Basic Random Generation
// ============================================================================

// TODO 1: Generate a random integer in range [min, max] (inclusive).
// Use: std::uniform_int_distribution

int random_int(int min, int max) {
    // TODO: Implement
    (void)min;
    (void)max;
    return 0;
}


// TODO 2: Generate a random double in range [min, max).
// Use: std::uniform_real_distribution

double random_double(double min, double max) {
    // TODO: Implement
    (void)min;
    (void)max;
    return 0.0;
}


// TODO 3: Simulate a coin flip. Return true for heads with probability p.
// Use: std::bernoulli_distribution

bool coin_flip(double p = 0.5) {
    // TODO: Implement
    (void)p;
    return false;
}


// TODO 4: Roll n dice with s sides each. Return the sum.
// Use: std::uniform_int_distribution in a loop

int roll_dice(int n, int sides) {
    // TODO: Implement
    (void)n;
    (void)sides;
    return 0;
}


// ============================================================================
// Part 2: Advanced Distributions
// ============================================================================

// TODO 5: Generate a random number from a normal distribution.
// Use: std::normal_distribution

double random_normal(double mean, double stddev) {
    // TODO: Implement
    (void)mean;
    (void)stddev;
    return 0.0;
}


// TODO 6: Generate random values with custom probabilities.
// Example: weighted_choice({0.5, 0.3, 0.2}) returns 0 (50%), 1 (30%), or 2 (20%)
// Use: std::discrete_distribution

int weighted_choice(const std::vector<double>& weights) {
    // TODO: Implement
    (void)weights;
    return 0;
}


// TODO 7: Generate a random number from a Poisson distribution.
// Useful for modeling rare events.
// Use: std::poisson_distribution

int random_poisson(double lambda) {
    // TODO: Implement
    (void)lambda;
    return 0;
}


// ============================================================================
// Part 3: Shuffling and Sampling
// ============================================================================

// TODO 8: Shuffle a vector in-place.
// Use: std::shuffle

void shuffle_vector(std::vector<int>& vec) {
    // TODO: Implement
    (void)vec;
}


// TODO 9: Select k random elements from a vector without replacement.
// Use: std::sample

std::vector<int> random_sample(const std::vector<int>& vec, int k) {
    // TODO: Implement
    (void)vec;
    (void)k;
    return {};
}


// TODO 10: Select a random element from a vector.
// Return a reference to allow modification.

int& random_element(std::vector<int>& vec) {
    // TODO: Implement
    // Hint: Use uniform_int_distribution to get a random index
    static int dummy = 0;  // Remove this and return actual element
    (void)vec;
    return dummy;
}


// ============================================================================
// Part 4: Practical Applications
// ============================================================================

// TODO 11: Generate a random password of given length.
// Use characters: a-z, A-Z, 0-9

std::string random_password(int length) {
    // TODO: Implement
    (void)length;
    return "";
}


// TODO 12: Simulate a game where player needs to roll >= target on d20.
// Run num_trials trials and return the success rate.

double d20_success_rate(int target, int num_trials) {
    // TODO: Implement
    (void)target;
    (void)num_trials;
    return 0.0;
}


// TODO 13: Generate random points inside a unit circle (x^2 + y^2 <= 1).
// Use rejection sampling.

std::pair<double, double> random_point_in_circle() {
    // TODO: Implement using rejection sampling
    // Generate (x, y) in [-1, 1] x [-1, 1], reject if outside circle
    return {0.0, 0.0};
}


// ============================================================================
// Tests
// ============================================================================

int main() {
    std::cout << "=== Exercise 17.1: Random Number Generation ===\n\n";

    // Test 1: random_int
    std::cout << "--- Test 1: random_int ---\n";
    std::cout << "random_int(1, 10): ";
    for (int i = 0; i < 10; ++i) {
        std::cout << random_int(1, 10) << ' ';
    }
    std::cout << '\n';

    // Test 2: random_double
    std::cout << "\n--- Test 2: random_double ---\n";
    std::cout << "random_double(0, 1): ";
    for (int i = 0; i < 5; ++i) {
        std::cout << random_double(0, 1) << ' ';
    }
    std::cout << '\n';

    // Test 3: coin_flip
    std::cout << "\n--- Test 3: coin_flip ---\n";
    int heads = 0;
    for (int i = 0; i < 1000; ++i) {
        if (coin_flip(0.7)) ++heads;
    }
    std::cout << "coin_flip(0.7) - heads: " << heads << "/1000\n";

    // Test 4: roll_dice
    std::cout << "\n--- Test 4: roll_dice ---\n";
    std::cout << "roll_dice(3, 6) - three d6: ";
    for (int i = 0; i < 5; ++i) {
        std::cout << roll_dice(3, 6) << ' ';
    }
    std::cout << '\n';

    // Test 5: random_normal
    std::cout << "\n--- Test 5: random_normal ---\n";
    std::cout << "random_normal(100, 15): ";
    for (int i = 0; i < 5; ++i) {
        std::cout << static_cast<int>(random_normal(100, 15)) << ' ';
    }
    std::cout << '\n';

    // Test 6: weighted_choice
    std::cout << "\n--- Test 6: weighted_choice ---\n";
    std::map<int, int> counts;
    for (int i = 0; i < 1000; ++i) {
        ++counts[weighted_choice({0.5, 0.3, 0.2})];
    }
    std::cout << "weighted_choice({0.5, 0.3, 0.2}) - 1000 samples:\n";
    for (auto [choice, count] : counts) {
        std::cout << "  " << choice << ": " << count << " (" << count/10 << "%)\n";
    }

    // Test 7: random_poisson
    std::cout << "\n--- Test 7: random_poisson ---\n";
    std::cout << "random_poisson(4): ";
    for (int i = 0; i < 10; ++i) {
        std::cout << random_poisson(4) << ' ';
    }
    std::cout << '\n';

    // Test 8: shuffle_vector
    std::cout << "\n--- Test 8: shuffle_vector ---\n";
    std::vector<int> v{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    shuffle_vector(v);
    std::cout << "Shuffled: ";
    for (int x : v) std::cout << x << ' ';
    std::cout << '\n';

    // Test 9: random_sample
    std::cout << "\n--- Test 9: random_sample ---\n";
    std::vector<int> population{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    auto sample = random_sample(population, 3);
    std::cout << "Sample of 3: ";
    for (int x : sample) std::cout << x << ' ';
    std::cout << '\n';

    // Test 10: random_element
    std::cout << "\n--- Test 10: random_element ---\n";
    std::vector<int> items{10, 20, 30, 40, 50};
    std::cout << "Random elements: ";
    for (int i = 0; i < 5; ++i) {
        std::cout << random_element(items) << ' ';
    }
    std::cout << '\n';

    // Test 11: random_password
    std::cout << "\n--- Test 11: random_password ---\n";
    std::cout << "Random passwords (length 12):\n";
    for (int i = 0; i < 3; ++i) {
        std::cout << "  " << random_password(12) << '\n';
    }

    // Test 12: d20_success_rate
    std::cout << "\n--- Test 12: d20_success_rate ---\n";
    std::cout << "d20 success rate (target 15): "
              << d20_success_rate(15, 10000) * 100 << "%\n";
    // Expected: ~30% (6 faces out of 20)

    // Test 13: random_point_in_circle
    std::cout << "\n--- Test 13: random_point_in_circle ---\n";
    std::cout << "Random points in unit circle:\n";
    for (int i = 0; i < 3; ++i) {
        auto [x, y] = random_point_in_circle();
        double r = std::sqrt(x*x + y*y);
        std::cout << "  (" << x << ", " << y << ") - r = " << r << '\n';
    }

    return 0;
}
