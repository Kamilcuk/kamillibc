/*
 * printuint64.h
 *
 *  Created on: 23 mar 2018
 *      Author: kamil
 */

#ifndef SRC_CLIBS_PRINTEX_H_
#define SRC_CLIBS_PRINTEX_H_

#include <limits.h>
#include <stdint.h>

char *printbinary(unsigned width, uintmax_t v, char result[width + 1]);
#define PRINTBINARY(width, v)  printbinary((width), (v), (char[(width) + 1]){0})
#define PRINTBINARY_t(v)        PRINTBINARY(sizeof(v) * CHAR_BIT, (v))

char *printuint64(uint64_t v, char result[21]);
#define PRINTUINT64(v)  printuint64((v), (char[21]){0})

char *printint64(int64_t v, char result[22]);
#define PRINTINT64(v)  printint64((v), (char[22]){0})

#define PRINTARRAY(fmt_arr, arr, arrsize, ...) do{ \
	const size_t _end = (arrsize); \
	for(size_t _i = 0; _i < _end; ++_i) { \
		printf((fmt_arr), (arr)[_i], ##__VA_ARGS__); \
	} \
}while(0)

#define PRINTARRAY_SEP(sep, fmt_arr, arr, arrsize, ...) do{ \
	const size_t _end = (arrsize); \
	for(size_t _i = 0; _i < _end; ++_i) { \
		if (_i != 0) { \
			printf(sep); \
		} \
		printf((fmt_arr), (arr)[_i], ##__VA_ARGS__); \
	} \
}while(0)

int printex_unittest();

#endif /* SRC_CLIBS_PRINTEX_H_ */
