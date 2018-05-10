
function(cubemx_make_get_variable result_var variable cubemx_dir)
	execute_process(
		COMMAND /bin/bash -c "make -s -f Makefile -f <(echo -e 'printvar-%: ; $(info $($*))') printvar-${variable}"
		WORKING_DIRECTORY "${cubemx_dir}"
		RESULT_VARIABLE ret
		OUTPUT_VARIABLE temp
	)
	if(ret)
		message(FATAL_ERROR "cubemx_make_get_variable make returned ${ret}")
	endif()
	set(${result_var} "${temp}" PARENT_SCOPE)
endfunction()

function(cubemx_get_asm_sources result_var cubemx_dir)
	file(GLOB temp ${cubemx_dir}/*.s)
	set(${result_var} "${temp}" PARENT_SCOPE)
endfunction()

function(cubemx_get_c_sources result_var cubemx_dir halrepo_dir)
	cubemx_make_get_variable(mx_c_src C_SOURCES "${cubemx_dir}")
	string(REPLACE " " ";" mx_c_src "${mx_c_src}")
	set(tmp "")
	foreach(i ${mx_c_src})
		if (${i} MATCHES ".*/STM32Cube_FW_*")
			string(REGEX REPLACE ".*/STM32Cube_FW_[^/]+/" "${halrepo_dir}/" i "${i}")
		else()
			set(i "${cubemx_dir}/${i}")
		endif()
		string(REGEX REPLACE "\n$" "" i "${i}")
		set(i "${CMAKE_SOURCE_DIR}/${i}")
		if (NOT EXISTS ${i}) 
			MESSAGE(FATAL_ERROR "${i} does not exists")
  		endif()
		list(APPEND tmp ${i})
	endforeach()
	set(${result_var} "${tmp}" PARENT_SCOPE)
endfunction()

function(cubemx_get_c_includes result_var cubemx_dir halrepo_dir)
	cubemx_make_get_variable(mx_c_inc C_INCLUDES "${cubemx_dir}")
	string(REPLACE "-I" "" mx_c_inc "${mx_c_inc}")
	string(REPLACE " " ";" mx_c_inc "${mx_c_inc}")
	list(REVERSE mx_c_inc)
	set(tmp "")
	foreach(i ${mx_c_inc})
		if (${i} MATCHES ".*/STM32Cube_FW_*")
			string(REGEX REPLACE ".*/STM32Cube_FW_[^/]+" "${halrepo_dir}/" i "${i}")
			string(REGEX REPLACE "\n$" "" i "${i}")
			set(i "${CMAKE_SOURCE_DIR}/${i}")
			if (NOT EXISTS ${i}) 
				MESSAGE(FATAL_ERROR ${i} does not exists)
  			endif()
  			list(APPEND tmp "${i}")
		endif()
	endforeach()
	set(${result_var} "${tmp}" PARENT_SCOPE)
endfunction()

function(cubemx_get_linker_script result_var cubemx_dir)
	cubemx_make_get_variable(temp LDSCRIPT "${cubemx_dir}")
	string(REGEX REPLACE "\n$" "" temp "${temp}")
	set(${result_var} "${CMAKE_SOURCE_DIR}/${cubemx_dir}/${temp}" PARENT_SCOPE)
endfunction()

function(cubemx_add_generate_mx_h_target target result_file cubemx_dir)
	file(GLOB mx_headers "${cubemx_dir}/Inc/*.h")
	get_filename_component(result_file_dir "${result_file}" DIRECTORY)
	file(GLOB mx_headers_relative RELATIVE "${result_file_dir}" "${cubemx_dir}/Inc/*.h")
	add_custom_command(
		OUTPUT "${result_file}"
		COMMAND echo "/* this file is autogonerated by ${CMAKE_CURRENT_LIST_FILE} cmake script */" > "${result_file}" 
		COMMAND printf "#include \"%s\"\\n" ${mx_headers_relative} >> "${result_file}"
		DEPENDS ${mx_headers} 
		COMMENT "Generate ${result_file} from cubemx headers" VERBATIM 
	)
	add_custom_target(${target} DEPENDS "${result_file}")
endfunction()

function(cubemx_add_hal_library target type c_inc_output_var cubemx_dir halrepo_dir)
	cubemx_get_asm_sources(mx_asm_src ${cubemx_dir})
	cubemx_get_c_sources(mx_c_src ${cubemx_dir} ${halrepo_dir})
	cubemx_get_c_includes(mx_c_inc ${cubemx_dir} ${halrepo_dir})
	list(APPEND mx_c_inc "${cubemx_dir}/Inc")
	add_library(hal ${type} ${mx_asm_src} ${mx_c_src})
	target_include_directories(${target} PUBLIC ${mx_c_inc})
	set(${c_inc_output_var} "${mx_c_inc}" PARENT_SCOPE)
endfunction()
