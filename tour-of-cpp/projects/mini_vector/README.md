# Mini Vector

A simplified implementation of `std::vector` to learn about resource management, templates, and iterator concepts in modern C++.

## Learning Objectives

After completing this project, you will understand:

1. **RAII (Resource Acquisition Is Initialization)**
   - Managing dynamic memory with constructors and destructors
   - Ensuring no memory leaks through proper cleanup

2. **Rule of Five**
   - Copy constructor and copy assignment operator
   - Move constructor and move assignment operator
   - Destructor

3. **Class Templates**
   - Writing generic containers using templates
   - Template type deduction and instantiation

4. **Operator Overloading**
   - Subscript operator `operator[]`
   - Copy and move assignment operators

5. **Exception Safety**
   - Bounds checking with `at()`
   - Strong exception guarantee concepts

6. **Iterator Support**
   - Providing `begin()` and `end()` for range-based for loops
   - Understanding iterator concepts

## Project Structure

```
mini_vector/
├── CMakeLists.txt          # Build configuration
├── README.md               # This file
├── mini_vector.h           # The Vector<T> implementation
├── main.cpp                # Demo program
└── tests/
    └── test_mini_vector.cpp # Catch2 unit tests
```

## Implementation Steps

### Step 1: Basic Structure
Start with the basic class template structure:
- Private members: `T* data_`, `size_t size_`, `size_t capacity_`
- Default constructor initializing to nullptr/0

### Step 2: Core Operations
Implement basic functionality:
- `push_back(const T&)` and `push_back(T&&)`
- `pop_back()`
- `size()`, `capacity()`, `empty()`
- Private `grow()` method for reallocation

### Step 3: Element Access
Add ways to access elements:
- `operator[](size_t)` - unchecked access
- `at(size_t)` - bounds-checked access throwing `std::out_of_range`
- `front()` and `back()`

### Step 4: Rule of Five
Implement proper resource management:
- Destructor: `delete[] data_`
- Copy constructor: deep copy
- Copy assignment: copy-and-swap idiom
- Move constructor: transfer ownership
- Move assignment: transfer ownership

### Step 5: Iterator Support
Add iterator support for range-based for:
- `begin()` returning pointer to first element
- `end()` returning pointer past last element
- Const overloads for both

## Building

```bash
mkdir build && cd build
cmake ..
cmake --build .
```

## Running

```bash
# Run the demo
./mini_vector_demo

# Run tests
ctest --output-on-failure
```

## Key Concepts from "A Tour of C++"

This project applies concepts from:
- **Chapter 5**: Essential Operations (copy, move, RAII)
- **Chapter 6**: Templates (class templates, concepts)
- **Chapter 13**: Containers (vector interface design)

## Extension Ideas

Once you've completed the basic implementation, consider adding:
- `emplace_back()` with perfect forwarding
- `reserve()` and `shrink_to_fit()`
- `insert()` and `erase()`
- Custom allocator support
- `constexpr` support for compile-time usage
