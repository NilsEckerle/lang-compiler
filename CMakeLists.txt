cmake_minimum_required(VERSION 3.14)
project(MyProject VERSION 1.0)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0" CACHE STRING "Debug flags" FORCE)

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
  src/parser/pratt_parser.cpp
  src/parser/syntax_tree.cpp
  src/parser/abstract_syntax_tree.cpp
  src/symbol_table.cpp
)
target_link_libraries(myproject_lib PUBLIC spdlog)

# Main executable
add_executable(main
  src/main.cpp
)

# Link to main executable
target_link_libraries(main PRIVATE myproject_lib argparse fmt)
# target_link_libraries(main PRIVATE myproject_lib fmt)

# # Enable testing
enable_testing()

# Test executable
add_executable(tests
  test/unittest/tokens.cpp
  test/unittest/lexer.cpp
  test/unittest/symbol_table.cpp
  test/unittest/parser.cpp
  test/unittest/pratt_parser.cpp

  test/moduletest/lexer.cpp
)

target_compile_options(tests PRIVATE
  $<$<CONFIG:Debug>:-g3 -O0 -ggdb>
)

target_link_libraries(tests
  myproject_lib
  gtest_main
  spdlog
  fmt
)

# Discover tests
include(GoogleTest)
gtest_discover_tests(tests)
