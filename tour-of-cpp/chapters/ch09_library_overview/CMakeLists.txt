# Chapter 9: Library Overview
#
# This chapter provides an overview of the C++ standard library:
# - Standard-library components
# - Headers and namespaces
# - Containers overview
# - Algorithms overview
# - Strings and I/O overview
# - Utility components

# ============================================================================
# Examples
# ============================================================================

# Standard Library Tour - Overview of major components
tour_add_example(ch09_std_library_tour examples/std_library_tour.cpp)

# Create a custom target to build all ch09 examples
add_custom_target(ch09_examples
    DEPENDS
        ch09_std_library_tour
)

# ============================================================================
# Tests
# ============================================================================

if(TOUR_BUILD_TESTS)
    add_executable(ch09_tests tests/test_ch09.cpp)
    target_link_libraries(ch09_tests PRIVATE Catch2::Catch2WithMain tour_warnings)

    include(Catch)
    catch_discover_tests(ch09_tests
        TEST_PREFIX "ch09::"
    )
endif()
