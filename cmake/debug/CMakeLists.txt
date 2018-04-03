
set(PROJECT_NAME "${PROJECT_NAME}_debug")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DDEBUG -g3")
include(../default/CMakeLists.txt)
