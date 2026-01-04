# Chapter 11: Input and Output

## TL;DR

C++ provides type-safe I/O through streams (`cout`, `cin`, file streams). For formatting, prefer **`std::format`** (C++20) over `iomanip` manipulators. Use **`std::getline`** for line-based input and **`std::filesystem`** (C++17) for portable file system operations. Always check stream state after I/O operations.

## Key Concepts

| Concept | One-liner | Example |
|---------|-----------|---------|
| `std::cout` | Standard output stream | `std::cout << "Hello\n";` |
| `std::cin` | Standard input stream | `std::cin >> value;` |
| `std::cerr` | Standard error stream | `std::cerr << "Error!\n";` |
| `std::getline` | Read entire line | `std::getline(std::cin, line);` |
| `std::format` | Type-safe formatting (C++20) | `std::format("{:.2f}", 3.14159)` |
| `std::ifstream` | File input stream | `std::ifstream file{"data.txt"};` |
| `std::ofstream` | File output stream | `std::ofstream out{"out.txt"};` |
| `std::stringstream` | String-based stream | `std::stringstream ss;` |
| `std::filesystem` | File system operations | `std::filesystem::exists(path)` |

## Things to Remember

- **Check stream state**: Use `if (stream)` or check `stream.fail()` after I/O.
- **`>>` skips whitespace**: Use `getline()` for lines or `std::noskipws` for whitespace.
- **Prefer `std::format`**: Cleaner and type-safer than `iomanip` manipulators.
- **RAII for files**: Streams close automatically when they go out of scope.
- **Buffering**: `cerr` is unbuffered (immediate), `cout` is buffered.
- **Text vs Binary**: Default is text mode; use `std::ios::binary` for binary.
- **Filesystem is portable**: Use `std::filesystem::path` for cross-platform paths.

## Modern C++ Highlights

What's new/better in C++20 compared to older styles:

| Feature | Old Style | Modern C++20 |
|---------|-----------|--------------|
| Formatting | `std::setw(10) << std::setprecision(2) << x` | `std::format("{:10.2f}", x)` |
| Print | `std::cout << x << "\n";` | `std::print("{}\n", x);` (C++23) |
| Path handling | String manipulation | `std::filesystem::path` |
| File exists | `stat()` or platform-specific | `std::filesystem::exists(p)` |
| Current dir | Platform-specific | `std::filesystem::current_path()` |

## Common Pitfalls

- **Mixing `>>` and `getline`**: `>>` leaves newline in buffer.
  ```cpp
  int n;
  std::string line;
  std::cin >> n;           // Reads number, leaves '\n'
  std::getline(std::cin, line);  // Gets empty string!

  // Fix: consume the newline
  std::cin >> n;
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  std::getline(std::cin, line);  // Now works correctly
  ```

- **Not checking stream state**: I/O can fail silently.
  ```cpp
  std::ifstream file{"nonexistent.txt"};
  std::string data;
  file >> data;  // BAD: no error check

  if (!file) {
      std::cerr << "Failed to open file\n";  // GOOD: check state
  }
  ```

- **Format string mismatch**: Wrong specifier for type.
  ```cpp
  std::format("{:d}", 3.14);  // BAD: :d is for integers
  std::format("{:f}", 3.14);  // GOOD: :f for floats
  std::format("{}", 3.14);    // BEST: auto-detect type
  ```

- **Forgetting to flush**: Output may not appear immediately.
  ```cpp
  std::cout << "Working...";  // May not appear
  std::cout << "Working..." << std::flush;  // Forces output
  std::cout << "Working..." << std::endl;   // Flushes with newline
  ```

- **Hardcoded path separators**: Use filesystem::path.
  ```cpp
  std::string path = "dir\\file.txt";  // BAD: Windows-specific
  auto path = std::filesystem::path{"dir"} / "file.txt";  // GOOD: portable
  ```

## Quick Reference

```cpp
// ============================================================================
// Output
// ============================================================================
#include <iostream>

std::cout << "Hello" << std::endl;  // With flush
std::cout << "Hello\n";              // More efficient (no flush)
std::cerr << "Error!\n";             // Unbuffered error output
std::clog << "Log message\n";        // Buffered log output

// ============================================================================
// Input
// ============================================================================
int n;
std::cin >> n;                       // Read integer (skips whitespace)

std::string word;
std::cin >> word;                    // Read word (stops at whitespace)

std::string line;
std::getline(std::cin, line);        // Read entire line

// Clear error state and ignore bad input
std::cin.clear();
std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

// Check if input succeeded
if (std::cin >> n) {
    // Success
}

// ============================================================================
// std::format (C++20)
// ============================================================================
#include <format>

std::format("Hello, {}!", name);           // Basic substitution
std::format("{:10}", value);               // Width 10
std::format("{:<10}", value);              // Left align, width 10
std::format("{:>10}", value);              // Right align, width 10
std::format("{:^10}", value);              // Center, width 10
std::format("{:+}", number);               // Always show sign
std::format("{:.2f}", 3.14159);            // 2 decimal places: "3.14"
std::format("{:08d}", 42);                 // Zero-padded: "00000042"
std::format("{:#x}", 255);                 // Hex with prefix: "0xff"
std::format("{:#b}", 10);                  // Binary with prefix: "0b1010"
std::format("{0} {0} {1}", "a", "b");      // Positional: "a a b"

// ============================================================================
// iomanip (older style)
// ============================================================================
#include <iomanip>

std::cout << std::setw(10) << value;       // Width
std::cout << std::setprecision(2) << x;    // Precision
std::cout << std::fixed << x;              // Fixed-point notation
std::cout << std::scientific << x;         // Scientific notation
std::cout << std::hex << n;                // Hexadecimal
std::cout << std::setfill('0') << n;       // Fill character
std::cout << std::left << std::setw(10);   // Left align
std::cout << std::boolalpha << true;       // "true" instead of "1"

// ============================================================================
// File Streams
// ============================================================================
#include <fstream>

// Writing
std::ofstream out{"file.txt"};
out << "Hello, file!\n";
out.close();  // Optional: closes automatically at destruction

// Reading
std::ifstream in{"file.txt"};
std::string line;
while (std::getline(in, line)) {
    std::cout << line << "\n";
}

// Binary mode
std::ofstream bin{"data.bin", std::ios::binary};
std::ifstream bin_in{"data.bin", std::ios::binary};

// Append mode
std::ofstream append{"log.txt", std::ios::app};

// Read and write
std::fstream both{"data.txt", std::ios::in | std::ios::out};

// ============================================================================
// String Streams
// ============================================================================
#include <sstream>

// Building strings
std::ostringstream oss;
oss << "Value: " << 42 << ", Pi: " << 3.14;
std::string result = oss.str();

// Parsing strings
std::istringstream iss{"10 20 30"};
int a, b, c;
iss >> a >> b >> c;

// Bidirectional
std::stringstream ss;
ss << "42";
int n;
ss >> n;

// ============================================================================
// std::filesystem (C++17)
// ============================================================================
#include <filesystem>
namespace fs = std::filesystem;

// Paths
fs::path p = "dir/file.txt";
fs::path p2 = fs::path{"dir"} / "subdir" / "file.txt";  // Portable concatenation
p.filename();       // "file.txt"
p.stem();           // "file"
p.extension();      // ".txt"
p.parent_path();    // "dir"

// Queries
fs::exists(p);                    // Does it exist?
fs::is_regular_file(p);           // Is it a file?
fs::is_directory(p);              // Is it a directory?
fs::file_size(p);                 // Size in bytes

// Operations
fs::create_directory("new_dir");
fs::create_directories("a/b/c");  // Creates all intermediate dirs
fs::copy_file("src", "dst");
fs::rename("old", "new");
fs::remove("file.txt");
fs::remove_all("dir");            // Recursive delete

// Iteration
for (const auto& entry : fs::directory_iterator("dir")) {
    std::cout << entry.path() << "\n";
}

// Recursive iteration
for (const auto& entry : fs::recursive_directory_iterator("dir")) {
    std::cout << entry.path() << "\n";
}

// Current directory
fs::current_path();               // Get current directory
fs::current_path("new_dir");      // Change directory
```

## Book Sections Covered

- **11.1** Introduction
- **11.2** Output
- **11.3** Input
- **11.4** I/O State
- **11.5** I/O of User-Defined Types
- **11.6** Output Formatting
- **11.7** Streams
- **11.8** C-style I/O
- **11.9** File System
- **11.10** Advice

## Exercises in This Chapter

| Exercise | Difficulty | Concepts Practiced |
|----------|------------|-------------------|
| ex01_file_io | **       | File reading, writing, error handling |
| ex02_formatting | **    | std::format, alignment, precision |

## Running the Examples

```bash
# Build all chapter 11 examples
cmake --build build --target ch11_examples

# Run individual examples
./build/bin/ch11_output
./build/bin/ch11_input
./build/bin/ch11_formatting
./build/bin/ch11_streams
./build/bin/ch11_filesystem

# Run chapter 11 tests
ctest --test-dir build -R ch11
```
