cmake_minimum_required(VERSION 3.11)
get_filename_component(name ${CMAKE_CURRENT_SOURCE_DIR} NAME)
project(kamcuk_${name})

include(CheckSymbolExists)
set(CMAKE_REQUIRED_DEFINITIONS -D_GNU_SOURCE=1)
check_symbol_exists(asprintf "stdio.h" HAVE_ASPRINTF)
if(HAVE_ASPRINTF)
  set_source_files_properties(src/kasprintf.c
    PROPERTIES COMPILE_DEFINITIONS HAVE_ASPRINTF=1
  )
endif()

file(GLOB_RECURSE srcs src/*.c src/*.cpp)
add_library(${PROJECT_NAME} ${srcs})
set_target_properties(${PROJECT_NAME} PROPERTIES
  C_STANDARD 11
  C_STANDARD_REQUIRED YES
  C_EXTENSIONS YES
)
target_include_directories(${PROJECT_NAME}
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src
)
if(NOT TARGET kamcuk_curb)
  add_subdirectory(../curb curb)
endif()
target_link_libraries(${PROJECT_NAME}
  PUBLIC kamcuk_curb rt
)

include(CTest)

# k_grep(<resultvar>
#        [FILENAMES]
#        [REGEX <regex>]
#        [ GLOB | GLOB_RECURSE ] [<globbing-expr>...]
# )
function(k_grep resultvar)
  cmake_parse_arguments(a
    "FILENAMES;GLOB;GLOB_RECURSE"
    "REGEX"
    ""
    ${ARGN})
  #
  #
  if(NOT a_REGEX)
    message(FATAL_ERROR "k_grep: REGEX has to be given")
  endif()
  set(regex ${a_REGEX})
  #
  if(a_GLOB_RECURSE)
    file(GLOB_RECURSE files ${a_UNPARSED_ARGUMENTS})
  else()
    file(GLOB files ${a_UNPARSED_ARGUMENTS})
  endif()
  #
  set(result)
  foreach(ii IN LISTS files)
    file(STRINGS ${ii} matched REGEX ${regex})
    if(matched)
      if(a_FILENAMES)
        list(APPEND result "${ii}")
      else()
        list(APPEND result "${matched}")
      endif()
    endif()
  endforeach()
  #
  set(${resultvar} ${result} PARENT_SCOPE)
endfunction()

k_grep(tests FILENAMES REGEX "KAMILLIBC_TEST" GLOB_RECURSE ./src/*.c)
foreach(testf IN LISTS tests)
  get_filename_component(test ${testf} NAME_WE)
  add_executable(${PROJECT_NAME}_${test} ${testf})
  add_test(NAME ${PROJECT_NAME}_${test} COMMAND ${PROJECT_NAME}_${test})
  target_compile_definitions(${PROJECT_NAME}_${test} PRIVATE KAMILLIBC_TEST=1)
endforeach()

file(GLOB tests tests/*.c tests/*.cpp)
list(FILTER tests EXCLUDE REGEX macroprint)
foreach(testf IN LISTS tests)
  get_filename_component(test ${testf} NAME_WE)
  add_executable(${PROJECT_NAME}_${test} ${testf})
  add_test(NAME ${PROJECT_NAME}_${test} COMMAND ${PROJECT_NAME}_${test})
  target_link_libraries(${PROJECT_NAME}_${test} PRIVATE ${PROJECT_NAME})
endforeach()

if(0)
  add_test(NAME ${PROJECT_NAME}_macroprint
    COMMAND
      ${CMAKE_C_COMPILER}
      -I${CMAKE_CURRENT_SOURCE_DIR}/src
      ${CMAKE_CURRENT_SOURCE_DIR}/tests/macroprint.c
  )
  set_tests_properties(${PROJECT_NAME}_macroprint PROPERTIES
    PASS_REGULAR_EXPRESSION "A\\\(\\\)=42"
  )
endif()

