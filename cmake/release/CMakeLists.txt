
set(PROJECT_NAME "${PROJECT_NAME}_release")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DNDEBUG")
include(../default/CMakeLists.txt)
