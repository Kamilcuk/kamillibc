cmake_minimum_required (VERSION 3.7)
project(kamillibc C)

# default compile options
SET(CMAKE_AR "gcc-ar")
SET(CMAKE_C_ARCHIVE_CREATE "<CMAKE_AR> qcs <TARGET> <LINK_FLAGS> <OBJECTS>")
SET(CMAKE_C_ARCHIVE_FINISH   true)
set(CMAKE_C_FLAGS          "-std=gnu11 -O3 -Wall -fPIC -flto")

# get sources
include_directories(src/lib)
file(GLOB_RECURSE src FOLLOW_SYMLINKS src/lib/*.c)
if(NOT KAMILLIBC_NOT_LINUX)
	include_directories(src/linux)
	file(GLOB_RECURSE linuxsrc FOLLOW_SYMLINKS src/linux/*.c)
	list(APPEND src ${linuxsrc})
endif()
	

# generate objects
add_library(${PROJECT_NAME}_object OBJECT ${src})
# generate static library
add_library(${PROJECT_NAME}_static STATIC $<TARGET_OBJECTS:${PROJECT_NAME}_object>)
# generate shared library
add_library(${PROJECT_NAME}_shared SHARED $<TARGET_OBJECTS:${PROJECT_NAME}_object>)

# generate test executable and run tests
if(NOT KAMILLIBC_DISABLE_TESTING)
	enable_testing()
	set(CTEST_OUTPUT_ON_FAILURE true)
	file(GLOB tests tests/*)
	foreach(test ${tests})
		message(STATUS ${test})
		file(GLOB testsrc ${test}/*.c)
		get_filename_component(name ${test} NAME)
		set(target ${PROJECT_NAME}_${name}_test)
		add_executable(${target} ${testsrc} $<TARGET_OBJECTS:${PROJECT_NAME}_object>)
		if(${name} MATCHES "async")
			target_link_libraries(${target} -lpthread)
		endif()
		add_test(${target} ${target})
	endforeach()
endif()

