/*
 * cdefs.h
 *
 *  Created on: 9 kwi 2018
 *      Author: kamil
 */
#ifndef SRC_LIB_UNI_CDEFS_H_
#define SRC_LIB_UNI_CDEFS_H_

#include <stdint.h>

// undef if someone pulled sys/cdefs.h or bsd/sys/cdefs.h

#if defined(__cplusplus)
#define	__BEGIN_DECLS	extern "C" {
#define	__END_DECLS	}
#else
#define	__BEGIN_DECLS
#define	__END_DECLS
#endif

#ifndef __weak
#ifdef __GNUC__
#define __weak  __attribute__((weak))
#else
#define __weak
#endif
#endif

#ifndef __packed
#ifdef __GNUC__
#define __packed  __attribute__((__packed__))
#else
#define __packed
#endif
#endif

#ifndef __nonnull
#if __GNUC__
#define __nonnull(x)  __attribute__((__nonnull__ x ))
#else
#define __nonnull(x)
#endif
#endif

#ifndef __is_array_of_constant_known_size
#if __GNUC__
#define __is_array_of_constant_known_size(x)  ( __builtin_constant_p((void*)x == (void*)&x)?1:0 )
#else
#define __is_array_of_constant_known_size(x)  ( (void*)&x[0] == (void*)&x )
#endif
#endif

#ifndef __arraycount
#if __GNUC__
#define __arraycount(x)  ( sizeof(char[__is_array_of_constant_known_size(x)?1:-1])?(sizeof(x)/sizeof((x)[0])):-1 )
#else
#define __arraycount(x)  (sizeof(x)/sizeof((x)[0]))
#endif
#endif

#ifndef __constructor
#if __GNUC__
#define __constructor  __attribute__((__constructor__))
#else
#define __constructor "Error constructor attribute not implemented"
#endif
#endif

#ifndef __packed
#if __GNUC__
#define	__packed  __attribute__((__packed__))
#else
#define	__packed
#endif
#endif

#ifndef __pure2
#if __GNUC__
#define __pure2  __attribute__((__const__))
#else
__pure
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

#ifndef __drunk
#ifdef __GNUC__
#define __drunk  __attribute__((__warning__("This code was written while being drunk")))
#else
#define __drunk /* This code was written while being drunk */
#endif
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
