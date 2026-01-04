// Generators - Lazy Sequence Generation
// Book reference: 14.3 Generators
//
// This example shows how to create lazy sequences using views::iota
// and custom generator patterns.

#include <algorithm>
#include <cmath>
#include <iostream>
#include <ranges>
#include <vector>

namespace views = std::views;
namespace ranges = std::ranges;

// ============================================================================
// Helper to print ranges
// ============================================================================

template <std::ranges::range R>
void print_range(const std::string& label, R&& r, int max_elements = 20) {
    std::cout << label << ": ";
    int count = 0;
    for (auto&& x : r) {
        if (count++ >= max_elements) {
            std::cout << "...";
            break;
        }
        std::cout << x << ' ';
    }
    std::cout << '\n';
}

// ============================================================================
// Custom Fibonacci generator using iterators
// ============================================================================

class FibonacciRange {
public:
    class Iterator {
    public:
        using iterator_category = std::input_iterator_tag;
        using value_type = long long;
        using difference_type = std::ptrdiff_t;
        using pointer = const long long*;
        using reference = long long;

        Iterator() : a_(0), b_(1), pos_(0), end_pos_(0) {}
        explicit Iterator(int count) : a_(0), b_(1), pos_(0), end_pos_(count) {}

        reference operator*() const { return a_; }

        Iterator& operator++() {
            auto next = a_ + b_;
            a_ = b_;
            b_ = next;
            ++pos_;
            return *this;
        }

        Iterator operator++(int) {
            Iterator tmp = *this;
            ++(*this);
            return tmp;
        }

        bool operator==(const Iterator& other) const {
            return pos_ >= end_pos_ && other.end_pos_ == 0;
        }

        bool operator!=(const Iterator& other) const {
            return !(*this == other);
        }

    private:
        long long a_, b_;
        int pos_, end_pos_;
    };

    explicit FibonacciRange(int count) : count_(count) {}

    Iterator begin() const { return Iterator(count_); }
    Iterator end() const { return Iterator(); }

private:
    int count_;
};

// ============================================================================
// Generator functions using views::iota + transform
// ============================================================================

auto make_squares(int n) {
    return views::iota(1, n + 1)
           | views::transform([](int x) { return x * x; });
}

auto make_triangular_numbers(int n) {
    return views::iota(1, n + 1)
           | views::transform([](int n) { return n * (n + 1) / 2; });
}

auto make_powers_of_2(int n) {
    return views::iota(0, n)
           | views::transform([](int exp) { return 1LL << exp; });
}

// ============================================================================
// Infinite sequence generators
// ============================================================================

auto even_numbers() {
    return views::iota(1)
           | views::filter([](int n) { return n % 2 == 0; });
}

auto primes() {
    return views::iota(2)
           | views::filter([](int n) {
                 if (n < 2) return false;
                 if (n == 2) return true;
                 if (n % 2 == 0) return false;
                 for (int i = 3; i * i <= n; i += 2) {
                     if (n % i == 0) return false;
                 }
                 return true;
             });
}

// ============================================================================
// Collatz sequence
// ============================================================================

std::vector<long long> collatz_sequence(long long n) {
    std::vector<long long> seq;
    while (n != 1) {
        seq.push_back(n);
        n = (n % 2 == 0) ? n / 2 : 3 * n + 1;
    }
    seq.push_back(1);
    return seq;
}

int main() {
    std::cout << "=== Generators and Lazy Sequences ===\n\n";

    // =========================================================================
    // Using views::iota as a generator
    // =========================================================================
    std::cout << "--- views::iota ---\n";

    print_range("iota(1, 11)", views::iota(1, 11));
    print_range("First 10 from iota(100)", views::iota(100) | views::take(10));
    std::cout << '\n';

    // =========================================================================
    // Custom sequence generators
    // =========================================================================
    std::cout << "--- Custom generators with iota + transform ---\n";

    print_range("First 10 squares", make_squares(10));
    print_range("First 10 triangular numbers", make_triangular_numbers(10));
    print_range("First 10 powers of 2", make_powers_of_2(10));
    std::cout << '\n';

    // =========================================================================
    // Infinite sequences with take
    // =========================================================================
    std::cout << "--- Infinite sequences ---\n";

    print_range("First 10 even numbers", even_numbers() | views::take(10));
    print_range("First 15 primes", primes() | views::take(15));
    std::cout << '\n';

    // =========================================================================
    // Custom iterator-based generator (Fibonacci)
    // =========================================================================
    std::cout << "--- Fibonacci (custom iterator) ---\n";

    print_range("First 20 Fibonacci numbers", FibonacciRange(20));
    std::cout << '\n';

    // =========================================================================
    // Collatz sequence example
    // =========================================================================
    std::cout << "--- Collatz sequence ---\n";

    for (int start : {7, 27}) {
        auto seq = collatz_sequence(start);
        std::cout << "Collatz(" << start << ") length: " << seq.size() << '\n';
        print_range("  Sequence", seq);
    }
    std::cout << '\n';

    // =========================================================================
    // Combining generators with other views
    // =========================================================================
    std::cout << "--- Combining generators ---\n";

    std::cout << "Squares of first 5 even numbers: ";
    for (auto x : even_numbers()
                  | views::take(5)
                  | views::transform([](int n) { return n * n; })) {
        std::cout << x << ' ';
    }
    std::cout << '\n';

    // =========================================================================
    // Using std::generate to fill containers
    // =========================================================================
    std::cout << "\n--- std::generate for containers ---\n";

    std::vector<int> generated(10);
    int counter = 0;
    std::generate(generated.begin(), generated.end(), [&counter]() {
        return counter++;
    });
    print_range("Generated sequence", generated);

    return 0;
}

// Key points:
// - views::iota is the foundation for many generators
// - Combine iota with transform and filter for custom sequences
// - Infinite sequences are fine - just use take() to limit them
// - Custom generators can use iterator classes
// - C++23 adds std::generator for coroutine-based generators
