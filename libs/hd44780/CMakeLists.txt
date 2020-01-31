get_filename_component(name ${CMAKE_CURRENT_SOURCE_DIR} NAME)
project(kamcuk_${name})
file(GLOB srcs *.c)
add_library(${PROJECT_NAME} ${srcs})
target_include_directories(${PROJECT_NAME}
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
