/* CC0 (Public domain) - see LICENSE file for details */
#ifndef CONTAINER_OF_H
#define CONTAINER_OF_H

#include <stddef.h>
#include <typecheck.h>

#define container_of(ptr, type, member) \
	((type *)((char *)(ptr) - offsetof(type, member) + !typecheck(*ptr, ((type *)0)->member)))

#endif /* CONTAINER_OF_H */
