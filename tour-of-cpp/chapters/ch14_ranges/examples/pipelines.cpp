// Pipelines - Composing Views with the Pipe Operator
// Book reference: 14.4 Pipelines
//
// Pipelines let you chain view adaptors using the | operator,
// creating readable, left-to-right data transformations.

#include <algorithm>
#include <iostream>
#include <map>
#include <ranges>
#include <string>
#include <vector>

namespace views = std::views;
namespace ranges = std::ranges;

// ============================================================================
// Helper struct for examples
// ============================================================================

struct Person {
    std::string name;
    int age;
    std::string city;
};

std::ostream& operator<<(std::ostream& os, const Person& p) {
    return os << p.name << " (" << p.age << ", " << p.city << ")";
}

int main() {
    std::cout << "=== Range Pipelines ===\n\n";

    // =========================================================================
    // Basic pipeline: filter -> transform -> take
    // =========================================================================
    std::cout << "--- Basic pipeline ---\n";

    std::vector<int> nums{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    // Without pipeline (nested calls - hard to read)
    std::cout << "Nested style (hard to read):\n  ";
    for (int x : views::take(
            views::transform(
                views::filter(nums, [](int n) { return n % 2 == 0; }),
                [](int n) { return n * n; }),
            3)) {
        std::cout << x << ' ';
    }
    std::cout << '\n';

    // With pipeline (reads left to right)
    std::cout << "Pipeline style (clear):\n  ";
    for (int x : nums
                 | views::filter([](int n) { return n % 2 == 0; })
                 | views::transform([](int n) { return n * n; })
                 | views::take(3)) {
        std::cout << x << ' ';
    }
    std::cout << "\n\n";

    // =========================================================================
    // Working with structs
    // =========================================================================
    std::cout << "--- Pipeline with structs ---\n";

    std::vector<Person> people{
        {"Alice", 30, "NYC"},
        {"Bob", 25, "LA"},
        {"Charlie", 35, "NYC"},
        {"Diana", 28, "Chicago"},
        {"Eve", 32, "NYC"},
        {"Frank", 45, "LA"}
    };

    // Get names of people from NYC over 30
    std::cout << "NYC residents over 30: ";
    for (const auto& name : people
                           | views::filter([](const Person& p) {
                                 return p.city == "NYC" && p.age > 30;
                             })
                           | views::transform([](const Person& p) {
                                 return p.name;
                             })) {
        std::cout << name << ' ';
    }
    std::cout << "\n\n";

    // =========================================================================
    // Range algorithms with projections
    // =========================================================================
    std::cout << "--- Range algorithms with projections ---\n";

    std::vector<Person> sorted_people = people;

    // Sort by age using projection (cleaner than custom comparator)
    ranges::sort(sorted_people, {}, &Person::age);
    std::cout << "Sorted by age:\n";
    for (const auto& p : sorted_people) {
        std::cout << "  " << p << '\n';
    }

    // Sort by name
    ranges::sort(sorted_people, {}, &Person::name);
    std::cout << "Sorted by name:\n";
    for (const auto& p : sorted_people) {
        std::cout << "  " << p << '\n';
    }
    std::cout << '\n';

    // =========================================================================
    // Complex pipeline example
    // =========================================================================
    std::cout << "--- Complex pipeline ---\n";

    // Process a string: split, filter empty, transform, join
    std::string sentence = "  hello   world  from   C++  ranges  ";

    std::cout << "Original: '" << sentence << "'\n";
    std::cout << "Words (split by space, filter empty):\n";

    for (auto word : sentence | views::split(' ')) {
        auto sv = std::string_view(word.begin(), word.end());
        if (!sv.empty()) {
            std::cout << "  '" << sv << "'\n";
        }
    }
    std::cout << '\n';

    // =========================================================================
    // Storing pipelines in variables
    // =========================================================================
    std::cout << "--- Reusable pipeline adaptors ---\n";

    // Create a reusable adaptor
    auto even_squares = views::filter([](int n) { return n % 2 == 0; })
                      | views::transform([](int n) { return n * n; });

    std::vector<int> v1{1, 2, 3, 4, 5};
    std::vector<int> v2{10, 11, 12, 13, 14, 15};

    std::cout << "v1 | even_squares: ";
    for (int x : v1 | even_squares) {
        std::cout << x << ' ';
    }
    std::cout << '\n';

    std::cout << "v2 | even_squares: ";
    for (int x : v2 | even_squares) {
        std::cout << x << ' ';
    }
    std::cout << "\n\n";

    // =========================================================================
    // Working with maps
    // =========================================================================
    std::cout << "--- Pipelines with maps ---\n";

    std::map<std::string, int> scores{
        {"Alice", 95}, {"Bob", 72}, {"Charlie", 88},
        {"Diana", 91}, {"Eve", 67}
    };

    std::cout << "High scorers (>= 85):\n";
    for (const auto& [name, score] : scores
                                    | views::filter([](const auto& pair) {
                                          return pair.second >= 85;
                                      })) {
        std::cout << "  " << name << ": " << score << '\n';
    }

    // Using keys and values views
    std::cout << "All names: ";
    for (const auto& name : scores | views::keys) {
        std::cout << name << ' ';
    }
    std::cout << '\n';

    // Sum of all scores using range algorithm
    auto all_scores = scores | views::values;
    int total = 0;
    for (int s : all_scores) total += s;
    std::cout << "Total score: " << total << "\n\n";

    // =========================================================================
    // Enumerate-like pattern (index + value)
    // =========================================================================
    std::cout << "--- Enumerate pattern ---\n";

    std::vector<std::string> fruits{"apple", "banana", "cherry", "date"};

    // Manual enumerate using iota + transform
    std::cout << "Indexed fruits:\n";
    int idx = 0;
    for (const auto& fruit : fruits) {
        std::cout << "  [" << idx++ << "] " << fruit << '\n';
    }
    // Note: C++23 adds views::enumerate for this

    // =========================================================================
    // Zip-like pattern (parallel iteration)
    // =========================================================================
    std::cout << "\n--- Parallel iteration ---\n";

    std::vector<std::string> names{"Alice", "Bob", "Charlie"};
    std::vector<int> ages{30, 25, 35};

    std::cout << "Paired data (manual):\n";
    auto name_it = names.begin();
    auto age_it = ages.begin();
    while (name_it != names.end() && age_it != ages.end()) {
        std::cout << "  " << *name_it << " is " << *age_it << " years old\n";
        ++name_it;
        ++age_it;
    }
    // Note: C++23 adds views::zip for this

    // =========================================================================
    // Practical example: Log processing
    // =========================================================================
    std::cout << "\n--- Practical example: Processing logs ---\n";

    std::vector<std::string> logs{
        "INFO: Server started",
        "DEBUG: Connection from 192.168.1.1",
        "ERROR: Database connection failed",
        "INFO: Retrying connection",
        "ERROR: Timeout exceeded",
        "INFO: Connection restored"
    };

    std::cout << "Error logs:\n";
    for (const auto& log : logs
                          | views::filter([](const std::string& s) {
                                return s.starts_with("ERROR");
                            })) {
        std::cout << "  " << log << '\n';
    }

    // Count errors
    auto errors = logs | views::filter([](const std::string& s) {
        return s.starts_with("ERROR");
    });
    std::cout << "Total errors: " << ranges::distance(errors) << '\n';

    return 0;
}

// Key points:
// - Use | to chain views left-to-right (readable!)
// - Pipelines can be stored in variables and reused
// - Range algorithms accept ranges directly (no begin/end)
// - Projections simplify sorting/searching by member
// - C++23 adds views::enumerate and views::zip
// - Views are composable building blocks
