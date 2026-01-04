# Tour of C++ - Learning Repository
# Based on Bjarne Stroustrup's "A Tour of C++ (Third Edition)"
#
# Supports: macOS (Apple Silicon) and Linux (x86_64)
# Compilers: Clang 15+ (default on macOS), GCC 12+ (common on Linux)

cmake_minimum_required(VERSION 3.20)

project(tour-of-cpp
    VERSION 1.0.0
    DESCRIPTION "Hands-on companion for A Tour of C++ (Third Edition)"
    LANGUAGES CXX
)

# ============================================================================
# C++ Standard Configuration
# ============================================================================

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Check for C++23 support and enable if available
include(CheckCXXCompilerFlag)
check_cxx_compiler_flag("-std=c++23" COMPILER_SUPPORTS_CXX23)
if(COMPILER_SUPPORTS_CXX23)
    message(STATUS "C++23 support detected - enabling where applicable")
    set(CMAKE_CXX_STANDARD 23)
endif()

# ============================================================================
# Build Options
# ============================================================================

option(TOUR_ENABLE_SANITIZERS "Enable Address and Undefined Behavior Sanitizers" OFF)
option(TOUR_ENABLE_WARNINGS_AS_ERRORS "Treat warnings as errors" ON)
option(TOUR_BUILD_TESTS "Build the test suite" ON)
option(TOUR_BUILD_PROJECTS "Build the mini projects" ON)

# ============================================================================
# Output Directories
# ============================================================================

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# ============================================================================
# Compiler Warnings
# ============================================================================

include(cmake/CompilerWarnings.cmake)

# ============================================================================
# Sanitizers (Debug builds)
# ============================================================================

if(TOUR_ENABLE_SANITIZERS)
    message(STATUS "Sanitizers enabled")

    if(CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        set(SANITIZER_FLAGS "-fsanitize=address,undefined -fno-omit-frame-pointer")
        set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${SANITIZER_FLAGS}")
        set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} ${SANITIZER_FLAGS}")
    else()
        message(WARNING "Sanitizers not supported for this compiler")
    endif()
endif()

# ============================================================================
# Platform-Specific Configuration
# ============================================================================

# Coroutines support
# Note: Modern compilers (Apple Clang 14+, GCC 10+) have coroutines enabled by default
# in C++20 mode, so we only add the flag for older compilers that need it.
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS "10")
    check_cxx_compiler_flag("-fcoroutines" COMPILER_SUPPORTS_FCOROUTINES)
    if(COMPILER_SUPPORTS_FCOROUTINES)
        add_compile_options(-fcoroutines)
    endif()
endif()

# macOS-specific: Suppress deprecation warnings for older APIs
if(APPLE)
    add_compile_definitions(_LIBCPP_DISABLE_DEPRECATION_WARNINGS)
endif()

# ============================================================================
# Dependencies
# ============================================================================

add_subdirectory(deps)

# ============================================================================
# Chapters (Examples, Exercises, Tests)
# ============================================================================

add_subdirectory(chapters)

# ============================================================================
# Mini Projects
# ============================================================================

if(TOUR_BUILD_PROJECTS)
    add_subdirectory(projects)
endif()

# ============================================================================
# Testing
# ============================================================================

if(TOUR_BUILD_TESTS)
    enable_testing()
    include(CTest)
endif()

# ============================================================================
# Summary
# ============================================================================

message(STATUS "")
message(STATUS "=== Tour of C++ Configuration ===")
message(STATUS "C++ Standard:       ${CMAKE_CXX_STANDARD}")
message(STATUS "Compiler:           ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "Build Type:         ${CMAKE_BUILD_TYPE}")
message(STATUS "Sanitizers:         ${TOUR_ENABLE_SANITIZERS}")
message(STATUS "Warnings as Errors: ${TOUR_ENABLE_WARNINGS_AS_ERRORS}")
message(STATUS "Build Tests:        ${TOUR_BUILD_TESTS}")
message(STATUS "Build Projects:     ${TOUR_BUILD_PROJECTS}")
message(STATUS "=================================")
message(STATUS "")
