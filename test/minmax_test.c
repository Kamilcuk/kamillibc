/*
 * minmax_test.h
 *
 *  Created on: 21 lut 2018
 *      Author: kamil
 */

#include "minmax.h"

#include <assert.h>

void _MAX_TEST()
{
	assert(MAX3(1,2,3)==3);
	assert(MAX3(1,3,2)==3);
	assert(MAX3(3,1,2)==3);
}
