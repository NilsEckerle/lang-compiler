.PHONY: default help clean build run

TARGET=lang
EXAMPLE=examples/hello_world.lang

default: help

help:
	@echo "$(TARGET)-language compiler"
	@echo "  clean - removes all build files"
	@echo "  build - builds the compiler"
	@echo "  run   - builds the compiler and runs the compiler on $(EXAMPLE)"

clean:
	rm -rf build
	rm -f $(TARGET)

build:
	@echo "TODO: implement build!!!"
	chmod +x $(TARGET)
	
run: build
	./$(TARGET) $(EXAMPLE)

