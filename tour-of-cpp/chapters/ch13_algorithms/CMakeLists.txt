# Chapter 13: Algorithms
#
# This chapter covers the C++ Standard Library algorithms:
# - Iterator categories and concepts
# - Predicates and function objects
# - Sorting, searching, and transforming
# - Parallel execution policies (C++17/20)

# ============================================================================
# Examples
# ============================================================================

# Iterator categories and usage
tour_add_example(ch13_iterators examples/iterators.cpp)

# Lambdas as predicates and function objects
tour_add_example(ch13_predicates examples/predicates.cpp)

# Algorithm overview - sort, find, transform, accumulate
tour_add_example(ch13_algorithm_overview examples/algorithm_overview.cpp)

# Parallel algorithms with execution policies
# Note: Apple libc++ does not support <execution> header, so skip on Apple platforms
if(NOT APPLE)
    tour_add_example(ch13_parallel_algorithms examples/parallel_algorithms.cpp)
endif()

# Create a custom target to build all ch13 examples
add_custom_target(ch13_examples
    DEPENDS
        ch13_iterators
        ch13_predicates
        ch13_algorithm_overview
        $<$<NOT:$<PLATFORM_ID:Darwin>>:ch13_parallel_algorithms>
)

# ============================================================================
# Exercises
# ============================================================================

tour_add_exercise(ch13_ex01_algorithms exercises/ex01_algorithms.cpp)
tour_add_exercise(ch13_ex02_custom_algorithm exercises/ex02_custom_algorithm.cpp)

# Create a custom target to build all ch13 exercises
add_custom_target(ch13_exercises
    DEPENDS
        ch13_ex01_algorithms
        ch13_ex02_custom_algorithm
)

# ============================================================================
# Tests
# ============================================================================

if(TOUR_BUILD_TESTS)
    add_executable(ch13_tests tests/test_ch13.cpp)
    target_link_libraries(ch13_tests PRIVATE Catch2::Catch2WithMain tour_warnings)

    include(Catch)
    catch_discover_tests(ch13_tests
        TEST_PREFIX "ch13::"
    )
endif()
