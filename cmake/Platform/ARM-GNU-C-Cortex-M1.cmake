message(STATUS "Configuring for ARM platform, GNU-C compiler, Cortex-M1 processor")
string(APPEND ARM_PROCESSOR_FLAGS " -mcpu=cortex-m1 -mthumb")

