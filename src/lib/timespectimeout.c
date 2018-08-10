/*
 * clocktimeout.c
 *
 *  Created on: 17 mar 2018
 *      Author: kamil
 */
#include "timespectimeout.h"

#include <time_ex.h>
#include <assert.h>
#include <limits.h>

struct timespec timespectimeout_start(void)
{
	struct timespec ts;
	clock_gettime(CLOCK_REALTIME, &ts);
	return ts;
}

void timespectimeout_init(struct timespec *start, struct timespec *timeout)
{
	assert(start != NULL);
	if (timeout != NULL) {
		*start = timespectimeout_start();
	}
}

bool timespectimeout_expired(struct timespec *start, struct timespec *timeout)
{
	assert(start != NULL);
	if (timeout == NULL) return false;
	if (timespec_cmp_eq(*timeout, (struct timespec){0, 0})) return true;
	struct timespec now;
	clock_gettime(CLOCK_REALTIME, &now);
	assert(timespec_cmp_gt(now, *start));
	const struct timespec diff = timespec_sub(now, *start);
	if (timespec_cmp_le(*timeout, diff)) {
		*timeout = timespec_init(0, 0);
		return true;
	}
	*timeout = timespec_sub(*timeout, diff);
	*start = now;
	return false;
}

