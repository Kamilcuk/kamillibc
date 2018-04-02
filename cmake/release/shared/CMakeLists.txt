set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fpic")
add_library(${PROJECT_NAME}_shared SHARED ${src})
