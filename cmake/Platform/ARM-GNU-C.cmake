message(STATUS "Configuring for ARM platform, GNU-C compiler")

if(NOT ARM_PROCESSOR_FLAGS)
	message(FATAL_ERROR "ARM_PROCESSOR_FLAGS is not set")
endif()

string(APPEND ARM_FLAGS " ${ARM_PROCESSOR_FLAGS} -ffunction-sections -fdata-sections")
string(APPEND CMAKE_ASM_FLAGS_INIT           " ${ARM_FLAGS} ")
string(APPEND CMAKE_C_FLAGS_INIT             " ${ARM_FLAGS} ")
string(APPEND CMAKE_CXX_FLAGS_INIT           " ${ARM_FLAGS} ")
string(APPEND CMAKE_EXE_LINKER_FLAGS_INIT    " -Wl,--gc-sections ")
string(APPEND CMAKE_SHARED_LINKER_FLAGS_INIT " -Wl,--gc-sections ")
string(APPEND CMAKE_MODULE_LINKER_FLAGS_INIT " -Wl,--gc-sections ")
