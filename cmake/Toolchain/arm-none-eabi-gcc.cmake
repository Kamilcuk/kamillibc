message(STATUS "Configuring for arm-none-eabi-gcc toolchain")

set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")
set(_CMAKE_TOOLCHAIN_PREFIX "arm-none-eabi")
find_program(CMAKE_C_COMPILER ${_CMAKE_TOOLCHAIN_PREFIX}-gcc)
find_program(CMAKE_CXX_COMPILER ${_CMAKE_TOOLCHAIN_PREFIX}-g++)
find_program(CMAKE_AR ${_CMAKE_TOOLCHAIN_PREFIX}-ar)
find_program(CMAKE_OBJCOPY ${_CMAKE_TOOLCHAIN_PREFIX}-objcopy)
