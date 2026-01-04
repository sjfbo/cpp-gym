/**
 * Chapter 4: Error Handling - Exceptions
 *
 * Demonstrates:
 * - Basic throw and try/catch
 * - Standard exception hierarchy
 * - Custom exception classes
 * - Exception rethrowing
 * - noexcept specification
 * - RAII for exception-safe resource management
 */

#include <iostream>
#include <stdexcept>
#include <string>
#include <memory>
#include <fstream>
#include <vector>

// =============================================================================
// Custom Exception Classes
// =============================================================================

/**
 * Custom exception for database-related errors.
 * Inherits from std::runtime_error for proper integration with std::exception.
 */
class DatabaseError : public std::runtime_error {
public:
    explicit DatabaseError(const std::string& msg)
        : std::runtime_error(msg)
    {}
};

/**
 * More specific database exception with additional context.
 * Demonstrates exception hierarchy.
 */
class ConnectionError : public DatabaseError {
public:
    ConnectionError(const std::string& host, int port)
        : DatabaseError("Failed to connect to " + host + ":" + std::to_string(port))
        , host_(host)
        , port_(port)
    {}

    const std::string& host() const noexcept { return host_; }
    int port() const noexcept { return port_; }

private:
    std::string host_;
    int port_;
};

/**
 * Exception for query-related errors.
 */
class QueryError : public DatabaseError {
public:
    QueryError(const std::string& query, const std::string& reason)
        : DatabaseError("Query failed: " + reason)
        , query_(query)
    {}

    const std::string& query() const noexcept { return query_; }

private:
    std::string query_;
};

// =============================================================================
// Basic Exception Throwing and Catching
// =============================================================================

/**
 * Demonstrates basic exception throwing.
 * @throws std::invalid_argument if value is negative
 */
int compute_square_root_approx(int value) {
    if (value < 0) {
        throw std::invalid_argument("Cannot compute square root of negative number: "
                                    + std::to_string(value));
    }

    // Simple integer approximation for demonstration
    int result = 0;
    while (result * result <= value) {
        ++result;
    }
    return result - 1;
}

void demonstrate_basic_exceptions() {
    std::cout << "=== Basic Exception Handling ===\n\n";

    // Successful case
    try {
        int result = compute_square_root_approx(16);
        std::cout << "Square root of 16 is approximately: " << result << '\n';
    }
    catch (const std::invalid_argument& e) {
        std::cerr << "Error: " << e.what() << '\n';
    }

    // Exception case
    try {
        int result = compute_square_root_approx(-5);
        std::cout << "This won't print: " << result << '\n';
    }
    catch (const std::invalid_argument& e) {
        std::cerr << "Caught invalid_argument: " << e.what() << '\n';
    }

    std::cout << '\n';
}

// =============================================================================
// Exception Hierarchy and Catching Order
// =============================================================================

/**
 * Simulates a database operation that can throw various exceptions.
 */
void database_operation(int operation_type) {
    switch (operation_type) {
        case 0:
            throw ConnectionError("localhost", 5432);
        case 1:
            throw QueryError("SELECT * FROM users", "table does not exist");
        case 2:
            throw DatabaseError("General database failure");
        case 3:
            throw std::runtime_error("Unexpected runtime error");
        default:
            // Success - no exception
            break;
    }
}

void demonstrate_exception_hierarchy() {
    std::cout << "=== Exception Hierarchy ===\n\n";

    for (int i = 0; i <= 4; ++i) {
        std::cout << "Operation " << i << ": ";
        try {
            database_operation(i);
            std::cout << "Success!\n";
        }
        // Most specific exceptions first
        catch (const ConnectionError& e) {
            std::cerr << "Connection error to " << e.host()
                      << ":" << e.port() << '\n';
        }
        catch (const QueryError& e) {
            std::cerr << "Query error: " << e.what()
                      << "\n  Query was: " << e.query() << '\n';
        }
        // More general exceptions later
        catch (const DatabaseError& e) {
            std::cerr << "Database error: " << e.what() << '\n';
        }
        catch (const std::exception& e) {
            std::cerr << "Standard exception: " << e.what() << '\n';
        }
        catch (...) {
            std::cerr << "Unknown exception!\n";
            throw;  // Rethrow unknown exceptions
        }
    }

    std::cout << '\n';
}

// =============================================================================
// Rethrowing Exceptions
// =============================================================================

/**
 * Demonstrates exception rethrowing for logging or cleanup.
 */
void process_with_logging(int value) {
    try {
        compute_square_root_approx(value);
    }
    catch (const std::exception& e) {
        // Log the error
        std::cerr << "[LOG] Exception occurred: " << e.what() << '\n';
        // Rethrow to let caller handle it
        throw;  // Preserves the original exception type
    }
}

/**
 * Demonstrates rethrowing with a different exception type.
 */
void process_with_translation(int value) {
    try {
        compute_square_root_approx(value);
    }
    catch (const std::invalid_argument& e) {
        // Translate to a different exception type
        throw std::runtime_error(std::string("Processing failed: ") + e.what());
    }
}

void demonstrate_rethrowing() {
    std::cout << "=== Rethrowing Exceptions ===\n\n";

    // Rethrow preserving type
    std::cout << "Rethrowing with preserved type:\n";
    try {
        process_with_logging(-10);
    }
    catch (const std::invalid_argument& e) {
        std::cout << "Caller caught invalid_argument (type preserved): "
                  << e.what() << '\n';
    }

    // Rethrow with translation
    std::cout << "\nRethrowing with translated type:\n";
    try {
        process_with_translation(-10);
    }
    catch (const std::runtime_error& e) {
        std::cout << "Caller caught runtime_error (translated): "
                  << e.what() << '\n';
    }

    std::cout << '\n';
}

// =============================================================================
// noexcept Specification
// =============================================================================

/**
 * Function marked noexcept - promises not to throw.
 * If it does throw, std::terminate() is called.
 */
int safe_divide(int a, int b) noexcept {
    if (b == 0) {
        return 0;  // Return default value instead of throwing
    }
    return a / b;
}

/**
 * Conditional noexcept based on type properties.
 */
template<typename T>
void swap_values(T& a, T& b) noexcept(std::is_nothrow_move_constructible_v<T> &&
                                       std::is_nothrow_move_assignable_v<T>) {
    T temp = std::move(a);
    a = std::move(b);
    b = std::move(temp);
}

/**
 * A move-only type with noexcept move operations.
 * This is important for std::vector to use move instead of copy.
 */
class Resource {
public:
    explicit Resource(int id) : id_(id) {
        std::cout << "  Resource " << id_ << " constructed\n";
    }

    // noexcept move constructor - enables efficient vector operations
    Resource(Resource&& other) noexcept
        : id_(other.id_) {
        other.id_ = -1;
        std::cout << "  Resource " << id_ << " moved\n";
    }

    // noexcept move assignment
    Resource& operator=(Resource&& other) noexcept {
        if (this != &other) {
            id_ = other.id_;
            other.id_ = -1;
        }
        return *this;
    }

    // Delete copy operations
    Resource(const Resource&) = delete;
    Resource& operator=(const Resource&) = delete;

    ~Resource() {
        if (id_ != -1) {
            std::cout << "  Resource " << id_ << " destroyed\n";
        }
    }

    int id() const noexcept { return id_; }

private:
    int id_;
};

void demonstrate_noexcept() {
    std::cout << "=== noexcept Specification ===\n\n";

    // Compile-time check for noexcept
    std::cout << "safe_divide is noexcept: "
              << std::boolalpha << noexcept(safe_divide(1, 2)) << '\n';

    // Using noexcept function
    std::cout << "safe_divide(10, 0) = " << safe_divide(10, 0) << '\n';
    std::cout << "safe_divide(10, 3) = " << safe_divide(10, 3) << '\n';

    // Demonstrate noexcept with vector (uses move when possible)
    std::cout << "\nCreating vector of Resources (noexcept move enables efficiency):\n";
    std::vector<Resource> resources;
    resources.reserve(3);  // Pre-allocate to show moves clearly

    resources.emplace_back(1);
    resources.emplace_back(2);
    resources.emplace_back(3);

    std::cout << '\n';
}

// =============================================================================
// RAII for Exception Safety
// =============================================================================

/**
 * RAII wrapper for a file handle.
 * Demonstrates automatic resource cleanup even when exceptions occur.
 */
class FileHandle {
public:
    explicit FileHandle(const std::string& filename)
        : filename_(filename)
        , file_(filename) {
        if (!file_.is_open()) {
            throw std::runtime_error("Failed to open file: " + filename);
        }
        std::cout << "  Opened file: " << filename << '\n';
    }

    // Destructor always runs, even during stack unwinding
    ~FileHandle() {
        if (file_.is_open()) {
            file_.close();
            std::cout << "  Closed file: " << filename_ << '\n';
        }
    }

    // Delete copy operations
    FileHandle(const FileHandle&) = delete;
    FileHandle& operator=(const FileHandle&) = delete;

    void write(const std::string& data) {
        file_ << data;
    }

private:
    std::string filename_;
    std::ofstream file_;
};

/**
 * Demonstrates that RAII ensures cleanup even when exceptions occur.
 */
void process_file(const std::string& filename, bool should_throw) {
    FileHandle file(filename);  // Resource acquired

    file.write("Some data\n");

    if (should_throw) {
        throw std::runtime_error("Error during processing!");
    }

    file.write("More data\n");
    // FileHandle destructor runs here (normal case)
}

void demonstrate_raii() {
    std::cout << "=== RAII for Exception Safety ===\n\n";

    // Normal case - destructor runs at end of scope
    std::cout << "Normal execution:\n";
    try {
        process_file("/tmp/test_normal.txt", false);
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << '\n';
    }

    std::cout << "\nException case - destructor still runs:\n";
    try {
        process_file("/tmp/test_exception.txt", true);
    }
    catch (const std::exception& e) {
        std::cerr << "Caught: " << e.what() << '\n';
    }

    // Using smart pointers for memory management
    std::cout << "\nSmart pointers with exceptions:\n";
    try {
        auto ptr = std::make_unique<int>(42);
        std::cout << "  Created unique_ptr with value: " << *ptr << '\n';
        throw std::runtime_error("Exception after allocation");
        // ptr is automatically deleted during stack unwinding
    }
    catch (const std::exception& e) {
        std::cerr << "  Caught: " << e.what() << '\n';
        std::cout << "  unique_ptr was automatically cleaned up\n";
    }

    std::cout << '\n';
}

// =============================================================================
// Main
// =============================================================================

int main() {
    std::cout << "Chapter 4: Exception Handling Examples\n";
    std::cout << "======================================\n\n";

    demonstrate_basic_exceptions();
    demonstrate_exception_hierarchy();
    demonstrate_rethrowing();
    demonstrate_noexcept();
    demonstrate_raii();

    std::cout << "All exception demonstrations completed.\n";
    return 0;
}
