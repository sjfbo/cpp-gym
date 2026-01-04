// Chapter 11 Tests: Input/Output
// These tests verify understanding of C++ I/O facilities.

#include <catch2/catch_test_macros.hpp>
#include <filesystem>
#include <format>
#include <fstream>
#include <sstream>
#include <string>

namespace fs = std::filesystem;

// ============================================================================
// Section 11.2: Output
// ============================================================================

TEST_CASE("Output: ostringstream basics", "[ch11][output]") {
    std::ostringstream oss;
    oss << "Hello, " << "World!";
    REQUIRE(oss.str() == "Hello, World!");
}

TEST_CASE("Output: ostringstream with numbers", "[ch11][output]") {
    std::ostringstream oss;
    oss << "Value: " << 42 << ", Pi: " << 3.14;
    REQUIRE(oss.str() == "Value: 42, Pi: 3.14");
}

TEST_CASE("Output: ostringstream clear and reuse", "[ch11][output]") {
    std::ostringstream oss;
    oss << "First";
    REQUIRE(oss.str() == "First");

    oss.str("");
    oss << "Second";
    REQUIRE(oss.str() == "Second");
}

// ============================================================================
// Section 11.3: Input
// ============================================================================

TEST_CASE("Input: istringstream basics", "[ch11][input]") {
    std::istringstream iss("Hello World");
    std::string word1;
    std::string word2;
    iss >> word1 >> word2;
    REQUIRE(word1 == "Hello");
    REQUIRE(word2 == "World");
}

TEST_CASE("Input: istringstream with numbers", "[ch11][input]") {
    std::istringstream iss("42 3.14 100");
    int a = 0;
    double b = 0.0;
    int c = 0;
    iss >> a >> b >> c;
    REQUIRE(a == 42);
    REQUIRE(b == 3.14);
    REQUIRE(c == 100);
}

TEST_CASE("Input: getline", "[ch11][input]") {
    std::istringstream iss("Line 1\nLine 2\nLine 3");
    std::string line;

    std::getline(iss, line);
    REQUIRE(line == "Line 1");

    std::getline(iss, line);
    REQUIRE(line == "Line 2");

    std::getline(iss, line);
    REQUIRE(line == "Line 3");
}

TEST_CASE("Input: getline with custom delimiter", "[ch11][input]") {
    std::istringstream iss("apple,banana,cherry");
    std::string item;

    std::getline(iss, item, ',');
    REQUIRE(item == "apple");

    std::getline(iss, item, ',');
    REQUIRE(item == "banana");

    std::getline(iss, item, ',');
    REQUIRE(item == "cherry");
}

TEST_CASE("Input: stream error state", "[ch11][input]") {
    std::istringstream iss("not_a_number");
    int n = 0;
    iss >> n;

    REQUIRE(iss.fail());
    REQUIRE_FALSE(iss.good());

    iss.clear();
    REQUIRE_FALSE(iss.fail());
}

// ============================================================================
// Section 11.6: Formatting (std::format)
// ============================================================================

TEST_CASE("Format: basic substitution", "[ch11][format]") {
    REQUIRE(std::format("Hello, {}!", "World") == "Hello, World!");
    REQUIRE(std::format("{} + {} = {}", 2, 3, 5) == "2 + 3 = 5");
}

TEST_CASE("Format: positional arguments", "[ch11][format]") {
    REQUIRE(std::format("{1} {0}", "World", "Hello") == "Hello World");
    REQUIRE(std::format("{0}{0}{0}", "x") == "xxx");
}

TEST_CASE("Format: width and alignment", "[ch11][format]") {
    REQUIRE(std::format("{:10}", 42) == "        42");  // Right-align (default for numbers)
    REQUIRE(std::format("{:<10}", 42) == "42        ");  // Left-align
    REQUIRE(std::format("{:>10}", 42) == "        42");  // Right-align
    REQUIRE(std::format("{:^10}", 42) == "    42    ");  // Center-align
}

TEST_CASE("Format: fill character", "[ch11][format]") {
    REQUIRE(std::format("{:*^10}", 42) == "****42****");
    REQUIRE(std::format("{:0>8}", 42) == "00000042");
}

TEST_CASE("Format: integer bases", "[ch11][format]") {
    REQUIRE(std::format("{:d}", 255) == "255");
    REQUIRE(std::format("{:x}", 255) == "ff");
    REQUIRE(std::format("{:X}", 255) == "FF");
    REQUIRE(std::format("{:o}", 255) == "377");
    REQUIRE(std::format("{:b}", 255) == "11111111");
}

TEST_CASE("Format: integer with prefix", "[ch11][format]") {
    REQUIRE(std::format("{:#x}", 255) == "0xff");
    REQUIRE(std::format("{:#X}", 255) == "0XFF");
    REQUIRE(std::format("{:#o}", 255) == "0377");
    REQUIRE(std::format("{:#b}", 255) == "0b11111111");
}

TEST_CASE("Format: floating point precision", "[ch11][format]") {
    double pi = 3.14159265358979;
    REQUIRE(std::format("{:.2f}", pi) == "3.14");
    REQUIRE(std::format("{:.4f}", pi) == "3.1416");
    REQUIRE(std::format("{:.0f}", pi) == "3");
}

TEST_CASE("Format: floating point notation", "[ch11][format]") {
    double val = 12345.6789;
    REQUIRE(std::format("{:e}", val).substr(0, 6) == "1.2345");  // Scientific (1.234568e+04)
    REQUIRE(std::format("{:f}", val) == "12345.678900");  // Fixed
}

TEST_CASE("Format: string width and truncation", "[ch11][format]") {
    REQUIRE(std::format("{:10}", "Hello") == "Hello     ");  // Left-align (default for strings)
    REQUIRE(std::format("{:>10}", "Hello") == "     Hello");
    REQUIRE(std::format("{:.3}", "Hello") == "Hel");  // Truncate
}

// ============================================================================
// Section 11.7: Streams
// ============================================================================

TEST_CASE("Streams: stringstream bidirectional", "[ch11][streams]") {
    std::stringstream ss;
    ss << 42 << " " << 3.14;

    int n = 0;
    double d = 0.0;
    ss >> n >> d;

    REQUIRE(n == 42);
    REQUIRE(d == 3.14);
}

TEST_CASE("Streams: file write and read", "[ch11][streams]") {
    const std::string filename = "/tmp/test_ch11_stream.txt";

    // Write
    {
        std::ofstream out(filename);
        REQUIRE(out.is_open());
        out << "Test content\n";
        out << "Line 2\n";
    }

    // Read
    {
        std::ifstream in(filename);
        REQUIRE(in.is_open());

        std::string line;
        std::getline(in, line);
        REQUIRE(line == "Test content");

        std::getline(in, line);
        REQUIRE(line == "Line 2");
    }

    // Cleanup
    fs::remove(filename);
}

TEST_CASE("Streams: file append mode", "[ch11][streams]") {
    const std::string filename = "/tmp/test_ch11_append.txt";

    // Initial write
    {
        std::ofstream out(filename);
        out << "First\n";
    }

    // Append
    {
        std::ofstream out(filename, std::ios::app);
        out << "Second\n";
    }

    // Read and verify
    {
        std::ifstream in(filename);
        std::ostringstream buffer;
        buffer << in.rdbuf();
        REQUIRE(buffer.str() == "First\nSecond\n");
    }

    fs::remove(filename);
}

TEST_CASE("Streams: read entire file", "[ch11][streams]") {
    const std::string filename = "/tmp/test_ch11_entire.txt";
    const std::string content = "Hello\nWorld\n";

    // Write
    {
        std::ofstream out(filename);
        out << content;
    }

    // Read entire file using rdbuf
    {
        std::ifstream in(filename);
        std::ostringstream buffer;
        buffer << in.rdbuf();
        REQUIRE(buffer.str() == content);
    }

    fs::remove(filename);
}

TEST_CASE("Streams: file not found", "[ch11][streams]") {
    std::ifstream in("/nonexistent/path/file.txt");
    REQUIRE_FALSE(in.is_open());
    REQUIRE_FALSE(in);
}

// ============================================================================
// Section 11.9: Filesystem
// ============================================================================

TEST_CASE("Filesystem: path components", "[ch11][filesystem]") {
    fs::path p = "/usr/local/bin/app.exe";

    REQUIRE(p.filename() == "app.exe");
    REQUIRE(p.stem() == "app");
    REQUIRE(p.extension() == ".exe");
    REQUIRE(p.parent_path() == "/usr/local/bin");
}

TEST_CASE("Filesystem: path concatenation", "[ch11][filesystem]") {
    fs::path dir = "/home/user";
    fs::path file = "document.txt";

    fs::path full = dir / "documents" / file;
    REQUIRE(full == "/home/user/documents/document.txt");
}

TEST_CASE("Filesystem: exists and is_directory", "[ch11][filesystem]") {
    REQUIRE(fs::exists("/tmp"));
    REQUIRE(fs::is_directory("/tmp"));
    REQUIRE_FALSE(fs::exists("/nonexistent_path_12345"));
}

TEST_CASE("Filesystem: create and remove directory", "[ch11][filesystem]") {
    fs::path test_dir = "/tmp/test_ch11_dir";

    // Clean up if exists from previous run
    if (fs::exists(test_dir)) {
        fs::remove_all(test_dir);
    }

    // Create
    REQUIRE(fs::create_directory(test_dir));
    REQUIRE(fs::exists(test_dir));
    REQUIRE(fs::is_directory(test_dir));

    // Remove
    REQUIRE(fs::remove(test_dir));
    REQUIRE_FALSE(fs::exists(test_dir));
}

TEST_CASE("Filesystem: create_directories (nested)", "[ch11][filesystem]") {
    fs::path test_dir = "/tmp/test_ch11_nested/a/b/c";

    // Clean up if exists
    if (fs::exists("/tmp/test_ch11_nested")) {
        fs::remove_all("/tmp/test_ch11_nested");
    }

    // Create nested directories
    REQUIRE(fs::create_directories(test_dir));
    REQUIRE(fs::exists(test_dir));

    // Cleanup
    fs::remove_all("/tmp/test_ch11_nested");
}

TEST_CASE("Filesystem: file_size", "[ch11][filesystem]") {
    const std::string filename = "/tmp/test_ch11_size.txt";
    const std::string content = "Hello, World!";  // 13 bytes

    {
        std::ofstream out(filename);
        out << content;
    }

    REQUIRE(fs::file_size(filename) == content.size());

    fs::remove(filename);
}

TEST_CASE("Filesystem: directory iteration", "[ch11][filesystem]") {
    fs::path test_dir = "/tmp/test_ch11_iter";

    // Setup
    fs::create_directories(test_dir);
    std::ofstream(test_dir / "file1.txt") << "1";
    std::ofstream(test_dir / "file2.txt") << "2";
    fs::create_directory(test_dir / "subdir");

    // Count entries
    int count = 0;
    for (const auto& entry : fs::directory_iterator(test_dir)) {
        (void)entry;  // Suppress unused warning
        ++count;
    }
    REQUIRE(count == 3);

    // Cleanup
    fs::remove_all(test_dir);
}

TEST_CASE("Filesystem: copy and rename", "[ch11][filesystem]") {
    const std::string src = "/tmp/test_ch11_src.txt";
    const std::string dst = "/tmp/test_ch11_dst.txt";
    const std::string renamed = "/tmp/test_ch11_renamed.txt";

    // Create source
    {
        std::ofstream out(src);
        out << "Original content";
    }

    // Copy
    fs::copy_file(src, dst);
    REQUIRE(fs::exists(dst));

    // Rename
    fs::rename(dst, renamed);
    REQUIRE_FALSE(fs::exists(dst));
    REQUIRE(fs::exists(renamed));

    // Verify content
    {
        std::ifstream in(renamed);
        std::string content;
        std::getline(in, content);
        REQUIRE(content == "Original content");
    }

    // Cleanup
    fs::remove(src);
    fs::remove(renamed);
}

TEST_CASE("Filesystem: error handling with error_code", "[ch11][filesystem]") {
    std::error_code ec;
    fs::file_size("/nonexistent", ec);
    REQUIRE(ec);  // Error occurred
}

TEST_CASE("Filesystem: current_path", "[ch11][filesystem]") {
    fs::path cwd = fs::current_path();
    REQUIRE_FALSE(cwd.empty());
    REQUIRE(fs::exists(cwd));
    REQUIRE(fs::is_directory(cwd));
}
