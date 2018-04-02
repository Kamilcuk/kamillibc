BUILD_TYPE:=RELEASE

all: release

release: BUILD_TYPE:=RELEASE

.SECONDARY:
FORCE:
.PHONY: Build
Build: CMakeLists.txt $(shell find cmake) FORCE
	cmake -H. -BBuild -DCMAKE_BUILD_TYPE=$(BUILD_TYPE)
	
.PHONY: debug
debug: BUILD_TYPE:=DEBUG
debug: Build FORCE
	$(MAKE) -CBuild kamil_debug_test
	$(MAKE) -CBuild test

.PHONY: clean
clean:
	rm -rf Build


