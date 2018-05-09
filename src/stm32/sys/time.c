/*
 * time.c
 *
 *  Created on: 25 mar 2018
 *      Author: kamil
 */
#include <time.h>
#include <time_ex.h>
#include <reent.h>
#include <cdefs.h>
#include <uni/rtc.h>

__weak
clock_t clock() {
	return clock_from_ms(HAL_GetTick());
}

__weak
int _gettimeofday_r(struct _reent *r, struct timeval *ptimeval, void *ptimezone)
{
	__USE(r);
	assert(ptimeval != NULL);
	assert(ptimezone == NULL);
	const time_t ms = rtc_time_ms(0);
	*ptimeval = TIMEVAL_FROM_INT_RATIO(ms, 1, 1000);
	return 0;
}
