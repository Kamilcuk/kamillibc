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
gen_jinja(
	${CMAKE_CURRENT_SOURCE_DIR}/src/qvalist.h.jinja
	${CMAKE_CURRENT_BINARY_DIR}/include/qvalist.h
)
add_library(${PROJECT_NAME} INTERFACE)
target_sources(${PROJECT_NAME} INTERFACE
	${CMAKE_CURRENT_BINARY_DIR}/include/qvalist.h
)
target_include_directories(${PROJECT_NAME} INTERFACE
	${CMAKE_CURRENT_BINARY_DIR}/include
)

include(CTest)
if(BUILD_TESTING)
	file(GLOB tests tests/*.c)
	foreach(testf IN LISTS tests)
		get_filename_component(test ${testf} NAME_WE)
		add_executable(${PROJECT_NAME}_${test} ${testf})
		add_test(NAME ${PROJECT_NAME}_${test} COMMAND ${PROJECT_NAME}_${test})
		target_link_libraries(${PROJECT_NAME}_${test} ${PROJECT_NAME})
	endforeach()
endif()
set_tests_properties(${PROJECT_NAME}_onetest PROPERTIES
	PASS_REGULAR_EXPRESSION "10 c 1.10000 some string"
)

