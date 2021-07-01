cmake_minimum_required(VERSION 3.11)
get_filename_component(name ${CMAKE_CURRENT_SOURCE_DIR} NAME)
project(kamcuk_${name} LANGUAGES C)

function(gen_jinja src out)
	add_custom_command(
		COMMAND
			python -c
"from jinja2 import Template; \
import sys; \
import os; \
os.makedirs(os.path.dirname(sys.argv[2]), exist_ok=True); \
print(Template(open(sys.argv[1]).read()).render(), file=open(sys.argv[2], 'w')) \
"
			"${src}" "${out}"
		DEPENDS "${src}"
		OUTPUT "${out}"
		VERBATIM
	)
endfunction()

function(test_extract_properties_from_file exename testname file)
	set(prefix "[ \t]*//// ")
	set(testprops
		PASS_REGULAR_EXPRESSION
		FAIL_REGULAR_EXPRESSION
		SKIP_REGULAR_EXPRESSION
		WILL_FAIL
		TIMEOUT
	)
	set(testprops_append
		LABELS
	)
	set(exeprops
		C_STANDARD
		C_STANDARD_REQUIRED
		C_EXTENSIONS
	)
	foreach(i IN LISTS testprops testprops_append exeprops)
		file(STRINGS ${file} expr REGEX "^${prefix}${i} ")
		if(NOT expr)
			continue()
		endif()
		string(REGEX REPLACE "^${prefix}${i} " "" expr "${expr}")
		
		if(${i} IN_LIST testprops_append)
			set_property(TESTS ${testname} APPEND PROPERTY ${i} ${expr})
			continue()
		endif()

		if(${i} MATCHES "_REGULAR_EXPRESSION")		
			# Because regular expressions are concatenated in set_tests_properties
			# dissallow the use of `^` and `$` anchors.
			# The check is only done if there are multiple prefixes.
			if("${expr}" MATCHES ";${prefix}${i} " AND 
					("${expr}" MATCHES ".^" OR "${expr}" MATCHES "\$."))
				message(FATAL_ERROR "Problem with item ${i} in ${file}")
			endif()
			string(REGEX REPLACE ";${prefix}${i} " "[^\\\\n]*\n" expr "${expr}")
		endif()
	
		if(${i} IN_LIST testprops)
			set_tests_properties(${testname} PROPERTIES ${i} "${expr}")
			continue()
		endif()
		if(${i} IN_LIST exeprops)
			set_target_properties(${exename} PROPERTIES ${i} "${expr}")
			continue()
		endif()

		message(FATAL_ERROR "Internal error: this is not possible")
	endforeach()
endfunction()

function(test_add_supersimple name file)
	add_executable(${name} ${file})
	target_link_libraries(${name} PRIVATE ${ARGN})
	add_test(NAME ${name} COMMAND ${name})
	test_extract_properties_from_file(${name} ${name} tests/onetest.c)
endfunction()

function(test_add_supersimple_dir dir)
	file(GLOB_RECURSE srcs ${dir}/*.c ${dir}/.cpp)
	foreach(src IN LISTS srcs)
		get_filename_component(test ${src} NAME_WE)
		test_add_supersimple(${PROJECT_NAME}_${test} ${src} ${ARGN})
	endforeach()
endfunction()

gen_jinja(
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

test_add_supersimple_dir(tests ${PROJECT_NAME})

