cmake_minimum_required(VERSION 3.20)
project(simple_json VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Library
add_library(simple_json STATIC
    json_parser.cpp
)
target_include_directories(simple_json PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Main executable
add_executable(simple_json_demo main.cpp)
target_link_libraries(simple_json_demo PRIVATE simple_json)

# Enable warnings
foreach(target simple_json simple_json_demo)
    target_compile_options(${target} PRIVATE
        $<$<CXX_COMPILER_ID:GNU,Clang,AppleClang>:-Wall -Wextra -Wpedantic>
        $<$<CXX_COMPILER_ID:MSVC>:/W4>
    )
endforeach()

# Testing
option(BUILD_TESTS "Build tests" ON)

if(BUILD_TESTS)
    include(FetchContent)
    FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG v3.5.0
    )
    FetchContent_MakeAvailable(Catch2)

    add_executable(test_json tests/test_json.cpp)
    target_link_libraries(test_json PRIVATE simple_json Catch2::Catch2WithMain)

    target_compile_options(test_json PRIVATE
        $<$<CXX_COMPILER_ID:GNU,Clang,AppleClang>:-Wall -Wextra -Wpedantic>
        $<$<CXX_COMPILER_ID:MSVC>:/W4>
    )

    include(CTest)
    include(Catch)
    catch_discover_tests(test_json)
endif()
