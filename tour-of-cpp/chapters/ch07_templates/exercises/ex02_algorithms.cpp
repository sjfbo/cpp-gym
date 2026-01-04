// Exercise 7.2: Implement Generic find and count Algorithms
// Book reference: 7.3 Function Templates
// Difficulty: **
//
// Instructions:
// Implement the following generic algorithms that work with any container
// supporting iterators:
//
// 1. my_find(begin, end, value)
//    - Find the first occurrence of value in the range [begin, end)
//    - Return iterator to the element if found, end if not found
//
// 2. my_count(begin, end, value)
//    - Count occurrences of value in the range [begin, end)
//    - Return the count as size_t
//
// 3. my_find_if(begin, end, predicate)
//    - Find the first element for which predicate returns true
//    - Return iterator to the element if found, end if not found
//
// 4. my_count_if(begin, end, predicate)
//    - Count elements for which predicate returns true
//    - Return the count as size_t
//
// Requirements:
// - Use iterator-based interface (not container-based)
// - Work with any forward iterator
// - Work with any comparable value type
// - Predicates should be callable with the element type
//
// Bonus challenges:
// - Implement my_all_of(begin, end, predicate) - true if all match
// - Implement my_any_of(begin, end, predicate) - true if any match
// - Implement my_none_of(begin, end, predicate) - true if none match
//
// Hints:
// - Use typename for dependent types (if needed)
// - Iterators support ++, !=, and * (dereference)
// - For predicates, just call them like functions: pred(*iter)

#include <array>
#include <iostream>
#include <list>
#include <string>
#include <vector>

// ============================================================================
// TODO: Implement your algorithms here
// ============================================================================

// TODO: Implement my_find - finds first occurrence of value
template<typename Iterator, typename T>
Iterator my_find(Iterator begin, Iterator end, const T& value) {
    // TODO: Iterate and compare with value
    (void)value;  // Remove when implemented
    return end;  // Stub: not found
}

// TODO: Implement my_count - counts occurrences of value
template<typename Iterator, typename T>
std::size_t my_count(Iterator begin, Iterator end, const T& value) {
    // TODO: Iterate and count matches
    (void)begin; (void)end; (void)value;  // Remove when implemented
    return 0;  // Stub: zero count
}

// TODO: Implement my_find_if - finds first element matching predicate
template<typename Iterator, typename Predicate>
Iterator my_find_if(Iterator begin, Iterator end, Predicate pred) {
    // TODO: Iterate and test predicate
    (void)pred;  // Remove when implemented
    return end;  // Stub: not found
}

// TODO: Implement my_count_if - counts elements matching predicate
template<typename Iterator, typename Predicate>
std::size_t my_count_if(Iterator begin, Iterator end, Predicate pred) {
    // TODO: Iterate and count predicate matches
    (void)begin; (void)end; (void)pred;  // Remove when implemented
    return 0;  // Stub: zero count
}

// BONUS: Implement my_all_of - true if all match predicate
template<typename Iterator, typename Predicate>
bool my_all_of(Iterator begin, Iterator end, Predicate pred) {
    (void)begin; (void)end; (void)pred;
    return false;  // Stub
}

// BONUS: Implement my_any_of - true if any match predicate
template<typename Iterator, typename Predicate>
bool my_any_of(Iterator begin, Iterator end, Predicate pred) {
    (void)begin; (void)end; (void)pred;
    return false;  // Stub
}

// BONUS: Implement my_none_of - true if none match predicate
template<typename Iterator, typename Predicate>
bool my_none_of(Iterator begin, Iterator end, Predicate pred) {
    (void)begin; (void)end; (void)pred;
    return false;  // Stub
}

// ============================================================================
// Test your implementation
// ============================================================================

void test_my_find() {
    std::cout << "=== Testing my_find ===\n";

    std::vector<int> vec = {1, 2, 3, 4, 5, 3, 6};

    // Find existing element
    auto it = my_find(vec.begin(), vec.end(), 3);
    if (it != vec.end()) {
        std::cout << "Found 3 at position " << (it - vec.begin()) << "\n";
    } else {
        std::cout << "ERROR: Should have found 3\n";
    }

    // Find non-existing element
    it = my_find(vec.begin(), vec.end(), 99);
    if (it == vec.end()) {
        std::cout << "Correctly did not find 99\n";
    } else {
        std::cout << "ERROR: Should not have found 99\n";
    }

    // Test with strings
    std::vector<std::string> words = {"apple", "banana", "cherry"};
    auto str_it = my_find(words.begin(), words.end(), std::string{"banana"});
    if (str_it != words.end()) {
        std::cout << "Found '" << *str_it << "' in words\n";
    }

    // Test with list (non-random access iterator)
    std::list<int> lst = {10, 20, 30, 40};
    auto lst_it = my_find(lst.begin(), lst.end(), 30);
    if (lst_it != lst.end()) {
        std::cout << "Found 30 in list\n";
    }
}

void test_my_count() {
    std::cout << "\n=== Testing my_count ===\n";

    std::vector<int> vec = {1, 2, 2, 3, 2, 4, 2, 5};

    size_t count = my_count(vec.begin(), vec.end(), 2);
    std::cout << "Count of 2: " << count << " (expected 4)\n";

    count = my_count(vec.begin(), vec.end(), 99);
    std::cout << "Count of 99: " << count << " (expected 0)\n";

    // Test with array
    std::array<char, 7> chars = {'a', 'b', 'a', 'c', 'a', 'b', 'a'};
    count = my_count(chars.begin(), chars.end(), 'a');
    std::cout << "Count of 'a': " << count << " (expected 4)\n";
}

void test_my_find_if() {
    std::cout << "\n=== Testing my_find_if ===\n";

    std::vector<int> vec = {1, 3, 5, 8, 9, 11};

    // Find first even number
    auto is_even = [](int x) { return x % 2 == 0; };
    auto it = my_find_if(vec.begin(), vec.end(), is_even);
    if (it != vec.end()) {
        std::cout << "First even: " << *it << " at position " << (it - vec.begin()) << "\n";
    }

    // Find first number > 10
    auto gt_10 = [](int x) { return x > 10; };
    it = my_find_if(vec.begin(), vec.end(), gt_10);
    if (it != vec.end()) {
        std::cout << "First > 10: " << *it << "\n";
    }

    // Test with strings
    std::vector<std::string> words = {"cat", "elephant", "dog", "hippopotamus"};
    auto long_word = [](const std::string& s) { return s.length() > 5; };
    auto str_it = my_find_if(words.begin(), words.end(), long_word);
    if (str_it != words.end()) {
        std::cout << "First word > 5 chars: " << *str_it << "\n";
    }
}

void test_my_count_if() {
    std::cout << "\n=== Testing my_count_if ===\n";

    std::vector<int> vec = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    // Count even numbers
    auto is_even = [](int x) { return x % 2 == 0; };
    size_t count = my_count_if(vec.begin(), vec.end(), is_even);
    std::cout << "Count of evens: " << count << " (expected 5)\n";

    // Count numbers > 5
    auto gt_5 = [](int x) { return x > 5; };
    count = my_count_if(vec.begin(), vec.end(), gt_5);
    std::cout << "Count of > 5: " << count << " (expected 5)\n";

    // Count with strings
    std::vector<std::string> words = {"a", "abc", "ab", "abcd", "abcde"};
    auto short_word = [](const std::string& s) { return s.length() <= 2; };
    count = my_count_if(words.begin(), words.end(), short_word);
    std::cout << "Count of short words (<=2 chars): " << count << " (expected 2)\n";
}

void test_bonus_algorithms() {
    std::cout << "\n=== Testing Bonus Algorithms ===\n";

    std::vector<int> all_positive = {1, 2, 3, 4, 5};
    std::vector<int> some_negative = {1, -2, 3, -4, 5};
    std::vector<int> all_negative = {-1, -2, -3, -4, -5};

    auto is_positive = [](int x) { return x > 0; };

    // Uncomment these tests when you implement the bonus functions

    // std::cout << "all_positive - all_of positive: "
    //           << std::boolalpha << my_all_of(all_positive.begin(), all_positive.end(), is_positive) << "\n";
    // std::cout << "some_negative - all_of positive: "
    //           << my_all_of(some_negative.begin(), some_negative.end(), is_positive) << "\n";

    // std::cout << "some_negative - any_of positive: "
    //           << my_any_of(some_negative.begin(), some_negative.end(), is_positive) << "\n";
    // std::cout << "all_negative - any_of positive: "
    //           << my_any_of(all_negative.begin(), all_negative.end(), is_positive) << "\n";

    // std::cout << "all_negative - none_of positive: "
    //           << my_none_of(all_negative.begin(), all_negative.end(), is_positive) << "\n";
    // std::cout << "some_negative - none_of positive: "
    //           << my_none_of(some_negative.begin(), some_negative.end(), is_positive) << "\n";

    std::cout << "(Bonus algorithms not yet implemented - uncomment tests when ready)\n";
}

void test_edge_cases() {
    std::cout << "\n=== Testing Edge Cases ===\n";

    // Empty container
    std::vector<int> empty;
    auto it = my_find(empty.begin(), empty.end(), 1);
    if (it == empty.end()) {
        std::cout << "Empty container: find correctly returns end\n";
    }

    size_t count = my_count(empty.begin(), empty.end(), 1);
    if (count == 0) {
        std::cout << "Empty container: count correctly returns 0\n";
    }

    // Single element
    std::vector<int> single = {42};
    it = my_find(single.begin(), single.end(), 42);
    if (it != single.end() && *it == 42) {
        std::cout << "Single element: correctly found 42\n";
    }

    it = my_find(single.begin(), single.end(), 99);
    if (it == single.end()) {
        std::cout << "Single element: correctly did not find 99\n";
    }
}

int main() {
    test_my_find();
    test_my_count();
    test_my_find_if();
    test_my_count_if();
    test_bonus_algorithms();
    test_edge_cases();

    std::cout << "\n=== All Tests Completed ===\n";
    std::cout << "If you see this without errors, your algorithms are working!\n";

    return 0;
}
