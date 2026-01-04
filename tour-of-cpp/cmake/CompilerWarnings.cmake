# CompilerWarnings.cmake
# Strict compiler warning flags for learning C++
#
# These warnings help catch common mistakes and encourage best practices.
# Using strict warnings is essential for learning proper C++.

# Create an interface library to apply warnings uniformly
add_library(tour_warnings INTERFACE)

# Get the warning flags based on compiler
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
    # Clang/AppleClang warnings
    # Note: Some pedantic warnings are relaxed for this learning codebase
    target_compile_options(tour_warnings INTERFACE
        -Wall                       # Basic warnings
        -Wextra                     # Extra warnings
        -Wpedantic                  # Strict ISO C++ compliance
        -Wno-unused-parameter       # Allow unused params (common in teaching code)
        -Wcast-align                # Pointer cast alignment issues
        -Wcast-qual                 # Cast removes qualifiers
        -Wdouble-promotion          # Float to double promotion
        -Wextra-semi                # Extra semicolons
        -Wformat=2                  # Printf format string issues
        -Wimplicit-fallthrough      # Missing break in switch
        -Wmissing-declarations      # Missing function declarations
        -Wnon-virtual-dtor          # Non-virtual destructor in polymorphic class
        -Wnull-dereference          # Null pointer dereference
        -Wold-style-cast            # C-style casts
        -Woverloaded-virtual        # Overloaded virtual functions
        -Wshadow                    # Variable shadowing
        -Wswitch-enum               # Missing switch cases for enums
        -Wundef                     # Undefined macro in #if
        -Wunreachable-code          # Unreachable code
        -Wunused                    # Unused variables/functions
        -Wwrite-strings             # String literal type issues
        -Wzero-as-null-pointer-constant  # Using 0 instead of nullptr
    )

elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    # GCC warnings
    target_compile_options(tour_warnings INTERFACE
        -Wall                       # Basic warnings
        -Wextra                     # Extra warnings
        -Wpedantic                  # Strict ISO C++ compliance
        -Wcast-align                # Pointer cast alignment issues
        -Wcast-qual                 # Cast removes qualifiers
        -Wconversion                # Implicit type conversions
        -Wdouble-promotion          # Float to double promotion
        -Wduplicated-branches       # Duplicated branches in if-else
        -Wduplicated-cond           # Duplicated conditions
        -Wformat=2                  # Printf format string issues
        -Wformat-overflow=2         # Format string overflow
        -Wformat-truncation=2       # Format string truncation
        -Wimplicit-fallthrough=5    # Missing break in switch
        -Wlogical-op                # Suspicious logical operations
        -Wmisleading-indentation    # Misleading indentation
        -Wmissing-declarations      # Missing function declarations
        -Wmissing-include-dirs      # Missing include directories
        -Wnon-virtual-dtor          # Non-virtual destructor in polymorphic class
        -Wnull-dereference          # Null pointer dereference
        -Wold-style-cast            # C-style casts
        -Woverloaded-virtual        # Overloaded virtual functions
        -Wpacked                    # Packed attribute issues
        -Wredundant-decls           # Redundant declarations
        -Wshadow                    # Variable shadowing
        -Wsign-conversion           # Sign conversion issues
        -Wstrict-null-sentinel      # Strict null sentinel usage
        -Wsuggest-attribute=noreturn # Suggest noreturn attribute
        -Wsuggest-override          # Suggest override keyword
        -Wswitch-enum               # Missing switch cases for enums
        -Wtrampolines               # Trampoline generation
        -Wundef                     # Undefined macro in #if
        -Wunreachable-code          # Unreachable code
        -Wunused                    # Unused variables/functions
        -Wuseless-cast              # Useless casts
        -Wwrite-strings             # String literal type issues
        -Wzero-as-null-pointer-constant  # Using 0 instead of nullptr
    )

elseif(MSVC)
    # MSVC warnings
    target_compile_options(tour_warnings INTERFACE
        /W4                         # High warning level
        /permissive-                # Strict standards conformance
        /w14242                     # Conversion, possible loss of data
        /w14254                     # Larger bit field assigned to smaller
        /w14263                     # Function doesn't override virtual
        /w14265                     # Class has virtual but no virtual dtor
        /w14287                     # Unsigned/negative constant mismatch
        /w14296                     # Expression is always true/false
        /w14311                     # Pointer truncation
        /w14545                     # Expression before comma evaluates to function
        /w14546                     # Function call before comma missing argument list
        /w14547                     # Operator before comma has no effect
        /w14549                     # Operator before comma has no effect
        /w14555                     # Expression has no effect
        /w14619                     # Warning number doesn't exist
        /w14640                     # Thread-unsafe static member initialization
        /w14826                     # Conversion is sign-extended
        /w14905                     # Wide string literal cast to LPSTR
        /w14906                     # String literal cast to LPWSTR
        /w14928                     # Illegal copy-initialization
    )
endif()

# Optionally treat warnings as errors (for learning, this is recommended)
if(TOUR_ENABLE_WARNINGS_AS_ERRORS)
    if(MSVC)
        target_compile_options(tour_warnings INTERFACE /WX)
    else()
        target_compile_options(tour_warnings INTERFACE -Werror)
    endif()
    message(STATUS "Warnings treated as errors: ON")
else()
    message(STATUS "Warnings treated as errors: OFF")
endif()

# Create a separate library for exercises (warnings but no -Werror)
# Exercises often have intentional issues for learning purposes
add_library(tour_warnings_exercises INTERFACE)

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
    target_compile_options(tour_warnings_exercises INTERFACE
        -Wall -Wextra -Wpedantic
    )
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(tour_warnings_exercises INTERFACE
        -Wall -Wextra -Wpedantic
    )
elseif(MSVC)
    target_compile_options(tour_warnings_exercises INTERFACE /W4)
endif()

# Function to apply warnings to a target
function(tour_apply_warnings target)
    target_link_libraries(${target} PRIVATE tour_warnings)
endfunction()

# Function to create an example executable with warnings
function(tour_add_example name source)
    add_executable(${name} ${source})
    tour_apply_warnings(${name})
endfunction()

# Function to create an exercise executable with relaxed warnings
# (no -Werror since exercises often have intentional issues for learning)
function(tour_add_exercise name source)
    add_executable(${name} ${source})
    target_link_libraries(${name} PRIVATE tour_warnings_exercises)
endfunction()
