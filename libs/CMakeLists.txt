include(k/subdirlist)

subdirlist(dirs ${CMAKE_CURRENT_SOURCE_DIR})
list(FILTER dirs EXCLUDE REGEX "AAA_template")
list(FILTER dirs EXCLUDE REGEX "arduino")

set(all "")
foreach(dir IN LISTS dirs)
	add_subdirectory(${dir})
	list(APPEND all kamcuk_${dir})
endforeach()

add_library(kamcuk_all INTERFACE)
target_link_libraries(kamcuk_all INTERFACE ${all})

