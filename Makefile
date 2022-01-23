# Makefile

include ./kcmakelib/make/Makefile

gitlab-ci: export RELEASE=1
gitlab-ci:
	+$(MAKE) memcheck sanitize coverage

cdash: export D=_build/cdash
cdash:
	mkdir -p "$(D)"
	find . -maxdepth 1 -mindepth 1 '!' -name _build | xargs -t -d'\n' cp -fat $(D)
	+$(MAKE) -C "$(D)" B=$(D) CMAKE_BUILD_TYPE=Release CMAKEFLAGS_GENERATOR="" CMAKE_C_FLAGS="--coverage -O" .cdash
.cdash: configure
	cd "$(B)" && pwd && ctest -T all

