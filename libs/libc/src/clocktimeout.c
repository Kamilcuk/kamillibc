/*
 * clocktimeout.c
 *
 *  Created on: 17 mar 2018
 *      Author: kamil
 */
#include "clocktimeout.h"

#include <assert.h>
#include <limits.h>

clock_t clocktimeout_start(clock_t *timeout)
{
	return timeout != NULL ? clock() : 0;
}

bool clocktimeout_expired(clock_t * restrict start, clock_t * restrict timeout)
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

