
CMAKE_BUILD_TYPE := Debug

B := _build/${CMAKE_BUILD_TYPE}

CMAKEFLAGS :=
CMAKEFLAGS += $(if $(shell which ninja 2>/dev/null),-GNinja,)
CMAKEFLAGS += -DCMAKE_BUILD_TYPE:STRING=${CMAKE_BUILD_TYPE}
CMAKEFLAGS += -DBUILD_TESTING:BOOL=ON
CMAKEFLAGS += -DCMAKE_RULE_MESSAGES:BOOL=OFF
CMAKEFLAGS += -DCMAKE_VERBOSE_MAKEFILE:BOOL=ON

NICE += $(if $(shell which nice 2>/dev/null),nice,)
NICE += $(if $(shell which ionice 2>/dev/null),ionice,)

CMAKE := $(NICE) cmake

all: build test

gitlab-ci: build test memcheck

build:
	$(CMAKE) -H. -B${B} $(CMAKEFLAGS)
	$(CMAKE) --build ${B} --target all

ifneq ($(OS),Windows_NT)
memcheck: build
	cd ${B} && $(NICE) ctest --output-on-failure -v -T memcheck
else
memcheck: build ;
endif

test: build
	cd ${B} && \
	$(NICE) ctest --output-on-failure -v \
	# || { cat Testing/*/*.log; false; }

test1: build
	cd ${B} && \
	$(NICE) ctest --output-on-failure -v -R kamcuk_curb_runtime_ \
	|| { cat Testing/*/*.log; false; }

clean:
	rm -vr $(B)
