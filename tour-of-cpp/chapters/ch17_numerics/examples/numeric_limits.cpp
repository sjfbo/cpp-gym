// Numeric Limits
// Book reference: 17.7 Numeric Limits
//
// The <limits> header provides std::numeric_limits to query
// properties of arithmetic types at compile time.

#include <cmath>
#include <cstdint>
#include <format>
#include <iomanip>
#include <iostream>
#include <limits>

int main() {
    std::cout << "=== Numeric Limits ===\n\n";

    // =========================================================================
    // Integer type limits
    // =========================================================================
    std::cout << "--- Integer Type Limits ---\n";

    std::cout << std::format("Type{:>20}{:>25}\n", "Min", "Max");
    std::cout << std::string(60, '-') << '\n';

    std::cout << std::format("int8_t{:>20}{:>25}\n",
        static_cast<int>(std::numeric_limits<int8_t>::min()),
        static_cast<int>(std::numeric_limits<int8_t>::max()));

    std::cout << std::format("uint8_t{:>19}{:>25}\n",
        static_cast<int>(std::numeric_limits<uint8_t>::min()),
        static_cast<int>(std::numeric_limits<uint8_t>::max()));

    std::cout << std::format("int16_t{:>19}{:>25}\n",
        std::numeric_limits<int16_t>::min(),
        std::numeric_limits<int16_t>::max());

    std::cout << std::format("int32_t{:>19}{:>25}\n",
        std::numeric_limits<int32_t>::min(),
        std::numeric_limits<int32_t>::max());

    std::cout << std::format("int64_t{:>19}{:>25}\n",
        std::numeric_limits<int64_t>::min(),
        std::numeric_limits<int64_t>::max());

    std::cout << std::format("size_t{:>20}{:>25}\n",
        std::numeric_limits<std::size_t>::min(),
        std::numeric_limits<std::size_t>::max());
    std::cout << '\n';

    // =========================================================================
    // Floating-point limits
    // =========================================================================
    std::cout << "--- Floating-Point Limits ---\n";

    auto print_float_info = [](const char* name, auto min, auto max, auto eps,
                               int digits, int digits10) {
        std::cout << name << ":\n";
        std::cout << std::format("  min (positive): {:.6e}\n", min);
        std::cout << std::format("  max: {:.6e}\n", max);
        std::cout << std::format("  epsilon: {:.6e}\n", eps);
        std::cout << std::format("  digits (binary): {}\n", digits);
        std::cout << std::format("  digits10 (decimal): {}\n", digits10);
    };

    print_float_info("float",
        std::numeric_limits<float>::min(),
        std::numeric_limits<float>::max(),
        std::numeric_limits<float>::epsilon(),
        std::numeric_limits<float>::digits,
        std::numeric_limits<float>::digits10);

    print_float_info("\ndouble",
        std::numeric_limits<double>::min(),
        std::numeric_limits<double>::max(),
        std::numeric_limits<double>::epsilon(),
        std::numeric_limits<double>::digits,
        std::numeric_limits<double>::digits10);
    std::cout << '\n';

    // =========================================================================
    // Special floating-point values
    // =========================================================================
    std::cout << "--- Special Floating-Point Values ---\n";

    std::cout << std::format("infinity: {}\n",
        std::numeric_limits<double>::infinity());
    std::cout << std::format("-infinity: {}\n",
        -std::numeric_limits<double>::infinity());
    std::cout << std::format("quiet NaN: {}\n",
        std::numeric_limits<double>::quiet_NaN());
    std::cout << std::format("signaling NaN: {}\n",
        std::numeric_limits<double>::signaling_NaN());
    std::cout << std::format("lowest (most negative): {:.6e}\n",
        std::numeric_limits<double>::lowest());
    std::cout << std::format("denorm_min (smallest positive subnormal): {:.6e}\n",
        std::numeric_limits<double>::denorm_min());
    std::cout << '\n';

    // =========================================================================
    // Type properties
    // =========================================================================
    std::cout << "--- Type Properties ---\n";

    auto print_properties = [](const char* name, bool is_integer, bool is_signed,
                               bool is_exact, bool has_infinity, bool has_nan) {
        std::cout << std::format("{}: integer={}, signed={}, exact={}, "
                                 "has_inf={}, has_nan={}\n",
            name, is_integer, is_signed, is_exact, has_infinity, has_nan);
    };

    print_properties("int",
        std::numeric_limits<int>::is_integer,
        std::numeric_limits<int>::is_signed,
        std::numeric_limits<int>::is_exact,
        std::numeric_limits<int>::has_infinity,
        std::numeric_limits<int>::has_quiet_NaN);

    print_properties("unsigned int",
        std::numeric_limits<unsigned>::is_integer,
        std::numeric_limits<unsigned>::is_signed,
        std::numeric_limits<unsigned>::is_exact,
        std::numeric_limits<unsigned>::has_infinity,
        std::numeric_limits<unsigned>::has_quiet_NaN);

    print_properties("double",
        std::numeric_limits<double>::is_integer,
        std::numeric_limits<double>::is_signed,
        std::numeric_limits<double>::is_exact,
        std::numeric_limits<double>::has_infinity,
        std::numeric_limits<double>::has_quiet_NaN);
    std::cout << '\n';

    // =========================================================================
    // Practical use: Safe arithmetic
    // =========================================================================
    std::cout << "--- Practical Use: Safe Arithmetic ---\n";

    // Check for overflow before addition
    auto safe_add = [](int a, int b) -> std::pair<bool, int> {
        constexpr int max_int = std::numeric_limits<int>::max();
        constexpr int min_int = std::numeric_limits<int>::min();

        if (b > 0 && a > max_int - b) {
            return {false, 0};  // Would overflow
        }
        if (b < 0 && a < min_int - b) {
            return {false, 0};  // Would underflow
        }
        return {true, a + b};
    };

    auto [ok1, result1] = safe_add(100, 200);
    std::cout << std::format("safe_add(100, 200): ok={}, result={}\n", ok1, result1);

    auto [ok2, result2] = safe_add(std::numeric_limits<int>::max(), 1);
    std::cout << std::format("safe_add(INT_MAX, 1): ok={}, result={}\n", ok2, result2);

    // =========================================================================
    // Practical use: Epsilon comparison
    // =========================================================================
    std::cout << "\n--- Practical Use: Floating-Point Comparison ---\n";

    double x = 0.1 + 0.2;
    double y = 0.3;

    std::cout << std::format("0.1 + 0.2 = {:.17f}\n", x);
    std::cout << std::format("0.3 = {:.17f}\n", y);
    std::cout << std::format("0.1 + 0.2 == 0.3: {}\n", x == y);

    // Epsilon-based comparison
    auto approximately_equal = [](double a, double b,
                                  double rel_eps = std::numeric_limits<double>::epsilon() * 100) {
        double diff = std::abs(a - b);
        double larger = std::max(std::abs(a), std::abs(b));
        return diff <= larger * rel_eps;
    };

    std::cout << std::format("approximately_equal(0.1+0.2, 0.3): {}\n",
        approximately_equal(x, y));

    // =========================================================================
    // Using in templates
    // =========================================================================
    std::cout << "\n--- Using in Templates ---\n";

    auto generic_info = []<typename T>(const char* name) {
        std::cout << std::format("{}: {} bytes, ", name, sizeof(T));
        if constexpr (std::numeric_limits<T>::is_integer) {
            std::cout << "integer, ";
            std::cout << std::format("range [{}, {}]\n",
                std::numeric_limits<T>::min(), std::numeric_limits<T>::max());
        } else {
            std::cout << "floating-point, ";
            std::cout << std::format("precision {} digits\n",
                std::numeric_limits<T>::digits10);
        }
    };

    generic_info.operator()<char>("char");
    generic_info.operator()<short>("short");
    generic_info.operator()<int>("int");
    generic_info.operator()<long>("long");
    generic_info.operator()<float>("float");
    generic_info.operator()<double>("double");

    return 0;
}

// Key points:
// - std::numeric_limits<T> provides type properties at compile time
// - Use min(), max(), lowest() for range information
// - Use epsilon() for floating-point comparison tolerance
// - Check is_integer, is_signed, has_infinity, etc. for type traits
// - Use infinity(), quiet_NaN() for special values
// - digits10 gives decimal precision
