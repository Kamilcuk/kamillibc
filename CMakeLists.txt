cmake_minimum_required (VERSION 3.4)

project(kamil ASM C)

set(COMMON_FLAGS    "-Wall -O3 -std=gnu11 -flto")
set(CMAKE_ASM_FLAGS "${COMMON_FLAGS}")
set(CMAKE_C_FLAGS   "${COMMON_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${COMMON_FLAGS}")

set(COMMON_FLAGS_DEBUG             "-DDEBUG -g")
set(CMAKE_ASM_FLAGS_DEBUG          "${COMMON_FLAGS_DEBUG}")
set(CMAKE_C_FLAGS_DEBUG            "${COMMON_FLAGS_DEBUG}")
set(CMAKE_EXE_LINKER_FLAGS_DEBUG   "${COMMON_FLAGS_DEBUG}")
set(COMMON_FLAGS_RELEASE           "-DNDEBUG ")
set(CMAKE_ASM_FLAGS_RELEASE        "${COMMON_FLAGS_RELEASE}")
set(CMAKE_C_FLAGS_RELEASE          "${COMMON_FLAGS_RELEASE}")
set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${COMMON_FLAGS_RELEASE}")

file(GLOB_RECURSE m4src FOLLOW_SYMLINKS src/*.m4)
foreach(m4 ${m4src})
	string(REGEX REPLACE "\\.[^.]*$" "" m4noext ${m4src})
	add_custom_command(
		OUTPUT ${m4}
		COMMAND m4 -P -s ${m4} > ${m4noext}
		VERBATIM
	)
endforeach()

add_custom_command(
	OUTPUT third_party/Unity
	COMMAND git submodule update --init --recursive third_party/Unity
	VERBATIM
)

include_directories(src/lib)
file(GLOB_RECURSE csrc FOLLOW_SYMLINKS src/lib/*.c src/linux/*.c)
add_library(${PROJECT_NAME} SHARED ${csrc})
add_library(${PROJECT_NAME}_static STATIC ${csrc})

file(GLOB_RECURSE ctest FOLLOW_SYMLINKS test/*.c)
add_executable(${PROJECT_NAME}test ${ctest} ${csrc})
target_link_libraries(${PROJECT_NAME}test rt)

enable_testing()
add_test(${PROJECT_NAME}test /bin/sh -c "tty=$(tty); ./${PROJECT_NAME}test 2>&1 | tee $tty")
