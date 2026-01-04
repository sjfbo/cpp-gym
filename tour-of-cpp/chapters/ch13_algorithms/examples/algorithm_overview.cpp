// Algorithm Overview - Common Algorithms Demonstrated
// Book reference: 13.5 Algorithm Overview
//
// The C++ Standard Library provides over 100 algorithms.
// This example covers the most commonly used ones:
// sort, find, transform, accumulate, and more.

#include <algorithm>
#include <iostream>
#include <iterator>
#include <numeric>
#include <random>
#include <string>
#include <vector>

// Helper to print vectors
template <typename T>
void print(const std::string& label, const std::vector<T>& v) {
    std::cout << label << ": ";
    for (const auto& x : v) std::cout << x << " ";
    std::cout << "\n";
}

int main() {
    std::cout << "=== Algorithm Overview ===\n\n";

    // -------------------------------------------------------------------------
    // Sorting algorithms
    // -------------------------------------------------------------------------
    std::cout << "--- Sorting ---\n";

    std::vector<int> nums{5, 2, 8, 1, 9, 3, 7, 4, 6};
    print("Original", nums);

    // std::sort - O(n log n), not stable
    std::sort(nums.begin(), nums.end());
    print("After sort()", nums);

    std::sort(nums.begin(), nums.end(), std::greater<>{});
    print("Descending sort", nums);

    // std::stable_sort - preserves order of equal elements
    std::vector<std::pair<int, char>> items{{1, 'a'}, {2, 'b'}, {1, 'c'}, {2, 'd'}};
    std::stable_sort(items.begin(), items.end(),
        [](const auto& a, const auto& b) { return a.first < b.first; });
    std::cout << "Stable sort: ";
    for (const auto& [num, ch] : items) std::cout << "(" << num << "," << ch << ") ";
    std::cout << "\n";

    // std::partial_sort - sort only first n elements
    std::vector<int> partial{5, 2, 8, 1, 9, 3, 7};
    std::partial_sort(partial.begin(), partial.begin() + 3, partial.end());
    print("partial_sort (top 3)", partial);

    // std::nth_element - partition around nth element
    std::vector<int> nth_demo{5, 2, 8, 1, 9, 3, 7, 4, 6};
    std::nth_element(nth_demo.begin(), nth_demo.begin() + 4, nth_demo.end());
    print("nth_element (4th)", nth_demo);
    std::cout << "4th element (median): " << nth_demo[4] << "\n";

    // std::is_sorted
    std::cout << "nums is sorted: " << std::is_sorted(nums.begin(), nums.end()) << "\n";

    // -------------------------------------------------------------------------
    // Searching algorithms
    // -------------------------------------------------------------------------
    std::cout << "\n--- Searching ---\n";

    std::vector<int> search_data{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    // std::find - linear search
    auto it = std::find(search_data.begin(), search_data.end(), 7);
    if (it != search_data.end()) {
        std::cout << "find(7): found at index " << std::distance(search_data.begin(), it) << "\n";
    }

    // std::find_if - search with predicate
    auto even_it = std::find_if(search_data.begin(), search_data.end(),
        [](int x) { return x > 5 && x % 2 == 0; });
    if (even_it != search_data.end()) {
        std::cout << "find_if(>5 and even): " << *even_it << "\n";
    }

    // std::binary_search - O(log n), requires sorted range
    bool found = std::binary_search(search_data.begin(), search_data.end(), 7);
    std::cout << "binary_search(7): " << (found ? "found" : "not found") << "\n";

    // std::lower_bound, std::upper_bound
    auto lower = std::lower_bound(search_data.begin(), search_data.end(), 5);
    auto upper = std::upper_bound(search_data.begin(), search_data.end(), 5);
    std::cout << "lower_bound(5): " << *lower << ", upper_bound(5): " << *upper << "\n";

    // std::equal_range - both bounds at once
    std::vector<int> with_dups{1, 2, 2, 2, 3, 4, 5};
    auto [lo, hi] = std::equal_range(with_dups.begin(), with_dups.end(), 2);
    std::cout << "equal_range(2): [" << std::distance(with_dups.begin(), lo)
              << ", " << std::distance(with_dups.begin(), hi) << ")\n";

    // -------------------------------------------------------------------------
    // Counting
    // -------------------------------------------------------------------------
    std::cout << "\n--- Counting ---\n";

    std::vector<int> count_data{1, 2, 3, 2, 4, 2, 5, 2, 6};

    auto count_2 = std::count(count_data.begin(), count_data.end(), 2);
    std::cout << "count(2): " << count_2 << "\n";

    auto count_even = std::count_if(count_data.begin(), count_data.end(),
        [](int x) { return x % 2 == 0; });
    std::cout << "count_if(even): " << count_even << "\n";

    // -------------------------------------------------------------------------
    // Transform and for_each
    // -------------------------------------------------------------------------
    std::cout << "\n--- Transform and For Each ---\n";

    std::vector<int> src{1, 2, 3, 4, 5};
    std::vector<int> dst(5);

    // std::transform - apply function to each element
    std::transform(src.begin(), src.end(), dst.begin(),
        [](int x) { return x * x; });
    print("transform (square)", dst);

    // Binary transform - combine two ranges
    std::vector<int> a{1, 2, 3, 4, 5};
    std::vector<int> b{10, 20, 30, 40, 50};
    std::vector<int> sum(5);
    std::transform(a.begin(), a.end(), b.begin(), sum.begin(),
        [](int x, int y) { return x + y; });
    print("transform (a + b)", sum);

    // std::for_each - apply function for side effects
    std::cout << "for_each (print): ";
    std::for_each(src.begin(), src.end(), [](int x) { std::cout << x << " "; });
    std::cout << "\n";

    // -------------------------------------------------------------------------
    // Accumulate and reduce
    // -------------------------------------------------------------------------
    std::cout << "\n--- Accumulate and Reduce ---\n";

    std::vector<int> acc_data{1, 2, 3, 4, 5};

    // std::accumulate - fold left
    auto sum_val = std::accumulate(acc_data.begin(), acc_data.end(), 0);
    std::cout << "accumulate (sum): " << sum_val << "\n";

    auto product = std::accumulate(acc_data.begin(), acc_data.end(), 1,
        std::multiplies<>{});
    std::cout << "accumulate (product): " << product << "\n";

    // Custom accumulator: build a string
    auto str = std::accumulate(acc_data.begin(), acc_data.end(), std::string{},
        [](std::string s, int x) { return s + std::to_string(x) + ","; });
    std::cout << "accumulate (string): " << str << "\n";

    // std::reduce - like accumulate but allows reordering (parallel-friendly)
    auto reduce_sum = std::reduce(acc_data.begin(), acc_data.end());
    std::cout << "reduce (sum): " << reduce_sum << "\n";

    // std::inner_product - dot product
    std::vector<int> v1{1, 2, 3};
    std::vector<int> v2{4, 5, 6};
    auto dot = std::inner_product(v1.begin(), v1.end(), v2.begin(), 0);
    std::cout << "inner_product (dot): " << dot << "\n";

    // -------------------------------------------------------------------------
    // Modifying algorithms
    // -------------------------------------------------------------------------
    std::cout << "\n--- Modifying Algorithms ---\n";

    // std::fill
    std::vector<int> filled(5);
    std::fill(filled.begin(), filled.end(), 42);
    print("fill(42)", filled);

    // std::generate
    std::vector<int> generated(5);
    int counter = 0;
    std::generate(generated.begin(), generated.end(), [&counter]() { return counter++; });
    print("generate(0,1,2...)", generated);

    // std::iota (numeric header)
    std::vector<int> iota_vec(5);
    std::iota(iota_vec.begin(), iota_vec.end(), 10);
    print("iota(10)", iota_vec);

    // std::replace
    std::vector<int> to_replace{1, 2, 3, 2, 4, 2};
    std::replace(to_replace.begin(), to_replace.end(), 2, 99);
    print("replace(2 -> 99)", to_replace);

    // std::reverse
    std::vector<int> to_reverse{1, 2, 3, 4, 5};
    std::reverse(to_reverse.begin(), to_reverse.end());
    print("reverse", to_reverse);

    // std::rotate
    std::vector<int> to_rotate{1, 2, 3, 4, 5};
    std::rotate(to_rotate.begin(), to_rotate.begin() + 2, to_rotate.end());
    print("rotate by 2", to_rotate);

    // std::shuffle
    std::vector<int> to_shuffle{1, 2, 3, 4, 5};
    std::random_device rd;
    std::mt19937 gen(rd());
    std::shuffle(to_shuffle.begin(), to_shuffle.end(), gen);
    print("shuffle", to_shuffle);

    // -------------------------------------------------------------------------
    // Removing elements (note: doesn't actually remove!)
    // -------------------------------------------------------------------------
    std::cout << "\n--- Remove Algorithms ---\n";

    std::vector<int> remove_demo{1, 2, 3, 2, 4, 2, 5};
    print("Original", remove_demo);

    // std::remove moves elements, returns new logical end
    auto new_end = std::remove(remove_demo.begin(), remove_demo.end(), 2);
    std::cout << "After remove(2), size unchanged: " << remove_demo.size() << "\n";
    print("Elements now", remove_demo);
    std::cout << "Logical end at index: " << std::distance(remove_demo.begin(), new_end) << "\n";

    // Erase the "removed" elements
    remove_demo.erase(new_end, remove_demo.end());
    print("After erase", remove_demo);

    // C++20: std::erase (does both steps)
    std::vector<int> erase_demo{1, 2, 3, 2, 4, 2, 5};
    std::erase(erase_demo, 2);
    print("std::erase(2)", erase_demo);

    // std::unique - remove consecutive duplicates
    std::vector<int> dups{1, 1, 2, 2, 2, 3, 3, 4};
    auto unique_end = std::unique(dups.begin(), dups.end());
    dups.erase(unique_end, dups.end());
    print("unique", dups);

    // -------------------------------------------------------------------------
    // Copying
    // -------------------------------------------------------------------------
    std::cout << "\n--- Copy Algorithms ---\n";

    std::vector<int> copy_src{1, 2, 3, 4, 5};
    std::vector<int> copy_dst(5);

    std::copy(copy_src.begin(), copy_src.end(), copy_dst.begin());
    print("copy", copy_dst);

    std::vector<int> copy_if_dst;
    std::copy_if(copy_src.begin(), copy_src.end(), std::back_inserter(copy_if_dst),
        [](int x) { return x % 2 == 1; });
    print("copy_if(odd)", copy_if_dst);

    // -------------------------------------------------------------------------
    // Min/Max
    // -------------------------------------------------------------------------
    std::cout << "\n--- Min/Max ---\n";

    std::vector<int> minmax_data{3, 1, 4, 1, 5, 9, 2, 6};

    auto min_it = std::min_element(minmax_data.begin(), minmax_data.end());
    auto max_it = std::max_element(minmax_data.begin(), minmax_data.end());
    std::cout << "min_element: " << *min_it << " at index "
              << std::distance(minmax_data.begin(), min_it) << "\n";
    std::cout << "max_element: " << *max_it << " at index "
              << std::distance(minmax_data.begin(), max_it) << "\n";

    auto [min, max] = std::minmax_element(minmax_data.begin(), minmax_data.end());
    std::cout << "minmax_element: [" << *min << ", " << *max << "]\n";

    // std::clamp
    for (int val : {-5, 5, 15}) {
        std::cout << "clamp(" << val << ", 0, 10) = " << std::clamp(val, 0, 10) << "\n";
    }

    return 0;
}

// Key takeaways:
// - Algorithms work on iterator ranges [begin, end)
// - Many algorithms return iterators (find, remove, partition)
// - remove doesn't actually remove - use erase-remove idiom or C++20 std::erase
// - Use binary_search, lower_bound, upper_bound on sorted ranges
// - accumulate for folding, transform for mapping
// - std::reduce is parallel-friendly (allows reordering)
