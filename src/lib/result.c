/*
 * result.c
 *
 *  Created on: 4 sie 2018
 *      Author: kamil
 */
#include "result.h"
#include <stdlib.h>
#include <stdio.h>

#define TEST_EQ(expr)  do { \
	if (!(expr)) { \
		fprintf(stderr, "%s:%d: error %s: expr '%s' failed\n", __FILE__, __LINE__, __func__, #expr); \
		return -__LINE__; \
	} \
} while(0)

res_int_int_t example_func(int v)
{
	if (v < 0) {
		return (res_int_int_t)RES_initErr(v);
	}
	return (res_int_int_t)RES_initOk(v);
}

int _res_unittest(void)
{
	res_int_int_t res = RES_initOk(10);
	res_int_int_t res2 = RES_initErr(-10);
	int var;
	int save;

	TEST_EQ(RES_isOk(res) == true);
	TEST_EQ(RES_isErr(res) == false);
	TEST_EQ(RES_getOk(res, var) == true);
	TEST_EQ(var == 10);
	save = var;
	TEST_EQ(RES_getErr(res, var) == false);
	TEST_EQ(var == save);

	RES_setErr(res, -20);
	TEST_EQ(RES_isOk(res) == false);
	TEST_EQ(RES_isErr(res) == true);
	save = var;
	TEST_EQ(RES_getOk(res, var) == false);
	TEST_EQ(save == var);
	TEST_EQ(RES_getErr(res, var) == true);
	TEST_EQ(var == -20);

	TEST_EQ(RES_isOk(res2) == false);
	TEST_EQ(RES_isErr(res2) == true);
	save = var;
	TEST_EQ(RES_getOk(res2, var) == false);
	TEST_EQ(save == var);
	TEST_EQ(RES_getErr(res2, var) == true);
	TEST_EQ(var == -10);

	RES_setOk(res2, 20);
	TEST_EQ(RES_isOk(res2) == true);
	TEST_EQ(RES_isErr(res2) == false);
	TEST_EQ(RES_getOk(res2, var) == true);
	TEST_EQ(var == 20);
	save = var;
	TEST_EQ(RES_getErr(res2, var) == false);
	TEST_EQ(var == save);

	// example usage
	{
		res_int_int_t res = example_func(1);
		int ok;
		if (!RES_getOk(res, ok)) {
			// handle error!
			int err = res.err;
			(void)err;
			abort();
		}
		// handle ok
		(void)ok;
	}
	return 0;
}
