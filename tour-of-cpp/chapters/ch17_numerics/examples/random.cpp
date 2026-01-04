// Random Number Generation
// Book reference: 17.5 Random Numbers
//
// The <random> header provides quality random number generation
// with various engines and distributions.

#include <algorithm>
#include <format>
#include <iomanip>
#include <iostream>
#include <map>
#include <random>
#include <string>
#include <vector>

int main() {
    std::cout << "=== Random Number Generation ===\n\n";

    // =========================================================================
    // Random engines
    // =========================================================================
    std::cout << "--- Random Engines ---\n";

    // std::random_device - hardware/OS entropy (for seeding)
    std::random_device rd;
    std::cout << "random_device entropy: " << rd.entropy() << '\n';

    // std::mt19937 - Mersenne Twister (most commonly used)
    std::mt19937 gen{rd()};
    std::cout << "mt19937 sample values: ";
    for (int i = 0; i < 5; ++i) {
        std::cout << gen() << ' ';
    }
    std::cout << '\n';

    // Deterministic seeding for reproducibility
    std::mt19937 gen_seeded{42};
    std::cout << "mt19937 with seed 42: ";
    for (int i = 0; i < 5; ++i) {
        std::cout << gen_seeded() << ' ';
    }
    std::cout << "\n\n";

    // =========================================================================
    // Uniform distributions
    // =========================================================================
    std::cout << "--- Uniform Distributions ---\n";

    std::mt19937 gen1{rd()};

    // Uniform integer distribution [1, 6]
    std::uniform_int_distribution<> dice{1, 6};
    std::cout << "Dice rolls: ";
    for (int i = 0; i < 10; ++i) {
        std::cout << dice(gen1) << ' ';
    }
    std::cout << '\n';

    // Uniform real distribution [0, 1)
    std::uniform_real_distribution<> uniform01{0.0, 1.0};
    std::cout << "Uniform [0,1): ";
    for (int i = 0; i < 5; ++i) {
        std::cout << std::format("{:.3f} ", uniform01(gen1));
    }
    std::cout << "\n\n";

    // =========================================================================
    // Normal (Gaussian) distribution
    // =========================================================================
    std::cout << "--- Normal Distribution ---\n";

    std::mt19937 gen2{rd()};
    std::normal_distribution<> normal{100.0, 15.0};  // mean=100, stddev=15

    std::cout << "IQ-like distribution (mean=100, std=15):\n";
    std::map<int, int> histogram;
    for (int i = 0; i < 10000; ++i) {
        int bucket = static_cast<int>(normal(gen2) / 10) * 10;
        ++histogram[bucket];
    }

    for (auto [bucket, count] : histogram) {
        if (count > 50) {
            std::cout << std::format("{:3d}: {}\n", bucket,
                std::string(count / 100, '*'));
        }
    }
    std::cout << '\n';

    // =========================================================================
    // Bernoulli distribution
    // =========================================================================
    std::cout << "--- Bernoulli Distribution ---\n";

    std::mt19937 gen3{rd()};
    std::bernoulli_distribution coin{0.5};
    std::bernoulli_distribution biased_coin{0.7};

    int heads = 0, biased_heads = 0;
    for (int i = 0; i < 1000; ++i) {
        if (coin(gen3)) ++heads;
        if (biased_coin(gen3)) ++biased_heads;
    }

    std::cout << std::format("Fair coin (p=0.5): {} heads / 1000\n", heads);
    std::cout << std::format("Biased coin (p=0.7): {} heads / 1000\n\n", biased_heads);

    // =========================================================================
    // Other distributions
    // =========================================================================
    std::cout << "--- Other Distributions ---\n";

    std::mt19937 gen4{rd()};

    // Binomial: number of successes in n trials
    std::binomial_distribution<> binomial{10, 0.5};  // 10 trials, p=0.5
    std::cout << "Binomial (n=10, p=0.5): ";
    for (int i = 0; i < 10; ++i) {
        std::cout << binomial(gen4) << ' ';
    }
    std::cout << '\n';

    // Poisson: events in fixed interval
    std::poisson_distribution<> poisson{4.0};  // lambda=4
    std::cout << "Poisson (lambda=4): ";
    for (int i = 0; i < 10; ++i) {
        std::cout << poisson(gen4) << ' ';
    }
    std::cout << '\n';

    // Exponential: time between events
    std::exponential_distribution<> exponential{1.0};
    std::cout << "Exponential (lambda=1): ";
    for (int i = 0; i < 5; ++i) {
        std::cout << std::format("{:.2f} ", exponential(gen4));
    }
    std::cout << "\n\n";

    // =========================================================================
    // Discrete distribution (custom probabilities)
    // =========================================================================
    std::cout << "--- Discrete Distribution ---\n";

    std::mt19937 gen5{rd()};

    // Roll a loaded die: 6 appears 30% of the time
    std::discrete_distribution<> loaded_die{10, 10, 10, 10, 10, 50};

    std::map<int, int> die_counts;
    for (int i = 0; i < 1000; ++i) {
        ++die_counts[loaded_die(gen5) + 1];  // +1 to make it 1-6
    }

    std::cout << "Loaded die (6 is more likely):\n";
    for (auto [face, count] : die_counts) {
        std::cout << std::format("  {}: {} ({}%)\n", face, count, count / 10);
    }
    std::cout << '\n';

    // =========================================================================
    // Shuffling
    // =========================================================================
    std::cout << "--- Shuffling ---\n";

    std::mt19937 gen6{rd()};

    std::vector<int> deck{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    std::cout << "Original: ";
    for (int x : deck) std::cout << x << ' ';
    std::cout << '\n';

    std::shuffle(deck.begin(), deck.end(), gen6);
    std::cout << "Shuffled: ";
    for (int x : deck) std::cout << x << ' ';
    std::cout << '\n';

    std::shuffle(deck.begin(), deck.end(), gen6);
    std::cout << "Shuffled again: ";
    for (int x : deck) std::cout << x << ' ';
    std::cout << "\n\n";

    // =========================================================================
    // Random sampling
    // =========================================================================
    std::cout << "--- Random Sampling ---\n";

    std::mt19937 gen7{rd()};
    std::vector<std::string> names{"Alice", "Bob", "Charlie", "Diana", "Eve"};

    std::vector<std::string> sample(2);
    std::sample(names.begin(), names.end(), sample.begin(), 2, gen7);

    std::cout << "Random sample of 2 from names: ";
    for (const auto& name : sample) {
        std::cout << name << ' ';
    }
    std::cout << "\n\n";

    // =========================================================================
    // Best practices demonstration
    // =========================================================================
    std::cout << "--- Best Practices ---\n";

    // BAD: Creating new engine/distribution each call
    std::cout << "Don't do this (inefficient):\n";
    std::cout << "  for each iteration: create engine, create dist, generate\n";

    // GOOD: Create once, reuse
    std::cout << "Do this instead:\n";
    std::cout << "  Create engine once, create distribution once, generate many\n";

    // Example of proper usage
    std::mt19937 engine{std::random_device{}()};
    std::uniform_int_distribution<> dist{1, 100};

    std::cout << "Proper usage - 5 random numbers: ";
    for (int i = 0; i < 5; ++i) {
        std::cout << dist(engine) << ' ';
    }
    std::cout << '\n';

    return 0;
}

// Key points:
// - Use std::random_device for seeding (true randomness)
// - Use std::mt19937 as the workhorse engine
// - Choose distribution based on your needs
// - Create engine and distribution once, reuse them
// - std::shuffle for random permutations
// - std::sample for random selection without replacement
// - NEVER use rand() in modern C++
