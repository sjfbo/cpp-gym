// Iterators - Categories, Usage, and Patterns
// Book reference: 13.2 Use of Iterators, 13.3 Iterator Types
//
// Iterators are the glue between containers and algorithms.
// They provide a uniform interface for traversing elements
// regardless of the underlying container implementation.

#include <algorithm>
#include <array>
#include <deque>
#include <forward_list>
#include <iostream>
#include <iterator>
#include <list>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <vector>

int main() {
    std::cout << "=== Iterator Categories and Usage ===\n\n";

    // -------------------------------------------------------------------------
    // Basic iterator usage
    // -------------------------------------------------------------------------
    std::cout << "--- Basic Iterator Usage ---\n";

    std::vector<int> v{10, 20, 30, 40, 50};

    // begin() and end()
    std::cout << "Forward iteration: ";
    for (auto it = v.begin(); it != v.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << "\n";

    // rbegin() and rend() - reverse iterators
    std::cout << "Reverse iteration: ";
    for (auto it = v.rbegin(); it != v.rend(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << "\n";

    // cbegin() and cend() - const iterators
    std::cout << "Const iteration: ";
    for (auto it = v.cbegin(); it != v.cend(); ++it) {
        // *it = 100;  // Would not compile - const iterator
        std::cout << *it << " ";
    }
    std::cout << "\n";

    // -------------------------------------------------------------------------
    // Random Access Iterators (vector, array, deque, string)
    // -------------------------------------------------------------------------
    std::cout << "\n--- Random Access Iterators ---\n";

    std::vector<int> ra{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    auto it = ra.begin();

    // All operations supported
    std::cout << "*it = " << *it << "\n";         // Dereference
    std::cout << "it[3] = " << it[3] << "\n";     // Subscript

    it += 5;
    std::cout << "After it += 5: *it = " << *it << "\n";

    it -= 2;
    std::cout << "After it -= 2: *it = " << *it << "\n";

    auto it2 = ra.begin() + 7;
    std::cout << "it2 - it = " << (it2 - it) << "\n";  // Distance

    std::cout << "it < it2: " << (it < it2 ? "true" : "false") << "\n";

    // std::distance works with all iterator types
    std::cout << "std::distance(begin, end) = "
              << std::distance(ra.begin(), ra.end()) << "\n";

    // -------------------------------------------------------------------------
    // Bidirectional Iterators (list, set, map)
    // -------------------------------------------------------------------------
    std::cout << "\n--- Bidirectional Iterators ---\n";

    std::list<int> lst{10, 20, 30, 40, 50};
    auto lit = lst.begin();

    std::cout << "*lit = " << *lit << "\n";

    ++lit; ++lit;  // Forward
    std::cout << "After ++lit twice: *lit = " << *lit << "\n";

    --lit;  // Backward
    std::cout << "After --lit: *lit = " << *lit << "\n";

    // Note: lit += 2 would NOT compile - not random access
    // Use std::advance instead
    std::advance(lit, 2);
    std::cout << "After std::advance(lit, 2): *lit = " << *lit << "\n";

    // std::prev and std::next
    auto prev_it = std::prev(lit);
    auto next_it = std::next(lst.begin(), 2);
    std::cout << "std::prev(lit) = " << *prev_it << "\n";
    std::cout << "std::next(begin, 2) = " << *next_it << "\n";

    // -------------------------------------------------------------------------
    // Forward Iterators (forward_list, unordered containers)
    // -------------------------------------------------------------------------
    std::cout << "\n--- Forward Iterators ---\n";

    std::forward_list<int> fwd{100, 200, 300};
    auto fit = fwd.begin();

    std::cout << "*fit = " << *fit << "\n";
    ++fit;
    std::cout << "After ++fit: *fit = " << *fit << "\n";

    // --fit would NOT compile - forward only
    // fit += 2 would NOT compile - not random access
    std::cout << "Forward iterators can only move forward\n";

    // -------------------------------------------------------------------------
    // Input/Output Iterators (streams)
    // -------------------------------------------------------------------------
    std::cout << "\n--- Input/Output Iterators ---\n";

    // Input iterator from stream
    std::istringstream iss("10 20 30 40 50");
    std::vector<int> from_stream;
    std::copy(std::istream_iterator<int>(iss),
              std::istream_iterator<int>(),
              std::back_inserter(from_stream));
    std::cout << "Read from stream: ";
    for (int x : from_stream) std::cout << x << " ";
    std::cout << "\n";

    // Output iterator to stream
    std::cout << "Writing to stream: ";
    std::copy(from_stream.begin(), from_stream.end(),
              std::ostream_iterator<int>(std::cout, " "));
    std::cout << "\n";

    // -------------------------------------------------------------------------
    // Insert Iterators
    // -------------------------------------------------------------------------
    std::cout << "\n--- Insert Iterators ---\n";

    std::vector<int> source{1, 2, 3};
    std::vector<int> dest;

    // back_inserter - calls push_back
    std::copy(source.begin(), source.end(), std::back_inserter(dest));
    std::cout << "After back_inserter: ";
    for (int x : dest) std::cout << x << " ";
    std::cout << "\n";

    // front_inserter - calls push_front (needs deque or list)
    std::list<int> front_dest;
    std::copy(source.begin(), source.end(), std::front_inserter(front_dest));
    std::cout << "After front_inserter: ";
    for (int x : front_dest) std::cout << x << " ";
    std::cout << "\n";

    // inserter - inserts at specified position
    std::vector<int> insert_dest{100, 200};
    std::copy(source.begin(), source.end(),
              std::inserter(insert_dest, insert_dest.begin() + 1));
    std::cout << "After inserter at position 1: ";
    for (int x : insert_dest) std::cout << x << " ";
    std::cout << "\n";

    // -------------------------------------------------------------------------
    // Iterator traits
    // -------------------------------------------------------------------------
    std::cout << "\n--- Iterator Traits ---\n";

    // Check iterator category at compile time
    using VecIterator = std::vector<int>::iterator;
    using ListIterator = std::list<int>::iterator;
    using FwdIterator = std::forward_list<int>::iterator;

    std::cout << "vector iterator is random access: "
              << std::is_same_v<
                    std::iterator_traits<VecIterator>::iterator_category,
                    std::random_access_iterator_tag> << "\n";

    std::cout << "list iterator is bidirectional: "
              << std::is_same_v<
                    std::iterator_traits<ListIterator>::iterator_category,
                    std::bidirectional_iterator_tag> << "\n";

    std::cout << "forward_list iterator is forward: "
              << std::is_same_v<
                    std::iterator_traits<FwdIterator>::iterator_category,
                    std::forward_iterator_tag> << "\n";

    // -------------------------------------------------------------------------
    // Common iterator patterns
    // -------------------------------------------------------------------------
    std::cout << "\n--- Common Patterns ---\n";

    std::vector<int> nums{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    // Find and use position
    auto found = std::find(nums.begin(), nums.end(), 5);
    if (found != nums.end()) {
        std::cout << "Found 5 at index: " << std::distance(nums.begin(), found) << "\n";
        std::cout << "Next element: " << *std::next(found) << "\n";
    }

    // Iterate over subrange
    std::cout << "Elements from index 3 to 7: ";
    for (auto iter = nums.begin() + 3; iter != nums.begin() + 7; ++iter) {
        std::cout << *iter << " ";
    }
    std::cout << "\n";

    // Iterate pairs/windows
    std::cout << "Adjacent pairs: ";
    for (auto iter = nums.begin(); iter != nums.end() && std::next(iter) != nums.end(); ++iter) {
        std::cout << "(" << *iter << "," << *std::next(iter) << ") ";
    }
    std::cout << "\n";

    // -------------------------------------------------------------------------
    // Map/set iterators (dereference gives pair/value)
    // -------------------------------------------------------------------------
    std::cout << "\n--- Map/Set Iterators ---\n";

    std::map<std::string, int> scores{{"Alice", 95}, {"Bob", 87}, {"Charlie", 92}};

    std::cout << "Map iteration:\n";
    for (auto map_it = scores.begin(); map_it != scores.end(); ++map_it) {
        // map_it->first and map_it->second
        std::cout << "  " << map_it->first << ": " << map_it->second << "\n";
    }

    // Modern: structured bindings (preferred)
    std::cout << "With structured bindings:\n";
    for (const auto& [name, score] : scores) {
        std::cout << "  " << name << ": " << score << "\n";
    }

    std::set<int> unique_nums{5, 2, 8, 1, 9};
    std::cout << "Set iteration (sorted): ";
    for (auto set_it = unique_nums.begin(); set_it != unique_nums.end(); ++set_it) {
        std::cout << *set_it << " ";
    }
    std::cout << "\n";

    return 0;
}

// Key takeaways:
// - Iterators abstract traversal, enabling generic algorithms
// - Different categories: Input, Output, Forward, Bidirectional, Random Access
// - Use std::advance, std::next, std::prev for portable iterator movement
// - std::distance computes elements between iterators
// - Insert iterators (back_inserter, etc.) adapt containers for output
// - Check iterator validity - many operations invalidate iterators
