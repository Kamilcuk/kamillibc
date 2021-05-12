cmake_minimum_required(VERSION 3.11)
project(STC LANGUAGES C)

include(FetchContent)
FetchContent_Declare(
	stcrepo
	GIT_REPOSITORY "https://github.com/tylov/STC"
)
FetchContent_GetProperties(stcrepo)
if(NOT stcrepo_POPULATED)
  FetchContent_Populate(stcrepo)
endif()

add_library(STC INTERFACE)
target_include_directories(STC INTERFACE ${stcrepo_SOURCE_DIR})

include(CTest)
if(BUILD_TESTING)
	add_executable(example_from_docs tests/example_from_docs.c)
	target_link_libraries(example_from_docs PUBLIC STC)
	add_test(NAME example_from_docs COMMAND example_from_docs)
endif()

