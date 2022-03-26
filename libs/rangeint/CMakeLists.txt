cmake_minimum_required(VERSION 3.11)
project(rangeint LANGUAGES C)
include(CTest)
include(${CMAKE_CURRENT_SOURCE_DIR}/../../kcmakelib/k/jinja2.cmake)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/src)
jinja2_render(
  INPUT ${CMAKE_CURRENT_SOURCE_DIR}/rangeint.in.h
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/src/rangeint.h
  trim_blocks True
  lstrip_blocks True
)
add_library(rangeint INTERFACE)
target_sources(rangeint INTERFACE ${CMAKE_CURRENT_BINARY_DIR}/src/rangeint.h)
target_include_directories(rangeint INTERFACE ${CMAKE_CURRENT_BINARY_DIR}/src)

if(NOT BUILD_TESTING)
  return()
endif()

file(GLOB tests tests/*.c)
foreach(ii IN LISTS tests)
  get_filename_component(name ${ii} NAME_WE)
  set(name "rangeint_${name}")
  add_executable(${name} ${ii})
  target_link_libraries(${name} PRIVATE rangeint)
  add_test(NAME ${name} COMMAND ${name})
endforeach()

