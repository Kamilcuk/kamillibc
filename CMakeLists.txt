cmake_minimum_required (VERSION 3.4)

project(kamil ASM C)

set(COMMON_FLAGS    "-Wall -O3 -std=gnu11")
set(CMAKE_ASM_FLAGS "${COMMON_FLAGS}")
set(CMAKE_C_FLAGS   "${COMMON_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${COMMON_FLAGS}")

set(COMMON_FLAGS_DEBUG             "-DDEBUG -g3")
set(CMAKE_ASM_FLAGS_DEBUG          "${COMMON_FLAGS_DEBUG}")
set(CMAKE_C_FLAGS_DEBUG            "${COMMON_FLAGS_DEBUG}")
set(CMAKE_EXE_LINKER_FLAGS_DEBUG   "${COMMON_FLAGS_DEBUG}")
set(COMMON_FLAGS_RELEASE           "-DNDEBUG -flto")
set(CMAKE_ASM_FLAGS_RELEASE        "${COMMON_FLAGS_RELEASE}")
set(CMAKE_C_FLAGS_RELEASE          "${COMMON_FLAGS_RELEASE}")
set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${COMMON_FLAGS_RELEASE}")

include_directories(src)
file(GLOB_RECURSE csrc FOLLOW_SYMLINKS src/*.c)
add_library(${PROJECT_NAME} SHARED ${csrc})

file(GLOB_RECURSE ctest FOLLOW_SYMLINKS test/*.c)
add_executable(${PROJECT_NAME}test ${ctest})
target_link_libraries(${PROJECT_NAME}test ${PROJECT_NAME})
