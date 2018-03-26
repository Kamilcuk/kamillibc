/*
 * printuint64.c
 *
 *  Created on: 23 mar 2018
 *      Author: kamil
 */

#include <printex.h>

#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <limits.h>
#include <stddef.h>
#include <assert.h>

static inline void printbinary_r_in_less8(int width, const char *v, char *result[width])
{
	for(unsigned int i = 0; i < width; ++i) {
		const char res = ( ( *v >> i ) & 1 ) + '0';
		(--(*result))[0] = res;
	}
}

static char *printbinary_r_in(int width, const char *v, char result[width+1])
{
	result = &result[width];
	result[0] = '\0';
    for(; width >= 8; width -= 8, ++v) {
    	printbinary_r_in_less8(CHAR_BIT, v, &result);
    }
    printbinary_r_in_less8(width, v, &result);
    return result;
}

char *printbinary_r(int width, unsigned long long v, char result[width+1])
{
	assert(result != NULL);
	return printbinary_r_in(width, (const char *)&v, result);
}

char *printuint64_r(uint64_t v, char result[21])
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

char *printuint64(uint64_t v) {
	static char bfr[21];
	return printuint64_r(v, bfr);
}

char *printint64_r(int64_t v, char result[22])
{
	result = printuint64_r(v < 0 ? -v : v, &result[1]);
	if ( v < 0 ) {
		(--result)[0] = '-';
	}
	return result;
}

char *printint64(int64_t v)
{
	static char result[22];
	return printint64_r(v, result);
}

int print_unittest()
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

	tmp = printbinary_r(2, 0b10, buf);
	TEST(tmp == buf);
	TEST(!strcmp(buf, "10"));

	tmp = printbinary_r(64, 0b0100000001000010111001011100101000111111011110101111111110100010, buf);
	TEST(tmp == buf);
	TEST(!strcmp(tmp,        "0100000001000010111001011100101000111111011110101111111110100010"));

	TEST(!strcmp(PRINTUINT64(1234567654321), "1234567654321"));

	TEST(!strcmp(PRINTINT64(-1234567654321), "-1234567654321"));

	TEST(!strcmp(
			PRINTBINARY(64, 0b0100000001000010111001011100101000111111011110101111111110100010),
			"0100000001000010111001011100101000111111011110101111111110100010")
	);

	return 0;
}
