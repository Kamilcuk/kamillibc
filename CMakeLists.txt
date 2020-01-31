cmake_minimum_required(VERSION 3.11)
project(kamcuk C)

if(CMAKE_COMPILER_IS_GNUC)
add_compile_options(
	-ggdb3
	-Wall
	-Wextra
	-Wno-unused-parameter
	-Wno-missing-field-initializers
)
endif()

enable_testing()
add_subdirectory(libs)