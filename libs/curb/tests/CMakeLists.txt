
if (CMAKE_COMPILER_IS_GNUCC)
  add_compile_options(
    -Wno-unused-value
    -O
  )
endif()

file(GLOB tests runtime/*.c)
foreach(ff IN LISTS tests)
	get_filename_component(test ${ff} NAME_WE)
	set(nn ${PROJECT_NAME}_ok_${test})
  #
	add_executable(${nn} ${ff})
  target_link_libraries(${nn} PUBLIC ${PROJECT_NAME})
  #
	add_test(NAME ${nn} COMMAND ${nn})
  if(${ff} MATCHES "fail")
    set_tests_properties(${nn} PROPERTIES WILL_FAIL TRUE)
  endif()
endforeach()

file(GLOB tests build/*.c)
foreach(ff IN LISTS tests)
	get_filename_component(test ${ff} NAME_WE)
	set(nn ${PROJECT_NAME}_builderror_${test})
  #
  add_executable(${nn} EXCLUDE_FROM_ALL ${ff})
	target_link_libraries(${nn} PUBLIC ${PROJECT_NAME})
  #
  set(args
    COMMAND sh -c
      "$1 -E touch_nocreate $2 && $1 --build $3 --target $4 2>&1"
      _ ${CMAKE_COMMAND} ${ff} ${CMAKE_BINARY_DIR} ${nn}
     WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  )
  add_test(NAME ${nn} ${args})
  add_test(NAME ${nn}_regex ${args})
  set_tests_properties(${nn} ${nn}_regex PROPERTIES
    RUN_SERIAL TRUE
    LABELS noMemcheck
  )
  if(${ff} MATCHES "fail")
    set_tests_properties(${nn} PROPERTIES WILL_FAIL TRUE)
    set_tests_properties(${nn}_regex PROPERTIES PASS_REGULAR_EXPRESSION
      "error: .* will fail"
    )
  elseif(${ff} MATCHES "warn")
    set_tests_properties(${nn}_regex PROPERTIES PASS_REGULAR_EXPRESSION
      "warning: .* will fail"
    )
  else()
    set_tests_properties(${nn}_regex PROPERTIES FAIL_REGULAR_EXPRESSION
       "curb .* will fail"
    )
  endif()
endforeach()

