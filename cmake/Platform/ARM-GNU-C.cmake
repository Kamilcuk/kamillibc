message(STATUS "Configuring for ARM platform, GNU-C compiler")

set(CMAKE_EXECUTABLE_SUFFIX ".elf")

if (NOT PROCESSOR_FLAGS)
	MESSAGE(FATAL_ERROR "PROCESSOR_FLASG is not set")
endif()

string(APPEND COMPILER_FLAGS " ${PROCESSOR_FLAGS} -ffunction-sections -fdata-sections -fomit-frame-pointer")
string(APPEND CMAKE_ASM_FLAGS_INIT           " ${COMPILER_FLAGS} -x assembler-with-cpp ")
string(APPEND CMAKE_C_FLAGS_INIT             " ${COMPILER_FLAGS} ")
string(APPEND CMAKE_CXX_FLAGS_INIT           " ${COMPILER_FLAGS} ")
string(APPEND CMAKE_EXE_LINKER_FLAGS_INIT    " -Wl,--gc-sections ")
string(APPEND CMAKE_SHARED_LINKER_FLAGS_INIT " -Wl,--gc-sections ")
string(APPEND CMAKE_MODULE_LINKER_FLAGS_INIT " -Wl,--gc-sections ")
