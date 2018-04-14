/*
 * unistd.h
 *
 *  Created on: 3 mar 2018
 *      Author: kamil
 */
#include <unistd.h>
#include <stdbool.h>
#include <poll.h>
#include <limits.h>
#include <uni/power.h>
#include <assert.h>
#include <time_ex.h>
#include <machine/rtc.h>
#include <string.h>
#include <uni/rtc.h>

__weak_symbol
int pause()
{
	pwrmode_enter(PWRMODE_SLEEP);
	return 0;
}

__weak_symbol
time_t time(time_t *t)
{
	assert(t == NULL);
	return rtc_time_ms(0)/1000;
}

__weak_symbol
int nanosleep(const struct timespec *rqtp, struct timespec *rmtp) {
	const clock_t startclock = clock();
	const clock_t stopclock = startclock + timespec_to_clock(*rqtp);
	pwrmode_enter_while(PWRMODE_SLEEP, stopclock > clock());
	memset(rmtp, 0, sizeof(*rmtp));
	return 0;
}
