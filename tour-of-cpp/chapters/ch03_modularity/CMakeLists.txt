# Chapter 3: Modularity
#
# This chapter covers:
# - Separate compilation (headers and implementation files)
# - Namespaces
# - Function arguments and return values
# - Modules (C++20 introduction)

# ============================================================================
# Examples
# ============================================================================

# Separate compilation example (multi-file)
add_executable(ch03_separate_compilation
    examples/separate_compilation/main.cpp
    examples/separate_compilation/vector.cpp
)
target_link_libraries(ch03_separate_compilation PRIVATE tour_warnings)

tour_add_example(ch03_namespaces examples/namespaces.cpp)
tour_add_example(ch03_function_args examples/function_args.cpp)

add_custom_target(ch03_examples
    DEPENDS
        ch03_separate_compilation
        ch03_namespaces
        ch03_function_args
)

# ============================================================================
# Exercises
# ============================================================================

tour_add_exercise(ch03_ex01_header exercises/ex01_header.cpp)
tour_add_exercise(ch03_ex02_namespace exercises/ex02_namespace.cpp)

add_custom_target(ch03_exercises
    DEPENDS
        ch03_ex01_header
        ch03_ex02_namespace
)

# ============================================================================
# Tests
# ============================================================================

if(TOUR_BUILD_TESTS)
    add_executable(ch03_tests tests/test_ch03.cpp)
    target_link_libraries(ch03_tests PRIVATE Catch2::Catch2WithMain tour_warnings)

    include(Catch)
    catch_discover_tests(ch03_tests
        TEST_PREFIX "ch03::"
    )
endif()
