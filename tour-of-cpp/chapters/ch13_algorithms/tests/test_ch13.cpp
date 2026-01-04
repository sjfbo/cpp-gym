// Chapter 13 Tests: Algorithms
// These tests verify understanding of C++ Standard Library algorithms.

#include <algorithm>
#include <catch2/catch_test_macros.hpp>
#include <functional>
#include <iterator>
#include <numeric>
#include <string>
#include <vector>

// ============================================================================
// Section 13.3: Iterators
// ============================================================================

TEST_CASE("iterators: basic operations", "[ch13][iterator]") {
    std::vector<int> v{1, 2, 3, 4, 5};

    auto it = v.begin();
    REQUIRE(*it == 1);

    ++it;
    REQUIRE(*it == 2);

    it += 2;
    REQUIRE(*it == 4);

    auto dist = std::distance(v.begin(), it);
    REQUIRE(dist == 3);
}

TEST_CASE("iterators: reverse iterators", "[ch13][iterator]") {
    std::vector<int> v{1, 2, 3, 4, 5};

    std::vector<int> reversed;
    for (auto it = v.rbegin(); it != v.rend(); ++it) {
        reversed.push_back(*it);
    }

    REQUIRE(reversed == std::vector<int>{5, 4, 3, 2, 1});
}

TEST_CASE("iterators: back_inserter", "[ch13][iterator]") {
    std::vector<int> src{1, 2, 3};
    std::vector<int> dst;

    std::copy(src.begin(), src.end(), std::back_inserter(dst));

    REQUIRE(dst == src);
}

// ============================================================================
// Section 13.4: Predicates
// ============================================================================

TEST_CASE("predicates: find_if with lambda", "[ch13][predicate]") {
    std::vector<int> v{1, 2, 3, 4, 5, 6};

    auto it = std::find_if(v.begin(), v.end(), [](int x) { return x > 3; });

    REQUIRE(it != v.end());
    REQUIRE(*it == 4);
}

TEST_CASE("predicates: count_if", "[ch13][predicate]") {
    std::vector<int> v{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    auto even_count = std::count_if(v.begin(), v.end(),
                                    [](int x) { return x % 2 == 0; });

    REQUIRE(even_count == 5);
}

TEST_CASE("predicates: all_of, any_of, none_of", "[ch13][predicate]") {
    std::vector<int> positives{1, 2, 3, 4, 5};
    std::vector<int> mixed{-1, 0, 1, 2, 3};
    std::vector<int> negatives{-5, -4, -3, -2, -1};

    auto is_positive = [](int x) { return x > 0; };

    REQUIRE(std::all_of(positives.begin(), positives.end(), is_positive));
    REQUIRE(std::any_of(mixed.begin(), mixed.end(), is_positive));
    REQUIRE(std::none_of(negatives.begin(), negatives.end(), is_positive));
}

TEST_CASE("predicates: partition", "[ch13][predicate]") {
    std::vector<int> v{1, 2, 3, 4, 5, 6, 7, 8};

    auto partition_point =
        std::partition(v.begin(), v.end(), [](int x) { return x % 2 == 0; });

    // All even numbers before partition point
    REQUIRE(std::all_of(v.begin(), partition_point,
                        [](int x) { return x % 2 == 0; }));

    // All odd numbers after partition point
    REQUIRE(std::all_of(partition_point, v.end(),
                        [](int x) { return x % 2 != 0; }));
}

// ============================================================================
// Section 13.5: Algorithm Overview - Sorting
// ============================================================================

TEST_CASE("sorting: std::sort", "[ch13][sort]") {
    std::vector<int> v{5, 2, 8, 1, 9};

    std::sort(v.begin(), v.end());

    REQUIRE(v == std::vector<int>{1, 2, 5, 8, 9});
}

TEST_CASE("sorting: std::sort with custom comparator", "[ch13][sort]") {
    std::vector<int> v{5, 2, 8, 1, 9};

    std::sort(v.begin(), v.end(), std::greater<>{});

    REQUIRE(v == std::vector<int>{9, 8, 5, 2, 1});
}

TEST_CASE("sorting: std::stable_sort", "[ch13][sort]") {
    std::vector<std::pair<int, char>> v{{1, 'a'}, {2, 'b'}, {1, 'c'}, {2, 'd'}};

    std::stable_sort(v.begin(), v.end(),
                     [](const auto& a, const auto& b) { return a.first < b.first; });

    // Stable sort preserves relative order of equal elements
    REQUIRE(v[0] == std::pair{1, 'a'});
    REQUIRE(v[1] == std::pair{1, 'c'});
    REQUIRE(v[2] == std::pair{2, 'b'});
    REQUIRE(v[3] == std::pair{2, 'd'});
}

TEST_CASE("sorting: std::partial_sort", "[ch13][sort]") {
    std::vector<int> v{5, 2, 8, 1, 9, 3, 7};

    std::partial_sort(v.begin(), v.begin() + 3, v.end());

    // First 3 elements are the smallest in sorted order
    REQUIRE(v[0] == 1);
    REQUIRE(v[1] == 2);
    REQUIRE(v[2] == 3);
}

TEST_CASE("sorting: std::nth_element", "[ch13][sort]") {
    std::vector<int> v{5, 2, 8, 1, 9, 3, 7, 4, 6};

    std::nth_element(v.begin(), v.begin() + 4, v.end());

    // The 4th element (index 4) is the median
    REQUIRE(v[4] == 5);

    // All elements before are <= v[4]
    REQUIRE(std::all_of(v.begin(), v.begin() + 4,
                        [&](int x) { return x <= v[4]; }));

    // All elements after are >= v[4]
    REQUIRE(std::all_of(v.begin() + 5, v.end(),
                        [&](int x) { return x >= v[4]; }));
}

TEST_CASE("sorting: std::is_sorted", "[ch13][sort]") {
    std::vector<int> sorted{1, 2, 3, 4, 5};
    std::vector<int> unsorted{1, 3, 2, 4, 5};

    REQUIRE(std::is_sorted(sorted.begin(), sorted.end()));
    REQUIRE_FALSE(std::is_sorted(unsorted.begin(), unsorted.end()));
}

// ============================================================================
// Section 13.5: Algorithm Overview - Searching
// ============================================================================

TEST_CASE("searching: std::find", "[ch13][search]") {
    std::vector<int> v{1, 2, 3, 4, 5};

    auto it = std::find(v.begin(), v.end(), 3);
    REQUIRE(it != v.end());
    REQUIRE(*it == 3);

    it = std::find(v.begin(), v.end(), 99);
    REQUIRE(it == v.end());
}

TEST_CASE("searching: std::binary_search", "[ch13][search]") {
    std::vector<int> v{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    REQUIRE(std::binary_search(v.begin(), v.end(), 5));
    REQUIRE_FALSE(std::binary_search(v.begin(), v.end(), 99));
}

TEST_CASE("searching: std::lower_bound and upper_bound", "[ch13][search]") {
    std::vector<int> v{1, 2, 2, 2, 3, 4, 5};

    auto lower = std::lower_bound(v.begin(), v.end(), 2);
    auto upper = std::upper_bound(v.begin(), v.end(), 2);

    REQUIRE(*lower == 2);
    REQUIRE(std::distance(v.begin(), lower) == 1);  // Index 1

    REQUIRE(*upper == 3);
    REQUIRE(std::distance(v.begin(), upper) == 4);  // Index 4
}

TEST_CASE("searching: std::equal_range", "[ch13][search]") {
    std::vector<int> v{1, 2, 2, 2, 3, 4, 5};

    auto [lo, hi] = std::equal_range(v.begin(), v.end(), 2);

    REQUIRE(std::distance(lo, hi) == 3);  // Three 2s
}

// ============================================================================
// Section 13.5: Algorithm Overview - Transform and Accumulate
// ============================================================================

TEST_CASE("transform: unary transform", "[ch13][transform]") {
    std::vector<int> src{1, 2, 3, 4, 5};
    std::vector<int> dst(5);

    std::transform(src.begin(), src.end(), dst.begin(),
                   [](int x) { return x * x; });

    REQUIRE(dst == std::vector<int>{1, 4, 9, 16, 25});
}

TEST_CASE("transform: binary transform", "[ch13][transform]") {
    std::vector<int> a{1, 2, 3};
    std::vector<int> b{10, 20, 30};
    std::vector<int> result(3);

    std::transform(a.begin(), a.end(), b.begin(), result.begin(),
                   std::plus<>{});

    REQUIRE(result == std::vector<int>{11, 22, 33});
}

TEST_CASE("accumulate: sum", "[ch13][accumulate]") {
    std::vector<int> v{1, 2, 3, 4, 5};

    auto sum = std::accumulate(v.begin(), v.end(), 0);

    REQUIRE(sum == 15);
}

TEST_CASE("accumulate: product", "[ch13][accumulate]") {
    std::vector<int> v{1, 2, 3, 4, 5};

    auto product = std::accumulate(v.begin(), v.end(), 1, std::multiplies<>{});

    REQUIRE(product == 120);
}

TEST_CASE("accumulate: custom operation", "[ch13][accumulate]") {
    std::vector<std::string> words{"Hello", " ", "World", "!"};

    auto sentence = std::accumulate(words.begin(), words.end(), std::string{});

    REQUIRE(sentence == "Hello World!");
}

TEST_CASE("reduce: sum (parallel-friendly)", "[ch13][reduce]") {
    std::vector<int> v{1, 2, 3, 4, 5};

    auto sum = std::reduce(v.begin(), v.end());

    REQUIRE(sum == 15);
}

TEST_CASE("inner_product: dot product", "[ch13][accumulate]") {
    std::vector<int> a{1, 2, 3};
    std::vector<int> b{4, 5, 6};

    auto dot = std::inner_product(a.begin(), a.end(), b.begin(), 0);

    // 1*4 + 2*5 + 3*6 = 4 + 10 + 18 = 32
    REQUIRE(dot == 32);
}

// ============================================================================
// Section 13.5: Algorithm Overview - Modifying Algorithms
// ============================================================================

TEST_CASE("modifying: std::fill", "[ch13][modify]") {
    std::vector<int> v(5);

    std::fill(v.begin(), v.end(), 42);

    REQUIRE(v == std::vector<int>{42, 42, 42, 42, 42});
}

TEST_CASE("modifying: std::generate", "[ch13][modify]") {
    std::vector<int> v(5);
    int n = 0;

    std::generate(v.begin(), v.end(), [&n]() { return n++; });

    REQUIRE(v == std::vector<int>{0, 1, 2, 3, 4});
}

TEST_CASE("modifying: std::iota", "[ch13][modify]") {
    std::vector<int> v(5);

    std::iota(v.begin(), v.end(), 10);

    REQUIRE(v == std::vector<int>{10, 11, 12, 13, 14});
}

TEST_CASE("modifying: std::replace", "[ch13][modify]") {
    std::vector<int> v{1, 2, 3, 2, 4, 2};

    std::replace(v.begin(), v.end(), 2, 99);

    REQUIRE(v == std::vector<int>{1, 99, 3, 99, 4, 99});
}

TEST_CASE("modifying: std::reverse", "[ch13][modify]") {
    std::vector<int> v{1, 2, 3, 4, 5};

    std::reverse(v.begin(), v.end());

    REQUIRE(v == std::vector<int>{5, 4, 3, 2, 1});
}

TEST_CASE("modifying: std::rotate", "[ch13][modify]") {
    std::vector<int> v{1, 2, 3, 4, 5};

    std::rotate(v.begin(), v.begin() + 2, v.end());

    REQUIRE(v == std::vector<int>{3, 4, 5, 1, 2});
}

// ============================================================================
// Section 13.5: Algorithm Overview - Removing
// ============================================================================

TEST_CASE("removing: erase-remove idiom", "[ch13][remove]") {
    std::vector<int> v{1, 2, 3, 2, 4, 2, 5};

    auto new_end = std::remove(v.begin(), v.end(), 2);
    v.erase(new_end, v.end());

    REQUIRE(v == std::vector<int>{1, 3, 4, 5});
}

TEST_CASE("removing: C++20 std::erase", "[ch13][remove]") {
    std::vector<int> v{1, 2, 3, 2, 4, 2, 5};

    std::erase(v, 2);

    REQUIRE(v == std::vector<int>{1, 3, 4, 5});
}

TEST_CASE("removing: std::erase_if", "[ch13][remove]") {
    std::vector<int> v{1, 2, 3, 4, 5, 6, 7, 8};

    std::erase_if(v, [](int x) { return x % 2 == 0; });

    REQUIRE(v == std::vector<int>{1, 3, 5, 7});
}

TEST_CASE("removing: std::unique", "[ch13][remove]") {
    std::vector<int> v{1, 1, 2, 2, 2, 3, 3, 4};

    auto new_end = std::unique(v.begin(), v.end());
    v.erase(new_end, v.end());

    REQUIRE(v == std::vector<int>{1, 2, 3, 4});
}

// ============================================================================
// Section 13.5: Algorithm Overview - Copying
// ============================================================================

TEST_CASE("copying: std::copy", "[ch13][copy]") {
    std::vector<int> src{1, 2, 3, 4, 5};
    std::vector<int> dst(5);

    std::copy(src.begin(), src.end(), dst.begin());

    REQUIRE(dst == src);
}

TEST_CASE("copying: std::copy_if", "[ch13][copy]") {
    std::vector<int> src{1, 2, 3, 4, 5, 6, 7, 8};
    std::vector<int> dst;

    std::copy_if(src.begin(), src.end(), std::back_inserter(dst),
                 [](int x) { return x % 2 == 0; });

    REQUIRE(dst == std::vector<int>{2, 4, 6, 8});
}

TEST_CASE("copying: std::copy_n", "[ch13][copy]") {
    std::vector<int> src{1, 2, 3, 4, 5};
    std::vector<int> dst(3);

    std::copy_n(src.begin(), 3, dst.begin());

    REQUIRE(dst == std::vector<int>{1, 2, 3});
}

// ============================================================================
// Section 13.5: Algorithm Overview - Min/Max
// ============================================================================

TEST_CASE("minmax: min_element and max_element", "[ch13][minmax]") {
    std::vector<int> v{3, 1, 4, 1, 5, 9, 2, 6};

    auto min_it = std::min_element(v.begin(), v.end());
    auto max_it = std::max_element(v.begin(), v.end());

    REQUIRE(*min_it == 1);
    REQUIRE(*max_it == 9);
}

TEST_CASE("minmax: minmax_element", "[ch13][minmax]") {
    std::vector<int> v{3, 1, 4, 1, 5, 9, 2, 6};

    auto [min_it, max_it] = std::minmax_element(v.begin(), v.end());

    REQUIRE(*min_it == 1);
    REQUIRE(*max_it == 9);
}

TEST_CASE("minmax: std::clamp", "[ch13][minmax]") {
    REQUIRE(std::clamp(-5, 0, 10) == 0);
    REQUIRE(std::clamp(5, 0, 10) == 5);
    REQUIRE(std::clamp(15, 0, 10) == 10);
}

// ============================================================================
// Section 13.5: Algorithm Overview - Set operations
// ============================================================================

TEST_CASE("set operations: set_union", "[ch13][set]") {
    std::vector<int> a{1, 2, 3, 4, 5};
    std::vector<int> b{3, 4, 5, 6, 7};
    std::vector<int> result;

    std::set_union(a.begin(), a.end(), b.begin(), b.end(),
                   std::back_inserter(result));

    REQUIRE(result == std::vector<int>{1, 2, 3, 4, 5, 6, 7});
}

TEST_CASE("set operations: set_intersection", "[ch13][set]") {
    std::vector<int> a{1, 2, 3, 4, 5};
    std::vector<int> b{3, 4, 5, 6, 7};
    std::vector<int> result;

    std::set_intersection(a.begin(), a.end(), b.begin(), b.end(),
                          std::back_inserter(result));

    REQUIRE(result == std::vector<int>{3, 4, 5});
}

TEST_CASE("set operations: set_difference", "[ch13][set]") {
    std::vector<int> a{1, 2, 3, 4, 5};
    std::vector<int> b{3, 4, 5, 6, 7};
    std::vector<int> result;

    std::set_difference(a.begin(), a.end(), b.begin(), b.end(),
                        std::back_inserter(result));

    REQUIRE(result == std::vector<int>{1, 2});
}

TEST_CASE("set operations: includes", "[ch13][set]") {
    std::vector<int> superset{1, 2, 3, 4, 5, 6, 7};
    std::vector<int> subset{2, 4, 6};
    std::vector<int> not_subset{2, 4, 8};

    REQUIRE(std::includes(superset.begin(), superset.end(),
                          subset.begin(), subset.end()));
    REQUIRE_FALSE(std::includes(superset.begin(), superset.end(),
                                not_subset.begin(), not_subset.end()));
}
