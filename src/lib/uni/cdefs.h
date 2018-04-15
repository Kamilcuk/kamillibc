/*
 * cdefs.h
 *
 *  Created on: 9 kwi 2018
 *      Author: kamil
 */

#ifndef SRC_LIB_UNI_CDEFS_H_
#define SRC_LIB_UNI_CDEFS_H_

#include <uni/_cdefs.h>

// omg libc

#ifndef __weak_symbol
#if __GNUC__
#define __weak_symbol __attribute__((__weak__))
#else
#define __weak_symbol
#endif
#endif

#ifndef __nonnull
#if __GNUC__
#define __nonnull(x) __attribute__((__nonnull__ x ))
#else
#define __nonnull(x)
#endif
#endif

#ifndef __arraycount
#define __arraycount(x)  (sizeof(x)/sizeof(x[0]))
#endif

#endif /* SRC_LIB_UNI_CDEFS_H_ */
