get_filename_component(name ${CMAKE_CURRENT_SOURCE_DIR} NAME)
project(kamcuk_${name})

file(GLOB_RECURSE srcs src/*.c src/*.cpp)
add_library(${PROJECT_NAME} ${srcs})
set_target_properties(${PROJECT_NAME} PROPERTIES
    C_STANDARD 99
    C_STANDARD_REQUIRED NO
    C_EXTENSIONS NO
)
target_include_directories(${PROJECT_NAME}
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src
)
target_link_libraries(${PROJECT_NAME}
	PUBLIC
)

set(tmp ${PROJECT_NAME}_example)
add_executable(${tmp} tests/example.c)
target_link_libraries(${tmp} ${PROJECT_NAME})
add_test(NAME ${tmp} COMMAND ${tmp})
set_tests_properties(${tmp} PROPERTIES PASS_REGULAR_EXPRESSION "Hello world!")
