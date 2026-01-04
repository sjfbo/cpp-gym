// Streams - File Streams and String Streams
// Book reference: 11.7 Streams
//
// This example demonstrates file I/O with fstream and in-memory
// string operations with stringstream.

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

int main() {
    std::cout << "=== File and String Streams ===\n\n";

    // ========================================================================
    // String Streams - ostringstream
    // ========================================================================
    std::cout << "--- ostringstream (output to string) ---\n";

    std::ostringstream oss;
    oss << "Hello, ";
    oss << "World! ";
    oss << "Number: " << 42;

    std::string result = oss.str();
    std::cout << "Built string: '" << result << "'\n";

    // Clear and reuse
    oss.str("");  // Clear contents
    oss.clear();  // Clear error flags
    oss << "Fresh start";
    std::cout << "After clear: '" << oss.str() << "'\n";
    std::cout << '\n';

    // ========================================================================
    // String Streams - istringstream
    // ========================================================================
    std::cout << "--- istringstream (input from string) ---\n";

    std::string data = "42 3.14 hello";
    std::istringstream iss(data);

    int n = 0;
    double d = 0.0;
    std::string s;
    iss >> n >> d >> s;

    std::cout << "Parsed: int=" << n << ", double=" << d
              << ", string='" << s << "'\n";

    // Parse CSV data
    std::string csv_line = "Alice,30,Engineer";
    std::istringstream csv_stream(csv_line);
    std::string name;
    std::string age;
    std::string job;

    std::getline(csv_stream, name, ',');
    std::getline(csv_stream, age, ',');
    std::getline(csv_stream, job, ',');

    std::cout << "CSV parsed: name='" << name << "', age=" << age
              << ", job='" << job << "'\n";
    std::cout << '\n';

    // ========================================================================
    // String Streams - stringstream (bidirectional)
    // ========================================================================
    std::cout << "--- stringstream (bidirectional) ---\n";

    std::stringstream ss;
    ss << 100 << " " << 200 << " " << 300;

    int x = 0;
    int y = 0;
    int z = 0;
    ss >> x >> y >> z;
    std::cout << "Read back: " << x << ", " << y << ", " << z << '\n';
    std::cout << '\n';

    // ========================================================================
    // File Output - ofstream
    // ========================================================================
    std::cout << "--- ofstream (write to file) ---\n";

    const std::string filename = "/tmp/test_output.txt";

    // Create and write to file
    {
        std::ofstream file(filename);
        if (!file) {
            std::cerr << "Failed to open file for writing\n";
            return 1;
        }

        file << "Line 1: Hello, file!\n";
        file << "Line 2: Number = " << 42 << '\n';
        file << "Line 3: Pi = " << 3.14159 << '\n';

        // File automatically closed when ofstream goes out of scope
    }
    std::cout << "Wrote to: " << filename << '\n';
    std::cout << '\n';

    // ========================================================================
    // File Input - ifstream
    // ========================================================================
    std::cout << "--- ifstream (read from file) ---\n";

    {
        std::ifstream file(filename);
        if (!file) {
            std::cerr << "Failed to open file for reading\n";
            return 1;
        }

        std::cout << "File contents:\n";
        std::string line;
        while (std::getline(file, line)) {
            std::cout << "  " << line << '\n';
        }
    }
    std::cout << '\n';

    // ========================================================================
    // File Modes
    // ========================================================================
    std::cout << "--- File Modes ---\n";

    // Append mode
    {
        std::ofstream file(filename, std::ios::app);
        file << "Line 4: Appended line\n";
    }
    std::cout << "Appended to file\n";

    // Read entire file at once
    {
        std::ifstream file(filename);
        std::ostringstream buffer;
        buffer << file.rdbuf();  // Read entire file
        std::cout << "Entire file (" << buffer.str().length() << " bytes):\n";
        std::cout << buffer.str();
    }
    std::cout << '\n';

    // ========================================================================
    // Binary File I/O
    // ========================================================================
    std::cout << "--- Binary File I/O ---\n";

    const std::string bin_filename = "/tmp/test_binary.bin";

    // Write binary data
    {
        std::ofstream file(bin_filename, std::ios::binary);
        std::vector<int> numbers = {1, 2, 3, 4, 5};

        // Write size first
        size_t size = numbers.size();
        file.write(reinterpret_cast<const char*>(&size), sizeof(size));

        // Write data
        file.write(reinterpret_cast<const char*>(numbers.data()),
                   static_cast<std::streamsize>(numbers.size() * sizeof(int)));
    }
    std::cout << "Wrote binary data to: " << bin_filename << '\n';

    // Read binary data
    {
        std::ifstream file(bin_filename, std::ios::binary);

        size_t size = 0;
        file.read(reinterpret_cast<char*>(&size), sizeof(size));

        std::vector<int> numbers(size);
        file.read(reinterpret_cast<char*>(numbers.data()),
                  static_cast<std::streamsize>(size * sizeof(int)));

        std::cout << "Read binary data: ";
        for (int val : numbers) {
            std::cout << val << " ";
        }
        std::cout << '\n';
    }
    std::cout << '\n';

    // ========================================================================
    // Error Handling
    // ========================================================================
    std::cout << "--- Error Handling ---\n";

    std::ifstream nonexistent("/nonexistent/path/file.txt");
    if (!nonexistent) {
        std::cout << "File open failed (as expected)\n";
    }

    if (!nonexistent.is_open()) {
        std::cout << "is_open() returned false\n";
    }

    // Stream states
    std::cout << "Stream states: good=" << nonexistent.good()
              << ", fail=" << nonexistent.fail()
              << ", bad=" << nonexistent.bad()
              << ", eof=" << nonexistent.eof() << '\n';

    return 0;
}

// Key points:
// - ostringstream: build strings incrementally
// - istringstream: parse strings as input
// - stringstream: bidirectional string stream
// - ofstream: write to files
// - ifstream: read from files
// - fstream: read/write files
// - Use is_open() or bool conversion to check success
// - Files auto-close when stream goes out of scope (RAII)
// - Use ios::binary for binary mode
// - Use ios::app to append to files
