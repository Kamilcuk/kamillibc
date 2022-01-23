cmake_minimum_required(VERSION 3.11)
project(kamcuk C)
add_subdirectory(./kcmakelib)

if(CMAKE_C_COMPILER_ID STREQUAL "GNU")
  add_compile_options(
    $<$<CONFIG:DEBUG>:-O0>
    $<$<CONFIG:DEBUG>:-ggdb3>
    -Wall
    -Wextra
    -Wno-unused-parameter
    -Wno-missing-field-initializers
    -Wno-type-limits
  )
endif()

add_subdirectory(./third_party/ckd)
include(CTest)
add_subdirectory(./libs)
