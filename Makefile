
.PHONY: all Debug Release RelWithDebInfo MinSizeRel
all Debug Release RelWithDebInfo MinSizeRel: CMAKE_BUILD_TYPE=$@
all Debug Release RelWithDebInfo MinSizeRel:
	cmake -H. -BBuild -DCMAKE_BUILD_TYPE=$(CMAKE_BUILD_TYPE)
	$(MAKE) -CBuild
	
.PHONY: test
test:
	$(MAKE) -CBuild test

.PHONY: clean
clean:
	rm -rf Build


