# Chapter 14: Ranges
#
# This chapter covers C++20 ranges library:
# - Range concepts and views
# - Range adaptors and pipelines
# - Lazy evaluation with views
# - Generator patterns

# ============================================================================
# Examples
# ============================================================================

# Views - lazy, non-owning range adaptors
tour_add_example(ch14_views examples/views.cpp)

# Generators - coroutine-based lazy sequences
tour_add_example(ch14_generators examples/generators.cpp)

# Pipelines - composing views with the pipe operator
tour_add_example(ch14_pipelines examples/pipelines.cpp)

# Create a custom target to build all ch14 examples
add_custom_target(ch14_examples
    DEPENDS
        ch14_views
        ch14_generators
        ch14_pipelines
)

# ============================================================================
# Exercises
# ============================================================================

tour_add_exercise(ch14_ex01_ranges exercises/ex01_ranges.cpp)

# Create a custom target to build all ch14 exercises
add_custom_target(ch14_exercises
    DEPENDS
        ch14_ex01_ranges
)

# ============================================================================
# Tests
# ============================================================================

if(TOUR_BUILD_TESTS)
    add_executable(ch14_tests tests/test_ch14.cpp)
    target_link_libraries(ch14_tests PRIVATE Catch2::Catch2WithMain tour_warnings)

    include(Catch)
    catch_discover_tests(ch14_tests
        TEST_PREFIX "ch14::"
    )
endif()
