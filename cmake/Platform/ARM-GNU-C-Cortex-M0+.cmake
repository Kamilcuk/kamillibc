message(STATUS "Configuring for ARM platform, GNU-C compiler, Cortex-M0+ processor")
string(APPEND ARM_PROCESSOR_FLAGS " -mcpu=cortex-m0plus -mthumb")

