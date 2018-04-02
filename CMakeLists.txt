cmake_minimum_required (VERSION 3.4)

project(kamil ASM C)

set(COMMON_FLAGS    "-Wall -O3 -std=gnu11 -flto")
set(CMAKE_ASM_FLAGS "${COMMON_FLAGS}")
set(CMAKE_C_FLAGS   "${COMMON_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${COMMON_FLAGS}")

file(GLOB_RECURSE m4src FOLLOW_SYMLINKS src/*.m4)
foreach(m4 ${m4src})
	string(REGEX REPLACE "\\.[^.]*$" "" m4noext ${m4src})
	add_custom_command(
		OUTPUT ${m4}
		COMMAND m4 -P -s ${m4} > ${m4noext}
		VERBATIM
	)
endforeach()

add_custom_command(
	OUTPUT third_party/Unity
	COMMAND git submodule update --init --recursive third_party/Unity
	VERBATIM
)

include_directories(src/lib)
file(GLOB_RECURSE src FOLLOW_SYMLINKS 
	src/lib/*.c src/linux/*.c)
file(GLOB_RECURSE testsrc FOLLOW_SYMLINKS 
	test/*.c)
	
add_subdirectory(cmake)

enable_testing()
macro(add_test_h name)
	add_test(${PROJECT_NAME}_${name}_test 
	/bin/sh -xc "tty=$(tty); ./cmake/${name}/${PROJECT_NAME}_${name}_test 2>&1 | tee $tty"
	)
endmacro()
add_test_h(debug)
add_test_h(default)
add_test_h(release)


