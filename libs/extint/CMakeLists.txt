cmake_minimum_required(VERSION 3.11)
project(extint LANGUAGES C)
include(CTest)
include(${CMAKE_CURRENT_SOURCE_DIR}/../../kcmakelib/k/jinja2.cmake)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/src)
jinja2_render(
  INPUT ${CMAKE_CURRENT_SOURCE_DIR}/extint.in.h
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/src/extint.h
  trim_blocks True
  lstrip_blocks True
)
add_library(extint INTERFACE)
target_sources(extint INTERFACE ${CMAKE_CURRENT_BINARY_DIR}/src/extint.h)
target_include_directories(extint INTERFACE ${CMAKE_CURRENT_BINARY_DIR}/src)

if(NOT BUILD_TESTING)
  return()
endif()

file(GLOB tests tests/*.c)
foreach(ii IN LISTS tests)
  get_filename_component(name ${ii} NAME_WE)
  set(name "extint_${name}")
  add_executable(${name} ${ii})
  target_link_libraries(${name} PRIVATE extint)
  add_test(NAME ${name} COMMAND ${name})
endforeach()

