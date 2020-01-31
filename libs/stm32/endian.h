/*
 * endian.h
 *
 *  Created on: 13 kwi 2018
 *      Author: kamil
 */
/* Inline functions to return unsigned integer values unchanged.
   Copyright (C) 2017 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>.  */
/* Copyright (C) 1997-2017 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>.  */
#ifndef THIRD_PARTY_KAMILLIBC_SRC_STM32_ENDIAN_H_
#define THIRD_PARTY_KAMILLIBC_SRC_STM32_ENDIAN_H_

#include <byteswap.h>
#include <stdint.h>

/* These inline functions are to ensure the appropriate type
   conversions and associated diagnostics from macros that convert to
   a given endianness.  */

static __inline __uint16_t
__uint16_identity (__uint16_t __x)
{
  return __x;
}

static __inline __uint32_t
__uint32_identity (__uint32_t __x)
{
  return __x;
}

static __inline __uint64_t
__uint64_identity (__uint64_t __x)
{
  return __x;
}

# if __BYTE_ORDER == __LITTLE_ENDIAN
#  define htobe16(x) __bswap_16 (x)
#  define htole16(x) __uint16_identity (x)
#  define be16toh(x) __bswap_16 (x)
#  define le16toh(x) __uint16_identity (x)

#  define htobe32(x) __bswap_32 (x)
#  define htole32(x) __uint32_identity (x)
#  define be32toh(x) __bswap_32 (x)
#  define le32toh(x) __uint32_identity (x)

#  define htobe64(x) __bswap_64 (x)
#  define htole64(x) __uint64_identity (x)
#  define be64toh(x) __bswap_64 (x)
#  define le64toh(x) __uint64_identity (x)

# else
#  define htobe16(x) __uint16_identity (x)
#  define htole16(x) __bswap_16 (x)
#  define be16toh(x) __uint16_identity (x)
#  define le16toh(x) __bswap_16 (x)

#  define htobe32(x) __uint32_identity (x)
#  define htole32(x) __bswap_32 (x)
#  define be32toh(x) __uint32_identity (x)
#  define le32toh(x) __bswap_32 (x)

#  define htobe64(x) __uint64_identity (x)
#  define htole64(x) __bswap_64 (x)
#  define be64toh(x) __uint64_identity (x)
#  define le64toh(x) __bswap_64 (x)
# endif


#endif /* THIRD_PARTY_KAMILLIBC_SRC_STM32_ENDIAN_H_ */
