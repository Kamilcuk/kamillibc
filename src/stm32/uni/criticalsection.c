/*
 * criticalsection.c
 *
 *  Created on: 25 kwi 2018
 *      Author: kamil
 */
#include <uni/criticalsection.h>
#include <machine/hal.h>

static unsigned int criticalsection_primask;

void criticalsection_enter_callback()
{
	criticalsection_primask = __get_PRIMASK();
	__disable_irq();
}

void criticalsection_exit_callback()
{
	__set_PRIMASK(criticalsection_primask);
}
