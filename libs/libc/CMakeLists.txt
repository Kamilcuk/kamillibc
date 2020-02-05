get_filename_component(name ${CMAKE_CURRENT_SOURCE_DIR} NAME)
project(kamcuk_${name})

file(GLOB_RECURSE srcs src/*.c src/*.cpp)
add_library(${PROJECT_NAME} ${srcs})
set_target_properties(${PROJECT_NAME} PROPERTIES
    C_STANDARD 11
    C_STANDARD_REQUIRED YES
    C_EXTENSIONS YES
)
target_include_directories(${PROJECT_NAME}
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src
)
target_link_libraries(${PROJECT_NAME}
	PUBLIC rt
)

file(GLOB tests tests/*.c tests/*.cpp)
foreach(testf IN LISTS tests)
	get_filename_component(test ${testf} NAME_WE)
	if(${test} STREQUAL "macroprint")
		continue()
	endif()
	
	add_executable(${PROJECT_NAME}_${test} ${testf})
	add_test(NAME ${PROJECT_NAME}_${test}
		COMMAND ${PROJECT_NAME}_${test})
	target_link_libraries(${PROJECT_NAME}_${test} ${PROJECT_NAME})
endforeach()

add_test(NAME ${PROJECT_NAME}_macroprint
	COMMAND sh -x -c 
		"\"$1\" -I\"$2\"/src -DTEST \"$2\"/tests/macroprint.c 2>&1"
		--
		${CMAKE_C_COMPILER}
		${CMAKE_CURRENT_SOURCE_DIR}
)
set_tests_properties(${PROJECT_NAME}_macroprint PROPERTIES
	PASS_REGULAR_EXPRESSION "A\\\(\\\)=42"
)

