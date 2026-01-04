# Tour of C++ - Learning Repository

A hands-on companion for working through Bjarne Stroustrup's **"A Tour of C++ (Third Edition)"**. This repository provides examples, exercises, and tests for each chapter, designed to run on both macOS (Apple Silicon) and Linux (x86_64).

## About the Book

"A Tour of C++" is a concise overview of modern C++ by the language's creator. This third edition covers C++20 and touches on C++23 features. It's ideal for programmers who already know some C++ or another language and want a quick, authoritative update on modern C++.

**Purchase the book**: [Addison-Wesley](https://www.informit.com/store/tour-of-c-plus-plus-9780136816485) | [Amazon](https://www.amazon.com/Tour-C-Bjarne-Stroustrup/dp/0136816487)

## Prerequisites

### Required Tools

| Tool | Minimum Version | Notes |
|------|-----------------|-------|
| CMake | 3.20+ | Build system |
| Clang | 15+ | Default on macOS |
| GCC | 12+ | Common on Linux |
| Ninja | 1.10+ | Recommended build tool |

### macOS (Apple Silicon / Intel)

```bash
# Install Xcode Command Line Tools (includes Clang)
xcode-select --install

# Install CMake and Ninja via Homebrew
brew install cmake ninja
```

### Linux (Ubuntu/Debian)

```bash
# Install GCC, CMake, and Ninja
sudo apt update
sudo apt install build-essential cmake ninja-build

# For C++20 features, you may need a newer GCC
sudo apt install g++-12  # or g++-13 for better std::format support
```

### Linux (Fedora)

```bash
sudo dnf install gcc-c++ cmake ninja-build
```

## Quick Start

```bash
# Clone the repository
git clone https://github.com/yourusername/tour-of-cpp.git
cd tour-of-cpp

# Configure with CMake (using Ninja)
cmake -B build -G Ninja

# Build everything
cmake --build build

# Run your first example
./build/bin/ch01_hello_world

# Run all tests
ctest --test-dir build
```

### Using CMake Presets

CMake presets provide pre-configured build options:

```bash
# Debug build with sanitizers (recommended for learning)
cmake --preset debug
cmake --build build/debug

# Release build (optimized)
cmake --preset release
cmake --build build/release

# Run tests
cmake --preset test
cmake --build build/test
ctest --preset test
```

## Project Structure

```
tour-of-cpp/
├── CMakeLists.txt              # Root CMake configuration
├── CMakePresets.json           # CMake presets for common configurations
├── .clang-format               # Code style (Stroustrup-inspired)
├── cmake/
│   └── CompilerWarnings.cmake  # Strict warning flags
├── deps/
│   └── CMakeLists.txt          # Dependencies (Catch2, fmt)
├── chapters/
│   ├── ch01_basics/            # Chapter 1: The Basics
│   ├── ch02_user_defined_types/# Chapter 2: User-Defined Types
│   ├── ch03_modularity/        # Chapter 3: Modularity
│   ├── ...                     # Chapters 4-18
│   └── ch18_concurrency/       # Chapter 18: Concurrency
└── projects/
    ├── mini_vector/            # Build your own vector
    ├── simple_json/            # JSON parser project
    └── thread_pool/            # Concurrency project
```

### Chapter Structure

Each chapter contains:

```
chXX_name/
├── README.md       # Chapter summary, key concepts, gotchas
├── CMakeLists.txt  # Build configuration
├── examples/       # Code examples from the book
│   ├── example1.cpp
│   └── example2.cpp
├── exercises/      # Practice exercises
│   ├── ex01_basic.cpp
│   └── ex02_advanced.cpp
└── tests/          # Tests to verify understanding
    └── test_chXX.cpp
```

## Chapter Overview

| Ch | Title | Key Topics |
|----|-------|------------|
| 1 | The Basics | Types, functions, pointers, references, `const` |
| 2 | User-Defined Types | Structs, classes, enums, unions |
| 3 | Modularity | Headers, namespaces, error handling basics |
| 4 | Error Handling | Exceptions, invariants, assertions |
| 5 | Classes | Concrete/abstract types, virtual functions |
| 6 | Essential Operations | Copy, move, RAII, operator overloading |
| 7 | Templates | Function/class templates, concepts intro |
| 8 | Concepts | Concept definitions, generic programming |
| 9 | Library Overview | Standard library organization |
| 10 | Strings and Regex | `string`, `string_view`, regex |
| 11 | I/O | Streams, formatting, filesystem |
| 12 | Containers | `vector`, `list`, `map`, `unordered_map` |
| 13 | Algorithms | Iterators, algorithms, parallel execution |
| 14 | Ranges | Views, generators, pipelines |
| 15 | Pointers and Containers | Smart pointers, `span`, `optional`, `variant` |
| 16 | Utilities | Time, function objects, type traits |
| 17 | Numerics | Math, complex, random, limits |
| 18 | Concurrency | Threads, mutexes, futures, coroutines |

## How to Use This Repository

### Recommended Workflow

1. **Read the chapter** in the book
2. **Study the examples** - Each example mirrors the book's code
   ```bash
   # Build and run a specific example
   ./build/bin/ch01_hello_world
   ```
3. **Do the exercises** - Practice what you learned
   ```bash
   # Exercises are TODO stubs - implement them!
   vim chapters/ch01_basics/exercises/ex01_hello.cpp
   cmake --build build
   ./build/bin/ch01_ex01_hello
   ```
4. **Run the tests** - Verify your understanding
   ```bash
   ctest --test-dir build -R ch01
   ```
5. **Review the README** - Check the chapter's key takeaways

### Build Commands Reference

```bash
# Build everything
cmake --build build

# Build a specific chapter's examples
cmake --build build --target ch01_examples

# Build a specific example
cmake --build build --target ch01_hello_world

# Run all tests
ctest --test-dir build

# Run tests for a specific chapter
ctest --test-dir build -R ch05

# Run tests with verbose output
ctest --test-dir build -V

# Clean and rebuild
cmake --build build --target clean
cmake --build build
```

## Platform-Specific Notes

### macOS (Apple Silicon M1/M2/M3/M4)

- Apple Clang has excellent C++20 support
- Most features work out of the box
- Coroutines require no special flags with Clang 15+
- `std::format` may require macOS 13+ or using the fmt fallback

### macOS (Intel)

- Same as Apple Silicon
- Rosetta 2 not needed for this project

### Linux

- GCC 12+ recommended for best C++20 support
- GCC 13+ provides native `std::format` support
- If `std::format` is unavailable, the build automatically uses fmt library

### Common Issues

**"Ninja not found"**
```bash
# macOS
brew install ninja

# Linux
sudo apt install ninja-build
```

**"C++20 features not available"**
- Ensure you have Clang 15+ or GCC 12+
- Check with `clang++ --version` or `g++ --version`

**"std::format not found"**
- The build will automatically fall back to fmt library
- On GCC, upgrade to GCC 13+ for native support

**Sanitizer errors**
- Sanitizers help catch bugs but can slow execution
- Disable with: `cmake -B build -DTOUR_ENABLE_SANITIZERS=OFF`

## Code Style

This project uses a Stroustrup-inspired style defined in `.clang-format`:

- 4-space indentation
- Braces on same line
- 100-character line limit
- Sorted includes

To format your code:
```bash
# Format a single file
clang-format -i chapters/ch01_basics/examples/hello_world.cpp

# Format all files
find chapters -name "*.cpp" -o -name "*.h" | xargs clang-format -i
```

## Contributing

Contributions are welcome! Please:

1. Follow the existing code style (run `clang-format`)
2. Add tests for new examples
3. Update chapter READMEs as needed
4. Test on both macOS and Linux if possible

## License

This learning repository is provided under the MIT License. The book "A Tour of C++" is copyrighted by Bjarne Stroustrup and Pearson Education.

## Acknowledgments

- **Bjarne Stroustrup** for creating C++ and writing this excellent book
- **Catch2** team for the testing framework
- **fmt** library authors for the formatting library

---

Happy learning! If you find this useful, consider starring the repository.
