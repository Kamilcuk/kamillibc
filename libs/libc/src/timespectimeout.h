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

struct timespec timespectimeout_start(void);
void timespectimeout_init(struct timespec *start, struct timespec *timeout);
bool timespectimeout_expired(struct timespec *start, struct timespec *timeout);

static inline void timespectimeout_update(struct timespec *start, struct timespec *timeout)
{
	(void)timespectimeout_expired(start, timeout);
}

#endif /* SRC_CLOCKTIMEOUT_H_ */
