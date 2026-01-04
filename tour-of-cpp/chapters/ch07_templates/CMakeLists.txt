# Chapter 7: Templates
#
# This chapter covers C++ templates for generic programming:
# - Class templates (parameterized types)
# - Function templates and type deduction
# - Generic lambdas and template lambdas (C++20)
# - Template mechanisms: specialization, SFINAE, if constexpr

# ============================================================================
# Examples
# ============================================================================

# Parameterized Types - Vector<T>, Pair<T,U> class templates
tour_add_example(ch07_parameterized_types examples/parameterized_types.cpp)

# Function Templates - generic algorithms, type deduction
tour_add_example(ch07_function_templates examples/function_templates.cpp)

# Lambda Templates - generic lambdas, template lambdas C++20
tour_add_example(ch07_lambda_templates examples/lambda_templates.cpp)

# Template Mechanisms - specialization, SFINAE intro, if constexpr
tour_add_example(ch07_template_mechanisms examples/template_mechanisms.cpp)

# Create a custom target to build all ch07 examples
add_custom_target(ch07_examples
    DEPENDS
        ch07_parameterized_types
        ch07_function_templates
        ch07_lambda_templates
        ch07_template_mechanisms
)

# ============================================================================
# Exercises
# ============================================================================

tour_add_exercise(ch07_ex01_stack exercises/ex01_stack.cpp)
tour_add_exercise(ch07_ex02_algorithms exercises/ex02_algorithms.cpp)

# Create a custom target to build all ch07 exercises
add_custom_target(ch07_exercises
    DEPENDS
        ch07_ex01_stack
        ch07_ex02_algorithms
)

# ============================================================================
# Tests
# ============================================================================

if(TOUR_BUILD_TESTS)
    add_executable(ch07_tests tests/test_ch07.cpp)
    target_link_libraries(ch07_tests PRIVATE Catch2::Catch2WithMain tour_warnings)

    include(Catch)
    catch_discover_tests(ch07_tests
        TEST_PREFIX "ch07::"
    )
endif()
