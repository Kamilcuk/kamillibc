cmake_minimum_required(VERSION 3.11)
project(stc LANGUAGES C)

set(stcdir "${CMAKE_CURRENT_BINARY_DIR}/stcdir")
file(MAKE_DIRECTORY "${stcdir}")

include(ExternalProject)
ExternalProject_Add(stcrepo
	GIT_REPOSITORY "https://github.com/tylov/STC"
	SOURCE_DIR "${stcdir}"
	CONFIGURE_COMMAND ""
	BUILD_COMMAND ""
	INSTALL_COMMAND ""
)

add_library(stc INTERFACE)
add_dependencies(stc stcrepo)
target_include_directories(stc INTERFACE
	"${stcdir}/include"
)


include(CTest)
if(BUILD_TESTING)
	file(GLOB tests tests/*.c)
	foreach(file IN LISTS tests)
		get_filename_component(name ${file} NAME_WE)
		add_executable(${name} ${file})
		target_link_libraries(${name} PUBLIC stc)
		add_test(NAME ${name} COMMAND ${name})
	endforeach()
endif()

