# Chapter 6: Essential Operations
#
# This chapter covers:
# - Copy and move operations
# - Resource management (RAII)
# - Operator overloading
# - User-defined literals
# - Rule of 0/3/5

# ============================================================================
# Examples
# ============================================================================

tour_add_example(ch06_copy_move examples/copy_move.cpp)
tour_add_example(ch06_resource_management examples/resource_management.cpp)
tour_add_example(ch06_operator_overloading examples/operator_overloading.cpp)
tour_add_example(ch06_user_defined_literals examples/user_defined_literals.cpp)

add_custom_target(ch06_examples
    DEPENDS
        ch06_copy_move
        ch06_resource_management
        ch06_operator_overloading
        ch06_user_defined_literals
)

# ============================================================================
# Exercises
# ============================================================================

tour_add_exercise(ch06_ex01_string exercises/ex01_string.cpp)
tour_add_exercise(ch06_ex02_raii exercises/ex02_raii.cpp)

add_custom_target(ch06_exercises
    DEPENDS
        ch06_ex01_string
        ch06_ex02_raii
)

# ============================================================================
# Tests
# ============================================================================

if(TOUR_BUILD_TESTS)
    add_executable(ch06_tests tests/test_ch06.cpp)
    target_link_libraries(ch06_tests PRIVATE Catch2::Catch2WithMain tour_warnings)

    include(Catch)
    catch_discover_tests(ch06_tests
        TEST_PREFIX "ch06::"
    )
endif()
