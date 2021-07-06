
#ExternalProject_Add(lua
#GIT_REPOSITORY https://github.com/lua/lua
#BUILD_COMMAND make CC=${CMAKE_C_COMPILER} CFLAGS=${CMAKE_C_FLAGS} MYLDFLAGS=${CMAKE_C_LINKER_FLAGS} LIBS= all

include(FetchContent)
FetchContent_Declare(lua
	GIT_REPOSITORY https://github.com/lua/lua
)
FetchContent_MakeAvailable(lua)
function(exesafe)
	cmake_parse_arguments(E "" "RESULT" "COMMAND" ${ARGN})
	execute_process(
		COMMAND ${E_COMMAND}
		OUTPUT_VARIABLE "${E_RESULT}"
		RESULT_VARIABLE err
	)
	if(err)
		message(FATAL_ERROR "Could not get lua sources: ${err}")
	endif()
	set("${E_RESULT}" "${${E_RESULT}}" PARENT_SCOPE)
endfunction()
file(WRITE ${CMAKE_BINARY_DIR}/lua.make "
__print_%:
	@echo '$($*)'
")
function(makegetsrc var resultvar)
	exesafe(
		COMMAND make -C ${lua_SOURCE_DIR} --no-print-directory -f makefile -f ${CMAKE_BINARY_DIR}/lua.make "__print_${var}"
		RESULT tmp
	)
	string(REGEX REPLACE "\\.o" ".c" tmp "${tmp}")
	string(REGEX REPLACE "[ \t\n]\+" ";" tmp "${tmp}")
	set(tmp2)
	foreach(i IN LISTS tmp)
		if(i)
			list(APPEND tmp2 "${lua_SOURCE_DIR}/${i}")
		endif()
	endforeach()
	# message(STATUS "AAAAAAAA ${tmp2}")
	set("${resultvar}" "${tmp2}" PARENT_SCOPE)
endfunction()
makegetsrc(CORE_O coresources)
add_library(lua_core EXCLUDE_FROM_ALL
  ${coresources} ${lua_SOURCE_DIR}/lua.c ${lua_SOURCE_DIR}/lauxlib.c)
target_include_directories(lua_core PUBLIC ${lua_SOURCE_DIR})
makegetsrc(LIB_O libsources)
add_library(lua_lib EXCLUDE_FROM_ALL ${libsources})
target_include_directories(lua_lib PUBLIC ${lua_SOURCE_DIR})
target_link_libraries(lua_lib PUBLIC lua_core)

