/*
 * unistd.h
 *
 *  Created on: 3 mar 2018
 *      Author: kamil
 */
#include <machine/rtc.h>
#include <machine/hal.h>
#include <uni/rtc.h>
#include <uni/pwrmode.h>
#include <time_ex.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <poll.h>
#include <limits.h>
#include <assert.h>

__attribute__((__weak__))
void abort(void) {
	NVIC_SystemReset();
}

__attribute__((__weak__))
int pause()
{
	pwrmode_enter(PWRMODE_SLEEP);
	return 0;
}

__attribute__((__weak__))
time_t time(time_t *t)
{
	assert(t == NULL);
	return clock()/CLOCKS_PER_SEC;
}

__attribute__((__weak__))
int nanosleep(const struct timespec *rqtp, struct timespec *rmtp)
{
	const clock_t startclock = clock();
	const clock_t stopclock = startclock + timespec_to_clock(*rqtp);
	PWRMODE_WHILE(PWRMODE_SLEEP, stopclock > clock());
	rmtp->tv_sec = 0;
	rmtp->tv_nsec = 0;
	return 0;
}
