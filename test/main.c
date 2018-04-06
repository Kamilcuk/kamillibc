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
#include <printmacro.h>
#include <unity/unity.h>

PRINTMACRO(__VERSION__)

static void rb_test() {
	TEST_ASSERT_EQUAL(0, rb_unittest());
}
static void minmax_test() {
	TEST_ASSERT_EQUAL(1, MINMAX(1,0,3));
	TEST_ASSERT_EQUAL(0, MINMAX(-1,0,3));
	TEST_ASSERT_EQUAL(3, MINMAX(4,0,3));
}
static void printex_test() {
	TEST_ASSERT_EQUAL(0, printex_unittest());
}
static void time_ex_test() {
	TEST_ASSERT_EQUAL(0, _time_ex_unittest());
}

int main() {
	setvbuf(stdout, NULL, _IOLBF, 100);
	setvbuf(stderr, NULL, _IOLBF, 100);

	int findmsg_unittest();
	try( findmsg_unittest() == 0 );

	UnityBegin(__FILENAME__);
	RUN_TEST(minmax_test);
	RUN_TEST(rb_test);
	RUN_TEST(printex_test);
	RUN_TEST(time_ex_test);
	try(UnityEnd() == 0);

	return 0;
}
