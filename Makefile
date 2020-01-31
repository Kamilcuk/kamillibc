
B := _build

cmake_flags :=
cmake_flags += $(if $(shell which ninja 2>/dev/null),-GNinja,)
CMAKE = cmake $(cmake_flags)

# nproc = $(if $(shell which nproc 2>/dev/null),$(shell nproc),1)

all: test
test:
	$(CMAKE) -H. -B${B} \
		-DCMAKE_BUILD_TYPE:STRING=Debug \
		-DBUILD_TESTING:BOOL=ON \
		-DCMAKE_RULE_MESSAGES:BOOL=OFF \
		-DCMAKE_VERBOSE_MAKEFILE:BOOL=ON
	nice ionice cmake --build ${B}
	cd ${B} && nice ionice ctest --output-on-failure -v
	@# || { cat Testing/*/*.log; false; }

clean:
	rm -vr $(B)
