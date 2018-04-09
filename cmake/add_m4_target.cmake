
function(add_m4_target targetname sourceslist)
	set(m4src ${sourceslist})
	add_custom_target(${targetname})
	foreach(m4 ${m4src})
		string(REGEX REPLACE "\\.m4$" "" m4noext ${m4src})
		add_custom_command(
			OUTPUT ${m4noext}
			COMMAND m4 -P -s ${m4} > ${m4noext}
			DEPENDS ${m4}
		)
		add_dependencies(${targetname} ${m4noext})
	endforeach()
endfunction()

