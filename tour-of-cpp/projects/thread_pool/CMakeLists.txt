cmake_minimum_required(VERSION 3.20)
project(thread_pool VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Find threading library
find_package(Threads REQUIRED)

# Main executable
add_executable(thread_pool_demo main.cpp)
target_include_directories(thread_pool_demo PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(thread_pool_demo PRIVATE Threads::Threads)

# Enable warnings
target_compile_options(thread_pool_demo PRIVATE
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

    add_executable(test_thread_pool tests/test_thread_pool.cpp)
    target_include_directories(test_thread_pool PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
    target_link_libraries(test_thread_pool PRIVATE Threads::Threads Catch2::Catch2WithMain)

    target_compile_options(test_thread_pool PRIVATE
        $<$<CXX_COMPILER_ID:GNU,Clang,AppleClang>:-Wall -Wextra -Wpedantic>
        $<$<CXX_COMPILER_ID:MSVC>:/W4>
    )

    include(CTest)
    include(Catch)
    catch_discover_tests(test_thread_pool)
endif()
