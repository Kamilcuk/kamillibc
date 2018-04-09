/*
 * clock.c
 *
 *  Created on: 7 kwi 2018
 *      Author: kamil
 */
#include <machine/hal.h>

#include <time.h>

clock_t clock()
{
	return HAL_GetTick() * (CLOCKS_PER_SEC/1000);
}
