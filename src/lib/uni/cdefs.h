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



#endif /* SRC_LIB_UNI_CDEFS_H_ */
