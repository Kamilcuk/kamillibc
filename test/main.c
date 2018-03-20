/*
 * main.c
 *
 *  Created on: 4 lut 2018
 *      Author: kamil
 */
#include <ringbuffer.h>
#include <try.h>
#include <assert.h>
#include <unity/unity.h>

static void rb_test()
{
	TEST_ASSERT_EQUAL(0, rb_unittest());
}
void minmax_test();
int findmsg_unittest();

int main() {
	try(findmsg_unittest() == 0);
	UnityBegin(__FILE__);
	RUN_TEST(minmax_test);
	RUN_TEST(rb_test);
	try(UnityEnd() == 0);
	return 0;
}
