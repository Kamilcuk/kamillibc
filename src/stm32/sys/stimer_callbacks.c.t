/*
 * timer_callbacks.c
 *
 *  Created on: 4 mar 2018
 *      Author: kamil
 */
#include <stimer/stimer.h>
#include <machine/hal.h>
#include <uni/rtc.h>

#include <minmaxof.h>
#include <time_ex.h>

#include <stdio.h>

static time_t ms_armed;

void stimer_arm_Callback(clock_t value)
{
	time_ms_t ms = clock_to_ms(value);
	rtc_set_alarm(0, 0, ms);
	if (ms != 0) {
		ms_armed = rtc_time_ms(0);
	}
}

clock_t stimer_elapsedTimeSinceLastArm_Callback()
{
	const time_ms_t now = rtc_time_ms(0);
	if (now < ms_armed) {
		// time_t rollover
		const time_ms_t nowll = now;
		const time_ms_t maxof_time_t = MAX_OF(time_ms_t);
		const time_ms_t ms_armedll = ms_armed;
		return maxof_time_t + nowll - ms_armedll;
	}
	return clock_from_ms( now - ms_armed );
}

void stimer_IRQDisable_Callback()
{
	NVIC_DisableIRQ(RTC_IRQn);
}

void stimer_IRQEnable_Callback()
{
	NVIC_EnableIRQ(RTC_IRQn);
}

