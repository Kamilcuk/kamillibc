/*
 * main.c
 *
 *  Created on: 4 lut 2018
 *      Author: kamil
 */
#include <ringbuffer.h>
#include <assert.h>
#include <printex.h>
#include <minmax.h>
#include <time_ex.h>
#include <filename.h>
#include <bitstr/bitstr.h>
#include <build_bug.h>
#include <macroprint.h>
#include <array_size.h>
#include <cdefs.h>
#include <typecheck.h>

#define TEST_EQ(ret, expr) do{ \
	if (!(expr)) { \
		fprintf(stderr, "Test '%s' failed\n", #expr); \
		ret = 1; \
	} \
}while(0)


int main() {

	BUILD_BUG_ON_MSG(0 == 1, "This should not fail");
	if (0 == 1) {
		BUILD_BUG();
	}
	BUILD_BUG_ON(BUILD_BUG_ON_ZERO(0 == 1) != 0);

	char b[5];
	BUILD_BUG_ON(ARRAY_SIZE(b) != 5);
	BUILD_BUG_ON(ARRAY_SIZE((char[5]){0}) != 5);

	BUILD_BUG_ON(typecheck(char, char) != 1);
	BUILD_BUG_ON(typecheck((char[5]){0}, b) != 1);
	BUILD_BUG_ON(typecheck(char, b[0]) != 1);

	int ret = 0;
	TEST_EQ(ret, rb_unittest() == 0);
	TEST_EQ(ret, MINMAX(1,0,3) == 1);
	TEST_EQ(ret, MINMAX(-1,0,3) == 0);
	TEST_EQ(ret, MINMAX(4,0,3) == 3);
	TEST_EQ(ret, printex_unittest() == 0);
	TEST_EQ(ret, _time_ex_unittest() == 0);
	TEST_EQ(ret, bitstr_unittest() == 0);
	return ret;
}
