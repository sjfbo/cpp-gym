// std::chrono - Time Points, Durations, and Clocks
// Book reference: 16.2 Time
//
// The <chrono> library provides type-safe time handling:
// - Durations: time intervals with compile-time unit tracking
// - Time points: specific instants in time
// - Clocks: sources of time (system, steady, high resolution)
// - C++20 adds calendar and time zone support

#include <chrono>
#include <iostream>
#include <thread>
#include <iomanip>
#include <sstream>

// Enable chrono literals (1s, 100ms, etc.)
using namespace std::chrono_literals;

// ============================================================================
// Durations
// ============================================================================

void duration_basics() {
    std::cout << "=== Duration Basics ===\n";

    // Create durations with explicit types
    std::chrono::hours h{2};
    std::chrono::minutes m{30};
    std::chrono::seconds s{45};
    [[maybe_unused]] std::chrono::milliseconds ms{500};
    [[maybe_unused]] std::chrono::microseconds us{1000};
    [[maybe_unused]] std::chrono::nanoseconds ns{1000000};

    // Using literals (more readable)
    [[maybe_unused]] auto h2 = 2h;
    [[maybe_unused]] auto m2 = 30min;
    [[maybe_unused]] auto s2 = 45s;
    [[maybe_unused]] auto ms2 = 500ms;
    [[maybe_unused]] auto us2 = 1000us;
    [[maybe_unused]] auto ns2 = 1000000ns;

    std::cout << "2 hours = " << h.count() << " hours\n";
    std::cout << "30 min = " << m.count() << " minutes\n";
    std::cout << "45 sec = " << s.count() << " seconds\n";

    // Arithmetic operations
    auto total_time = 1h + 30min + 45s;
    std::cout << "1h + 30min + 45s = "
              << std::chrono::duration_cast<std::chrono::seconds>(total_time).count()
              << " seconds\n";

    // Multiplication and division
    auto doubled = 5s * 2;
    auto halved = 10s / 2;
    std::cout << "5s * 2 = " << doubled.count() << "s\n";
    std::cout << "10s / 2 = " << halved.count() << "s\n";

    // Comparison
    std::cout << "1h > 59min: " << std::boolalpha << (1h > 59min) << "\n";
    std::cout << "1000ms == 1s: " << (1000ms == 1s) << "\n";
}

void duration_conversions() {
    std::cout << "\n=== Duration Conversions ===\n";

    auto duration = 2h + 30min + 45s + 500ms;

    // Implicit conversion to finer granularity (no precision loss)
    std::chrono::milliseconds as_ms = duration;
    std::cout << "Total: " << as_ms.count() << " milliseconds\n";

    // Explicit cast required for coarser granularity (precision loss)
    auto as_seconds = std::chrono::duration_cast<std::chrono::seconds>(duration);
    auto as_minutes = std::chrono::duration_cast<std::chrono::minutes>(duration);
    auto as_hours = std::chrono::duration_cast<std::chrono::hours>(duration);

    std::cout << "As seconds: " << as_seconds.count() << "\n";
    std::cout << "As minutes: " << as_minutes.count() << "\n";
    std::cout << "As hours: " << as_hours.count() << "\n";

    // floor, ceil, round (C++17)
    auto ms = 1500ms;
    std::cout << "\n1500ms:\n";
    std::cout << "  floor to seconds: "
              << std::chrono::floor<std::chrono::seconds>(ms).count() << "s\n";
    std::cout << "  ceil to seconds: "
              << std::chrono::ceil<std::chrono::seconds>(ms).count() << "s\n";
    std::cout << "  round to seconds: "
              << std::chrono::round<std::chrono::seconds>(ms).count() << "s\n";

    // Custom duration types
    using days = std::chrono::duration<int, std::ratio<86400>>;
    using weeks = std::chrono::duration<int, std::ratio<604800>>;

    days d{7};
    weeks w{1};
    std::cout << "\n7 days == 1 week: " << (d == w) << "\n";
}

// ============================================================================
// Time Points
// ============================================================================

void time_point_basics() {
    std::cout << "\n=== Time Points ===\n";

    // Get current time
    auto now = std::chrono::system_clock::now();
    std::cout << "Time since epoch: "
              << std::chrono::duration_cast<std::chrono::seconds>(
                     now.time_since_epoch()).count()
              << " seconds\n";

    // Time point arithmetic
    auto future = now + 24h;
    [[maybe_unused]] auto past = now - 1h;

    auto diff = future - now;
    std::cout << "Difference: "
              << std::chrono::duration_cast<std::chrono::hours>(diff).count()
              << " hours\n";

    // Convert to time_t for traditional formatting
    auto time_t_now = std::chrono::system_clock::to_time_t(now);
    std::cout << "Current time: " << std::ctime(&time_t_now);

    // Convert from time_t
    auto from_time_t = std::chrono::system_clock::from_time_t(time_t_now);
    (void)from_time_t;  // Suppress unused warning
}

// ============================================================================
// Different Clocks
// ============================================================================

void clock_types() {
    std::cout << "\n=== Clock Types ===\n";

    // system_clock - wall clock time, can be adjusted
    auto sys_now = std::chrono::system_clock::now();
    std::cout << "system_clock:\n";
    std::cout << "  Is steady: " << std::boolalpha
              << std::chrono::system_clock::is_steady << "\n";

    // steady_clock - monotonic, for measuring intervals
    auto steady_now = std::chrono::steady_clock::now();
    std::cout << "steady_clock:\n";
    std::cout << "  Is steady: " << std::chrono::steady_clock::is_steady << "\n";

    // high_resolution_clock - highest precision available
    auto hr_now = std::chrono::high_resolution_clock::now();
    std::cout << "high_resolution_clock:\n";
    std::cout << "  Is steady: "
              << std::chrono::high_resolution_clock::is_steady << "\n";

    // Note: high_resolution_clock is typically an alias for steady_clock or system_clock
    std::cout << "  (Usually an alias for "
              << (std::is_same_v<std::chrono::high_resolution_clock,
                                  std::chrono::steady_clock>
                      ? "steady_clock"
                      : "system_clock")
              << ")\n";

    (void)sys_now;
    (void)steady_now;
    (void)hr_now;
}

// ============================================================================
// Measuring Elapsed Time
// ============================================================================

void stopwatch_example() {
    std::cout << "\n=== Measuring Elapsed Time ===\n";

    // Always use steady_clock for measuring elapsed time
    // (not affected by system time changes)

    auto start = std::chrono::steady_clock::now();

    // Simulate some work
    std::this_thread::sleep_for(100ms);
    long sum = 0;
    for (int i = 0; i < 1000000; ++i) {
        sum += i;
    }

    auto end = std::chrono::steady_clock::now();

    // Calculate elapsed time
    auto elapsed = end - start;

    std::cout << "Elapsed time:\n";
    std::cout << "  Nanoseconds: "
              << std::chrono::duration_cast<std::chrono::nanoseconds>(elapsed).count()
              << "\n";
    std::cout << "  Microseconds: "
              << std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count()
              << "\n";
    std::cout << "  Milliseconds: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count()
              << "\n";

    // Using floating-point duration for more precision
    std::chrono::duration<double, std::milli> elapsed_ms = elapsed;
    std::cout << "  Milliseconds (float): " << std::fixed << std::setprecision(3)
              << elapsed_ms.count() << "\n";

    std::chrono::duration<double> elapsed_s = elapsed;
    std::cout << "  Seconds (float): " << elapsed_s.count() << "\n";

    (void)sum;  // Suppress unused warning
}

// Simple stopwatch class
class Stopwatch {
public:
    void start() {
        start_ = std::chrono::steady_clock::now();
        running_ = true;
    }

    void stop() {
        end_ = std::chrono::steady_clock::now();
        running_ = false;
    }

    template<typename Duration = std::chrono::milliseconds>
    auto elapsed() const {
        auto end = running_ ? std::chrono::steady_clock::now() : end_;
        return std::chrono::duration_cast<Duration>(end - start_);
    }

private:
    std::chrono::steady_clock::time_point start_;
    std::chrono::steady_clock::time_point end_;
    bool running_ = false;
};

void stopwatch_class_demo() {
    std::cout << "\n=== Stopwatch Class ===\n";

    Stopwatch sw;

    sw.start();
    std::this_thread::sleep_for(50ms);
    std::cout << "Running elapsed: " << sw.elapsed().count() << "ms\n";

    std::this_thread::sleep_for(50ms);
    sw.stop();

    std::cout << "Final elapsed: " << sw.elapsed().count() << "ms\n";
    std::cout << "In microseconds: " << sw.elapsed<std::chrono::microseconds>().count() << "us\n";
}

// ============================================================================
// C++20 Calendar Features
// ============================================================================

void calendar_demo() {
    std::cout << "\n=== C++20 Calendar Features ===\n";

    using namespace std::chrono;

    // Year, month, day
    auto y = year{2024};
    auto m = month{12};
    auto d = day{25};

    // Combining into year_month_day
    year_month_day christmas{y, m, d};
    std::cout << "Christmas: " << static_cast<int>(christmas.year()) << "-"
              << static_cast<unsigned>(christmas.month()) << "-"
              << static_cast<unsigned>(christmas.day()) << "\n";

    // Using literals
    auto xmas = 2024y/December/25;
    std::cout << "Same date: " << static_cast<int>(xmas.year()) << "-"
              << static_cast<unsigned>(xmas.month()) << "-"
              << static_cast<unsigned>(xmas.day()) << "\n";

    // Get today's date
    auto today = floor<days>(system_clock::now());
    year_month_day ymd{today};
    std::cout << "Today: " << static_cast<int>(ymd.year()) << "-"
              << static_cast<unsigned>(ymd.month()) << "-"
              << static_cast<unsigned>(ymd.day()) << "\n";

    // Validity checking
    auto invalid = 2024y/February/30;
    std::cout << "Feb 30 valid: " << std::boolalpha << invalid.ok() << "\n";

    // Weekday
    weekday wd{today};
    std::cout << "Day of week (0=Sun): " << wd.c_encoding() << "\n";

    // Last day of month
    auto last_feb = 2024y/February/last;
    year_month_day last_feb_date{last_feb};
    std::cout << "Last day of Feb 2024: "
              << static_cast<unsigned>(last_feb_date.day()) << "\n";
}

// ============================================================================
// Timeout Patterns
// ============================================================================

void timeout_patterns() {
    std::cout << "\n=== Timeout Patterns ===\n";

    // sleep_for - sleep for a duration
    std::cout << "Sleeping for 100ms...\n";
    std::this_thread::sleep_for(100ms);
    std::cout << "Awake!\n";

    // sleep_until - sleep until a time point
    auto wake_time = std::chrono::steady_clock::now() + 50ms;
    std::cout << "Sleeping until time point...\n";
    std::this_thread::sleep_until(wake_time);
    std::cout << "Awake!\n";

    // Common pattern: deadline-based timeout
    auto deadline = std::chrono::steady_clock::now() + 200ms;
    int iterations = 0;

    while (std::chrono::steady_clock::now() < deadline) {
        ++iterations;
        std::this_thread::sleep_for(10ms);
    }

    std::cout << "Completed " << iterations << " iterations before deadline\n";
}

int main() {
    duration_basics();
    duration_conversions();
    time_point_basics();
    clock_types();
    stopwatch_example();
    stopwatch_class_demo();
    calendar_demo();
    timeout_patterns();

    return 0;
}

// Key points:
// - Use chrono literals (1s, 100ms) for readable code
// - Use steady_clock for measuring elapsed time
// - Use system_clock for wall-clock time
// - duration_cast for explicit lossy conversions
// - floor/ceil/round for controlled rounding behavior
// - C++20 adds calendar types (year_month_day) and time zones
