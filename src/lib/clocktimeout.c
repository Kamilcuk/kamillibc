/*
 * clocktimeout.c
 *
 *  Created on: 17 mar 2018
 *      Author: kamil
 */
#include "clocktimeout.h"

#include <assert.h>
#include <limits.h>

void clocktimeout_init(clock_t *start, clock_t *timeout)
{
	assert(start != NULL);
	if (timeout != NULL) {
		*start = clock();
	}
}

bool clocktimeout_expired(clock_t *start, clock_t *timeout)
{
	assert(start != NULL);
	if (timeout == NULL) return false;
	if (*timeout == 0) return true;
	const clock_t now = clock();
	assert(now >= *start);
	const clock_t diff = now - *start;
	if ( *timeout <= diff ) {
		*timeout = 0;
		return true;
	}
	*timeout -= diff;
	*start = now;
	return false;
}

int clocktimeout_timeout_to_polltimeout(clock_t *timeout)
{
	int polltimeout;
	if (timeout == NULL) {
		polltimeout = -1;
	} else {
		clock_t timeout_poll = *timeout;
		if (CLOCKS_PER_SEC != 1000) {
			timeout_poll *= 1000 / CLOCKS_PER_SEC;
		}
		polltimeout = timeout_poll > INT_MAX ? INT_MAX : timeout_poll;
	}
	return polltimeout;
}
