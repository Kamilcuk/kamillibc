set(PROJECT_NAME "${PROJECT_NAME}")
add_subdirectories(GLOB ./*)
add_executable(${PROJECT_NAME}_test ${testsrc})
target_link_libraries(${PROJECT_NAME}_test ${PROJECT_NAME}_static)
