
add_library(${PROJECT_NAME}_static STATIC ${src})

add_library(${PROJECT_NAME}_shared SHARED ${src})
target_link_libraries(${PROJECT_NAME}_shared ${PROJECT_NAME}_static)

add_executable(${PROJECT_NAME}_test ${testsrc})
target_link_libraries(${PROJECT_NAME}_test ${PROJECT_NAME}_static)
