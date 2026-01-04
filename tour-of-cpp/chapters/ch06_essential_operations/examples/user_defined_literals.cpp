// Chapter 6: Essential Operations - User-Defined Literals
//
// This example demonstrates:
// - Creating custom literal suffixes
// - Cooked vs raw literals
// - Compile-time literal computation
// - Practical applications: units, string types, etc.

#include <chrono>
#include <cstddef>
#include <iostream>
#include <string>
#include <string_view>

// =============================================================================
// 1. Basic User-Defined Literals: Distance Units
// =============================================================================

namespace distance {

// Strong type for distance in meters
class Meters {
    double value_;

public:
    constexpr explicit Meters(double v) : value_{v} {}
    [[nodiscard]] constexpr double value() const { return value_; }

    // Arithmetic
    constexpr Meters operator+(Meters other) const {
        return Meters{value_ + other.value_};
    }

    constexpr Meters operator-(Meters other) const {
        return Meters{value_ - other.value_};
    }

    constexpr Meters operator*(double scalar) const {
        return Meters{value_ * scalar};
    }

    constexpr Meters operator/(double scalar) const {
        return Meters{value_ / scalar};
    }

    // Comparison
    constexpr auto operator<=>(const Meters& other) const = default;

    friend std::ostream& operator<<(std::ostream& os, Meters m) {
        return os << m.value_ << " m";
    }
};

// User-defined literals for distance
// Note: Must be in namespace or inline namespace for ADL

// Meters
constexpr Meters operator""_m(long double v) {
    return Meters{static_cast<double>(v)};
}

constexpr Meters operator""_m(unsigned long long v) {
    return Meters{static_cast<double>(v)};
}

// Kilometers
constexpr Meters operator""_km(long double v) {
    return Meters{static_cast<double>(v) * 1000.0};
}

constexpr Meters operator""_km(unsigned long long v) {
    return Meters{static_cast<double>(v) * 1000.0};
}

// Centimeters
constexpr Meters operator""_cm(long double v) {
    return Meters{static_cast<double>(v) / 100.0};
}

constexpr Meters operator""_cm(unsigned long long v) {
    return Meters{static_cast<double>(v) / 100.0};
}

// Miles
constexpr Meters operator""_mi(long double v) {
    return Meters{static_cast<double>(v) * 1609.344};
}

constexpr Meters operator""_mi(unsigned long long v) {
    return Meters{static_cast<double>(v) * 1609.344};
}

}  // namespace distance

// =============================================================================
// 2. Time Literals (Similar to std::chrono::literals)
// =============================================================================

namespace time_literals {

// Strong type for duration in seconds
class Seconds {
    double value_;

public:
    constexpr explicit Seconds(double v) : value_{v} {}
    [[nodiscard]] constexpr double value() const { return value_; }

    constexpr Seconds operator+(Seconds other) const {
        return Seconds{value_ + other.value_};
    }

    constexpr Seconds operator-(Seconds other) const {
        return Seconds{value_ - other.value_};
    }

    constexpr auto operator<=>(const Seconds& other) const = default;

    friend std::ostream& operator<<(std::ostream& os, Seconds s) {
        return os << s.value_ << " s";
    }
};

// Seconds
constexpr Seconds operator""_s(long double v) {
    return Seconds{static_cast<double>(v)};
}

constexpr Seconds operator""_s(unsigned long long v) {
    return Seconds{static_cast<double>(v)};
}

// Minutes
constexpr Seconds operator""_min(long double v) {
    return Seconds{static_cast<double>(v) * 60.0};
}

constexpr Seconds operator""_min(unsigned long long v) {
    return Seconds{static_cast<double>(v) * 60.0};
}

// Hours
constexpr Seconds operator""_h(long double v) {
    return Seconds{static_cast<double>(v) * 3600.0};
}

constexpr Seconds operator""_h(unsigned long long v) {
    return Seconds{static_cast<double>(v) * 3600.0};
}

// Milliseconds
constexpr Seconds operator""_ms(long double v) {
    return Seconds{static_cast<double>(v) / 1000.0};
}

constexpr Seconds operator""_ms(unsigned long long v) {
    return Seconds{static_cast<double>(v) / 1000.0};
}

}  // namespace time_literals

// =============================================================================
// 3. String Literals
// =============================================================================

namespace string_literals {

// A simple "encoded" string type (for demonstration)
class EncodedString {
    std::string data_;

public:
    explicit EncodedString(std::string_view sv) {
        // Simple "encoding": shift each character by 1
        data_.reserve(sv.size());
        for (char c : sv) {
            data_ += static_cast<char>(c + 1);
        }
    }

    [[nodiscard]] std::string decode() const {
        std::string result;
        result.reserve(data_.size());
        for (char c : data_) {
            result += static_cast<char>(c - 1);
        }
        return result;
    }

    [[nodiscard]] const std::string& encoded() const { return data_; }
};

// String literal operator (takes const char* and size)
EncodedString operator""_enc(const char* str, std::size_t len) {
    return EncodedString{std::string_view{str, len}};
}

}  // namespace string_literals

// =============================================================================
// 4. Compile-Time Computation with Literals
// =============================================================================

namespace binary_literals {

// Parse binary string at compile time
consteval unsigned long long parse_binary(const char* str) {
    unsigned long long result = 0;
    while (*str) {
        result <<= 1;
        if (*str == '1') {
            result |= 1;
        } else if (*str != '0') {
            throw "Invalid binary digit";  // Compile-time error
        }
        ++str;
    }
    return result;
}

// Raw literal operator (gets the literal as a string)
template<char... Chars>
consteval unsigned long long operator""_b() {
    constexpr char str[] = {Chars..., '\0'};
    return parse_binary(str);
}

}  // namespace binary_literals

// =============================================================================
// 5. Complex Number Literals
// =============================================================================

namespace complex_literals {

class Complex {
    double re_, im_;

public:
    constexpr Complex(double re = 0.0, double im = 0.0) : re_{re}, im_{im} {}

    [[nodiscard]] constexpr double real() const { return re_; }
    [[nodiscard]] constexpr double imag() const { return im_; }

    constexpr Complex operator+(Complex other) const {
        return Complex{re_ + other.re_, im_ + other.im_};
    }

    constexpr Complex operator-(Complex other) const {
        return Complex{re_ - other.re_, im_ - other.im_};
    }

    constexpr Complex operator*(Complex other) const {
        return Complex{
            re_ * other.re_ - im_ * other.im_,
            re_ * other.im_ + im_ * other.re_
        };
    }

    friend std::ostream& operator<<(std::ostream& os, Complex c) {
        os << c.re_;
        if (c.im_ >= 0) os << "+";
        os << c.im_ << "i";
        return os;
    }

    // Allow double + Complex (needed for 3.0 + 4.0_i syntax)
    friend constexpr Complex operator+(double lhs, Complex rhs) {
        return Complex{lhs + rhs.re_, rhs.im_};
    }
};

// Imaginary unit literal
constexpr Complex operator""_i(long double v) {
    return Complex{0.0, static_cast<double>(v)};
}

constexpr Complex operator""_i(unsigned long long v) {
    return Complex{0.0, static_cast<double>(v)};
}

}  // namespace complex_literals

// =============================================================================
// 6. Percentage Literal
// =============================================================================

namespace percent_literals {

class Percentage {
    double value_;  // Stored as decimal (0.0 to 1.0)

public:
    constexpr explicit Percentage(double v) : value_{v / 100.0} {}

    [[nodiscard]] constexpr double as_decimal() const { return value_; }
    [[nodiscard]] constexpr double as_percent() const { return value_ * 100.0; }

    constexpr double apply_to(double base) const {
        return base * value_;
    }

    friend std::ostream& operator<<(std::ostream& os, Percentage p) {
        return os << p.as_percent() << "%";
    }
};

constexpr Percentage operator""_pct(long double v) {
    return Percentage{static_cast<double>(v)};
}

constexpr Percentage operator""_pct(unsigned long long v) {
    return Percentage{static_cast<double>(v)};
}

}  // namespace percent_literals

// =============================================================================
// Main Demonstration
// =============================================================================

int main() {
    std::cout << "=== User-Defined Literals Demo ===\n";

    // -------------------------------------------------------------------------
    std::cout << "\n--- Distance Literals ---\n";
    {
        using namespace distance;

        constexpr auto marathon = 42.195_km;
        constexpr auto sprint = 100_m;
        constexpr auto small = 50_cm;
        constexpr auto mile = 1_mi;

        std::cout << "Marathon: " << marathon << "\n";
        std::cout << "Sprint: " << sprint << "\n";
        std::cout << "Small distance: " << small << "\n";
        std::cout << "One mile: " << mile << "\n";

        constexpr auto total = 1_km + 500_m + 200_cm;
        std::cout << "1_km + 500_m + 200_cm = " << total << "\n";

        // Compile-time computation
        static_assert((1_km).value() == 1000.0);
        static_assert((2_km + 500_m).value() == 2500.0);
    }

    // -------------------------------------------------------------------------
    std::cout << "\n--- Time Literals ---\n";
    {
        using namespace time_literals;

        constexpr auto duration = 1_h + 30_min + 45_s;
        std::cout << "1_h + 30_min + 45_s = " << duration << "\n";

        constexpr auto quick = 500_ms;
        std::cout << "500_ms = " << quick << "\n";

        // Comparison works
        static_assert(1_h > 30_min);
        static_assert(60_s == 1_min);
    }

    // -------------------------------------------------------------------------
    std::cout << "\n--- String Literals ---\n";
    {
        using namespace string_literals;

        auto secret = "Hello, World!"_enc;
        std::cout << "Original: Hello, World!\n";
        std::cout << "Encoded: " << secret.encoded() << "\n";
        std::cout << "Decoded: " << secret.decode() << "\n";
    }

    // -------------------------------------------------------------------------
    std::cout << "\n--- Complex Literals ---\n";
    {
        using namespace complex_literals;

        constexpr auto z1 = 3.0 + 4.0_i;
        constexpr auto z2 = 1.0 + 2.0_i;

        std::cout << "z1 = " << z1 << "\n";
        std::cout << "z2 = " << z2 << "\n";
        std::cout << "z1 + z2 = " << (z1 + z2) << "\n";
        std::cout << "z1 * z2 = " << (z1 * z2) << "\n";

        // Pure imaginary
        constexpr auto pure_i = 1_i;
        std::cout << "i * i = " << (pure_i * pure_i) << " (should be -1)\n";
    }

    // -------------------------------------------------------------------------
    std::cout << "\n--- Percentage Literals ---\n";
    {
        using namespace percent_literals;

        constexpr auto tax_rate = 8.25_pct;
        constexpr auto discount = 20_pct;

        double price = 100.0;
        std::cout << "Price: $" << price << "\n";
        std::cout << "Tax rate: " << tax_rate << "\n";
        std::cout << "Tax amount: $" << tax_rate.apply_to(price) << "\n";

        std::cout << "Discount: " << discount << "\n";
        std::cout << "Discount amount: $" << discount.apply_to(price) << "\n";
    }

    // -------------------------------------------------------------------------
    std::cout << "\n--- Standard Library Literals ---\n";
    {
        using namespace std::literals;

        // std::string literal (avoids constructing from const char*)
        auto str = "Hello, std::string!"s;
        std::cout << "String: " << str << " (size: " << str.size() << ")\n";

        // std::string_view literal
        auto sv = "Hello, string_view!"sv;
        std::cout << "String view: " << sv << " (size: " << sv.size() << ")\n";

        // Chrono literals
        using namespace std::chrono_literals;
        auto duration = 2h + 30min + 15s + 500ms;
        std::cout << "Duration: " << duration.count() << " ms\n";
    }

    std::cout << "\n=== End of Demo ===\n";
    return 0;
}
