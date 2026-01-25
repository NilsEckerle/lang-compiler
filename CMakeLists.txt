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

##################################################
# LLVM SETUP
##################################################

# Find LLVM package
find_package(LLVM REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

# Add LLVM definitions and include directories
include_directories(${LLVM_INCLUDE_DIRS})
separate_arguments(LLVM_DEFINITIONS_LIST NATIVE_COMMAND ${LLVM_DEFINITIONS})
add_definitions(${LLVM_DEFINITIONS_LIST})

# Get LLVM library configuration
# LLVM 21 uses a monolithic library, so we use llvm-config to get the correct flags
execute_process(
  COMMAND llvm-config --libs
  OUTPUT_VARIABLE LLVM_LIBS
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

execute_process(
  COMMAND llvm-config --ldflags
  OUTPUT_VARIABLE LLVM_LDFLAGS
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

execute_process(
  COMMAND llvm-config --system-libs
  OUTPUT_VARIABLE LLVM_SYSTEM_LIBS
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

# Combine all LLVM linking flags
set(llvm_libs ${LLVM_LDFLAGS} ${LLVM_LIBS} ${LLVM_SYSTEM_LIBS})

##################################################
# THIRD PARTY LIBRARY
##################################################

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
  src/code_generation/print_test_visitor.cpp
  src/code_generation/codegen_visitor.cpp
)
# Link LLVM libraries to myproject_lib
target_link_libraries(myproject_lib PUBLIC spdlog ${llvm_libs})

# Main executable
add_executable(main
  src/main.cpp
)

# Link to main executable
target_link_libraries(main PRIVATE myproject_lib argparse fmt)
# target_link_libraries(main PRIVATE myproject_lib fmt)

##################################################
# TESTING
##################################################

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
