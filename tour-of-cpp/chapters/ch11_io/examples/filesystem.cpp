// Filesystem - std::filesystem Basics (C++17)
// Book reference: 11.9 Filesystem
//
// This example demonstrates the std::filesystem library for
// platform-independent path manipulation and file operations.

#include <filesystem>
#include <fstream>
#include <iostream>

namespace fs = std::filesystem;

int main() {
    std::cout << "=== std::filesystem Basics ===\n\n";

    // ========================================================================
    // Path Operations
    // ========================================================================
    std::cout << "--- Path Operations ---\n";

    fs::path p1 = "/usr/local/bin/app";
    std::cout << "Path: " << p1 << '\n';
    std::cout << "  root_path:     " << p1.root_path() << '\n';
    std::cout << "  root_name:     " << p1.root_name() << '\n';
    std::cout << "  root_directory:" << p1.root_directory() << '\n';
    std::cout << "  relative_path: " << p1.relative_path() << '\n';
    std::cout << "  parent_path:   " << p1.parent_path() << '\n';
    std::cout << "  filename:      " << p1.filename() << '\n';
    std::cout << "  stem:          " << p1.stem() << '\n';
    std::cout << "  extension:     " << p1.extension() << '\n';
    std::cout << '\n';

    // ========================================================================
    // Path Construction
    // ========================================================================
    std::cout << "--- Path Construction ---\n";

    // Using / operator for path concatenation
    fs::path dir = "/tmp";
    fs::path file = "data.txt";
    fs::path full = dir / "subdir" / file;
    std::cout << "Concatenated: " << full << '\n';

    // Append and concat
    fs::path p2 = "/home/user";
    p2 /= "documents";  // Appends with separator
    std::cout << "After /=: " << p2 << '\n';

    p2 += ".bak";  // Concatenates without separator
    std::cout << "After +=: " << p2 << '\n';
    std::cout << '\n';

    // ========================================================================
    // File Status
    // ========================================================================
    std::cout << "--- File Status ---\n";

    fs::path test_path = "/tmp";
    if (fs::exists(test_path)) {
        std::cout << test_path << " exists\n";

        if (fs::is_directory(test_path)) {
            std::cout << "  It's a directory\n";
        }
        if (fs::is_regular_file(test_path)) {
            std::cout << "  It's a regular file\n";
        }
        if (fs::is_symlink(test_path)) {
            std::cout << "  It's a symlink\n";
        }
    }

    fs::path nonexistent = "/nonexistent/path";
    std::cout << nonexistent << " exists: " << fs::exists(nonexistent) << '\n';
    std::cout << '\n';

    // ========================================================================
    // Current Directory
    // ========================================================================
    std::cout << "--- Current Directory ---\n";

    fs::path cwd = fs::current_path();
    std::cout << "Current directory: " << cwd << '\n';
    std::cout << '\n';

    // ========================================================================
    // Directory Operations
    // ========================================================================
    std::cout << "--- Directory Operations ---\n";

    fs::path test_dir = "/tmp/cpp_filesystem_test";

    // Create directory
    if (fs::create_directories(test_dir / "subdir")) {
        std::cout << "Created: " << test_dir / "subdir" << '\n';
    }

    // Create a test file
    fs::path test_file = test_dir / "test.txt";
    {
        std::ofstream ofs(test_file);
        ofs << "Hello, filesystem!\n";
    }
    std::cout << "Created file: " << test_file << '\n';
    std::cout << '\n';

    // ========================================================================
    // File Information
    // ========================================================================
    std::cout << "--- File Information ---\n";

    if (fs::exists(test_file)) {
        std::cout << "File: " << test_file << '\n';
        std::cout << "  Size: " << fs::file_size(test_file) << " bytes\n";

        auto ftime = fs::last_write_time(test_file);
        auto sctp = std::chrono::time_point_cast<std::chrono::system_clock::duration>(
            ftime - fs::file_time_type::clock::now() + std::chrono::system_clock::now()
        );
        auto time_t = std::chrono::system_clock::to_time_t(sctp);
        std::cout << "  Last modified: " << std::ctime(&time_t);
    }
    std::cout << '\n';

    // ========================================================================
    // Directory Iteration
    // ========================================================================
    std::cout << "--- Directory Iteration ---\n";

    // Create more test files
    {
        std::ofstream(test_dir / "file1.txt") << "content1";
        std::ofstream(test_dir / "file2.cpp") << "content2";
        std::ofstream(test_dir / "file3.h") << "content3";
    }

    std::cout << "Contents of " << test_dir << ":\n";
    for (const auto& entry : fs::directory_iterator(test_dir)) {
        std::cout << "  " << entry.path().filename();
        if (entry.is_directory()) {
            std::cout << "/";
        }
        std::cout << '\n';
    }
    std::cout << '\n';

    // Recursive iteration
    std::cout << "Recursive contents:\n";
    for (const auto& entry : fs::recursive_directory_iterator(test_dir)) {
        std::cout << "  " << entry.path() << '\n';
    }
    std::cout << '\n';

    // ========================================================================
    // Copy, Rename, Remove
    // ========================================================================
    std::cout << "--- Copy, Rename, Remove ---\n";

    // Copy file
    fs::path copied = test_dir / "copied.txt";
    fs::copy_file(test_file, copied, fs::copy_options::overwrite_existing);
    std::cout << "Copied to: " << copied << '\n';

    // Rename/move
    fs::path renamed = test_dir / "renamed.txt";
    fs::rename(copied, renamed);
    std::cout << "Renamed to: " << renamed << '\n';

    // Remove single file
    fs::remove(renamed);
    std::cout << "Removed: " << renamed << '\n';
    std::cout << '\n';

    // ========================================================================
    // Error Handling
    // ========================================================================
    std::cout << "--- Error Handling ---\n";

    // Using error_code (non-throwing)
    std::error_code ec;
    fs::file_size("/nonexistent", ec);
    if (ec) {
        std::cout << "Error (error_code): " << ec.message() << '\n';
    }

    // Exceptions are thrown by default
    try {
        fs::file_size("/nonexistent");  // Will throw
    } catch (const fs::filesystem_error& e) {
        std::cout << "Exception: " << e.what() << '\n';
    }
    std::cout << '\n';

    // ========================================================================
    // Cleanup
    // ========================================================================
    std::cout << "--- Cleanup ---\n";

    // Remove directory and all contents
    auto removed = fs::remove_all(test_dir);
    std::cout << "Removed " << removed << " files/directories\n";

    return 0;
}

// Key points:
// - Use fs::path for platform-independent paths
// - / operator concatenates paths with separators
// - exists(), is_directory(), is_regular_file() check status
// - create_directories() creates parent directories too
// - directory_iterator for non-recursive listing
// - recursive_directory_iterator for recursive listing
// - Use error_code overloads for non-throwing operations
// - remove_all() recursively deletes directories
