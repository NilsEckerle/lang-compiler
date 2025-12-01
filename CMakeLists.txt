cmake_minimum_required(VERSION 3.14)
project(MyProject VERSION 1.0)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Compiler warnings
if(MSVC)
    add_compile_options(/W4 /WX)
else()
    add_compile_options(-Wall -Wextra -Wpedantic -Werror)
endif()

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

# Fetch spdlog
FetchContent_Declare(
    spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog.git
    GIT_TAG v1.16.0
)
FetchContent_MakeAvailable(spdlog)

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
    src/tokens.cpp
    src/lexer/lexer.cpp
    src/parser/parser.cpp
    src/parser/syntax_tree.cpp
)

# Main executable
add_executable(main
    src/main.cpp
)

# Link to main executable
target_link_libraries(main PRIVATE myproject_lib argparse)
target_link_libraries(main PRIVATE myproject_lib spdlog)

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
    fmt
)

# Discover tests
include(GoogleTest)
gtest_discover_tests(tests)
