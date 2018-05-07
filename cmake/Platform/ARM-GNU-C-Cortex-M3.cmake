message(STATUS "Configuring for ARM platform, GNU-C compiler, Cortex-M0+ processor")
string(APPEND PROCESSOR_FLAGS " -mcpu=cortex-m0plus -mthumb")
