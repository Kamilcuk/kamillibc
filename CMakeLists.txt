cmake_minimum_required (VERSION 3.7)
project(kamil C)

# default compile options
SET(CMAKE_AR "gcc-ar")
SET(CMAKE_C_ARCHIVE_CREATE "<CMAKE_AR> qcs <TARGET> <LINK_FLAGS> <OBJECTS>")
SET(CMAKE_C_ARCHIVE_FINISH   true)
set(CMAKE_C_FLAGS          "-std=gnu11 -O3 -Wall -fPIC -flto")

# get sources
include_directories(src/linux src/lib)
file(GLOB_RECURSE src FOLLOW_SYMLINKS src/lib/*.c src/linux/*.c)
file(GLOB_RECURSE testsrc FOLLOW_SYMLINKS test/*.c)

# generate objects
add_library(${PROJECT_NAME}_object OBJECT ${src})
set(objects $<TARGET_OBJECTS:${PROJECT_NAME}_object>)
# generate static library
add_library(${PROJECT_NAME}_static STATIC ${objects})
# generate shared library
add_library(${PROJECT_NAME}_shared SHARED ${objects})
# generate test executable and run tests
add_executable(${PROJECT_NAME}_test ${testsrc} ${objects})
enable_testing()
set(CTEST_OUTPUT_ON_FAILURE true)
add_test(${PROJECT_NAME}_test ${PROJECT_NAME}_test)

