# Chapter 1: The Basics
#
# This chapter covers fundamental C++ concepts:
# - Programs and main()
# - Functions
# - Types, variables, and arithmetic
# - Scope and lifetime
# - Constants (const, constexpr)
# - Pointers, arrays, and references
# - Tests (if/else, switch)
# - Mapping to hardware

# ============================================================================
# Examples
# ============================================================================

# Hello World - The classic first program
tour_add_example(ch01_hello_world examples/hello_world.cpp)

# Functions - Function basics, overloading, default arguments
tour_add_example(ch01_functions examples/functions.cpp)

# Types and Variables - Built-in types, initialization
tour_add_example(ch01_types_variables examples/types_variables.cpp)

# Pointers, Arrays, and References
tour_add_example(ch01_pointers_arrays_refs examples/pointers_arrays_refs.cpp)

# Control Flow - if, switch, loops
tour_add_example(ch01_control_flow examples/control_flow.cpp)

# Constants - const and constexpr
tour_add_example(ch01_constants examples/constants.cpp)

# Create a custom target to build all ch01 examples
add_custom_target(ch01_examples
    DEPENDS
        ch01_hello_world
        ch01_functions
        ch01_types_variables
        ch01_pointers_arrays_refs
        ch01_control_flow
        ch01_constants
)

# ============================================================================
# Exercises
# ============================================================================

tour_add_exercise(ch01_ex01_hello exercises/ex01_hello.cpp)
tour_add_exercise(ch01_ex02_functions exercises/ex02_functions.cpp)
tour_add_exercise(ch01_ex03_types exercises/ex03_types.cpp)
tour_add_exercise(ch01_ex04_pointers_refs exercises/ex04_pointers_refs.cpp)
tour_add_exercise(ch01_ex05_scope exercises/ex05_scope.cpp)

# Create a custom target to build all ch01 exercises
add_custom_target(ch01_exercises
    DEPENDS
        ch01_ex01_hello
        ch01_ex02_functions
        ch01_ex03_types
        ch01_ex04_pointers_refs
        ch01_ex05_scope
)

# ============================================================================
# Tests
# ============================================================================

if(TOUR_BUILD_TESTS)
    add_executable(ch01_tests tests/test_ch01.cpp)
    target_link_libraries(ch01_tests PRIVATE Catch2::Catch2WithMain tour_warnings)

    include(Catch)
    catch_discover_tests(ch01_tests
        TEST_PREFIX "ch01::"
    )
endif()
