message(STATUS "Configuring for ARM platform, GNU-C compiler, Cortex-M3 processor")
string(APPEND PROCESSOR_FLAGS " -mcpu=cortex-m3 -mthumb")
