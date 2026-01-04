// Views - Lazy Range Adaptors
// Book reference: 14.2 Views
//
// Views are lightweight, non-owning ranges that transform elements lazily.
// They don't copy data; they compute values on-demand during iteration.

#include <algorithm>
#include <iostream>
#include <ranges>
#include <string>
#include <vector>

int main() {
    std::cout << "=== C++20 Range Views ===\n\n";

    namespace views = std::views;

    std::vector<int> nums{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    // =========================================================================
    // Basic views: filter and transform
    // =========================================================================
    std::cout << "--- filter and transform ---\n";

    // filter: keep elements matching a predicate
    std::cout << "Even numbers: ";
    for (int x : nums | views::filter([](int n) { return n % 2 == 0; })) {
        std::cout << x << ' ';
    }
    std::cout << '\n';

    // transform: apply a function to each element
    std::cout << "Squares: ";
    for (int x : nums | views::transform([](int n) { return n * n; })) {
        std::cout << x << ' ';
    }
    std::cout << '\n';

    // Combining filter and transform
    std::cout << "Squares of evens: ";
    for (int x : nums
                 | views::filter([](int n) { return n % 2 == 0; })
                 | views::transform([](int n) { return n * n; })) {
        std::cout << x << ' ';
    }
    std::cout << "\n\n";

    // =========================================================================
    // take and drop
    // =========================================================================
    std::cout << "--- take and drop ---\n";

    std::cout << "First 5: ";
    for (int x : nums | views::take(5)) {
        std::cout << x << ' ';
    }
    std::cout << '\n';

    std::cout << "Skip first 5: ";
    for (int x : nums | views::drop(5)) {
        std::cout << x << ' ';
    }
    std::cout << '\n';

    std::cout << "Elements 3-7 (drop 2, take 5): ";
    for (int x : nums | views::drop(2) | views::take(5)) {
        std::cout << x << ' ';
    }
    std::cout << "\n\n";

    // =========================================================================
    // take_while and drop_while
    // =========================================================================
    std::cout << "--- take_while and drop_while ---\n";

    std::vector<int> data{1, 2, 3, 10, 4, 5, 6};

    std::cout << "Original: ";
    for (int x : data) std::cout << x << ' ';
    std::cout << '\n';

    std::cout << "take_while(< 10): ";
    for (int x : data | views::take_while([](int n) { return n < 10; })) {
        std::cout << x << ' ';
    }
    std::cout << '\n';

    std::cout << "drop_while(< 5): ";
    for (int x : data | views::drop_while([](int n) { return n < 5; })) {
        std::cout << x << ' ';
    }
    std::cout << "\n\n";

    // =========================================================================
    // reverse
    // =========================================================================
    std::cout << "--- reverse ---\n";

    std::cout << "Reversed: ";
    for (int x : nums | views::reverse) {
        std::cout << x << ' ';
    }
    std::cout << '\n';

    std::cout << "Last 3 (reverse, take 3, reverse): ";
    for (int x : nums | views::reverse | views::take(3) | views::reverse) {
        std::cout << x << ' ';
    }
    std::cout << "\n\n";

    // =========================================================================
    // iota: generate sequences
    // =========================================================================
    std::cout << "--- iota (sequence generator) ---\n";

    std::cout << "iota(1, 6): ";
    for (int x : views::iota(1, 6)) {
        std::cout << x << ' ';
    }
    std::cout << '\n';

    std::cout << "First 5 from infinite iota(100): ";
    for (int x : views::iota(100) | views::take(5)) {
        std::cout << x << ' ';
    }
    std::cout << "\n\n";

    // =========================================================================
    // elements, keys, values (for pairs/tuples)
    // =========================================================================
    std::cout << "--- elements, keys, values ---\n";

    std::vector<std::pair<std::string, int>> scores{
        {"Alice", 95}, {"Bob", 87}, {"Charlie", 92}
    };

    std::cout << "Names (keys): ";
    for (const auto& name : scores | views::keys) {
        std::cout << name << ' ';
    }
    std::cout << '\n';

    std::cout << "Scores (values): ";
    for (int score : scores | views::values) {
        std::cout << score << ' ';
    }
    std::cout << "\n\n";

    // =========================================================================
    // split: split string by delimiter
    // =========================================================================
    std::cout << "--- split ---\n";

    std::string csv = "apple,banana,cherry,date";
    std::cout << "Splitting '" << csv << "' by ',':\n";

    for (auto word : csv | views::split(',')) {
        std::cout << "  '" << std::string_view(word.begin(), word.end()) << "'\n";
    }
    std::cout << '\n';

    // =========================================================================
    // join: flatten nested ranges
    // =========================================================================
    std::cout << "--- join ---\n";

    std::vector<std::vector<int>> nested{{1, 2}, {3, 4, 5}, {6}};
    std::cout << "Nested: [[1,2], [3,4,5], [6]]\n";
    std::cout << "Joined: ";
    for (int x : nested | views::join) {
        std::cout << x << ' ';
    }
    std::cout << "\n\n";

    // =========================================================================
    // Views are lazy - demonstration
    // =========================================================================
    std::cout << "--- Laziness demonstration ---\n";

    int transform_count = 0;
    auto lazy_view = nums
        | views::filter([](int n) { return n % 2 == 0; })
        | views::transform([&transform_count](int x) {
              ++transform_count;
              return x * 2;
          })
        | views::take(2);

    std::cout << "View created, transforms called: " << transform_count << '\n';

    std::cout << "First two even numbers doubled: ";
    for (int x : lazy_view) {
        std::cout << x << ' ';
    }
    std::cout << '\n';
    std::cout << "After iteration, transforms called: " << transform_count << '\n';

    return 0;
}

// Key points:
// - Views are lazy: they compute on-demand, not upfront
// - Views don't own data: they reference the underlying range
// - Use | to compose views into pipelines
// - views::iota can generate infinite sequences
// - views::filter, transform, take, drop are the most common
