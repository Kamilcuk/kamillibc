/*
 * printuint64.c
 *
 *  Created on: 23 mar 2018
 *      Author: kamil
 */

#include <printex.h>

#include <limits.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <assert.h>

static inline void printbinary_in_char(int width, const char *v, char *result[-width])
{
	for(unsigned int i = 0; i < width; ++i) {
		const char res = ( ( *v >> i ) & 1 ) + '0';
		(--(*result))[0] = res;
	}
}

static char *printbinary_in(int width, const char *v, char result[width+1])
{
	result = &result[width];
	result[0] = '\0';
    for(; width >= CHAR_BIT; width -= CHAR_BIT, ++v) {
    	printbinary_r_in_char(CHAR_BIT, v, &result);
    }
    printbinary_r_in_char(width, v, &result);
    return result;
}

char *printbinary(int width, uintmax_t v, char result[width+1])
{
	assert(result != NULL);
	return printbinary_r_in(width, (const char *)&v, result);
}

char *printuint64(uint64_t v, char result[21])
{
	assert(result != NULL);
	result = &result[20];
	result[0] = '\0';
	for (bool first = true; v || first; first = false) {
#if printuint64_r_USE_DIV
		div_t d = div(v, 10);
		const char c = d.qout + '0';
		v = d.rem;
#else
		const char c = v % 10 + '0';
		v /= 10;
#endif
		(--result)[0] = c;
	}
	return result;
}

char *printint64(int64_t v, char result[21])
{
	result = printuint64_r(llabs(v), &result[1]);
	if ( v < 0 ) {
		(--result)[0] = '-';
	}
	return result;
}

int printex_unittest()
{
#define TEST(expr) do{ if(!(expr)) { assert(0); return -__LINE__; } }while(0)
	char *tmp, buf[65];

	tmp = printuint64_r(1234567654321, buf);
	TEST(tmp >= &buf[0] && tmp < &buf[21]);
	TEST(!strcmp(tmp, "1234567654321"));

	tmp = printint64_r(1234567654321, buf);
	TEST(tmp >= &buf[0] && tmp < &buf[21]);
	TEST(!strcmp(tmp, "1234567654321"));

	tmp = printint64_r(-1234567654321, buf);
	TEST(tmp >= &buf[0] && tmp < &buf[21]);
	TEST(!strcmp(tmp, "-1234567654321"));

	tmp = printbinary_r(2, 0x2, buf);
	TEST(tmp == buf);
	TEST(!strcmp(buf, "10"));

	tmp = printbinary_r(64, 0xcd0aa5f8cc389648, buf);
	TEST(tmp == buf);
	TEST(!strcmp(tmp, "1100110100001010101001011111100011001100001110001001011001001000"));

	TEST(!strcmp(PRINTUINT64(18446744073709551615ull), "18446744073709551615"));

	TEST(!strcmp(PRINTINT64( 9223372036854775807ll),  "9223372036854775807"));
	TEST(!strcmp(PRINTINT64(-9223372036854775807ll), "-9223372036854775807"));

	TEST(!strcmp(PRINTBINARY(64, 0xcd0aa5f8cc389648),
			"1100110100001010101001011111100011001100001110001001011001001000")
	);

	return 0;
}
