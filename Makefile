
.PHONY: all Debug Release RelWithDebInfo MinSizeRel
all Debug Release RelWithDebInfo MinSizeRel: CMAKE_BUILD_TYPE=$@
all Debug Release RelWithDebInfo MinSizeRel:
	cmake -H. -Bbuild -DCMAKE_BUILD_TYPE=$(CMAKE_BUILD_TYPE)
	$(MAKE) -Cbuild
	
.PHONY: test
test:
	$(MAKE) -Cbuild test

.PHONY: clean
clean:
	rm -rf build


