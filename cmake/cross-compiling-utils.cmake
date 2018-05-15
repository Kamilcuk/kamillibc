
function(add_bin_target target)
	if(NOT CMAKE_OBJCOPY)
		message(FATAL_ERROR "CMAKE_OBJCOPY not defined")
	endif()
    add_custom_target("${target}.bin" ALL
		COMMAND "${CMAKE_OBJCOPY}" -O binary "${target}${CMAKE_EXECUTABLE_SUFFIX}" "${target}.bin"
    	DEPENDS "${target}"
        COMMENT "Generating ${target}.bin")
endfunction()

function(add_hex_target target)
	if(NOT CMAKE_OBJCOPY)
		message(FATAL_ERROR "CMAKE_OBJCOPY not defined")
	endif()
    add_custom_target("${target}.hex" ALL
		COMMAND "${CMAKE_OBJCOPY}" -O ihex "${target}${CMAKE_EXECUTABLE_SUFFIX}" "${target}.hex"
    	DEPENDS "${target}"
        COMMENT "Generating ${target}.hex")
endfunction()

function(print_executable_size target)
	if(NOT CMAKE_SIZE_UTIL)
		message(FATAL_ERROR "CMAKE_SIZE_UTIL not defined")
	endif()
    add_custom_command(TARGET "${target}" POST_BUILD
        COMMAND "${CMAKE_SIZE_UTIL}" -B "${CMAKE_BINARY_DIR}/${target}${CMAKE_EXECUTABLE_SUFFIX}"
        COMMENT "Priting ${target}${CMAKE_EXECUTABLE_SUFFIX} size information")
endfunction()
