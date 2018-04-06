cmake_minimum_required (VERSION 3.4)
project(kamil C)

# default compile options
set(COMMON_FLAGS    "-std=gnu11 -O3 -fPIC -flto -Wall")
set(CMAKE_ASM_FLAGS "${COMMON_FLAGS}")
set(CMAKE_C_FLAGS   "${COMMON_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${COMMON_FLAGS}")

# generate .h files from .m4
file(GLOB_RECURSE m4src FOLLOW_SYMLINKS src/*.m4)
add_custom_target(generate_src_from_m4)
foreach(m4 ${m4src})
	string(REGEX REPLACE "\\.[^.]*$" "" m4noext ${m4src})
	add_custom_command(
		OUTPUT ${m4noext}
		COMMAND m4 -P -s ${m4} > ${m4noext}
		DEPENDS ${m4}
	)
	add_dependencies(generate_src_from_m4 ${m4noext})
endforeach()

# git submodule init
foreach(name Unity ccan pt)
	if(NOT EXISTS third-party/${name})
		execute_process(COMMAND git submodule update --init --recursive third-party/${name})
	endif()
endforeach()

# generate config.h needed by ccan modules. src/ccan/ccan_config.h is included by src/lib/config.h 
add_custom_command(
	OUTPUT src/ccan/ccan_config.h
	COMMAND $(MAKE) -C${CMAKE_SOURCE_DIR}/third-party/ccan config.h 
	COMMAND cp -v ${CMAKE_SOURCE_DIR}/third-party/ccan/config.h ${CMAKE_SOURCE_DIR}/src/ccan/ccan_config.h
)
add_custom_target(ccan_config DEPENDS src/ccan/ccan_config.h)

# get sources
include_directories(src/linux src/lib src/ccan)
file(GLOB_RECURSE src FOLLOW_SYMLINKS src/lib/*.c src/linux/*.c)
file(GLOB_RECURSE testsrc FOLLOW_SYMLINKS test/*.c)

# add ccan library
add_subdirectory(src/ccan)
add_dependencies(ccan ccan_config)

# generate static library
add_library(${PROJECT_NAME}_static STATIC ${src})
add_dependencies(${PROJECT_NAME}_static ccan)

# generate shared library
add_library(${PROJECT_NAME}_shared SHARED ${src})
target_link_libraries(${PROJECT_NAME}_shared ${PROJECT_NAME}_static)

# generate test executable and run tests
add_executable(${PROJECT_NAME}_test ${testsrc})
target_link_libraries(${PROJECT_NAME}_test ${PROJECT_NAME}_static)
set(CTEST_OUTPUT_ON_FAILURE TRUE)
enable_testing()
add_test(${PROJECT_NAME}_test ${PROJECT_NAME}_test)

