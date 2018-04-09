/*
 * criticalsection.c
 *
 *  Created on: 7 kwi 2018
 *      Author: kamil
 */
#include <uni/criticalsection.h>

#include <uni/cdefs.h>

#include <assert.h>

static unsigned int CriticalSectionCnt = 0;

__weak_symbol
void pwr_CriticalSectionEnter()
{
	if (++CriticalSectionCnt == 1) {
		pwr_CriticalSectionEnter_callback();
	}
}

void pwr_CriticalSectionExit()
{
	assert(CriticalSectionCnt > 0);
	if (--CriticalSectionCnt == 0) {
		pwr_CriticalSectionExit_callback();
	}
}
