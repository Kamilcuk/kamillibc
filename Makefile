
B := _build

cmake_flags :=
cmake_flags += $(if $(shell which ninja 2>/dev/null),-GNinja,)
NICE = $(if $(shell which nice 2>/dev/null),nice,)
NICE += $(if $(shell which ionice 2>/dev/null),ionice,)
CMAKE = $(NICE) cmake

# nproc = $(if $(shell which nproc 2>/dev/null),$(shell nproc),1)

all: test
test:
	$(CMAKE) -H. -B${B} $(cmake_flags) \
		-DCMAKE_BUILD_TYPE:STRING=Debug \
		-DBUILD_TESTING:BOOL=ON \
		-DCMAKE_RULE_MESSAGES:BOOL=OFF \
		-DCMAKE_VERBOSE_MAKEFILE:BOOL=ON
	$(CMAKE) --build ${B} --target all
	cd ${B} && $(NICE) ctest --output-on-failure -v
	@# || { cat Testing/*/*.log; false; }

clean:
	rm -vr $(B)
