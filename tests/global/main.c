/*
 * main.c
 *
 *  Created on: 4 lut 2018
 *      Author: kamil
 */
#include <ringbuffer.h>
#include <try.h>
#include <assert.h>
#include <printex.h>
#include <minmax.h>
#include <time_ex.h>
#include <filename.h>
#include <bitstr/bitstr.h>
#include <macroprint.h>

#ifdef __GNUC__
MACROPRINT(__GNUC__.__GNUC_MINOR__.__GNUC_PATCHLEVEL__)
#endif
MACROPRINT(__VERSION__)

#define TEST(ret, expr) do{ \
	if (!(expr)) { \
		fprintf(stderr, "Test '%s' failed\n", #expr); \
		ret = 1; \
	} \
}while(0)


int main() {
	int ret = 0;
	int findmsg_unittest();
	TEST(ret, findmsg_unittest() == 0);
	TEST(ret, rb_unittest() == 0);
	TEST(ret, MINMAX(1,0,3) == 1);
	TEST(ret, MINMAX(-1,0,3) == 0);
	TEST(ret, MINMAX(4,0,3) == 3);
	TEST(ret, printex_unittest() == 0);
	TEST(ret, _time_ex_unittest() == 0);
	TEST(ret, bitstr_unittest() == 0);
	return ret;
}
