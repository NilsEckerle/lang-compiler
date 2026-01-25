.PHONY: test build clean setup run examples

TARGET=lang-compiler

JOBS := $(shell nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 4)

default: build

setup:
	mkdir -p build

clean:
	rm -rf build
	rm $(TARGET)

build: setup
	cd build && cmake .. && cmake --build . -j$(JOBS)
	cp build/main $(TARGET)

rebuild: clean build

test: build
	cd build && GTEST_COLOR=1 ctest --output-on-failure

run: build
	@echo
	./build/main $(ARGS)

debug: setup
	cd build && cmake -DCMAKE_BUILD_TYPE=Debug .. && cmake --build . -j$(JOBS)

release: setup
	cd build && cmake -DCMAKE_BUILD_TYPE=Release .. && cmake --build . -j$(JOBS)

examples: build
	@echo "========== Running all examples =========="
	@for file in examples/*.lang; do \
		echo "========== Processing $$file =========="; \
		./build/main "$$file"; \
		echo ""; \
	done

example-%: build
	@echo "========== Running examples/$*.lang =========="
	./build/main examples/$*.lang
