B=build

all:
	cmake -H. -B${B} -DCMAKE_BUILD_TYPE=Debug -DCMAKE_RULE_MESSAGES:BOOL=OFF -DCMAKE_VERBOSE_MAKEFILE:BOOL=ON
	$(MAKE) --no-print-directory make_silenttest
arm:
	cmake -H. -B${B} \
		-DCMAKE_RULE_MESSAGES:BOOL=OFF -DCMAKE_VERBOSE_MAKEFILE:BOOL=ON \
		-DCMAKE_SYSTEM_NAME=ARM -DCMAKE_
	$(MAKE) --no-print-directory make_silenttest
make_silenttest:
	make -C ${B} --no-print-directory
	cd ${B} && ctest -v || { cat Testing/*/*.log; false; }
gitlab-ci:
	cmake -H. -B${B} -DCMAKE_VERBOSE_MAKEFILE:BOOL=ON
	cmake --build ${B}
	cd ${B} && ctest -v
nice:
	cmake -H. -B${B}
	cmake --build ${B}
	cd ${B} && ctest -v
clean:
	rm -vr $(B)
