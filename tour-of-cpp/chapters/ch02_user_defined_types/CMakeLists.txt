# Chapter 2: User-Defined Types
#
# This chapter covers:
# - Structures (struct)
# - Classes
# - Enumerations (enum, enum class)
# - Unions

# ============================================================================
# Examples
# ============================================================================

tour_add_example(ch02_structures examples/structures.cpp)
tour_add_example(ch02_classes examples/classes.cpp)
tour_add_example(ch02_enumerations examples/enumerations.cpp)
tour_add_example(ch02_unions examples/unions.cpp)

add_custom_target(ch02_examples
    DEPENDS
        ch02_structures
        ch02_classes
        ch02_enumerations
        ch02_unions
)

# ============================================================================
# Exercises
# ============================================================================

tour_add_exercise(ch02_ex01_struct exercises/ex01_struct.cpp)
tour_add_exercise(ch02_ex02_class exercises/ex02_class.cpp)
tour_add_exercise(ch02_ex03_enum exercises/ex03_enum.cpp)

add_custom_target(ch02_exercises
    DEPENDS
        ch02_ex01_struct
        ch02_ex02_class
        ch02_ex03_enum
)

# ============================================================================
# Tests
# ============================================================================

if(TOUR_BUILD_TESTS)
    add_executable(ch02_tests tests/test_ch02.cpp)
    target_link_libraries(ch02_tests PRIVATE Catch2::Catch2WithMain tour_warnings)

    include(Catch)
    catch_discover_tests(ch02_tests
        TEST_PREFIX "ch02::"
    )
endif()
