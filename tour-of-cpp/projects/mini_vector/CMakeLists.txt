cmake_minimum_required(VERSION 3.20)
project(mini_vector VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Main executable
add_executable(mini_vector_demo main.cpp)
target_include_directories(mini_vector_demo PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# Enable warnings
target_compile_options(mini_vector_demo PRIVATE
    $<$<CXX_COMPILER_ID:GNU,Clang,AppleClang>:-Wall -Wextra -Wpedantic>
    $<$<CXX_COMPILER_ID:MSVC>:/W4>
)

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

    add_executable(test_mini_vector tests/test_mini_vector.cpp)
    target_include_directories(test_mini_vector PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
    target_link_libraries(test_mini_vector PRIVATE Catch2::Catch2WithMain)

    target_compile_options(test_mini_vector PRIVATE
        $<$<CXX_COMPILER_ID:GNU,Clang,AppleClang>:-Wall -Wextra -Wpedantic>
        $<$<CXX_COMPILER_ID:MSVC>:/W4>
    )

    include(CTest)
    include(Catch)
    catch_discover_tests(test_mini_vector)
endif()
