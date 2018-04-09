/*
 * power.c
 *
 *  Created on: 28 mar 2018
 *      Author: kamil
 */
#include <uni/power.h>
#include <uni/wdg.h>

#include <machine/hal.h>

#include <unistd.h>
#include <cdefs.h>

static unsigned int CriticalSectionPrimask;

void pwr_CriticalSectionEnter_callback()
{
	CriticalSectionPrimask = __get_PRIMASK();
	__disable_irq();
}

void pwr_CriticalSectionExit_callback()
{
	__set_PRIMASK(CriticalSectionPrimask);
}

__weak_symbol
void pwrmode_enter_pre(pwrmode_t mode)
{
	__USE(mode);
	switch(mode) {
	case PWRMODE_STOP:
	case PWRMODE_STOP_NOSYSTICK:
	case PWRMODE_STANDBY:
		fsync(STDOUT_FILENO);
		break;
	default:
		break;
	}
}

__weak_symbol
void pwrmode_enter_post(pwrmode_t mode)
{
	__USE(mode);
}

__weak_symbol
void pwrmode_enter(pwrmode_t mode)
{
	switch(mode) {
	case PWRMODE_RUN:
		break;
	case PWRMODE_LOW_POWER_RUN:
		break;
	case PWRMODE_SLEEP:
		if (NVIC_IsInInterrupt()) return;
		HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON, PWR_SLEEPENTRY_WFI);
		wdg_refresh();
		break;
	case PWRMODE_SLEEP_NOSYSTICK:
		HAL_SuspendTick();
		pwrmode_enter(PWRMODE_SLEEP);
		HAL_ResumeTick();
		break;
	case PWRMODE_STOP:
		assert(!NVIC_IsInInterrupt());
		HAL_PWR_EnterSTOPMode(PWR_MAINREGULATOR_ON, PWR_STOPENTRY_WFI);
		break;
	case PWRMODE_STOP_NOSYSTICK:
		HAL_SuspendTick();
		pwrmode_enter(PWRMODE_STOP);
		HAL_ResumeTick();
		break;
	case PWRMODE_STANDBY:
		HAL_PWR_EnterSTANDBYMode();
		break;
	default:
		assert(0);
		break;
	}
	wdg_refresh();
}

__weak_symbol
void pwrmode_restore(pwrmode_t mode)
{
	__USE(mode);
}
