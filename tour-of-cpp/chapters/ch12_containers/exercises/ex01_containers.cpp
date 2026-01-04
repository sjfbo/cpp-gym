// Exercise 12.1: Choosing the Right Container
// Book reference: 12.8 Container Overview
// Difficulty: **
//
// Instructions:
// This exercise tests your ability to choose the appropriate container
// for different scenarios. Implement each function using the most
// suitable container for the task.
//
// For each TODO, choose between: vector, list, map, unordered_map, set, etc.
// Consider:
// - What operations are performed most frequently?
// - Is ordering important?
// - Is lookup speed critical?
// - Are there duplicates?

#include <iostream>
#include <string>
#include <vector>

// TODO 1: Word Frequency Counter
// Given a vector of words, return a container that maps each unique word
// to the number of times it appears.
// Consider: Do you need ordering? What about lookup speed?
//
// Example: {"apple", "banana", "apple", "cherry", "banana", "apple"}
// Should produce: {"apple": 3, "banana": 2, "cherry": 1}

// Return type: Choose the appropriate container type
// Hint: You'll need to include the appropriate header
auto count_words(const std::vector<std::string>& words) {
    // TODO: Implement using the most appropriate container
    // Delete the line below and add your implementation
    (void)words;
    return 0;  // Replace with actual return
}


// TODO 2: Recent Items Cache (LRU-like behavior)
// Implement a container that maintains insertion order and allows
// efficient removal from the front and insertion at the back.
// Maximum size should be limited to 'max_size'.
//
// Operations needed:
// - Add item to back
// - Remove item from front when over capacity
// - Iterate in insertion order
//
// Which container is best for this?

// Return type: Choose the appropriate container type
auto create_recent_cache(const std::vector<std::string>& items, std::size_t max_size) {
    // TODO: Implement using the most appropriate container
    // Add each item, remove from front if over max_size
    // Delete the lines below and add your implementation
    (void)items;
    (void)max_size;
    return 0;  // Replace with actual return
}


// TODO 3: Sorted Unique Values
// Given a vector of integers that may contain duplicates,
// return a container with unique values in sorted order.
//
// Example: {5, 2, 8, 2, 9, 1, 5, 3}
// Should produce: {1, 2, 3, 5, 8, 9}
//
// Which container automatically handles both requirements?

// Return type: Choose the appropriate container type
auto get_sorted_unique(const std::vector<int>& values) {
    // TODO: Implement using the most appropriate container
    // Delete the lines below and add your implementation
    (void)values;
    return 0;  // Replace with actual return
}


// TODO 4: Phone Book with Multiple Numbers per Person
// Implement a phone book where each person can have multiple phone numbers.
// Should support:
// - Adding a phone number for a name
// - Getting all phone numbers for a name
// - Names should be in alphabetical order
//
// Which container allows duplicate keys and maintains order?

// Return type: Choose the appropriate container type
// You may need to create a helper struct or use an existing container

class PhoneBook {
public:
    // TODO: Add a private member variable of the appropriate container type

    void add_number(const std::string& name, const std::string& number) {
        // TODO: Implement
        (void)name;
        (void)number;
    }

    // Returns all numbers for a given name
    std::vector<std::string> get_numbers(const std::string& name) const {
        // TODO: Implement
        (void)name;
        return {};
    }

    // Print all entries in alphabetical order by name
    void print_all() const {
        // TODO: Implement
    }
};


// TODO 5: Fast Membership Testing
// Given a large list of valid usernames, implement a function that
// efficiently checks if a given username is valid.
//
// Requirements:
// - Checking validity should be O(1) average case
// - Memory usage is acceptable
// - Order doesn't matter
//
// Which container provides O(1) lookup?

class UsernameValidator {
public:
    // Constructor takes list of valid usernames
    explicit UsernameValidator(const std::vector<std::string>& valid_users) {
        // TODO: Store usernames in the most appropriate container
        (void)valid_users;
    }

    bool is_valid(const std::string& username) const {
        // TODO: Implement O(1) average case lookup
        (void)username;
        return false;
    }

private:
    // TODO: Add the appropriate container member
};


// ============================================================================
// Test your implementations
// ============================================================================

int main() {
    std::cout << "=== Exercise 12.1: Choosing the Right Container ===\n\n";

    // Test 1: Word Frequency Counter
    std::cout << "--- Test 1: Word Frequency Counter ---\n";
    std::vector<std::string> words{"apple", "banana", "apple", "cherry", "banana", "apple"};
    // TODO: Uncomment and test when implemented
    // auto word_counts = count_words(words);
    // Expected: apple: 3, banana: 2, cherry: 1
    std::cout << "TODO: Implement count_words()\n\n";

    // Test 2: Recent Items Cache
    std::cout << "--- Test 2: Recent Items Cache ---\n";
    std::vector<std::string> items{"a", "b", "c", "d", "e", "f"};
    // TODO: Uncomment and test when implemented
    // auto cache = create_recent_cache(items, 3);
    // Expected: only the 3 most recent items ("d", "e", "f")
    std::cout << "TODO: Implement create_recent_cache()\n\n";

    // Test 3: Sorted Unique Values
    std::cout << "--- Test 3: Sorted Unique Values ---\n";
    std::vector<int> values{5, 2, 8, 2, 9, 1, 5, 3};
    // TODO: Uncomment and test when implemented
    // auto unique_sorted = get_sorted_unique(values);
    // Expected: 1, 2, 3, 5, 8, 9
    std::cout << "TODO: Implement get_sorted_unique()\n\n";

    // Test 4: Phone Book
    std::cout << "--- Test 4: Phone Book ---\n";
    PhoneBook pb;
    pb.add_number("Alice", "555-1234");
    pb.add_number("Bob", "555-5678");
    pb.add_number("Alice", "555-9999");  // Alice has two numbers
    // TODO: Uncomment when implemented
    // pb.print_all();
    // auto alice_nums = pb.get_numbers("Alice");
    std::cout << "TODO: Implement PhoneBook\n\n";

    // Test 5: Username Validator
    std::cout << "--- Test 5: Username Validator ---\n";
    std::vector<std::string> valid_users{"alice", "bob", "charlie", "david"};
    UsernameValidator validator(valid_users);
    // TODO: Uncomment when implemented
    // std::cout << "alice valid: " << (validator.is_valid("alice") ? "yes" : "no") << "\n";
    // std::cout << "eve valid: " << (validator.is_valid("eve") ? "yes" : "no") << "\n";
    std::cout << "TODO: Implement UsernameValidator\n\n";

    std::cout << "Complete all TODOs and uncomment the tests!\n";

    return 0;
}

// Hints:
// 1. For word counting: unordered_map<string, int> is fast, map<string, int> gives sorted output
// 2. For the cache: std::deque or std::list work well for front/back operations
// 3. For sorted unique: std::set handles both automatically
// 4. For phone book with duplicates: std::multimap preserves order and allows duplicate keys
// 5. For fast lookup: std::unordered_set provides O(1) average contains check
