
macro(kcuk_getname name)
	get_filename_component(${name} ${CMAKE_CURRENT_LIST_DIR} NAME)
	string(PREPEND ${name} "kcuk_")
endmacro()

macro(kcuk_src_common)

	kcuk_getname(name)
	
	file(GLOB_RECURSE ${name}_src ${CMAKE_CURRENT_LIST_DIR}/*.c)
	set(${name}_src ${${name}_src} PARENT_SCOPE)
	add_library(${name}_objects OBJECT ${${name}_src})
	target_include_directories(${name}_objects PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../lib)
	target_include_directories(${name}_objects PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
	get_property(${name}_incs TARGET ${name}_objects PROPERTY INCLUDE_DIRECTORIES)
	set(${name}_incs ${${name}_incs} PARENT_SCOPE)
	
	add_library(${name}_static STATIC $<TARGET_OBJECTS:${name}_objects>)
	target_include_directories(${name}_static PUBLIC ${${name}_incs})
	
endmacro()

function(kcuk_tests_common)

	kcuk_getname(name)
	
	include(${CMAKE_CURRENT_LIST_DIR}/../../cmake/subdirlist.cmake)
	subdirlist(dirs ${CMAKE_CURRENT_LIST_DIR})
	foreach(dir ${dirs})
		file(GLOB tmp ${dir}/*.c)
		set(target ${name}_test_${dir})
		add_executable(${target} ${tmp})
		target_link_libraries(${target} kcuk_lib_static)
		target_link_libraries(${target} ${name}_static)
		add_test(${target} ${target})
	endforeach()
	
endfunction()
