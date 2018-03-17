/*
 * clocktimeout.c
 *
 *  Created on: 17 mar 2018
 *      Author: kamil
 */
#include "clocktimeout.h"

#include <assert.h>

void clocktimeout_init(clock_t *start, clock_t *timeout)
{
	assert(start!=NULL);
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
	assert(now > *start);
	const clock_t diff = now - *start;
	if ( *timeout <= diff ) {
		*timeout = 0;
		return true;
	}
	*timeout -= diff;
	*start = now;
	return false;
}
