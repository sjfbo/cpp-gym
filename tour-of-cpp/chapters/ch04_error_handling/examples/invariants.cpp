/**
 * Chapter 4: Error Handling - Invariants
 *
 * Demonstrates:
 * - Class invariants (conditions that must always hold)
 * - Preconditions (what must be true when a function is called)
 * - Postconditions (what must be true when a function returns)
 * - Defensive programming patterns
 */

#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <algorithm>
#include <cassert>

// =============================================================================
// Class Invariants
// =============================================================================

/**
 * A Vector2D class that maintains the invariant that its magnitude
 * is always cached and up-to-date.
 *
 * Invariant: magnitude_ == sqrt(x_*x_ + y_*y_)
 */
class Vector2D {
public:
    Vector2D(double x, double y)
        : x_(x), y_(y), magnitude_(compute_magnitude()) {
        check_invariant();
    }

    // Getters maintain the invariant (no state change)
    double x() const noexcept { return x_; }
    double y() const noexcept { return y_; }
    double magnitude() const noexcept { return magnitude_; }

    // Setters must update the cached magnitude to maintain invariant
    void set_x(double x) {
        x_ = x;
        magnitude_ = compute_magnitude();  // Maintain invariant
        check_invariant();
    }

    void set_y(double y) {
        y_ = y;
        magnitude_ = compute_magnitude();  // Maintain invariant
        check_invariant();
    }

    Vector2D normalized() const {
        // Precondition: magnitude must be non-zero
        if (magnitude_ == 0.0) {
            throw std::domain_error("Cannot normalize zero vector");
        }
        return Vector2D(x_ / magnitude_, y_ / magnitude_);
    }

private:
    double compute_magnitude() const {
        return std::sqrt(x_ * x_ + y_ * y_);
    }

    void check_invariant() const {
        // In debug builds, verify the invariant holds
        assert(std::abs(magnitude_ - compute_magnitude()) < 1e-10 &&
               "Invariant violated: cached magnitude is incorrect");
    }

    double x_;
    double y_;
    double magnitude_;  // Cached value, invariant: always equals sqrt(x^2 + y^2)
};

void demonstrate_vector_invariant() {
    std::cout << "=== Class Invariant: Vector2D ===\n\n";

    Vector2D v(3.0, 4.0);
    std::cout << "Vector(" << v.x() << ", " << v.y() << ")\n";
    std::cout << "Magnitude: " << v.magnitude() << " (should be 5)\n";

    v.set_x(0.0);
    v.set_y(1.0);
    std::cout << "\nAfter modification: Vector(" << v.x() << ", " << v.y() << ")\n";
    std::cout << "Magnitude: " << v.magnitude() << " (invariant maintained)\n";

    std::cout << '\n';
}

// =============================================================================
// Preconditions and Postconditions
// =============================================================================

/**
 * A BankAccount class demonstrating preconditions and postconditions.
 *
 * Invariant: balance_ >= 0 (no overdrafts allowed)
 */
class BankAccount {
public:
    /**
     * Constructor
     * @param initial_balance Starting balance
     * @throws std::invalid_argument if initial_balance < 0
     *
     * Precondition: initial_balance >= 0
     * Postcondition: balance() == initial_balance
     */
    explicit BankAccount(double initial_balance)
        : balance_(initial_balance) {
        // Check precondition
        if (initial_balance < 0) {
            throw std::invalid_argument(
                "Initial balance cannot be negative: " +
                std::to_string(initial_balance));
        }
        // Postcondition is trivially satisfied
        check_invariant();
    }

    /**
     * Deposit money into the account.
     * @param amount Amount to deposit
     * @throws std::invalid_argument if amount <= 0
     *
     * Precondition: amount > 0
     * Postcondition: balance() == old_balance + amount
     */
    void deposit(double amount) {
        // Check precondition
        if (amount <= 0) {
            throw std::invalid_argument(
                "Deposit amount must be positive: " +
                std::to_string(amount));
        }

        double old_balance = balance_;  // Store for postcondition check
        balance_ += amount;

        // Verify postcondition in debug builds
        assert(std::abs(balance_ - (old_balance + amount)) < 1e-10 &&
               "Postcondition violated: balance not updated correctly");

        check_invariant();
    }

    /**
     * Withdraw money from the account.
     * @param amount Amount to withdraw
     * @throws std::invalid_argument if amount <= 0
     * @throws std::runtime_error if amount > balance (insufficient funds)
     *
     * Precondition: amount > 0
     * Precondition: amount <= balance()
     * Postcondition: balance() == old_balance - amount
     */
    void withdraw(double amount) {
        // Check preconditions
        if (amount <= 0) {
            throw std::invalid_argument(
                "Withdrawal amount must be positive: " +
                std::to_string(amount));
        }
        if (amount > balance_) {
            throw std::runtime_error(
                "Insufficient funds: requested " + std::to_string(amount) +
                ", available " + std::to_string(balance_));
        }

        double old_balance = balance_;
        balance_ -= amount;

        // Verify postcondition
        assert(std::abs(balance_ - (old_balance - amount)) < 1e-10 &&
               "Postcondition violated: balance not updated correctly");

        check_invariant();
    }

    double balance() const noexcept { return balance_; }

private:
    void check_invariant() const {
        assert(balance_ >= 0 && "Invariant violated: negative balance");
    }

    double balance_;
};

void demonstrate_bank_account() {
    std::cout << "=== Preconditions and Postconditions: BankAccount ===\n\n";

    try {
        BankAccount account(100.0);
        std::cout << "Created account with balance: $" << account.balance() << '\n';

        account.deposit(50.0);
        std::cout << "After depositing $50: $" << account.balance() << '\n';

        account.withdraw(30.0);
        std::cout << "After withdrawing $30: $" << account.balance() << '\n';

        // This will throw - precondition violation
        std::cout << "\nAttempting to deposit negative amount...\n";
        account.deposit(-10.0);
    }
    catch (const std::invalid_argument& e) {
        std::cout << "Precondition violation caught: " << e.what() << '\n';
    }

    try {
        BankAccount account(50.0);
        std::cout << "\nAttempting to overdraw...\n";
        account.withdraw(100.0);
    }
    catch (const std::runtime_error& e) {
        std::cout << "Precondition violation caught: " << e.what() << '\n';
    }

    std::cout << '\n';
}

// =============================================================================
// Container with Strong Invariants
// =============================================================================

/**
 * A sorted vector that maintains the invariant of being sorted.
 *
 * Invariant: elements are always in sorted order
 */
template<typename T>
class SortedVector {
public:
    SortedVector() = default;

    /**
     * Insert an element while maintaining sorted order.
     * Postcondition: is_sorted() returns true
     */
    void insert(const T& value) {
        auto pos = std::lower_bound(data_.begin(), data_.end(), value);
        data_.insert(pos, value);
        check_invariant();
    }

    /**
     * Remove an element.
     * @throws std::out_of_range if element not found
     */
    void remove(const T& value) {
        auto it = std::lower_bound(data_.begin(), data_.end(), value);
        if (it == data_.end() || *it != value) {
            throw std::out_of_range("Element not found in sorted vector");
        }
        data_.erase(it);
        check_invariant();
    }

    /**
     * Check if value exists using binary search.
     * Precondition: none (always valid)
     * Postcondition: returns true iff value is in container
     */
    bool contains(const T& value) const {
        return std::binary_search(data_.begin(), data_.end(), value);
    }

    size_t size() const noexcept { return data_.size(); }
    bool empty() const noexcept { return data_.empty(); }

    // Iterator access (const only to prevent invariant violation)
    auto begin() const { return data_.cbegin(); }
    auto end() const { return data_.cend(); }

    // Debug helper
    bool is_sorted() const {
        return std::is_sorted(data_.begin(), data_.end());
    }

private:
    void check_invariant() const {
        assert(is_sorted() && "Invariant violated: vector is not sorted");
    }

    std::vector<T> data_;
};

void demonstrate_sorted_vector() {
    std::cout << "=== Invariant: SortedVector ===\n\n";

    SortedVector<int> sv;

    // Insert elements out of order
    sv.insert(5);
    sv.insert(2);
    sv.insert(8);
    sv.insert(1);
    sv.insert(9);

    std::cout << "Inserted: 5, 2, 8, 1, 9\n";
    std::cout << "Stored order: ";
    for (int val : sv) {
        std::cout << val << ' ';
    }
    std::cout << "\n(Invariant maintained: always sorted)\n";

    std::cout << "\nContains 5? " << std::boolalpha << sv.contains(5) << '\n';
    std::cout << "Contains 7? " << sv.contains(7) << '\n';

    sv.remove(2);
    std::cout << "\nAfter removing 2: ";
    for (int val : sv) {
        std::cout << val << ' ';
    }
    std::cout << '\n';

    std::cout << '\n';
}

// =============================================================================
// Design by Contract Pattern
// =============================================================================

/**
 * Helper macros for Design by Contract (DbC).
 * In production code, consider using a library like Boost.Contract.
 */

// Precondition check - throws on violation
#define REQUIRE(condition, message) \
    if (!(condition)) { \
        throw std::invalid_argument( \
            std::string("Precondition failed: ") + message); \
    }

// Postcondition check - throws on violation
#define ENSURE(condition, message) \
    if (!(condition)) { \
        throw std::logic_error( \
            std::string("Postcondition failed: ") + message); \
    }

// Invariant check - throws on violation
#define INVARIANT(condition, message) \
    if (!(condition)) { \
        throw std::logic_error( \
            std::string("Invariant violated: ") + message); \
    }

/**
 * A Range class using Design by Contract macros.
 */
class Range {
public:
    /**
     * Construct a range [low, high].
     * Precondition: low <= high
     */
    Range(int low, int high)
        : low_(low), high_(high) {
        REQUIRE(low <= high, "low must be <= high");
        check_invariant();
    }

    /**
     * Check if value is within range.
     */
    bool contains(int value) const noexcept {
        return value >= low_ && value <= high_;
    }

    /**
     * Expand range to include value.
     * Postcondition: contains(value) is true
     */
    void expand_to_include(int value) {
        if (value < low_) low_ = value;
        if (value > high_) high_ = value;

        ENSURE(contains(value), "value must be contained after expansion");
        check_invariant();
    }

    /**
     * Intersect with another range.
     * Precondition: ranges must overlap
     * Postcondition: result is subset of both original ranges
     */
    Range intersect(const Range& other) const {
        int new_low = std::max(low_, other.low_);
        int new_high = std::min(high_, other.high_);

        REQUIRE(new_low <= new_high, "ranges must overlap for intersection");

        return Range(new_low, new_high);
    }

    int low() const noexcept { return low_; }
    int high() const noexcept { return high_; }
    int size() const noexcept { return high_ - low_ + 1; }

private:
    void check_invariant() const {
        INVARIANT(low_ <= high_, "low must be <= high");
    }

    int low_;
    int high_;
};

void demonstrate_design_by_contract() {
    std::cout << "=== Design by Contract ===\n\n";

    Range r(1, 10);
    std::cout << "Range: [" << r.low() << ", " << r.high() << "]\n";
    std::cout << "Contains 5? " << std::boolalpha << r.contains(5) << '\n';
    std::cout << "Contains 15? " << r.contains(15) << '\n';

    r.expand_to_include(15);
    std::cout << "\nAfter expanding to include 15: ["
              << r.low() << ", " << r.high() << "]\n";

    Range r2(5, 20);
    Range intersection = r.intersect(r2);
    std::cout << "\nIntersection with [5, 20]: ["
              << intersection.low() << ", " << intersection.high() << "]\n";

    // Demonstrate precondition failure
    std::cout << "\nAttempting to create invalid range [10, 5]...\n";
    try {
        Range invalid(10, 5);
    }
    catch (const std::invalid_argument& e) {
        std::cout << "Caught: " << e.what() << '\n';
    }

    // Demonstrate precondition failure on intersection
    std::cout << "\nAttempting to intersect non-overlapping ranges...\n";
    try {
        Range a(1, 5);
        Range b(10, 15);
        a.intersect(b);
    }
    catch (const std::invalid_argument& e) {
        std::cout << "Caught: " << e.what() << '\n';
    }

    std::cout << '\n';
}

// =============================================================================
// Main
// =============================================================================

int main() {
    std::cout << "Chapter 4: Invariants Examples\n";
    std::cout << "==============================\n\n";

    demonstrate_vector_invariant();
    demonstrate_bank_account();
    demonstrate_sorted_vector();
    demonstrate_design_by_contract();

    std::cout << "All invariant demonstrations completed.\n";
    return 0;
}
