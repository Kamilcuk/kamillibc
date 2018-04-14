/*
 * clocktimeout.h
 *
 *  Created on: 17 mar 2018
 *      Author: kamil
 */

#ifndef SRC_CLOCKTIMEOUT_H_
#define SRC_CLOCKTIMEOUT_H_

#include <time.h>
#include <stdbool.h>

/**
 * timeout is measured since fixed point in time called "start"
 * start is filled in clocktimeout_init
 * timeout == NULL -> endless timeout
 * *timeout == 0 -> timeout expired
 * value of timeout is decremented in every _expired function call
 * value of start changes in every _expired function call
 * maximum timeout is equal to MAX_OF(clock_t)
 */

clock_t clocktimeout_start();
void clocktimeout_init(clock_t *start, clock_t *timeout);
bool clocktimeout_expired(clock_t *start, clock_t *timeout);

int clocktimeout_timeout_to_polltimeout(clock_t *timeout);

static inline void clocktimeout_update(clock_t *start, clock_t *timeout)
{
	(void)clocktimeout_expired(start, timeout);
}

#endif /* SRC_CLOCKTIMEOUT_H_ */
