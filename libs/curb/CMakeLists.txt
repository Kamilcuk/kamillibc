get_filename_component(name ${CMAKE_CURRENT_SOURCE_DIR} NAME)
project(kamcuk_${name})

file(GLOB_RECURSE srcs src/*.c)
add_library(${PROJECT_NAME} ${srcs})
set_target_properties(${PROJECT_NAME} PROPERTIES C_EXTENSIONS YES)
target_include_directories(${PROJECT_NAME}
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src
)

if(BUILD_TESTING)

add_subdirectory(tests)

endif(BUILD_TESTING)
