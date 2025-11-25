cmake_minimum_required(VERSION 3.14)
project(MyProject VERSION 1.0)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories
include_directories(include)

# Fetch third-party libraries
include(FetchContent)

# Fetch argparse
FetchContent_Declare(
    argparse
    GIT_REPOSITORY https://github.com/p-ranav/argparse.git
    GIT_TAG v3.1
)
FetchContent_MakeAvailable(argparse)

# Fetch Google Test
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.14.0
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Create a library with your core source files
add_library(myproject_lib
    src/lexer/lexer.cpp
    src/tokens.cpp
)

# Main executable
add_executable(main
    src/main.cpp
)

# Link library and argparse to main executable
target_link_libraries(main PRIVATE myproject_lib argparse)

# Enable testing
enable_testing()

# Test executable
add_executable(tests
    test/unittest/lexer.cpp
    test/moduletest/lexer.cpp
)

target_link_libraries(tests
    myproject_lib
    gtest
    gtest_main
)

# Discover tests
include(GoogleTest)
gtest_discover_tests(tests)
