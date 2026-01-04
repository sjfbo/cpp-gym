# Chapter 11: Input and Output
#
# This chapter covers I/O operations in C++:
# - Output with cout and formatting
# - Input with cin and getline
# - std::format (C++20)
# - File streams and string streams
# - std::filesystem operations

# ============================================================================
# Examples
# ============================================================================

# Output - cout and basic output operations
tour_add_example(ch11_output examples/output.cpp)

# Input - cin, getline, and input parsing
tour_add_example(ch11_input examples/input.cpp)

# Formatting - std::format and iomanip
tour_add_example(ch11_formatting examples/formatting.cpp)

# Streams - File streams and string streams
tour_add_example(ch11_streams examples/streams.cpp)

# Filesystem - std::filesystem operations
tour_add_example(ch11_filesystem examples/filesystem.cpp)

# Create a custom target to build all ch11 examples
add_custom_target(ch11_examples
    DEPENDS
        ch11_output
        ch11_input
        ch11_formatting
        ch11_streams
        ch11_filesystem
)

# ============================================================================
# Exercises
# ============================================================================

tour_add_exercise(ch11_ex01_file_io exercises/ex01_file_io.cpp)
tour_add_exercise(ch11_ex02_formatting exercises/ex02_formatting.cpp)

# Create a custom target to build all ch11 exercises
add_custom_target(ch11_exercises
    DEPENDS
        ch11_ex01_file_io
        ch11_ex02_formatting
)

# ============================================================================
# Tests
# ============================================================================

if(TOUR_BUILD_TESTS)
    add_executable(ch11_tests tests/test_ch11.cpp)
    target_link_libraries(ch11_tests PRIVATE Catch2::Catch2WithMain tour_warnings)

    include(Catch)
    catch_discover_tests(ch11_tests
        TEST_PREFIX "ch11::"
    )
endif()
