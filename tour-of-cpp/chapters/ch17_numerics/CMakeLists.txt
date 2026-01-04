# Chapter 17: Numerics
#
# This chapter covers C++ numerical operations:
# - Mathematical functions (<cmath>)
# - Complex numbers (<complex>)
# - Random number generation (<random>)
# - Numeric limits and properties (<limits>)

# ============================================================================
# Examples
# ============================================================================

# Mathematical functions from <cmath>
tour_add_example(ch17_math_functions examples/math_functions.cpp)

# Complex number arithmetic
tour_add_example(ch17_complex_numbers examples/complex_numbers.cpp)

# Random number generation
tour_add_example(ch17_random examples/random.cpp)

# Numeric limits and type properties
tour_add_example(ch17_numeric_limits examples/numeric_limits.cpp)

# Create a custom target to build all ch17 examples
add_custom_target(ch17_examples
    DEPENDS
        ch17_math_functions
        ch17_complex_numbers
        ch17_random
        ch17_numeric_limits
)

# ============================================================================
# Exercises
# ============================================================================

tour_add_exercise(ch17_ex01_random exercises/ex01_random.cpp)
tour_add_exercise(ch17_ex02_math exercises/ex02_math.cpp)

# Create a custom target to build all ch17 exercises
add_custom_target(ch17_exercises
    DEPENDS
        ch17_ex01_random
        ch17_ex02_math
)

# ============================================================================
# Tests
# ============================================================================

if(TOUR_BUILD_TESTS)
    add_executable(ch17_tests tests/test_ch17.cpp)
    target_link_libraries(ch17_tests PRIVATE Catch2::Catch2WithMain tour_warnings)

    include(Catch)
    catch_discover_tests(ch17_tests
        TEST_PREFIX "ch17::"
    )
endif()
