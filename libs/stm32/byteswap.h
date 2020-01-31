/*
 * byteswap.h
 *
 *  Created on: 13 kwi 2018
 *      Author: kamil
 */

#ifndef _BYTESWAP_H
#define _BYTESWAP_H

#include <machine/endian.h>

// from newlib
#define bswap_16(x) __bswap16(x)
#define bswap_32(x) __bswap32(x)
#define bswap_64(x) __bswap64(x)

#endif /* _BYTESWAP_H */
