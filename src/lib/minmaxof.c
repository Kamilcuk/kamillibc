/*
 * minmaxof.c
 *
 *  Created on: 17 mar 2018
 *      Author: kamil
 */
#include <minmaxof.h>
#include <assert.h>
#include <limits.h>
#include <inttypes.h>
#include <stdio.h>

#define _(type,how,max,min) do{ \
		printf("MAX_OF("#type") = %"how" == %"how"\n", MAX_OF(type), (type)max); \
		printf("MIN_OF("#type") = %"how" == %"how"\n", MIN_OF(type), (type)min); \
		static_assert(MAX_OF(type) == max, ""); \
		static_assert(MIN_OF(type) == min, ""); \
	}while(0)

static inline void minmaxof_static_assert()
{
	_(char, "d", CHAR_MAX, CHAR_MIN);
	_(signed char, "d", SCHAR_MAX, SCHAR_MIN);
	_(unsigned char, "d", UCHAR_MAX, 0);
	_(signed int, "d", INT_MAX, INT_MIN);
	_(unsigned int, "u", UINT_MAX, (unsigned int)0);
	_(signed long, "ld", LONG_MAX, LONG_MIN);
	_(unsigned long, "lu", ULONG_MAX, (unsigned long)0);
	_(signed long long, "lld", LLONG_MAX, LLONG_MIN);
	_(unsigned long long, "llu", ULLONG_MAX, (unsigned long)0);
	_(int8_t, PRId8, INT8_MAX, INT8_MIN);
	_(uint8_t, PRIu8, UINT8_MAX, 0);
	_(int16_t, PRId16, INT16_MAX, INT16_MIN);
	_(uint16_t, PRIu16, UINT16_MAX, 0);
	_(int32_t, PRId32, INT32_MAX, INT32_MIN);
	_(uint32_t, PRIu32, UINT32_MAX, 0);
	_(int64_t, PRId64, INT64_MAX, INT64_MIN);
	_(uint64_t, PRIu64, UINT64_MAX, 0);
}
