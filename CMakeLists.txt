cmake_minimum_required(VERSION 3.10)

string(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

if(${CMAKE_SYSTEM} MATCHES "ARM")
	include(Toolchain/arm-none-eabi-gcc.cmake)
endif()

project(kcuk C)

string(APPEND CMAKE_C_FLAGS " -Wall -pedantic")

enable_testing()

include(cppcheck)

if(${CMAKE_SYSTEM} MATCHES "Linux")
	link_libraries(rt pthread)
	add_subdirectory(src/linux)
	add_subdirectory(tests/linux)
endif() 

add_subdirectory(src/lib)
add_subdirectory(tests/lib)

set(kcuk_src $<TARGET_OBJECTS:kcuk_lib_objects>)
if(${CMAKE_SYSTEM} MATCHES "Linux")
	string(APPEND kcuk_src ";$<TARGET_OBJECTS:kcuk_linux_objects>")
endif()
add_library(kcuk_static STATIC ${kcuk_src})
