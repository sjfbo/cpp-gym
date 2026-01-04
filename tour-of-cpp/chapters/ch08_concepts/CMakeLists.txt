# Chapter 8: Concepts
#
# This chapter covers C++20 concepts and generic programming:
# - Concepts as compile-time predicates
# - Standard library concepts
# - Defining custom concepts
# - Requires clauses and expressions
# - Variadic templates and fold expressions
# - Concept-constrained template programming

# ============================================================================
# Examples
# ============================================================================

# Concepts Introduction - Using standard library concepts
tour_add_example(ch08_concepts_intro examples/concepts_intro.cpp)

# Concept Definitions - Creating custom concepts and compound requirements
tour_add_example(ch08_concept_definitions examples/concept_definitions.cpp)

# Generic Programming - Concepts with algorithms and constrained templates
tour_add_example(ch08_generic_programming examples/generic_programming.cpp)

# Variadic Templates - Parameter packs, fold expressions, sizeof...
tour_add_example(ch08_variadic_templates examples/variadic_templates.cpp)

# Create a custom target to build all ch08 examples
add_custom_target(ch08_examples
    DEPENDS
        ch08_concepts_intro
        ch08_concept_definitions
        ch08_generic_programming
        ch08_variadic_templates
)

# ============================================================================
# Exercises
# ============================================================================

tour_add_exercise(ch08_ex01_concepts exercises/ex01_concepts.cpp)
tour_add_exercise(ch08_ex02_variadic exercises/ex02_variadic.cpp)

# Create a custom target to build all ch08 exercises
add_custom_target(ch08_exercises
    DEPENDS
        ch08_ex01_concepts
        ch08_ex02_variadic
)

# ============================================================================
# Tests
# ============================================================================

if(TOUR_BUILD_TESTS)
    add_executable(ch08_tests tests/test_ch08.cpp)
    target_link_libraries(ch08_tests PRIVATE Catch2::Catch2WithMain tour_warnings)

    include(Catch)
    catch_discover_tests(ch08_tests
        TEST_PREFIX "ch08::"
    )
endif()
