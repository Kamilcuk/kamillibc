/*
 * power.c
 *
 *  Created on: 28 mar 2018
 *      Author: kamil
 */
#include <uni/pwrmode.h>

#include <uni/wdg.h>
#include <cdefs.h>

#include <machine/hal.h>

#include <assert.h>
#include <unistd.h>

__attribute__((__weak__))
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
	case PWRMODE_SOFT_RESET:
		NVIC_SystemReset();
		break;
	default:
		assert(0);
		break;
	}
	wdg_refresh();
}

__attribute__((__weak__))
void pwrmode_restore(pwrmode_t mode)
{
	__USE(mode);
}
