// Formatting - Modern String Formatting with std::format (C++20)
// Book reference: 11.6 Formatting
//
// This example demonstrates std::format for type-safe, Python-style
// string formatting introduced in C++20.

#include <format>
#include <iostream>
#include <numbers>
#include <string>
#include <vector>

int main() {
    std::cout << "=== Modern Formatting with std::format ===\n\n";

    // ========================================================================
    // Basic Formatting
    // ========================================================================
    std::cout << "--- Basic Formatting ---\n";

    // Simple substitution
    std::string hello = std::format("Hello, {}!", "World");
    std::cout << hello << '\n';

    // Multiple arguments
    std::string multi = std::format("{} + {} = {}", 2, 3, 5);
    std::cout << multi << '\n';

    // Mixed types
    std::string mixed = std::format("Int: {}, Double: {}, String: {}",
                                     42, 3.14, "text");
    std::cout << mixed << '\n';
    std::cout << '\n';

    // ========================================================================
    // Positional Arguments
    // ========================================================================
    std::cout << "--- Positional Arguments ---\n";

    // Reorder arguments by index
    std::string reorder = std::format("{1} before {0}", "second", "first");
    std::cout << reorder << '\n';

    // Repeat arguments
    std::string repeat = std::format("{0} {0} {0}", "echo");
    std::cout << repeat << '\n';
    std::cout << '\n';

    // ========================================================================
    // Width and Alignment
    // ========================================================================
    std::cout << "--- Width and Alignment ---\n";

    // Right-aligned (default for numbers)
    std::cout << std::format("|{:10}|\n", 42);

    // Left-aligned
    std::cout << std::format("|{:<10}|\n", 42);

    // Right-aligned explicit
    std::cout << std::format("|{:>10}|\n", 42);

    // Center-aligned
    std::cout << std::format("|{:^10}|\n", 42);

    // Custom fill character
    std::cout << std::format("|{:*^10}|\n", 42);
    std::cout << std::format("|{:0>10}|\n", 42);
    std::cout << '\n';

    // ========================================================================
    // Integer Formatting
    // ========================================================================
    std::cout << "--- Integer Formatting ---\n";

    int value = 255;
    std::cout << std::format("Decimal:     {}\n", value);
    std::cout << std::format("Decimal:     {:d}\n", value);
    std::cout << std::format("Binary:      {:b}\n", value);
    std::cout << std::format("Octal:       {:o}\n", value);
    std::cout << std::format("Hex (lower): {:x}\n", value);
    std::cout << std::format("Hex (upper): {:X}\n", value);

    // With prefix
    std::cout << std::format("Binary:      {:#b}\n", value);
    std::cout << std::format("Octal:       {:#o}\n", value);
    std::cout << std::format("Hex:         {:#x}\n", value);

    // Thousands separator
    std::cout << std::format("With separator: {:L}\n", 1000000);
    std::cout << '\n';

    // ========================================================================
    // Floating-Point Formatting
    // ========================================================================
    std::cout << "--- Floating-Point Formatting ---\n";

    double pi = std::numbers::pi;
    std::cout << std::format("Default:     {}\n", pi);
    std::cout << std::format("Fixed:       {:f}\n", pi);
    std::cout << std::format("Scientific:  {:e}\n", pi);
    std::cout << std::format("General:     {:g}\n", pi);
    std::cout << std::format("Hex float:   {:a}\n", pi);

    // Precision
    std::cout << std::format("2 decimals:  {:.2f}\n", pi);
    std::cout << std::format("6 decimals:  {:.6f}\n", pi);
    std::cout << std::format("10 decimals: {:.10f}\n", pi);

    // Width and precision combined
    std::cout << std::format("Width 12, 4 decimals: |{:12.4f}|\n", pi);
    std::cout << '\n';

    // ========================================================================
    // String Formatting
    // ========================================================================
    std::cout << "--- String Formatting ---\n";

    std::string text = "Hello";
    std::cout << std::format("Default:   |{}|\n", text);
    std::cout << std::format("Width 10:  |{:10}|\n", text);
    std::cout << std::format("Left:      |{:<10}|\n", text);
    std::cout << std::format("Center:    |{:^10}|\n", text);
    std::cout << std::format("Truncate:  |{:.3}|\n", text);
    std::cout << '\n';

    // ========================================================================
    // Boolean and Pointer Formatting
    // ========================================================================
    std::cout << "--- Boolean and Pointer ---\n";

    std::cout << std::format("Bool default: {}\n", true);
    std::cout << std::format("Bool as string: {:s}\n", true);

    int x = 42;
    std::cout << std::format("Pointer: {}\n", static_cast<void*>(&x));
    std::cout << '\n';

    // ========================================================================
    // Practical Examples
    // ========================================================================
    std::cout << "--- Practical Examples ---\n";

    // Table formatting
    std::cout << std::format("{:<15} {:>10} {:>10}\n", "Item", "Qty", "Price");
    std::cout << std::format("{:-<15} {:->10} {:->10}\n", "", "", "");
    std::cout << std::format("{:<15} {:>10} {:>10.2f}\n", "Apple", 5, 1.99);
    std::cout << std::format("{:<15} {:>10} {:>10.2f}\n", "Banana", 12, 0.50);
    std::cout << std::format("{:<15} {:>10} {:>10.2f}\n", "Cherry", 100, 0.10);
    std::cout << '\n';

    // Progress bar
    int progress = 75;
    int bar_width = 20;
    int filled = progress * bar_width / 100;
    std::cout << std::format("[{:=>{}}{:>{}}] {}%\n",
                             "", filled,
                             "", bar_width - filled,
                             progress);

    // Debug output
    std::string name = "test";
    int count = 42;
    double ratio = 0.75;
    std::cout << std::format("Debug: name='{}', count={}, ratio={:.2f}\n",
                             name, count, ratio);

    return 0;
}

// Key points:
// - std::format uses {} placeholders (like Python)
// - Format spec: {[index]:[fill][align][width][.precision][type]}
// - Align: < (left), > (right), ^ (center)
// - Types: d (int), b (binary), o (octal), x (hex), f (fixed), e (scientific)
// - # adds prefix (0b, 0o, 0x)
// - Much safer and more readable than printf
