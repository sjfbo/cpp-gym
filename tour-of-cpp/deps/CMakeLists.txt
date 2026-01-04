# Dependencies for Tour of C++
#
# Uses CMake FetchContent to download dependencies at configure time.
# This ensures reproducible builds across platforms.

include(FetchContent)

# ============================================================================
# Catch2 v3 - Modern C++ Testing Framework
# ============================================================================
# Catch2 v3 is a header-only-friendly testing framework with excellent
# Apple Silicon support. It's ideal for learning C++ because:
# - Minimal boilerplate
# - Expressive assertions
# - BDD-style test cases
# - Easy to read test output

if(TOUR_BUILD_TESTS)
    message(STATUS "Fetching Catch2 v3...")

    FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG        v3.5.2  # Latest stable v3 release
        GIT_SHALLOW    TRUE
    )

    # Disable Catch2's own tests and extras
    set(CATCH_INSTALL_DOCS OFF CACHE BOOL "" FORCE)
    set(CATCH_INSTALL_EXTRAS OFF CACHE BOOL "" FORCE)
    set(CATCH_DEVELOPMENT_BUILD OFF CACHE BOOL "" FORCE)

    FetchContent_MakeAvailable(Catch2)

    # Make Catch2's CMake modules available for test discovery
    list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
    set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} PARENT_SCOPE)

    message(STATUS "Catch2 v3 configured successfully")
endif()

# ============================================================================
# fmt library (fallback for std::format if not available)
# ============================================================================
# Some compilers don't yet fully support std::format. The fmt library
# provides a compatible implementation and is the basis for std::format.

# Check if std::format is available
include(CheckCXXSourceCompiles)
set(CMAKE_REQUIRED_FLAGS "-std=c++20")
check_cxx_source_compiles("
    #include <format>
    int main() {
        auto s = std::format(\"Hello, {}!\", \"World\");
        return 0;
    }
" HAS_STD_FORMAT)

if(NOT HAS_STD_FORMAT)
    message(STATUS "std::format not available, fetching fmt library...")

    FetchContent_Declare(
        fmt
        GIT_REPOSITORY https://github.com/fmtlib/fmt.git
        GIT_TAG        10.2.1
        GIT_SHALLOW    TRUE
    )

    FetchContent_MakeAvailable(fmt)

    # Create an alias that examples can use
    add_library(format_lib INTERFACE)
    target_link_libraries(format_lib INTERFACE fmt::fmt)
    target_compile_definitions(format_lib INTERFACE USE_FMT_LIBRARY)

    message(STATUS "fmt library configured as std::format fallback")
else()
    message(STATUS "std::format is available natively")

    # Create a no-op interface library for consistent linking
    add_library(format_lib INTERFACE)
endif()
