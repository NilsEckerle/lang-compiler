.PHONY: test build clean setup run

JOBS := $(shell nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 4)

default: build

setup:
	mkdir -p build

clean:
	rm -rf build

build: setup
	cd build && cmake .. && cmake --build . -j$(JOBS)

rebuild: clean build

test: build
	cd build && ctest --output-on-failure

run: build
	@echo
	./build/main $(ARGS)

debug: setup
	cd build && cmake -DCMAKE_BUILD_TYPE=Debug .. && cmake --build . -j$(JOBS)

release: setup
	cd build && cmake -DCMAKE_BUILD_TYPE=Release .. && cmake --build . -j$(JOBS)
