/*
 * assert_failed.c
 *
 *  Created on: 25 lut 2018
 *      Author: kamil
 */
#include <inttypes.h>
#include <stdio.h>
#include <assert.h>

void assert_failed(uint8_t* file, uint32_t line) {
#ifdef __NEWLIB__
	__assert_func((const char *)file, line, NULL, "");
#elif defined(__GLIBC__)
	__assert_fail("", (const char *)file, line, NULL);
#else
	assert(0);
#endif
}
