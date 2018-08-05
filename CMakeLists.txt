cmake_minimum_required (VERSION 3.7)

if(${CMAKE_SYSTEM} MATCHES "ARM")
	string(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
	include(Toolchain/arm-none-eabi-gcc.cmake)
endif()

project(kcuk C)

enable_testing()

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
