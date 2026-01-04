# Chapter 16: Utilities
#
# This chapter covers utility components:
# - Time handling with <chrono>
# - Function adaption with std::function, std::bind
# - Type traits and type functions
# - Move semantics and perfect forwarding

# ============================================================================
# Examples
# ============================================================================

# chrono - time points, durations, clocks
tour_add_example(ch16_chrono examples/chrono.cpp)

# function adaption - std::function, std::bind, lambdas
tour_add_example(ch16_function_adaption examples/function_adaption.cpp)

# type functions - type_traits, conditional, enable_if
tour_add_example(ch16_type_functions examples/type_functions.cpp)

# move and forward - perfect forwarding, move semantics
tour_add_example(ch16_move_forward examples/move_forward.cpp)

# Create a custom target to build all ch16 examples
add_custom_target(ch16_examples
    DEPENDS
        ch16_chrono
        ch16_function_adaption
        ch16_type_functions
        ch16_move_forward
)

# ============================================================================
# Exercises
# ============================================================================

tour_add_exercise(ch16_ex01_chrono exercises/ex01_chrono.cpp)
tour_add_exercise(ch16_ex02_type_traits exercises/ex02_type_traits.cpp)

# Create a custom target to build all ch16 exercises
add_custom_target(ch16_exercises
    DEPENDS
        ch16_ex01_chrono
        ch16_ex02_type_traits
)

# ============================================================================
# Tests
# ============================================================================

if(TOUR_BUILD_TESTS)
    add_executable(ch16_tests tests/test_ch16.cpp)
    target_link_libraries(ch16_tests PRIVATE Catch2::Catch2WithMain tour_warnings)

    include(Catch)
    catch_discover_tests(ch16_tests
        TEST_PREFIX "ch16::"
    )
endif()
