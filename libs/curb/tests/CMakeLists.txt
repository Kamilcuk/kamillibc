
# runtime test fail with exit so they don't abort
# abort is hard to handle for ctest aparently
set(n ${PROJECT_NAME}_exit)
add_library(${n} ${srcs})
set_target_properties(${n} PROPERTIES C_EXTENSIONS YES)
target_include_directories(${n} PUBLIC
	$<TARGET_PROPERTY:${PROJECT_NAME},INTERFACE_INCLUDE_DIRECTORIES>
)
target_compile_definitions(${n} PUBLIC
	CURB_DEFAULT_HANDLER=curb_handler_exit
)

file(GLOB tests *.c)
foreach(testf IN LISTS tests)
	get_filename_component(test ${testf} NAME_WE)
	
	set(n ${PROJECT_NAME}_runtime_${test})
	add_executable(${n} ${testf})
	target_compile_options(${n} PUBLIC $<${CMAKE_COMPILER_IS_GNUCC}:-O0>)
	add_test(NAME ${n} COMMAND ${n}) 
	if(${testf} MATCHES "fail" OR ${testf} MATCHES "warn")
		target_link_libraries(${n} PUBLIC ${PROJECT_NAME}_exit)
		set_tests_properties(${n} PROPERTIES WILL_FAIL TRUE)
	else()
		target_link_libraries(${n} PUBLIC ${PROJECT_NAME})
	endif()
	
	set(n ${PROJECT_NAME}_build_${test})
    add_executable(${n} EXCLUDE_FROM_ALL ${testf})
    target_compile_options(${n} PUBLIC $<${CMAKE_COMPILER_IS_GNUCC}:-O>)
	target_link_libraries(${n} PUBLIC ${PROJECT_NAME})
    add_test(NAME ${n}
        COMMAND ${CMAKE_COMMAND} --build . --target ${n} --config $<CONFIGURATION>
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}) 
	if(${testf} MATCHES "fail")
		set_tests_properties(${n} PROPERTIES WILL_FAIL TRUE)
	elseif(${testf} MATCHES "warn")
	endif()

endforeach()

#set_tests_properties(${PROJECT_NAME}_build_curb_1 PROPERTIES PASS_REGULAR_EXPRESSION
#	"error: curb(a >= 0) will fail")
#set_tests_properties(${PROJECT_NAME}_build_curb_warn PROPERTIES PASS_REGULAR_EXPRESSION
#	"warning: curb(a == 2) will fail")
