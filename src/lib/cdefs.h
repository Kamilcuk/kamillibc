/*
 * cdefs.h
 *
 *  Created on: 9 kwi 2018
 *      Author: kamil
 */
#ifndef SRC_LIB_UNI_CDEFS_H_
#define SRC_LIB_UNI_CDEFS_H_

#include <stdint.h>

#ifndef __is_array_of_constant_known_size
#if __GNUC__
#define __is_array_of_constant_known_size(x)  (__builtin_constant_p((void*)x == &x))
#else
#define __is_array_of_constant_known_size(x)  ((void*)&x[0] == &x)
#endif
#endif

#ifndef __arraycount
#if __GNUC__
#define __arraycount(x)  (sizeof(x) / sizeof((x)[0]) + sizeof(struct { int:-!(__is_array_of_constant_known_size(x)); }))
#else
#define __arraycount(x)  (sizeof(x) / sizeof((x)[0]))
#endif
#endif

#ifndef __USE
#define __USE(x)  ((void)(x))
#endif

#ifndef __UNCONST
#define __UNCONST(x)  ((void *)(uintptr_t)(const void *)(x))
#endif

#ifndef __UNVOLATILE
#define __UNVOLATILE(x)  ((void *)(uintptr_t)(const void *)(x))
#endif

#ifndef __P
#define	__P(protos)	protos		/* full-blown ANSI C */
#endif
#ifndef __CONCAT1
#define	__CONCAT1(x,y)	x ## y
#endif
#ifndef __CONCAT
#define	__CONCAT(x,y)	__CONCAT1(x,y)
#endif
#ifndef __XCONCAT
#define __XCONCAT(x,y)  __CONCAT1(x,y)
#endif
#ifndef __STRING
#define	__STRING(x)	#x		/* stringify without expanding x */
#endif
#ifndef __XSTRING
#define	__XSTRING(x)	__STRING(x)	/* expand x, then stringify */
#endif

#ifndef __CONCAT3
#define	__CONCAT3(x,y,z)	__CONCAT1(__CONCAT1(x,y),z)
#endif

#endif /* SRC_LIB_UNI_CDEFS_H_ */
