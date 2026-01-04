// Chapter 6: Exercise 2 - RAII Wrapper for File Handle
//
// Objective: Implement an RAII wrapper for C-style file handles (FILE*)
//            that ensures proper resource cleanup.
//
// Requirements:
// 1. Constructor opens file, throws on failure
// 2. Destructor closes file (if open)
// 3. Non-copyable (files can't be duplicated this way)
// 4. Movable (transfer ownership)
// 5. Provide read/write operations
// 6. Provide a way to check if file is open
//
// Tips:
// - Use std::fopen, std::fclose, std::fread, std::fwrite, std::fputs, std::fgets
// - Remember to set the moved-from object to a valid state
// - Consider what happens if the file fails to open

#include <cstdio>
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>

class FileHandle {
    std::FILE* file_;
    std::string filename_;

public:
    // ==========================================================================
    // TODO: Implement the constructor
    // ==========================================================================

    // Constructor: Opens file with given mode
    // Should throw std::runtime_error if file cannot be opened
    // Mode examples: "r" (read), "w" (write), "a" (append), "rb" (read binary)
    FileHandle(const std::string& filename, const std::string& mode)
        : file_{nullptr}, filename_{filename} {
        // TODO: Implement this
        // Hint: Use std::fopen to open the file
        // Hint: Throw std::runtime_error if fopen returns nullptr
        (void)mode;  // Remove this line when implementing
    }

    // ==========================================================================
    // TODO: Implement the destructor
    // ==========================================================================

    ~FileHandle() {
        // TODO: Implement this
        // Hint: Close the file if it's open
        // Hint: Check if file_ is not nullptr before closing
    }

    // ==========================================================================
    // TODO: Disable copying
    // ==========================================================================

    // Copy constructor - should be deleted
    // FileHandle(const FileHandle&) = ???

    // Copy assignment - should be deleted
    // FileHandle& operator=(const FileHandle&) = ???

    // ==========================================================================
    // TODO: Implement move operations
    // ==========================================================================

    // Move constructor
    FileHandle(FileHandle&& other) noexcept
        : file_{nullptr}, filename_{} {
        // TODO: Implement this
        // Hint: Take ownership of other's file handle
        // Hint: Set other.file_ to nullptr
        (void)other;  // Remove this line when implementing
    }

    // Move assignment
    FileHandle& operator=(FileHandle&& other) noexcept {
        // TODO: Implement this
        // Hint: Close current file if open
        // Hint: Take ownership of other's file handle
        // Hint: Set other.file_ to nullptr
        (void)other;  // Remove this line when implementing
        return *this;
    }

    // ==========================================================================
    // TODO: Implement utility methods
    // ==========================================================================

    // Check if file is open
    [[nodiscard]] bool is_open() const {
        // TODO: Implement this
        return false;
    }

    // Get the filename
    [[nodiscard]] const std::string& filename() const {
        return filename_;
    }

    // Get the underlying FILE* (for advanced usage)
    [[nodiscard]] std::FILE* get() const {
        return file_;
    }

    // ==========================================================================
    // TODO: Implement file operations
    // ==========================================================================

    // Write a string to the file
    void write(const std::string& text) {
        // TODO: Implement this
        // Hint: Use std::fputs or std::fwrite
        // Hint: Throw std::runtime_error if file is not open
        (void)text;  // Remove this line when implementing
    }

    // Write a line (string + newline) to the file
    void write_line(const std::string& text) {
        // TODO: Implement this
        // Hint: Write the text, then write a newline
        (void)text;  // Remove this line when implementing
    }

    // Read up to 'count' characters from the file
    [[nodiscard]] std::string read(std::size_t count) {
        // TODO: Implement this
        // Hint: Use std::fread
        // Hint: Return empty string if file is not open or at EOF
        (void)count;  // Remove this line when implementing
        return "";
    }

    // Read a line from the file (up to newline or EOF)
    [[nodiscard]] std::string read_line() {
        // TODO: Implement this
        // Hint: Use std::fgets with a buffer
        // Hint: Remove trailing newline if present
        return "";
    }

    // Read entire file contents
    [[nodiscard]] std::string read_all() {
        // TODO: Implement this
        // Hint: Read in chunks until EOF
        // Hint: Use feof() to check for end of file
        return "";
    }

    // Close the file explicitly (optional, destructor also closes)
    void close() {
        // TODO: Implement this
        // Hint: Close file if open, set file_ to nullptr
    }

    // Flush buffered data to file
    void flush() {
        // TODO: Implement this
        // Hint: Use std::fflush
        (void)0;  // Remove this line when implementing
    }

    // Check if at end of file
    [[nodiscard]] bool eof() const {
        // TODO: Implement this
        // Hint: Use std::feof
        return true;
    }
};

// =============================================================================
// Test your implementation
// =============================================================================

int main() {
    std::cout << "=== FileHandle RAII Exercise ===\n\n";

    const std::string test_filename = "test_output.txt";

    // Test 1: Create and write to file
    std::cout << "Test 1: Create and write to file\n";
    try {
        FileHandle file{test_filename, "w"};
        std::cout << "  File opened: " << (file.is_open() ? "true" : "false")
                  << " (expected: true)\n";

        file.write_line("Hello, RAII!");
        file.write_line("This is a test file.");
        file.write("No newline here");
        std::cout << "  Write operations completed\n";

        // File automatically closed when 'file' goes out of scope
    } catch (const std::exception& e) {
        std::cout << "  Error: " << e.what() << "\n";
    }

    // Test 2: Read from file
    std::cout << "\nTest 2: Read from file\n";
    try {
        FileHandle file{test_filename, "r"};

        std::string line1 = file.read_line();
        std::cout << "  Line 1: \"" << line1 << "\" (expected: Hello, RAII!)\n";

        std::string line2 = file.read_line();
        std::cout << "  Line 2: \"" << line2 << "\" (expected: This is a test file.)\n";

        std::string rest = file.read(100);
        std::cout << "  Rest: \"" << rest << "\" (expected: No newline here)\n";

    } catch (const std::exception& e) {
        std::cout << "  Error: " << e.what() << "\n";
    }

    // Test 3: Read all at once
    std::cout << "\nTest 3: Read all at once\n";
    try {
        FileHandle file{test_filename, "r"};
        std::string content = file.read_all();
        std::cout << "  Content length: " << content.length() << " bytes\n";
        std::cout << "  Content:\n---\n" << content << "\n---\n";
    } catch (const std::exception& e) {
        std::cout << "  Error: " << e.what() << "\n";
    }

    // Test 4: Move semantics
    std::cout << "\nTest 4: Move semantics\n";
    try {
        FileHandle file1{test_filename, "r"};
        std::cout << "  file1 open: " << (file1.is_open() ? "true" : "false") << "\n";

        FileHandle file2 = std::move(file1);
        std::cout << "  After move:\n";
        std::cout << "    file1 open: " << (file1.is_open() ? "true" : "false")
                  << " (expected: false)\n";
        std::cout << "    file2 open: " << (file2.is_open() ? "true" : "false")
                  << " (expected: true)\n";

        // file2 automatically closed when it goes out of scope
    } catch (const std::exception& e) {
        std::cout << "  Error: " << e.what() << "\n";
    }

    // Test 5: Move assignment
    std::cout << "\nTest 5: Move assignment\n";
    try {
        FileHandle file1{test_filename, "r"};
        FileHandle file2{test_filename, "r"};  // Open same file again

        file1 = std::move(file2);
        std::cout << "  After move assignment:\n";
        std::cout << "    file1 open: " << (file1.is_open() ? "true" : "false")
                  << " (expected: true)\n";
        std::cout << "    file2 open: " << (file2.is_open() ? "true" : "false")
                  << " (expected: false)\n";

    } catch (const std::exception& e) {
        std::cout << "  Error: " << e.what() << "\n";
    }

    // Test 6: Error handling - non-existent file
    std::cout << "\nTest 6: Error handling - non-existent file\n";
    try {
        FileHandle file{"non_existent_file_xyz.txt", "r"};
        std::cout << "  ERROR: Should have thrown exception!\n";
    } catch (const std::runtime_error& e) {
        std::cout << "  Correctly caught exception: " << e.what() << "\n";
    }

    // Test 7: Explicit close
    std::cout << "\nTest 7: Explicit close\n";
    try {
        FileHandle file{test_filename, "r"};
        std::cout << "  Before close: " << (file.is_open() ? "open" : "closed") << "\n";
        file.close();
        std::cout << "  After close: " << (file.is_open() ? "open" : "closed")
                  << " (expected: closed)\n";

        // Destructor should handle already-closed file gracefully
    } catch (const std::exception& e) {
        std::cout << "  Error: " << e.what() << "\n";
    }

    // Cleanup: remove test file
    std::remove(test_filename.c_str());
    std::cout << "\nTest file cleaned up.\n";

    std::cout << "\n=== Exercise Complete ===\n";
    return 0;
}
