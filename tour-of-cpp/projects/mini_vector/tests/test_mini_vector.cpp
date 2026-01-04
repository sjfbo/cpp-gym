#include <catch2/catch_test_macros.hpp>
#include "mini_vector.h"
#include <string>
#include <utility>

TEST_CASE("Vector default construction", "[vector][constructor]") {
    mini::Vector<int> v;

    REQUIRE(v.empty());
    REQUIRE(v.size() == 0);
    REQUIRE(v.capacity() == 0);
}

TEST_CASE("Vector construction with size", "[vector][constructor]") {
    mini::Vector<int> v(5);

    REQUIRE(v.size() == 5);
    REQUIRE(v.capacity() >= 5);
    REQUIRE_FALSE(v.empty());

    // Elements should be value-initialized (0 for int)
    for (size_t i = 0; i < v.size(); ++i) {
        REQUIRE(v[i] == 0);
    }
}

TEST_CASE("Vector construction with size and value", "[vector][constructor]") {
    mini::Vector<int> v(5, 42);

    REQUIRE(v.size() == 5);
    for (size_t i = 0; i < v.size(); ++i) {
        REQUIRE(v[i] == 42);
    }
}

TEST_CASE("Vector initializer list construction", "[vector][constructor]") {
    mini::Vector<int> v{1, 2, 3, 4, 5};

    REQUIRE(v.size() == 5);
    REQUIRE(v[0] == 1);
    REQUIRE(v[1] == 2);
    REQUIRE(v[2] == 3);
    REQUIRE(v[3] == 4);
    REQUIRE(v[4] == 5);
}

TEST_CASE("Vector push_back", "[vector][modifier]") {
    mini::Vector<int> v;

    v.push_back(10);
    REQUIRE(v.size() == 1);
    REQUIRE(v[0] == 10);

    v.push_back(20);
    REQUIRE(v.size() == 2);
    REQUIRE(v[1] == 20);

    v.push_back(30);
    REQUIRE(v.size() == 3);
    REQUIRE(v[2] == 30);
}

TEST_CASE("Vector push_back with move", "[vector][modifier]") {
    mini::Vector<std::string> v;

    std::string s = "hello";
    v.push_back(std::move(s));

    REQUIRE(v.size() == 1);
    REQUIRE(v[0] == "hello");
    // s should be empty after move (implementation-defined but typically empty)
}

TEST_CASE("Vector pop_back", "[vector][modifier]") {
    mini::Vector<int> v{1, 2, 3};

    v.pop_back();
    REQUIRE(v.size() == 2);
    REQUIRE(v.back() == 2);

    v.pop_back();
    REQUIRE(v.size() == 1);
    REQUIRE(v.back() == 1);

    v.pop_back();
    REQUIRE(v.empty());
}

TEST_CASE("Vector element access", "[vector][access]") {
    mini::Vector<int> v{10, 20, 30};

    SECTION("operator[]") {
        REQUIRE(v[0] == 10);
        REQUIRE(v[1] == 20);
        REQUIRE(v[2] == 30);

        v[1] = 25;
        REQUIRE(v[1] == 25);
    }

    SECTION("at() with valid index") {
        REQUIRE(v.at(0) == 10);
        REQUIRE(v.at(1) == 20);
        REQUIRE(v.at(2) == 30);
    }

    SECTION("at() with invalid index throws") {
        REQUIRE_THROWS_AS(v.at(3), std::out_of_range);
        REQUIRE_THROWS_AS(v.at(100), std::out_of_range);
    }

    SECTION("front() and back()") {
        REQUIRE(v.front() == 10);
        REQUIRE(v.back() == 30);

        v.front() = 5;
        v.back() = 35;

        REQUIRE(v.front() == 5);
        REQUIRE(v.back() == 35);
    }
}

TEST_CASE("Vector const element access", "[vector][access]") {
    const mini::Vector<int> v{10, 20, 30};

    REQUIRE(v[0] == 10);
    REQUIRE(v.at(1) == 20);
    REQUIRE(v.front() == 10);
    REQUIRE(v.back() == 30);
}

TEST_CASE("Vector copy constructor", "[vector][copy]") {
    mini::Vector<int> original{1, 2, 3, 4, 5};
    mini::Vector<int> copy(original);

    REQUIRE(copy.size() == original.size());

    // Verify deep copy
    for (size_t i = 0; i < original.size(); ++i) {
        REQUIRE(copy[i] == original[i]);
    }

    // Modify copy shouldn't affect original
    copy[0] = 100;
    REQUIRE(original[0] == 1);
}

TEST_CASE("Vector copy assignment", "[vector][copy]") {
    mini::Vector<int> original{1, 2, 3};
    mini::Vector<int> copy{10, 20};

    copy = original;

    REQUIRE(copy.size() == 3);
    REQUIRE(copy[0] == 1);
    REQUIRE(copy[1] == 2);
    REQUIRE(copy[2] == 3);

    // Self-assignment should be safe
    copy = copy;
    REQUIRE(copy.size() == 3);
}

TEST_CASE("Vector move constructor", "[vector][move]") {
    mini::Vector<int> original{1, 2, 3, 4, 5};
    size_t original_size = original.size();

    mini::Vector<int> moved(std::move(original));

    REQUIRE(moved.size() == original_size);
    REQUIRE(moved[0] == 1);
    REQUIRE(moved[4] == 5);

    // Original should be empty after move
    REQUIRE(original.empty());
    REQUIRE(original.data() == nullptr);
}

TEST_CASE("Vector move assignment", "[vector][move]") {
    mini::Vector<int> original{1, 2, 3};
    mini::Vector<int> target{10, 20};

    target = std::move(original);

    REQUIRE(target.size() == 3);
    REQUIRE(target[0] == 1);
    REQUIRE(original.empty());
}

TEST_CASE("Vector iterators", "[vector][iterator]") {
    mini::Vector<int> v{1, 2, 3, 4, 5};

    SECTION("begin() and end()") {
        REQUIRE(v.begin() == v.data());
        REQUIRE(v.end() == v.data() + v.size());
    }

    SECTION("range-based for loop") {
        int sum = 0;
        for (int val : v) {
            sum += val;
        }
        REQUIRE(sum == 15);
    }

    SECTION("modification through iterator") {
        for (auto& val : v) {
            val *= 2;
        }
        REQUIRE(v[0] == 2);
        REQUIRE(v[4] == 10);
    }
}

TEST_CASE("Vector const iterators", "[vector][iterator]") {
    const mini::Vector<int> v{1, 2, 3};

    int sum = 0;
    for (auto it = v.cbegin(); it != v.cend(); ++it) {
        sum += *it;
    }
    REQUIRE(sum == 6);
}

TEST_CASE("Vector capacity operations", "[vector][capacity]") {
    mini::Vector<int> v;

    SECTION("reserve increases capacity") {
        v.reserve(100);
        REQUIRE(v.capacity() >= 100);
        REQUIRE(v.size() == 0);
    }

    SECTION("reserve with smaller value does nothing") {
        v.reserve(100);
        size_t cap = v.capacity();
        v.reserve(50);
        REQUIRE(v.capacity() == cap);
    }

    SECTION("shrink_to_fit") {
        v.reserve(100);
        v.push_back(1);
        v.push_back(2);
        v.shrink_to_fit();
        REQUIRE(v.capacity() == 2);
    }
}

TEST_CASE("Vector clear", "[vector][modifier]") {
    mini::Vector<int> v{1, 2, 3, 4, 5};
    size_t old_capacity = v.capacity();

    v.clear();

    REQUIRE(v.empty());
    REQUIRE(v.size() == 0);
    REQUIRE(v.capacity() == old_capacity); // Capacity unchanged
}

TEST_CASE("Vector resize", "[vector][modifier]") {
    mini::Vector<int> v{1, 2, 3};

    SECTION("resize larger") {
        v.resize(5);
        REQUIRE(v.size() == 5);
        REQUIRE(v[3] == 0);
        REQUIRE(v[4] == 0);
    }

    SECTION("resize smaller") {
        v.resize(2);
        REQUIRE(v.size() == 2);
        REQUIRE(v[0] == 1);
        REQUIRE(v[1] == 2);
    }

    SECTION("resize with value") {
        v.resize(5, 99);
        REQUIRE(v[3] == 99);
        REQUIRE(v[4] == 99);
    }
}

TEST_CASE("Vector swap", "[vector][modifier]") {
    mini::Vector<int> v1{1, 2, 3};
    mini::Vector<int> v2{10, 20};

    v1.swap(v2);

    REQUIRE(v1.size() == 2);
    REQUIRE(v1[0] == 10);
    REQUIRE(v2.size() == 3);
    REQUIRE(v2[0] == 1);
}

TEST_CASE("Vector comparison operators", "[vector][comparison]") {
    mini::Vector<int> v1{1, 2, 3};
    mini::Vector<int> v2{1, 2, 3};
    mini::Vector<int> v3{1, 2, 4};
    mini::Vector<int> v4{1, 2};

    REQUIRE(v1 == v2);
    REQUIRE_FALSE(v1 != v2);

    REQUIRE(v1 != v3);
    REQUIRE(v1 != v4);
}

TEST_CASE("Vector emplace_back", "[vector][modifier]") {
    struct Point {
        int x, y;
        Point(int x = 0, int y = 0) : x(x), y(y) {}
    };

    mini::Vector<Point> points;
    points.emplace_back(1, 2);
    points.emplace_back(3, 4);

    REQUIRE(points.size() == 2);
    REQUIRE(points[0].x == 1);
    REQUIRE(points[0].y == 2);
    REQUIRE(points[1].x == 3);
    REQUIRE(points[1].y == 4);
}

TEST_CASE("Vector with strings", "[vector][string]") {
    mini::Vector<std::string> v;
    v.push_back("hello");
    v.push_back("world");

    REQUIRE(v.size() == 2);
    REQUIRE(v[0] == "hello");
    REQUIRE(v[1] == "world");

    // Test copy
    mini::Vector<std::string> copy = v;
    REQUIRE(copy[0] == "hello");

    // Test move
    mini::Vector<std::string> moved = std::move(v);
    REQUIRE(moved[0] == "hello");
    REQUIRE(v.empty());
}

TEST_CASE("Vector capacity growth", "[vector][capacity]") {
    mini::Vector<int> v;

    // Push many elements and verify capacity grows
    for (int i = 0; i < 100; ++i) {
        v.push_back(i);
        REQUIRE(v.capacity() >= v.size());
    }

    REQUIRE(v.size() == 100);
    REQUIRE(v[99] == 99);
}
