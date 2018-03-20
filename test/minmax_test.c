/*
 * minmax_test.h
 *
 *  Created on: 21 lut 2018
 *      Author: kamil
 */

#include "minmax.h"

#include <assert.h>
#include <unity/unity.h>

void minmax_test()
{
	TEST_ASSERT_EQUAL(1, MINMAX(1,0,3));
	TEST_ASSERT_EQUAL(0, MINMAX(-1,0,3));
	TEST_ASSERT_EQUAL(3, MINMAX(4,0,3));
}
