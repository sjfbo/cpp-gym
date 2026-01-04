# Chapter 18: Concurrency
#
# This chapter covers C++ concurrency support:
# - Threads (<thread>)
# - Mutual exclusion (<mutex>)
# - Condition variables (<condition_variable>)
# - Futures and promises (<future>)
# - Atomics (<atomic>)

# ============================================================================
# Examples
# ============================================================================

# Thread creation and management
tour_add_example(ch18_threads examples/threads.cpp)

# Sharing data safely with mutexes
tour_add_example(ch18_sharing_data examples/sharing_data.cpp)

# Condition variables for synchronization
tour_add_example(ch18_condition_variables examples/condition_variables.cpp)

# Futures and promises for async results
tour_add_example(ch18_futures_promises examples/futures_promises.cpp)

# Create a custom target to build all ch18 examples
add_custom_target(ch18_examples
    DEPENDS
        ch18_threads
        ch18_sharing_data
        ch18_condition_variables
        ch18_futures_promises
)

# ============================================================================
# Exercises
# ============================================================================

tour_add_exercise(ch18_ex01_threads exercises/ex01_threads.cpp)
tour_add_exercise(ch18_ex02_producer_consumer exercises/ex02_producer_consumer.cpp)

# Create a custom target to build all ch18 exercises
add_custom_target(ch18_exercises
    DEPENDS
        ch18_ex01_threads
        ch18_ex02_producer_consumer
)

# ============================================================================
# Tests
# ============================================================================

if(TOUR_BUILD_TESTS)
    add_executable(ch18_tests tests/test_ch18.cpp)
    target_link_libraries(ch18_tests PRIVATE Catch2::Catch2WithMain tour_warnings)

    include(Catch)
    catch_discover_tests(ch18_tests
        TEST_PREFIX "ch18::"
    )
endif()
