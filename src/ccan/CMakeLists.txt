
file(GLOB_RECURSE ccan_src FOLLOW_SYMLINKS ccan/mem/*.c)
add_library(ccan STATIC ${ccan_src})
