include(${CMAKE_CURRENT_SOURCE_DIR}/../cmake/subdirlist.cmake)
subdirlist(dirs ${CMAKE_CURRENT_SOURCE_DIR})

foreach(dir IN LISTS dirs)
	add_subdirectory(${dir})
endforeach()

set(all "")
foreach(dir IN LISTS dirs)
	list(APPEND all kamcuk_${dir})
endforeach()
list(FILTER all EXCLUDE REGEX "AAA_template")

add_library(kamcuk_all INTERFACE)
target_link_libraries(kamcuk_all INTERFACE ${all})
