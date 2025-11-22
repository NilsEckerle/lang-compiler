.PHONY: default help clean build run

TARGET=lang
EXAMPLE=examples/hello_world.lang

# Compiler and flags
CXX=g++
CXXFLAGS=-Wall -Wextra -std=c++17 -Iinclude
LDFLAGS=

# Directories
SRC_DIR=src
INCLUDE_DIR=include
BUILD_DIR=build

# Find all .cpp files in src/
SOURCES=$(wildcard $(SRC_DIR)/*.cpp)
# Generate object file paths in build/
OBJECTS=$(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SOURCES))

default: help

help:
	@echo "$(TARGET)-language compiler"
	@echo "  clean - removes all build files"
	@echo "  build - builds the compiler"
	@echo "  run   - builds the compiler and runs the compiler on $(EXAMPLE)"

clean:
	rm -rf $(BUILD_DIR)
	rm -f $(TARGET)

# Create build directory if it doesn't exist
setup:
	mkdir -p $(BUILD_DIR)

# Rule to compile .cpp files to .o files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | setup
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Link object files to create the executable
$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) $(LDFLAGS) -o $(TARGET)

# Build target depends on the executable
build: $(TARGET)
	@echo "Build complete!"
	
run: build
	./$(TARGET) $(EXAMPLE)
