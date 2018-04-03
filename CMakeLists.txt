cmake_minimum_required (VERSION 3.4)

project(kamil C)

set(COMMON_FLAGS    "-std=gnu11 -O3 -fPIC -flto -pedantic -Wall")
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

set(CTEST_OUTPUT_ON_FAILURE TRUE)
enable_testing()
add_test(${PROJECT_NAME}_debug_test   ./cmake/debug/${PROJECT_NAME}_debug_test)
add_test(${PROJECT_NAME}_test         ./cmake/default/${PROJECT_NAME}_test)
add_test(${PROJECT_NAME}_release_test ./cmake/release/${PROJECT_NAME}_release_test)

