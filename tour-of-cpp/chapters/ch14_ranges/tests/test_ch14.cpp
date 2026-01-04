// Chapter 14 Tests: Ranges
// These tests verify understanding of C++20 ranges and views.

#include <algorithm>
#include <catch2/catch_test_macros.hpp>
#include <map>
#include <ranges>
#include <string>
#include <vector>

namespace views = std::views;
namespace ranges = std::ranges;

// ============================================================================
// Section 14.2: Views
// ============================================================================

TEST_CASE("views: filter", "[ch14][views]") {
    std::vector<int> v{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    std::vector<int> evens;
    for (int x : v | views::filter([](int n) { return n % 2 == 0; })) {
        evens.push_back(x);
    }

    REQUIRE(evens == std::vector<int>{2, 4, 6, 8, 10});
}

TEST_CASE("views: transform", "[ch14][views]") {
    std::vector<int> v{1, 2, 3, 4, 5};

    std::vector<int> squares;
    for (int x : v | views::transform([](int n) { return n * n; })) {
        squares.push_back(x);
    }

    REQUIRE(squares == std::vector<int>{1, 4, 9, 16, 25});
}

TEST_CASE("views: take and drop", "[ch14][views]") {
    std::vector<int> v{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    std::vector<int> first_five;
    for (int x : v | views::take(5)) {
        first_five.push_back(x);
    }
    REQUIRE(first_five == std::vector<int>{1, 2, 3, 4, 5});

    std::vector<int> after_five;
    for (int x : v | views::drop(5)) {
        after_five.push_back(x);
    }
    REQUIRE(after_five == std::vector<int>{6, 7, 8, 9, 10});
}

TEST_CASE("views: take_while and drop_while", "[ch14][views]") {
    std::vector<int> v{1, 2, 3, 10, 4, 5, 6};

    std::vector<int> before_10;
    for (int x : v | views::take_while([](int n) { return n < 10; })) {
        before_10.push_back(x);
    }
    REQUIRE(before_10 == std::vector<int>{1, 2, 3});

    std::vector<int> from_10;
    for (int x : v | views::drop_while([](int n) { return n < 10; })) {
        from_10.push_back(x);
    }
    REQUIRE(from_10 == std::vector<int>{10, 4, 5, 6});
}

TEST_CASE("views: reverse", "[ch14][views]") {
    std::vector<int> v{1, 2, 3, 4, 5};

    std::vector<int> reversed;
    for (int x : v | views::reverse) {
        reversed.push_back(x);
    }

    REQUIRE(reversed == std::vector<int>{5, 4, 3, 2, 1});
}

TEST_CASE("views: iota", "[ch14][views]") {
    std::vector<int> result;
    for (int x : views::iota(1, 6)) {
        result.push_back(x);
    }

    REQUIRE(result == std::vector<int>{1, 2, 3, 4, 5});
}

TEST_CASE("views: iota infinite with take", "[ch14][views]") {
    std::vector<int> result;
    for (int x : views::iota(100) | views::take(5)) {
        result.push_back(x);
    }

    REQUIRE(result == std::vector<int>{100, 101, 102, 103, 104});
}

TEST_CASE("views: keys and values", "[ch14][views]") {
    std::map<std::string, int> m{{"a", 1}, {"b", 2}, {"c", 3}};

    std::vector<std::string> keys;
    for (const auto& k : m | views::keys) {
        keys.push_back(k);
    }
    REQUIRE(keys == std::vector<std::string>{"a", "b", "c"});

    std::vector<int> values;
    for (int v : m | views::values) {
        values.push_back(v);
    }
    REQUIRE(values == std::vector<int>{1, 2, 3});
}

TEST_CASE("views: join", "[ch14][views]") {
    std::vector<std::vector<int>> nested{{1, 2}, {3, 4, 5}, {6}};

    std::vector<int> flattened;
    for (int x : nested | views::join) {
        flattened.push_back(x);
    }

    REQUIRE(flattened == std::vector<int>{1, 2, 3, 4, 5, 6});
}

TEST_CASE("views: split", "[ch14][views]") {
    std::string s = "a,b,c,d";

    std::vector<std::string> parts;
    for (auto part : s | views::split(',')) {
        parts.emplace_back(part.begin(), part.end());
    }

    REQUIRE(parts == std::vector<std::string>{"a", "b", "c", "d"});
}

// ============================================================================
// Section 14.4: Pipelines
// ============================================================================

TEST_CASE("pipelines: chained views", "[ch14][pipeline]") {
    std::vector<int> v{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    std::vector<int> result;
    for (int x : v
                 | views::filter([](int n) { return n % 2 == 0; })
                 | views::transform([](int n) { return n * n; })
                 | views::take(3)) {
        result.push_back(x);
    }

    // Even numbers: 2, 4, 6, 8, 10
    // Squared: 4, 16, 36, 64, 100
    // Take 3: 4, 16, 36
    REQUIRE(result == std::vector<int>{4, 16, 36});
}

TEST_CASE("pipelines: reusable adaptor", "[ch14][pipeline]") {
    auto even_doubled = views::filter([](int n) { return n % 2 == 0; })
                      | views::transform([](int n) { return n * 2; });

    std::vector<int> v1{1, 2, 3, 4, 5};
    std::vector<int> result1;
    for (int x : v1 | even_doubled) {
        result1.push_back(x);
    }
    REQUIRE(result1 == std::vector<int>{4, 8});

    std::vector<int> v2{10, 11, 12, 13};
    std::vector<int> result2;
    for (int x : v2 | even_doubled) {
        result2.push_back(x);
    }
    REQUIRE(result2 == std::vector<int>{20, 24});
}

// ============================================================================
// Range algorithms
// ============================================================================

TEST_CASE("ranges: sort", "[ch14][algorithm]") {
    std::vector<int> v{5, 2, 8, 1, 9};

    ranges::sort(v);
    REQUIRE(v == std::vector<int>{1, 2, 5, 8, 9});

    ranges::sort(v, std::greater{});
    REQUIRE(v == std::vector<int>{9, 8, 5, 2, 1});
}

TEST_CASE("ranges: sort with projection", "[ch14][algorithm]") {
    struct Person {
        std::string name;
        int age;
    };

    std::vector<Person> people{
        {"Alice", 30}, {"Bob", 25}, {"Charlie", 35}
    };

    ranges::sort(people, {}, &Person::age);

    REQUIRE(people[0].name == "Bob");
    REQUIRE(people[1].name == "Alice");
    REQUIRE(people[2].name == "Charlie");
}

TEST_CASE("ranges: find", "[ch14][algorithm]") {
    std::vector<int> v{1, 2, 3, 4, 5};

    auto it = ranges::find(v, 3);
    REQUIRE(it != v.end());
    REQUIRE(*it == 3);

    it = ranges::find(v, 99);
    REQUIRE(it == v.end());
}

TEST_CASE("ranges: find_if", "[ch14][algorithm]") {
    std::vector<int> v{1, 2, 3, 4, 5};

    auto it = ranges::find_if(v, [](int n) { return n > 3; });
    REQUIRE(it != v.end());
    REQUIRE(*it == 4);
}

TEST_CASE("ranges: all_of, any_of, none_of", "[ch14][algorithm]") {
    std::vector<int> positives{1, 2, 3, 4, 5};
    std::vector<int> mixed{-1, 0, 1, 2, 3};
    std::vector<int> negatives{-5, -4, -3, -2, -1};

    auto is_positive = [](int n) { return n > 0; };

    REQUIRE(ranges::all_of(positives, is_positive));
    REQUIRE(ranges::any_of(mixed, is_positive));
    REQUIRE(ranges::none_of(negatives, is_positive));
}

TEST_CASE("ranges: count and count_if", "[ch14][algorithm]") {
    std::vector<int> v{1, 2, 2, 3, 2, 4, 2};

    REQUIRE(ranges::count(v, 2) == 4);
    REQUIRE(ranges::count_if(v, [](int n) { return n % 2 == 0; }) == 5);
}

TEST_CASE("ranges: copy", "[ch14][algorithm]") {
    std::vector<int> src{1, 2, 3, 4, 5};
    std::vector<int> dst(5);

    ranges::copy(src, dst.begin());

    REQUIRE(dst == src);
}

TEST_CASE("ranges: transform", "[ch14][algorithm]") {
    std::vector<int> src{1, 2, 3, 4, 5};
    std::vector<int> dst(5);

    ranges::transform(src, dst.begin(), [](int n) { return n * 2; });

    REQUIRE(dst == std::vector<int>{2, 4, 6, 8, 10});
}

TEST_CASE("ranges: distance", "[ch14][algorithm]") {
    std::vector<int> v{1, 2, 3, 4, 5};

    auto count = ranges::distance(v);
    REQUIRE(count == 5);

    auto filtered = v | views::filter([](int n) { return n % 2 == 0; });
    REQUIRE(ranges::distance(filtered) == 2);
}

// ============================================================================
// Views are lazy
// ============================================================================

TEST_CASE("views: laziness", "[ch14][views]") {
    std::vector<int> v{1, 2, 3, 4, 5};
    int call_count = 0;

    auto view = v | views::transform([&call_count](int n) {
        ++call_count;
        return n * 2;
    }) | views::take(2);

    // No calls yet - view is lazy
    REQUIRE(call_count == 0);

    // Iterate to trigger evaluation
    std::vector<int> result;
    for (int x : view) {
        result.push_back(x);
    }

    // Only 2 calls made (take(2))
    REQUIRE(call_count == 2);
    REQUIRE(result == std::vector<int>{2, 4});
}

// ============================================================================
// Views reference data
// ============================================================================

TEST_CASE("views: reference semantics", "[ch14][views]") {
    std::vector<int> v{1, 2, 3, 4, 5};
    auto view = v | views::transform([](int n) { return n * 2; });

    // Modify original
    v[0] = 100;

    // View reflects the change
    std::vector<int> result;
    for (int x : view) {
        result.push_back(x);
    }

    REQUIRE(result[0] == 200);  // 100 * 2
}
