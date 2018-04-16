/*
 * string_ex.c
 *
 *  Created on: 16 kwi 2018
 *      Author: Kamil Cukrowski
 *     License: All rights reserved. Copyright by Netemera(C).
 */
#include <string_ex/string_ex.h>

/* Private Macros ------------------------------------------------------------ */

#define STRINGEX_H_ARRCMP(NAME, TYPE) \
extern bool NAME(const TYPE ptr[restrict], const TYPE val, size_t size);

#define STRINGEX_C_ARRCMP(NAME, TYPE) \
bool NAME(const TYPE ptr[restrict], const TYPE val, size_t size) \
{ \
	while(size--) { \
		if (ptr++[0] != val) { \
			return false; \
		} \
	} \
	return true; \
}

/* Extern Functions Declarations ---------------------------------------------- */

STRINGEX_H_ARRCMP(arrcmpc, char)
STRINGEX_C_ARRCMP(arrcmpc, char)
STRINGEX_H_ARRCMP(arrcmpi, int)
STRINGEX_C_ARRCMP(arrcmpi, int)
STRINGEX_H_ARRCMP(arrcmpl, long)
STRINGEX_C_ARRCMP(arrcmpl, long)
STRINGEX_H_ARRCMP(arrcmpll, long long)
STRINGEX_C_ARRCMP(arrcmpll, long long)

/* Unittest -------------------------------------------------- */

int _string_ex_unittest() {
#define TEST_ARRCMP(FUNC,TYPE) \
	for(const TYPE arr[] = {0x01,0x02,0x03}; FUNC(arr, 0x00, sizeof(arr)/sizeof(arr[0])) != 0;) { return -__LINE__; break; } \
	for(const TYPE arr[] = {0x01,0x02,0x03}; FUNC(arr, 0x01, sizeof(arr)/sizeof(arr[0])) != 0;) { return -__LINE__; break; } \
	for(const TYPE arr[] = {0x01,0x01,0x01}; FUNC(arr, 0x01, sizeof(arr)/sizeof(arr[0])) == 0;) { return -__LINE__; break; }
	TEST_ARRCMP(arrcmpc, char);
	TEST_ARRCMP(arrcmpi, int);
	TEST_ARRCMP(arrcmpl, long);
	TEST_ARRCMP(arrcmpll, long long);
	return 0;
}
