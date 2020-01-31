get_filename_component(name ${CMAKE_CURRENT_SOURCE_DIR} NAME)
project(kamcuk_${name})

file(GLOB_RECURSE srcs src/*.c src/*.cpp)
add_library(${PROJECT_NAME} ${srcs})
target_include_directories(${PROJECT_NAME}
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src
)
target_link_libraries(${PROJECT_NAME}
	PUBLIC
)

file(GLOB tests tests/*.c tests/*.cpp)
foreach(testf IN LISTS tests)
	get_filename_component(test ${testf} NAME_WE)
	add_executable(${PROJECT_NAME}_${test} ${testf})
	add_test(NAME ${PROJECT_NAME}_${test}
		COMMAND ${PROJECT_NAME}_${test})
	target_link_libraries(${PROJECT_NAME}_${test} ${PROJECT_NAME})
endforeach()
