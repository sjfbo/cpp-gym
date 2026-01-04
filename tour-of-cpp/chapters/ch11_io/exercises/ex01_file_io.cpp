// Exercise 11.1: File I/O Operations
// Book reference: 11.7 Streams, 11.9 Filesystem
// Difficulty: **
//
// Instructions:
// Implement the following file I/O functions.
// Each function has TODO comments explaining what to do.
//
// Hints:
// - Use std::ifstream for reading, std::ofstream for writing
// - Check stream state with if(stream) or stream.is_open()
// - Use std::getline for line-by-line reading
// - Use std::filesystem for file operations

#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace fs = std::filesystem;

// ============================================================================
// Exercise 1: Read entire file into string
// ============================================================================

// Read entire file contents into a string
// Returns empty string if file cannot be opened
// Example: read_file("test.txt") -> "Hello\nWorld\n"
std::string read_file(const std::string& filename) {
    // TODO: Open file and read all contents into a string
    // Hint: Use ostringstream with file.rdbuf(), or read line by line
    return "";
}

// ============================================================================
// Exercise 2: Write string to file
// ============================================================================

// Write content to file, overwriting if exists
// Returns true on success, false on failure
// Example: write_file("test.txt", "Hello") -> true
bool write_file(const std::string& filename, const std::string& content) {
    // TODO: Open file and write content
    // Hint: Use std::ofstream, check if open succeeded
    return false;
}

// ============================================================================
// Exercise 3: Append to file
// ============================================================================

// Append content to file (create if doesn't exist)
// Returns true on success, false on failure
// Example: append_file("log.txt", "New line\n") -> true
bool append_file(const std::string& filename, const std::string& content) {
    // TODO: Open file in append mode and write content
    // Hint: Use std::ios::app flag
    return false;
}

// ============================================================================
// Exercise 4: Read file lines
// ============================================================================

// Read file into vector of lines (without newline characters)
// Returns empty vector if file cannot be opened
// Example: read_lines("test.txt") -> {"line1", "line2", "line3"}
std::vector<std::string> read_lines(const std::string& filename) {
    std::vector<std::string> lines;
    // TODO: Read file line by line using std::getline
    return lines;
}

// ============================================================================
// Exercise 5: Count lines in file
// ============================================================================

// Count number of lines in a file
// Returns -1 if file cannot be opened
// Example: count_lines("test.txt") -> 42
int count_lines(const std::string& filename) {
    // TODO: Open file and count lines
    // Hint: Use std::getline in a loop
    return -1;
}

// ============================================================================
// Exercise 6: Copy file
// ============================================================================

// Copy source file to destination
// Returns true on success, false on failure
// Example: copy_file("src.txt", "dst.txt") -> true
bool copy_file_content(const std::string& source, const std::string& dest) {
    // TODO: Read source file and write to destination
    // Hint: Can use rdbuf() for efficient copying
    return false;
}

// ============================================================================
// Exercise 7: File exists check
// ============================================================================

// Check if file exists using std::filesystem
// Example: file_exists("test.txt") -> true/false
bool file_exists(const std::string& filename) {
    // TODO: Use std::filesystem::exists
    return false;
}

// ============================================================================
// Exercise 8: Get file size
// ============================================================================

// Get file size in bytes using std::filesystem
// Returns -1 if file doesn't exist
// Example: get_file_size("test.txt") -> 1024
long long get_file_size(const std::string& filename) {
    // TODO: Use std::filesystem::file_size
    // Hint: Check if file exists first
    return -1;
}

// ============================================================================
// Test your implementations
// ============================================================================

int main() {
    std::cout << "=== File I/O Exercises ===\n\n";

    const std::string test_file = "/tmp/ex11_test.txt";
    const std::string test_file2 = "/tmp/ex11_test2.txt";

    // Test write_file
    std::cout << "--- Testing write_file ---\n";
    bool written = write_file(test_file, "Line 1\nLine 2\nLine 3\n");
    std::cout << "write_file: " << (written ? "success" : "failed") << '\n';

    // Test read_file
    std::cout << "\n--- Testing read_file ---\n";
    std::string content = read_file(test_file);
    std::cout << "read_file content:\n" << content;

    // Test append_file
    std::cout << "\n--- Testing append_file ---\n";
    bool appended = append_file(test_file, "Line 4 (appended)\n");
    std::cout << "append_file: " << (appended ? "success" : "failed") << '\n';
    std::cout << "After append:\n" << read_file(test_file);

    // Test read_lines
    std::cout << "\n--- Testing read_lines ---\n";
    auto lines = read_lines(test_file);
    std::cout << "read_lines returned " << lines.size() << " lines:\n";
    for (size_t i = 0; i < lines.size(); ++i) {
        std::cout << "  [" << i << "]: '" << lines[i] << "'\n";
    }

    // Test count_lines
    std::cout << "\n--- Testing count_lines ---\n";
    int line_count = count_lines(test_file);
    std::cout << "count_lines: " << line_count << '\n';

    // Test copy_file_content
    std::cout << "\n--- Testing copy_file_content ---\n";
    bool copied = copy_file_content(test_file, test_file2);
    std::cout << "copy_file_content: " << (copied ? "success" : "failed") << '\n';
    if (copied) {
        std::cout << "Copied content:\n" << read_file(test_file2);
    }

    // Test file_exists
    std::cout << "\n--- Testing file_exists ---\n";
    std::cout << "file_exists('" << test_file << "'): "
              << (file_exists(test_file) ? "true" : "false") << '\n';
    std::cout << "file_exists('/nonexistent'): "
              << (file_exists("/nonexistent") ? "true" : "false") << '\n';

    // Test get_file_size
    std::cout << "\n--- Testing get_file_size ---\n";
    long long size = get_file_size(test_file);
    std::cout << "get_file_size('" << test_file << "'): " << size << " bytes\n";

    // Cleanup
    std::cout << "\n--- Cleanup ---\n";
    if (fs::exists(test_file)) fs::remove(test_file);
    if (fs::exists(test_file2)) fs::remove(test_file2);
    std::cout << "Removed test files\n";

    return 0;
}

// Expected output when correctly implemented:
// --- Testing write_file ---
// write_file: success
//
// --- Testing read_file ---
// read_file content:
// Line 1
// Line 2
// Line 3
//
// --- Testing append_file ---
// append_file: success
// After append:
// Line 1
// Line 2
// Line 3
// Line 4 (appended)
//
// --- Testing read_lines ---
// read_lines returned 4 lines:
//   [0]: 'Line 1'
//   [1]: 'Line 2'
//   [2]: 'Line 3'
//   [3]: 'Line 4 (appended)'
//
// --- Testing count_lines ---
// count_lines: 4
//
// --- Testing copy_file_content ---
// copy_file_content: success
// Copied content:
// Line 1
// Line 2
// Line 3
// Line 4 (appended)
//
// --- Testing file_exists ---
// file_exists('/tmp/ex11_test.txt'): true
// file_exists('/nonexistent'): false
//
// --- Testing get_file_size ---
// get_file_size('/tmp/ex11_test.txt'): ~40 bytes
