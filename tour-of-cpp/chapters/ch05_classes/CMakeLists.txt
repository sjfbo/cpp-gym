# Chapter 5: Classes
#
# This chapter covers:
# - Concrete types (value semantics)
# - Abstract types (interfaces)
# - Virtual functions (polymorphism)
# - Class hierarchies (inheritance)

# ============================================================================
# Examples
# ============================================================================

tour_add_example(ch05_concrete_types examples/concrete_types.cpp)
tour_add_example(ch05_abstract_types examples/abstract_types.cpp)
tour_add_example(ch05_virtual_functions examples/virtual_functions.cpp)
tour_add_example(ch05_class_hierarchies examples/class_hierarchies.cpp)

add_custom_target(ch05_examples
    DEPENDS
        ch05_concrete_types
        ch05_abstract_types
        ch05_virtual_functions
        ch05_class_hierarchies
)

# ============================================================================
# Exercises
# ============================================================================

tour_add_exercise(ch05_ex01_shape exercises/ex01_shape.cpp)
tour_add_exercise(ch05_ex02_container exercises/ex02_container.cpp)

add_custom_target(ch05_exercises
    DEPENDS
        ch05_ex01_shape
        ch05_ex02_container
)

# ============================================================================
# Tests
# ============================================================================

if(TOUR_BUILD_TESTS)
    add_executable(ch05_tests tests/test_ch05.cpp)
    target_link_libraries(ch05_tests PRIVATE Catch2::Catch2WithMain tour_warnings)

    include(Catch)
    catch_discover_tests(ch05_tests
        TEST_PREFIX "ch05::"
    )
endif()
