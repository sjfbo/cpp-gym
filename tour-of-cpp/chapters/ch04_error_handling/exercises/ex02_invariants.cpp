/**
 * Exercise 02: Class Invariants
 *
 * Implement a Date class that maintains strict invariants.
 *
 * Requirements:
 * 1. The Date must always represent a valid calendar date
 * 2. Year must be in range [1, 9999]
 * 3. Month must be in range [1, 12]
 * 4. Day must be valid for the given month and year
 * 5. Must handle leap years correctly
 *
 * Learning objectives:
 * - Understanding and enforcing class invariants
 * - Implementing preconditions and postconditions
 * - Defensive programming with proper error handling
 * - Designing APIs that prevent invalid states
 */

#include <iostream>
#include <string>
#include <stdexcept>
#include <cassert>
#include <array>
#include <compare>

// =============================================================================
// TODO: Implement the Date class
// =============================================================================

/**
 * A Date class that always represents a valid calendar date.
 *
 * Invariant: The date is always valid according to the Gregorian calendar
 * - year in [1, 9999]
 * - month in [1, 12]
 * - day in [1, days_in_month(year, month)]
 */
class Date {
public:
    /**
     * Construct a date from year, month, day.
     *
     * @throws std::out_of_range if the date is invalid
     *
     * Preconditions: year in [1,9999], month in [1,12], day valid for month/year
     * Postcondition: The Date represents the specified valid date
     */
    Date(int year, int month, int day)
        : year_(year), month_(month), day_(day) {
        // Validate year
        if (year < 1 || year > 9999) {
            throw std::out_of_range(
                "Year must be between 1 and 9999, got: " + std::to_string(year));
        }

        // Validate month
        if (month < 1 || month > 12) {
            throw std::out_of_range(
                "Month must be between 1 and 12, got: " + std::to_string(month));
        }

        // Validate day
        int max_day = days_in_month(year, month);
        if (day < 1 || day > max_day) {
            throw std::out_of_range(
                "Day must be between 1 and " + std::to_string(max_day) +
                " for " + std::to_string(year) + "-" + std::to_string(month) +
                ", got: " + std::to_string(day));
        }

        check_invariant();
    }

    // Accessors - maintain invariant (const, no state change)
    int year() const noexcept { return year_; }
    int month() const noexcept { return month_; }
    int day() const noexcept { return day_; }

    /**
     * Check if this date's year is a leap year.
     */
    bool is_leap_year() const noexcept {
        return is_leap_year(year_);
    }

    /**
     * Get the day of the week (0 = Sunday, 6 = Saturday).
     * Uses Zeller's congruence.
     */
    int day_of_week() const noexcept {
        int y = year_;
        int m = month_;

        if (m < 3) {
            m += 12;
            y -= 1;
        }

        int k = y % 100;
        int j = y / 100;

        int h = (day_ + (13 * (m + 1)) / 5 + k + k / 4 + j / 4 - 2 * j) % 7;
        return ((h + 6) % 7);  // Adjust to Sunday = 0
    }

    /**
     * Get the name of the day of the week.
     */
    std::string day_of_week_name() const {
        static const std::array<std::string, 7> names = {
            "Sunday", "Monday", "Tuesday", "Wednesday",
            "Thursday", "Friday", "Saturday"
        };
        return names[day_of_week()];
    }

    /**
     * Get the name of the month.
     */
    std::string month_name() const {
        static const std::array<std::string, 12> names = {
            "January", "February", "March", "April", "May", "June",
            "July", "August", "September", "October", "November", "December"
        };
        return names[month_ - 1];
    }

    /**
     * Add days to the date.
     * Postcondition: Returns a valid date that is `days` days after this date
     */
    Date add_days(int days) const {
        // Simple implementation: iterate day by day
        // A production implementation would be more efficient
        int y = year_;
        int m = month_;
        int d = day_;

        if (days >= 0) {
            while (days > 0) {
                int remaining_in_month = days_in_month(y, m) - d;
                if (days <= remaining_in_month) {
                    d += days;
                    days = 0;
                } else {
                    days -= (remaining_in_month + 1);
                    d = 1;
                    if (++m > 12) {
                        m = 1;
                        ++y;
                    }
                }
            }
        } else {
            days = -days;
            while (days > 0) {
                if (days < d) {
                    d -= days;
                    days = 0;
                } else {
                    days -= d;
                    if (--m < 1) {
                        m = 12;
                        --y;
                    }
                    d = days_in_month(y, m);
                }
            }
        }

        return Date(y, m, d);
    }

    /**
     * Add months to the date.
     * If the resulting day doesn't exist, clamps to end of month.
     */
    Date add_months(int months) const {
        int y = year_;
        int m = month_ + months;

        // Normalize months
        while (m > 12) {
            m -= 12;
            ++y;
        }
        while (m < 1) {
            m += 12;
            --y;
        }

        // Clamp day to valid range for new month
        int d = std::min(day_, days_in_month(y, m));

        return Date(y, m, d);
    }

    /**
     * Add years to the date.
     * Feb 29 becomes Feb 28 in non-leap years.
     */
    Date add_years(int years) const {
        int y = year_ + years;
        int d = day_;

        // Handle Feb 29 in non-leap years
        if (month_ == 2 && day_ == 29 && !is_leap_year(y)) {
            d = 28;
        }

        return Date(y, month_, d);
    }

    /**
     * Format the date as ISO 8601 (YYYY-MM-DD).
     */
    std::string to_string() const {
        char buffer[11];
        std::snprintf(buffer, sizeof(buffer), "%04d-%02d-%02d",
                      year_, month_, day_);
        return std::string(buffer);
    }

    // Comparison operators (C++20 spaceship operator)
    auto operator<=>(const Date&) const = default;
    bool operator==(const Date&) const = default;

    // Static helper functions
    static bool is_leap_year(int year) noexcept {
        return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
    }

    static int days_in_month(int year, int month) noexcept {
        static const std::array<int, 12> days = {
            31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
        };

        assert(month >= 1 && month <= 12);

        if (month == 2 && is_leap_year(year)) {
            return 29;
        }
        return days[month - 1];
    }

    static int days_in_year(int year) noexcept {
        return is_leap_year(year) ? 366 : 365;
    }

private:
    void check_invariant() const {
        // Invariant checks (active in debug builds)
        assert(year_ >= 1 && year_ <= 9999 && "Year out of range");
        assert(month_ >= 1 && month_ <= 12 && "Month out of range");
        assert(day_ >= 1 && day_ <= days_in_month(year_, month_) &&
               "Day out of range");
    }

    int year_;
    int month_;
    int day_;
};

// =============================================================================
// Stream output operator
// =============================================================================

std::ostream& operator<<(std::ostream& os, const Date& date) {
    return os << date.to_string();
}

// =============================================================================
// Tests
// =============================================================================

void test_construction() {
    std::cout << "=== Testing Date Construction ===\n\n";

    // Valid dates
    Date d1(2024, 1, 15);
    std::cout << "Created: " << d1 << " (" << d1.day_of_week_name() << ")\n";

    Date d2(2024, 2, 29);  // Leap year
    std::cout << "Created: " << d2 << " (leap year Feb 29)\n";

    Date d3(2023, 2, 28);  // Non-leap year
    std::cout << "Created: " << d3 << " (non-leap year Feb 28)\n";

    // Boundary dates
    Date d_min(1, 1, 1);
    std::cout << "Min date: " << d_min << '\n';

    Date d_max(9999, 12, 31);
    std::cout << "Max date: " << d_max << '\n';

    std::cout << '\n';
}

void test_invalid_construction() {
    std::cout << "=== Testing Invalid Date Construction ===\n\n";

    auto try_create = [](int y, int m, int d, const std::string& desc) {
        try {
            Date date(y, m, d);
            std::cout << "ERROR: Should have thrown for " << desc << '\n';
        }
        catch (const std::out_of_range& e) {
            std::cout << "Correctly rejected " << desc << ":\n  "
                      << e.what() << '\n';
        }
    };

    try_create(0, 1, 1, "year 0");
    try_create(10000, 1, 1, "year 10000");
    try_create(2024, 0, 1, "month 0");
    try_create(2024, 13, 1, "month 13");
    try_create(2024, 1, 0, "day 0");
    try_create(2024, 1, 32, "day 32 in January");
    try_create(2023, 2, 29, "Feb 29 in non-leap year");
    try_create(2024, 4, 31, "day 31 in April");

    std::cout << '\n';
}

void test_leap_year() {
    std::cout << "=== Testing Leap Year Logic ===\n\n";

    struct TestCase {
        int year;
        bool expected;
    };

    std::array<TestCase, 8> cases = {{
        {2000, true},   // Divisible by 400
        {1900, false},  // Divisible by 100 but not 400
        {2024, true},   // Divisible by 4 but not 100
        {2023, false},  // Not divisible by 4
        {2100, false},  // Divisible by 100 but not 400
        {2400, true},   // Divisible by 400
        {1600, true},   // Divisible by 400
        {1700, false}   // Divisible by 100 but not 400
    }};

    for (const auto& tc : cases) {
        bool actual = Date::is_leap_year(tc.year);
        std::cout << tc.year << " is "
                  << (actual ? "" : "not ")
                  << "a leap year: "
                  << (actual == tc.expected ? "PASS" : "FAIL")
                  << '\n';
    }

    std::cout << '\n';
}

void test_date_arithmetic() {
    std::cout << "=== Testing Date Arithmetic ===\n\n";

    Date d(2024, 1, 15);
    std::cout << "Starting date: " << d << '\n';

    // Add days
    std::cout << "  + 10 days: " << d.add_days(10) << '\n';
    std::cout << "  + 30 days: " << d.add_days(30) << '\n';
    std::cout << "  + 365 days: " << d.add_days(365) << '\n';
    std::cout << "  - 20 days: " << d.add_days(-20) << '\n';

    // Add months
    std::cout << "  + 1 month: " << d.add_months(1) << '\n';
    std::cout << "  + 12 months: " << d.add_months(12) << '\n';
    std::cout << "  - 2 months: " << d.add_months(-2) << '\n';

    // End of month handling
    Date end_jan(2024, 1, 31);
    std::cout << "\nJan 31, 2024 + 1 month: " << end_jan.add_months(1)
              << " (clamped to Feb 29)\n";

    // Leap year handling
    Date leap_day(2024, 2, 29);
    std::cout << "Feb 29, 2024 + 1 year: " << leap_day.add_years(1)
              << " (becomes Feb 28)\n";
    std::cout << "Feb 29, 2024 + 4 years: " << leap_day.add_years(4)
              << " (stays Feb 29)\n";

    std::cout << '\n';
}

void test_comparison() {
    std::cout << "=== Testing Date Comparison ===\n\n";

    Date d1(2024, 1, 15);
    Date d2(2024, 1, 20);
    Date d3(2024, 1, 15);
    Date d4(2023, 12, 31);

    std::cout << d1 << " == " << d3 << ": "
              << std::boolalpha << (d1 == d3) << '\n';
    std::cout << d1 << " < " << d2 << ": " << (d1 < d2) << '\n';
    std::cout << d1 << " > " << d4 << ": " << (d1 > d4) << '\n';
    std::cout << d1 << " != " << d2 << ": " << (d1 != d2) << '\n';

    std::cout << '\n';
}

void test_day_of_week() {
    std::cout << "=== Testing Day of Week ===\n\n";

    // Known dates for verification
    struct TestCase {
        int y, m, d;
        std::string expected_day;
    };

    std::array<TestCase, 5> cases = {{
        {2024, 1, 1, "Monday"},       // New Year 2024
        {2024, 7, 4, "Thursday"},     // US Independence Day 2024
        {2024, 12, 25, "Wednesday"},  // Christmas 2024
        {1969, 7, 20, "Sunday"},      // Moon landing
        {2000, 1, 1, "Saturday"}      // Y2K
    }};

    for (const auto& tc : cases) {
        Date d(tc.y, tc.m, tc.d);
        std::string actual = d.day_of_week_name();
        std::cout << d << " is " << actual << ": "
                  << (actual == tc.expected_day ? "PASS" : "FAIL")
                  << '\n';
    }

    std::cout << '\n';
}

void test_invariant_preservation() {
    std::cout << "=== Testing Invariant Preservation ===\n\n";

    // All operations should produce valid dates
    Date start(2024, 1, 15);

    // Series of operations
    Date d = start;
    d = d.add_days(1000);
    d = d.add_months(-15);
    d = d.add_years(5);
    d = d.add_days(-500);

    std::cout << "After chain of operations: " << d << '\n';
    std::cout << "Date is valid: "
              << (d.year() >= 1 && d.year() <= 9999 &&
                  d.month() >= 1 && d.month() <= 12 &&
                  d.day() >= 1 && d.day() <= Date::days_in_month(d.year(), d.month())
                  ? "YES" : "NO")
              << '\n';

    // Edge case: Feb 29 + 1 year should still be valid
    Date feb29(2024, 2, 29);
    Date next_year = feb29.add_years(1);
    std::cout << "\nFeb 29 + 1 year: " << next_year << '\n';
    std::cout << "Result is valid: "
              << (next_year.day() <= Date::days_in_month(next_year.year(),
                                                         next_year.month())
                  ? "YES" : "NO")
              << '\n';

    std::cout << '\n';
}

int main() {
    std::cout << "Exercise 02: Class Invariants - Date Class\n";
    std::cout << "==========================================\n\n";

    test_construction();
    test_invalid_construction();
    test_leap_year();
    test_date_arithmetic();
    test_comparison();
    test_day_of_week();
    test_invariant_preservation();

    std::cout << "All tests completed.\n";
    return 0;
}
