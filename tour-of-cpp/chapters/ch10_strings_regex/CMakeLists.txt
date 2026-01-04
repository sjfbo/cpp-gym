# Chapter 10: Strings and Regular Expressions
#
# This chapter covers string handling and pattern matching:
# - std::string operations
# - std::string_view for non-owning references
# - Regular expressions with std::regex
# - Pattern matching, searching, and replacing

# ============================================================================
# Examples
# ============================================================================

# Strings - std::string operations
tour_add_example(ch10_strings examples/strings.cpp)

# String Views - Non-owning string references
tour_add_example(ch10_string_views examples/string_views.cpp)

# Regular Expressions - Pattern matching
tour_add_example(ch10_regex examples/regex.cpp)

# Create a custom target to build all ch10 examples
add_custom_target(ch10_examples
    DEPENDS
        ch10_strings
        ch10_string_views
        ch10_regex
)

# ============================================================================
# Exercises
# ============================================================================

tour_add_exercise(ch10_ex01_strings exercises/ex01_strings.cpp)
tour_add_exercise(ch10_ex02_regex exercises/ex02_regex.cpp)

# Create a custom target to build all ch10 exercises
add_custom_target(ch10_exercises
    DEPENDS
        ch10_ex01_strings
        ch10_ex02_regex
)

# ============================================================================
# Tests
# ============================================================================

if(TOUR_BUILD_TESTS)
    add_executable(ch10_tests tests/test_ch10.cpp)
    target_link_libraries(ch10_tests PRIVATE Catch2::Catch2WithMain tour_warnings)

    include(Catch)
    catch_discover_tests(ch10_tests
        TEST_PREFIX "ch10::"
    )
endif()
