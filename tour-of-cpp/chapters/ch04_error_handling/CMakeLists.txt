# Chapter 4: Error Handling
# CMakeLists.txt for error handling examples and exercises

cmake_minimum_required(VERSION 3.20)
project(ch04_error_handling LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Compiler warnings
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-Wall -Wextra -Wpedantic)
elseif(MSVC)
    add_compile_options(/W4)
endif()

# ------------------------------------------------------------------------------
# Examples
# ------------------------------------------------------------------------------

add_executable(exceptions examples/exceptions.cpp)
add_executable(invariants examples/invariants.cpp)
add_executable(assertions examples/assertions.cpp)

# ------------------------------------------------------------------------------
# Exercises
# ------------------------------------------------------------------------------

add_executable(ex01_exceptions exercises/ex01_exceptions.cpp)
add_executable(ex02_invariants exercises/ex02_invariants.cpp)

# ------------------------------------------------------------------------------
# Tests (Catch2)
# ------------------------------------------------------------------------------

# Find or fetch Catch2
find_package(Catch2 3 QUIET)
if(NOT Catch2_FOUND)
    include(FetchContent)
    FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG v3.4.0
    )
    FetchContent_MakeAvailable(Catch2)
endif()

add_executable(test_ch04 tests/test_ch04.cpp)
target_link_libraries(test_ch04 PRIVATE Catch2::Catch2WithMain)

# Enable CTest integration
include(CTest)
include(Catch)
catch_discover_tests(test_ch04)
