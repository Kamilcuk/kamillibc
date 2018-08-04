B=build

all:
	cmake -H. -B${B} -DCMAKE_RULE_MESSAGES:BOOL=OFF -DCMAKE_VERBOSE_MAKEFILE:BOOL=ON
	make -C ${B} --no-print-directory
	cd ${B} && ctest -v
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
