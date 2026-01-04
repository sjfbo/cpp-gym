// std::list - Doubly-Linked List
// Book reference: 12.3 list
//
// std::list provides O(1) insertion and removal at any position
// if you have an iterator. Iterators remain valid through insertions
// and removals (except for the removed element itself).

#include <algorithm>
#include <iostream>
#include <list>
#include <vector>

// Helper to print list contents
template <typename T>
void print_list(const std::string& label, const std::list<T>& lst) {
    std::cout << label << ": [";
    bool first = true;
    for (const auto& elem : lst) {
        if (!first) std::cout << ", ";
        std::cout << elem;
        first = false;
    }
    std::cout << "] (size=" << lst.size() << ")\n";
}

int main() {
    std::cout << "=== std::list Operations ===\n\n";

    // -------------------------------------------------------------------------
    // Construction
    // -------------------------------------------------------------------------
    std::cout << "--- Construction ---\n";

    std::list<int> l1;                    // Empty list
    std::list<int> l2(5);                 // 5 zero-initialized elements
    std::list<int> l3(5, 42);             // 5 elements, all 42
    std::list<int> l4{1, 2, 3, 4, 5};     // Initializer list

    print_list("l1 (empty)", l1);
    print_list("l2 (5 zeros)", l2);
    print_list("l3 (5 x 42)", l3);
    print_list("l4 (init list)", l4);

    // -------------------------------------------------------------------------
    // Insertion (O(1) at any position with iterator)
    // -------------------------------------------------------------------------
    std::cout << "\n--- Insertion ---\n";

    std::list<int> lst;
    print_list("Initial", lst);

    lst.push_back(3);       // Add to back
    lst.push_front(1);      // Add to front
    print_list("After push_front(1), push_back(3)", lst);

    lst.emplace_back(4);    // Construct in-place at back
    lst.emplace_front(0);   // Construct in-place at front
    print_list("After emplace_front(0), emplace_back(4)", lst);

    // Insert in the middle
    auto it = std::find(lst.begin(), lst.end(), 3);
    lst.insert(it, 2);  // Insert 2 before 3
    print_list("After insert 2 before 3", lst);

    // Insert multiple elements
    it = std::find(lst.begin(), lst.end(), 4);
    lst.insert(it, {3, 3, 3});  // Insert before 4
    print_list("After insert {3,3,3} before 4", lst);

    // -------------------------------------------------------------------------
    // Removal
    // -------------------------------------------------------------------------
    std::cout << "\n--- Removal ---\n";

    std::list<int> rem_demo{1, 2, 3, 2, 4, 2, 5};
    print_list("Initial", rem_demo);

    rem_demo.pop_front();
    print_list("After pop_front()", rem_demo);

    rem_demo.pop_back();
    print_list("After pop_back()", rem_demo);

    // remove() - removes all matching elements (list-specific!)
    rem_demo.remove(2);
    print_list("After remove(2)", rem_demo);

    // remove_if() - remove elements matching predicate
    std::list<int> remove_if_demo{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    remove_if_demo.remove_if([](int x) { return x % 2 == 0; });
    print_list("After remove_if (even)", remove_if_demo);

    // -------------------------------------------------------------------------
    // Splice - transfer elements between lists (O(1)!)
    // -------------------------------------------------------------------------
    std::cout << "\n--- Splice (O(1) Transfer) ---\n";

    std::list<int> src{100, 200, 300};
    std::list<int> dst{1, 2, 3, 4, 5};

    print_list("Source before splice", src);
    print_list("Dest before splice", dst);

    // Splice entire source list into destination
    auto pos = dst.begin();
    std::advance(pos, 2);  // Position at index 2 (before '3')
    dst.splice(pos, src);  // Move all from src to dst

    print_list("Source after splice", src);
    print_list("Dest after splice", dst);

    // Splice single element
    std::list<int> src2{999};
    dst.splice(dst.begin(), src2);
    print_list("After splicing 999 to front", dst);

    // -------------------------------------------------------------------------
    // Merge - merge sorted lists (O(n))
    // -------------------------------------------------------------------------
    std::cout << "\n--- Merge Sorted Lists ---\n";

    std::list<int> sorted1{1, 3, 5, 7, 9};
    std::list<int> sorted2{2, 4, 6, 8, 10};

    print_list("Sorted list 1", sorted1);
    print_list("Sorted list 2", sorted2);

    sorted1.merge(sorted2);  // Moves elements from sorted2

    print_list("After merge into list 1", sorted1);
    print_list("List 2 (now empty)", sorted2);

    // -------------------------------------------------------------------------
    // Sort and unique (list has its own sort!)
    // -------------------------------------------------------------------------
    std::cout << "\n--- Sort and Unique ---\n";

    std::list<int> unsorted{5, 2, 2, 8, 1, 1, 1, 9, 3};
    print_list("Unsorted", unsorted);

    // std::sort won't work (needs random access iterators)
    // Use list::sort() instead
    unsorted.sort();
    print_list("After sort()", unsorted);

    unsorted.unique();  // Remove consecutive duplicates
    print_list("After unique()", unsorted);

    // Sort with custom comparator
    std::list<int> desc_demo{5, 2, 8, 1, 9, 3};
    desc_demo.sort(std::greater<int>{});
    print_list("Sorted descending", desc_demo);

    // -------------------------------------------------------------------------
    // Reverse
    // -------------------------------------------------------------------------
    std::cout << "\n--- Reverse ---\n";

    std::list<int> rev_demo{1, 2, 3, 4, 5};
    print_list("Before reverse", rev_demo);

    rev_demo.reverse();
    print_list("After reverse", rev_demo);

    // -------------------------------------------------------------------------
    // Iterator stability
    // -------------------------------------------------------------------------
    std::cout << "\n--- Iterator Stability ---\n";

    std::list<int> stable{1, 2, 3, 4, 5};
    auto iter = std::find(stable.begin(), stable.end(), 3);
    std::cout << "Iterator points to: " << *iter << "\n";

    // Insert and remove elements - iterator stays valid!
    stable.push_front(0);
    stable.push_back(6);
    stable.remove(2);
    stable.remove(4);

    std::cout << "After insertions and removals...\n";
    print_list("List now", stable);
    std::cout << "Original iterator still points to: " << *iter << "\n";

    // -------------------------------------------------------------------------
    // When to use list vs vector
    // -------------------------------------------------------------------------
    std::cout << "\n--- Use Cases ---\n";
    std::cout << "Use std::list when:\n";
    std::cout << "  - Frequent insertion/removal in the middle\n";
    std::cout << "  - Iterator stability is critical\n";
    std::cout << "  - No random access needed\n";
    std::cout << "  - splice() for O(1) transfer between lists\n\n";

    std::cout << "Use std::vector when:\n";
    std::cout << "  - Random access is needed (v[i])\n";
    std::cout << "  - Cache locality matters (almost always)\n";
    std::cout << "  - Most operations are at the end\n";
    std::cout << "  - Memory overhead should be minimal\n";

    return 0;
}

// Key takeaways:
// - std::list provides O(1) insertion/removal with iterators
// - Iterators remain valid through modifications (except removed elements)
// - Use list::sort() not std::sort (no random access iterators)
// - splice() transfers elements in O(1) time
// - merge() combines sorted lists efficiently
// - Consider cache effects: vector is often faster despite O(n) insert
