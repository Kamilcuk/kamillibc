message(STATUS "Configuring for ARM platform, GNU-C compiler, Cortex-M4 processor")
string(APPEND ARM_PROCESSOR_FLAGS " -mcpu=cortex-m4 -mthumb")

