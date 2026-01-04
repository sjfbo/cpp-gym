// std::vector - The Workhorse Dynamic Array
// Book reference: 12.2 vector
//
// std::vector is the default container for most use cases.
// It provides contiguous storage, cache-friendly access,
// and efficient amortized O(1) insertion at the end.

#include <algorithm>
#include <iostream>
#include <vector>

// Helper to print vector contents
template <typename T>
void print_vector(const std::string& label, const std::vector<T>& v) {
    std::cout << label << ": [";
    for (std::size_t i = 0; i < v.size(); ++i) {
        std::cout << v[i];
        if (i < v.size() - 1) std::cout << ", ";
    }
    std::cout << "] (size=" << v.size() << ", capacity=" << v.capacity() << ")\n";
}

int main() {
    std::cout << "=== std::vector Operations ===\n\n";

    // -------------------------------------------------------------------------
    // Construction and initialization
    // -------------------------------------------------------------------------
    std::cout << "--- Construction ---\n";

    std::vector<int> v1;                    // Empty vector
    std::vector<int> v2(5);                 // 5 zero-initialized elements
    std::vector<int> v3(5, 42);             // 5 elements, all 42
    std::vector<int> v4{1, 2, 3, 4, 5};     // Initializer list
    std::vector<int> v5(v4.begin(), v4.end()); // From iterators

    print_vector("v1 (empty)", v1);
    print_vector("v2 (5 zeros)", v2);
    print_vector("v3 (5 x 42)", v3);
    print_vector("v4 (init list)", v4);
    print_vector("v5 (from iterators)", v5);

    // -------------------------------------------------------------------------
    // Adding elements
    // -------------------------------------------------------------------------
    std::cout << "\n--- Adding Elements ---\n";

    std::vector<int> v;
    print_vector("Initial", v);

    v.push_back(10);      // Copy/move to end
    print_vector("After push_back(10)", v);

    v.emplace_back(20);   // Construct in-place (more efficient for objects)
    print_vector("After emplace_back(20)", v);

    v.insert(v.begin(), 5);  // Insert at position
    print_vector("After insert at begin", v);

    v.insert(v.end(), {30, 40, 50});  // Insert multiple
    print_vector("After insert {30,40,50}", v);

    // -------------------------------------------------------------------------
    // Capacity management
    // -------------------------------------------------------------------------
    std::cout << "\n--- Capacity Management ---\n";

    std::vector<int> cap_demo;
    std::cout << "Empty vector: size=" << cap_demo.size()
              << ", capacity=" << cap_demo.capacity() << "\n";

    cap_demo.reserve(100);  // Pre-allocate without adding elements
    std::cout << "After reserve(100): size=" << cap_demo.size()
              << ", capacity=" << cap_demo.capacity() << "\n";

    for (int i = 0; i < 10; ++i) {
        cap_demo.push_back(i);
    }
    std::cout << "After adding 10 elements: size=" << cap_demo.size()
              << ", capacity=" << cap_demo.capacity() << "\n";

    cap_demo.shrink_to_fit();  // Request to reduce capacity
    std::cout << "After shrink_to_fit(): size=" << cap_demo.size()
              << ", capacity=" << cap_demo.capacity() << "\n";

    // -------------------------------------------------------------------------
    // Element access
    // -------------------------------------------------------------------------
    std::cout << "\n--- Element Access ---\n";

    std::vector<int> access{10, 20, 30, 40, 50};

    std::cout << "v[0] = " << access[0] << " (no bounds check)\n";
    std::cout << "v.at(1) = " << access.at(1) << " (with bounds check)\n";
    std::cout << "v.front() = " << access.front() << "\n";
    std::cout << "v.back() = " << access.back() << "\n";
    std::cout << "v.data() = " << access.data() << " (raw pointer to array)\n";

    // at() throws on out-of-bounds
    try {
        [[maybe_unused]] auto val = access.at(100);
    } catch (const std::out_of_range& e) {
        std::cout << "Caught out_of_range: " << e.what() << "\n";
    }

    // -------------------------------------------------------------------------
    // Iterators
    // -------------------------------------------------------------------------
    std::cout << "\n--- Iterators ---\n";

    std::vector<int> iter_demo{1, 2, 3, 4, 5};

    std::cout << "Forward iteration: ";
    for (auto it = iter_demo.begin(); it != iter_demo.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << "\n";

    std::cout << "Reverse iteration: ";
    for (auto it = iter_demo.rbegin(); it != iter_demo.rend(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << "\n";

    std::cout << "Range-for (modern): ";
    for (const auto& elem : iter_demo) {
        std::cout << elem << " ";
    }
    std::cout << "\n";

    // Modifying through iterators
    for (auto& elem : iter_demo) {
        elem *= 2;
    }
    print_vector("After doubling", iter_demo);

    // -------------------------------------------------------------------------
    // Removing elements
    // -------------------------------------------------------------------------
    std::cout << "\n--- Removing Elements ---\n";

    std::vector<int> remove_demo{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    print_vector("Initial", remove_demo);

    remove_demo.pop_back();
    print_vector("After pop_back()", remove_demo);

    remove_demo.erase(remove_demo.begin());
    print_vector("After erase(begin())", remove_demo);

    remove_demo.erase(remove_demo.begin() + 2, remove_demo.begin() + 4);
    print_vector("After erase range [2,4)", remove_demo);

    // C++20: std::erase and std::erase_if
    std::vector<int> erase_demo{1, 2, 3, 2, 4, 2, 5};
    print_vector("Before std::erase", erase_demo);

    auto erased = std::erase(erase_demo, 2);  // Remove all 2s
    std::cout << "Erased " << erased << " elements\n";
    print_vector("After std::erase(v, 2)", erase_demo);

    std::vector<int> erase_if_demo{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    std::erase_if(erase_if_demo, [](int x) { return x % 2 == 0; });
    print_vector("After erase_if (even)", erase_if_demo);

    // -------------------------------------------------------------------------
    // Common algorithms with vector
    // -------------------------------------------------------------------------
    std::cout << "\n--- Common Algorithms ---\n";

    std::vector<int> algo_demo{5, 2, 8, 1, 9, 3, 7, 4, 6};
    print_vector("Unsorted", algo_demo);

    std::sort(algo_demo.begin(), algo_demo.end());
    print_vector("After sort", algo_demo);

    std::sort(algo_demo.begin(), algo_demo.end(), std::greater<int>{});
    print_vector("Sorted descending", algo_demo);

    auto found = std::find(algo_demo.begin(), algo_demo.end(), 5);
    if (found != algo_demo.end()) {
        std::cout << "Found 5 at index " << std::distance(algo_demo.begin(), found) << "\n";
    }

    // Binary search on sorted vector
    std::sort(algo_demo.begin(), algo_demo.end());
    bool exists = std::binary_search(algo_demo.begin(), algo_demo.end(), 5);
    std::cout << "Binary search for 5: " << (exists ? "found" : "not found") << "\n";

    // -------------------------------------------------------------------------
    // 2D vectors
    // -------------------------------------------------------------------------
    std::cout << "\n--- 2D Vectors (Matrix) ---\n";

    // Create a 3x4 matrix initialized to 0
    std::vector<std::vector<int>> matrix(3, std::vector<int>(4, 0));

    // Fill with values
    for (std::size_t i = 0; i < matrix.size(); ++i) {
        for (std::size_t j = 0; j < matrix[i].size(); ++j) {
            matrix[i][j] = static_cast<int>(i * matrix[i].size() + j);
        }
    }

    // Print matrix
    std::cout << "3x4 Matrix:\n";
    for (const auto& row : matrix) {
        std::cout << "  ";
        for (int val : row) {
            std::cout << val << " ";
        }
        std::cout << "\n";
    }

    return 0;
}

// Key takeaways:
// - Use std::vector as your default container
// - reserve() when you know approximate size to avoid reallocations
// - emplace_back() is often more efficient than push_back()
// - Use range-for when you don't need the index
// - C++20 std::erase/std::erase_if simplify element removal
// - Iterators can be invalidated by operations that resize the vector
