# Chapter 12: Containers
#
# This chapter covers the C++ Standard Library containers:
# - Sequential containers (vector, list, deque)
# - Associative containers (map, set, multimap, multiset)
# - Unordered containers (unordered_map, unordered_set)
# - Container operations and iterator patterns

# ============================================================================
# Examples
# ============================================================================

# std::vector - The workhorse dynamic array
tour_add_example(ch12_vector examples/vector.cpp)

# std::list - Doubly-linked list
tour_add_example(ch12_list examples/list.cpp)

# std::map and std::multimap - Ordered associative containers
tour_add_example(ch12_map examples/map.cpp)

# std::unordered_map - Hash-based containers
tour_add_example(ch12_unordered_map examples/unordered_map.cpp)

# Create a custom target to build all ch12 examples
add_custom_target(ch12_examples
    DEPENDS
        ch12_vector
        ch12_list
        ch12_map
        ch12_unordered_map
)

# ============================================================================
# Exercises
# ============================================================================

tour_add_exercise(ch12_ex01_containers exercises/ex01_containers.cpp)
tour_add_exercise(ch12_ex02_map exercises/ex02_map.cpp)

# Create a custom target to build all ch12 exercises
add_custom_target(ch12_exercises
    DEPENDS
        ch12_ex01_containers
        ch12_ex02_map
)

# ============================================================================
# Tests
# ============================================================================

if(TOUR_BUILD_TESTS)
    add_executable(ch12_tests tests/test_ch12.cpp)
    target_link_libraries(ch12_tests PRIVATE Catch2::Catch2WithMain tour_warnings)

    include(Catch)
    catch_discover_tests(ch12_tests
        TEST_PREFIX "ch12::"
    )
endif()
