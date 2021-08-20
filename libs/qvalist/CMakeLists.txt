cmake_minimum_required(VERSION 3.11)
get_filename_component(name ${CMAKE_CURRENT_SOURCE_DIR} NAME)
project(kamcuk_${name} LANGUAGES C)

include(../../kcmakelib/kcmakelib.cmake)
include(k/jinja2)
include(k/test_extract_properties_from_file)

function(test_add_supersimple name file)
	add_executable(${name} ${file})
	target_link_libraries(${name} PRIVATE ${ARGN})
	add_test(NAME ${name} COMMAND ${name})
	test_extract_properties_from_file(${name} ${name} tests/onetest.c)
endfunction()

gen_jinja2(
	${CMAKE_CURRENT_SOURCE_DIR}/src/qvalist.h.jinja
	${CMAKE_CURRENT_SOURCE_DIR}/src/qvalist.h
)
add_library(${PROJECT_NAME} INTERFACE)
target_sources(${PROJECT_NAME} INTERFACE
	${CMAKE_CURRENT_SOURCE_DIR}/src/qvalist.h
)
target_include_directories(${PROJECT_NAME} INTERFACE
	${CMAKE_CURRENT_SOURCE_DIR}/src
)

include(CTest)
if(NOT BUILD_TESTING)
	return()
endif()

function(test_add_supersimple_dir dir)
	file(GLOB_RECURSE srcs ${dir}/*.c ${dir}/.cpp)
	foreach(src IN LISTS srcs)
		get_filename_component(test ${src} NAME_WE)
		test_add_supersimple(${PROJECT_NAME}_${test} ${src} ${ARGN})
	endforeach()
endfunction()

test_add_supersimple_dir(tests ${PROJECT_NAME})

