
file(GLOB ccan_src FOLLOW_SYMLINKS ccan/mem/*.c)
add_library(ccan_object OBJECT ${ccan_src})
