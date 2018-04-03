BUILD_TYPE:=RELEASE

all: Build
	$(MAKE) -CBuild
	$(MAKE) test

.SECONDARY:

FORCE:

.PHONY: Build
Build: CMakeLists.txt $(shell find cmake) FORCE
	cmake -H. -BBuild -DCMAKE_BUILD_TYPE=$(BUILD_TYPE)

.PHONY: debug
debug: BUILD_TYPE:=DEBUG
debug: Build FORCE
	$(MAKE) -CBuild kamil_debug_test
	$(MAKE) test

.PHONY: test
test: FORCE
	$(MAKE) -CBuild CTEST_OUTPUT_ON_FAILURE=1 test

.PHONY: clean
clean:
	rm -rf Build


