/*
 * power.c
 *
 *  Created on: 28 mar 2018
 *      Author: kamil
 */
#include <osal/power.h>

#include <unistd.h>
#include <sys/cdefs.h>

static unsigned int CriticalSectionPrimask;
static unsigned int CriticalSectionCnt;

void pwr_CriticalSectionEnter()
{
	if (++CriticalSectionCnt == 1) {
		CriticalSectionPrimask = __get_PRIMASK();
		__disable_irq();
	}
}

void pwr_CriticalSectionExit()
{
	assert(CriticalSectionCnt > 0);
	if (--CriticalSectionCnt == 0) {
		__set_PRIMASK(CriticalSectionPrimask);
	}
}

__weak_symbol
void pwr_mode_enter_pre(pwrmode_t mode)
{
	fsync(STDOUT_FILENO);
}

__weak_symbol
void pwr_mode_enter_post(pwrmode_t mode)
{

}

__weak_symbol
void pwr_mode_restore(pwrmode_t from_mode)
{

}
