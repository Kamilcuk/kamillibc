cmake_minimum_required(VERSION 3.11)
get_filename_component(name ${CMAKE_CURRENT_SOURCE_DIR} NAME)
project(kamcuk_${name} LANGUAGES C)

file(GLOB_RECURSE srcs src/*.c)
add_library(${PROJECT_NAME} ${srcs})
set_target_properties(${PROJECT_NAME} PROPERTIES
  C_EXTENSIONS YES
)
target_include_directories(${PROJECT_NAME}
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src
)

include(CheckSymbolExists)
set(CMAKE_REQUIRED_DEFINITIONS -D_GNU_SOURCE=1)
check_symbol_exists(program_invocation_short_name errno.h HAS_PROGRAM_INVOCATION_SHORT_NAME)
if(HAS_PROGRAM_INVOCATION_SHORT_NAME)
  target_compile_definitions(${PROJECT_NAME} PRIVATE
    HAS_PROGRAM_INVOCATION_SHORT_NAME=1
  )
endif()

include(CTest)
if(BUILD_TESTING)
  add_subdirectory(tests)
endif(BUILD_TESTING)

