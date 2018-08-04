cmake_minimum_required (VERSION 3.7)

project(kcuk C)

enable_testing()

if(${CMAKE_SYSTEM} MATCHES "Linux")
	link_libraries(rt pthread)
endif() 

add_subdirectory(src/lib)
add_subdirectory(tests/lib)

if(${CMAKE_SYSTEM} MATCHES "Linux")
	add_subdirectory(src/linux)
	add_subdirectory(tests/linux)
endif()

set(kcuk_src $<TARGET_OBJECTS:kcuk_lib_objects>)
if(${CMAKE_SYSTEM} MATCHES "Linux")
	string(APPEND kcuk_src ";$<TARGET_OBJECTS:kcuk_linux_objects>")
endif()
add_library(kcuk_static STATIC ${kcuk_src})
