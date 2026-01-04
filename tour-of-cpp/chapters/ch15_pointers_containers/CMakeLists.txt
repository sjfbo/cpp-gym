# Chapter 15: Pointers and Containers
#
# This chapter covers smart pointers and utility containers:
# - unique_ptr and make_unique
# - shared_ptr, weak_ptr, and make_shared
# - std::span for contiguous sequences
# - std::optional, std::variant, std::any

# ============================================================================
# Examples
# ============================================================================

# unique_ptr - exclusive ownership and custom deleters
tour_add_example(ch15_unique_ptr examples/unique_ptr.cpp)

# shared_ptr - shared ownership, weak_ptr, cycles
tour_add_example(ch15_shared_ptr examples/shared_ptr.cpp)

# span - non-owning view of contiguous sequences
tour_add_example(ch15_span examples/span.cpp)

# optional, variant, any - type-safe alternatives
tour_add_example(ch15_optional_variant examples/optional_variant.cpp)

# Create a custom target to build all ch15 examples
add_custom_target(ch15_examples
    DEPENDS
        ch15_unique_ptr
        ch15_shared_ptr
        ch15_span
        ch15_optional_variant
)

# ============================================================================
# Exercises
# ============================================================================

tour_add_exercise(ch15_ex01_smart_ptrs exercises/ex01_smart_ptrs.cpp)
tour_add_exercise(ch15_ex02_optional exercises/ex02_optional.cpp)

# Create a custom target to build all ch15 exercises
add_custom_target(ch15_exercises
    DEPENDS
        ch15_ex01_smart_ptrs
        ch15_ex02_optional
)

# ============================================================================
# Tests
# ============================================================================

if(TOUR_BUILD_TESTS)
    add_executable(ch15_tests tests/test_ch15.cpp)
    target_link_libraries(ch15_tests PRIVATE Catch2::Catch2WithMain tour_warnings)

    include(Catch)
    catch_discover_tests(ch15_tests
        TEST_PREFIX "ch15::"
    )
endif()
