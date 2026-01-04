// std::span - Non-owning View of Contiguous Sequences
// Book reference: 15.3.1 span
//
// std::span provides a lightweight, non-owning view over contiguous data:
// - Works with arrays, vectors, and other contiguous containers
// - Similar to string_view but for any element type
// - Can be fixed-size or dynamic-size
// - Does NOT own the data - be careful about lifetimes!

#include <algorithm>
#include <array>
#include <iostream>
#include <numeric>
#include <span>
#include <vector>

// ============================================================================
// Basic span usage
// ============================================================================

void print_span(std::span<const int> data) {
    std::cout << "[ ";
    for (int x : data) {
        std::cout << x << " ";
    }
    std::cout << "]\n";
}

void basic_usage() {
    std::cout << "=== Basic span Usage ===\n";

    // span from vector
    std::vector<int> vec{1, 2, 3, 4, 5};
    std::span<int> vec_span{vec};
    std::cout << "From vector: ";
    print_span(vec_span);

    // span from C-style array
    int arr[] = {10, 20, 30, 40, 50};
    std::span<int> arr_span{arr};
    std::cout << "From C array: ";
    print_span(arr_span);

    // span from std::array
    std::array<int, 4> std_arr{100, 200, 300, 400};
    std::span<int> std_arr_span{std_arr};
    std::cout << "From std::array: ";
    print_span(std_arr_span);

    // Implicit conversion in function call
    std::cout << "Direct call: ";
    print_span(vec);  // vector implicitly converts to span
}

// ============================================================================
// Modifying through span
// ============================================================================

void double_values(std::span<int> data) {
    for (int& x : data) {
        x *= 2;
    }
}

void modification_demo() {
    std::cout << "\n=== Modifying Through span ===\n";

    std::vector<int> vec{1, 2, 3, 4, 5};

    std::cout << "Before: ";
    print_span(vec);

    double_values(vec);

    std::cout << "After doubling: ";
    print_span(vec);
}

// ============================================================================
// Fixed-extent vs Dynamic-extent spans
// ============================================================================

// Fixed-extent span: size known at compile time
void process_fixed(std::span<int, 3> data) {
    std::cout << "Fixed span of 3 elements: ";
    print_span(data);
}

// Dynamic-extent span: size determined at runtime
void process_dynamic(std::span<int> data) {
    std::cout << "Dynamic span of " << data.size() << " elements: ";
    print_span(data);
}

void extent_demo() {
    std::cout << "\n=== Fixed vs Dynamic Extent ===\n";

    int arr5[] = {1, 2, 3, 4, 5};
    std::array<int, 3> arr3{10, 20, 30};

    // Dynamic-extent accepts any size
    process_dynamic(arr5);
    process_dynamic(arr3);

    // Fixed-extent requires exact size
    process_fixed(arr3);
    // process_fixed(arr5);  // Would not compile: wrong size

    // Creating fixed-extent span manually
    std::span<int, 3> fixed{arr5, 3};  // First 3 elements
    process_fixed(fixed);
}

// ============================================================================
// Subspans
// ============================================================================

void subspan_demo() {
    std::cout << "\n=== Subspans ===\n";

    std::vector<int> vec{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    std::span<int> full{vec};

    std::cout << "Full: ";
    print_span(full);

    // first() - get first N elements
    std::cout << "First 3: ";
    print_span(full.first(3));

    // last() - get last N elements
    std::cout << "Last 3: ";
    print_span(full.last(3));

    // subspan() - get range [offset, offset + count)
    std::cout << "Middle (3 to 6): ";
    print_span(full.subspan(3, 4));

    // subspan() with only offset - from offset to end
    std::cout << "From index 5: ";
    print_span(full.subspan(5));

    // Compile-time subspans
    std::span<int, 10> fixed{vec};
    auto first_five = fixed.first<5>();  // Returns span<int, 5>
    std::cout << "Compile-time first 5: ";
    print_span(first_five);
}

// ============================================================================
// span Element Access
// ============================================================================

void element_access_demo() {
    std::cout << "\n=== Element Access ===\n";

    std::vector<int> vec{10, 20, 30, 40, 50};
    std::span<int> s{vec};

    // Subscript operator (unchecked)
    std::cout << "s[2] = " << s[2] << "\n";

    // front() and back()
    std::cout << "Front: " << s.front() << ", Back: " << s.back() << "\n";

    // data() - raw pointer
    int* ptr = s.data();
    std::cout << "*data() = " << *ptr << "\n";

    // size() and size_bytes()
    std::cout << "Size: " << s.size() << " elements\n";
    std::cout << "Size in bytes: " << s.size_bytes() << "\n";

    // empty()
    std::span<int> empty_span;
    std::cout << "Empty span is empty: " << std::boolalpha << empty_span.empty() << "\n";
}

// ============================================================================
// span with Algorithms
// ============================================================================

void algorithm_demo() {
    std::cout << "\n=== span with Algorithms ===\n";

    std::vector<int> vec{5, 2, 8, 1, 9, 3, 7, 4, 6};
    std::span<int> s{vec};

    std::cout << "Original: ";
    print_span(s);

    // Sort
    std::ranges::sort(s);
    std::cout << "Sorted: ";
    print_span(s);

    // Find
    auto it = std::ranges::find(s, 5);
    if (it != s.end()) {
        std::cout << "Found 5 at index: " << std::distance(s.begin(), it) << "\n";
    }

    // Accumulate
    int sum = std::accumulate(s.begin(), s.end(), 0);
    std::cout << "Sum: " << sum << "\n";

    // Transform in place
    std::ranges::transform(s, s.begin(), [](int x) { return x * x; });
    std::cout << "Squared: ";
    print_span(s);
}

// ============================================================================
// span of bytes
// ============================================================================

void bytes_demo() {
    std::cout << "\n=== span of Bytes ===\n";

    struct Point {
        float x, y, z;
    };

    Point p{1.0f, 2.0f, 3.0f};

    // Get byte representation (read-only)
    auto bytes = std::as_bytes(std::span{&p, 1});
    std::cout << "Point as " << bytes.size() << " bytes: ";
    for (auto b : bytes) {
        std::cout << std::hex << static_cast<int>(static_cast<unsigned char>(b)) << " ";
    }
    std::cout << std::dec << "\n";

    // Writable byte span
    auto writable_bytes = std::as_writable_bytes(std::span{&p, 1});
    std::cout << "Writable bytes size: " << writable_bytes.size() << "\n";
}

// ============================================================================
// Avoiding Dangling spans
// ============================================================================

void dangling_warning() {
    std::cout << "\n=== Lifetime Considerations ===\n";

    // GOOD: span from data that outlives it
    std::vector<int> vec{1, 2, 3};
    std::span<int> good_span{vec};
    std::cout << "Good span: ";
    print_span(good_span);

    // BAD: Would create dangling span
    // std::span<int> bad_span;
    // {
    //     std::vector<int> temp{1, 2, 3};
    //     bad_span = std::span{temp};
    // }  // temp destroyed, bad_span is now dangling!

    // Also BAD: Returning span to local
    // auto bad_func() -> std::span<int> {
    //     std::vector<int> local{1, 2, 3};
    //     return std::span{local};  // Dangling!
    // }

    std::cout << "Always ensure the underlying data outlives the span!\n";
}

// ============================================================================
// span vs Other Options
// ============================================================================

void comparison_demo() {
    std::cout << "\n=== When to Use span ===\n";

    // Use span when you need a non-owning view that can accept
    // different contiguous container types

    std::vector<int> vec{1, 2, 3};
    int arr[] = {4, 5, 6};
    std::array<int, 3> std_arr{7, 8, 9};

    // All can be passed to the same function
    auto process = [](std::span<const int> data, const char* source) {
        int sum = 0;
        for (int x : data) sum += x;
        std::cout << "Sum from " << source << ": " << sum << "\n";
    };

    process(vec, "vector");
    process(arr, "C array");
    process(std_arr, "std::array");

    // When NOT to use span:
    // - When you need ownership (use vector, array, or unique_ptr)
    // - When you need string operations (use string_view)
    // - For non-contiguous containers (use iterators or ranges)
}

int main() {
    basic_usage();
    modification_demo();
    extent_demo();
    subspan_demo();
    element_access_demo();
    algorithm_demo();
    bytes_demo();
    dangling_warning();
    comparison_demo();

    return 0;
}

// Key points:
// - span is a lightweight, non-owning view of contiguous data
// - Works with vectors, arrays, and C-style arrays uniformly
// - Can be fixed-extent (size at compile time) or dynamic-extent
// - Supports subspans via first(), last(), subspan()
// - Does NOT own data - ensure underlying data outlives the span
// - Prefer std::span<const T> for read-only access
// - Use as_bytes() / as_writable_bytes() for raw byte access
