.PHONY: test build clean setup run

default: build

setup:
	mkdir -p build

clean:
	rm -rf build

build: setup
	cd build && cmake .. && cmake --build .

rebuild: clean build

test: build
	cd build && ctest --output-on-failure

run: build
	@echo
	./build/main $(ARGS)

debug: setup
	cd build && cmake -DCMAKE_BUILD_TYPE=Debug .. && cmake --build .

release: setup
	cd build && cmake -DCMAKE_BUILD_TYPE=Release .. && cmake --build .

