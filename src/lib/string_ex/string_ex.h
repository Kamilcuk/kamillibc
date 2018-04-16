/*
 * string_ex.h
 *
 *  Created on: 16 kwi 2018
 *      Author: Kamil Cukrowski
 *     License: All rights reserved. Copyright by Netemera(C).
 */

#ifndef SRC_LIB_STRING_EX_STRING_EX_H_
#define SRC_LIB_STRING_EX_STRING_EX_H_

#include <stddef.h>
#include <stdbool.h>

/**
 * Returns true if all ptr0 values are equal to val
 */
bool arrcmpc(const char ptr[restrict], char val, size_t size);
bool arrcmpi(const int ptr[restrict], int val, size_t size);
bool arrcmpl(const long ptr[restrict], long val, size_t size);
bool arrcmpll(const long long ptr[restrict], long long val, size_t size);

int _string_ex_unittest();

#endif /* SRC_LIB_STRING_EX_STRING_EX_H_ */
