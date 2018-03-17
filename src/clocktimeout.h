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
 * timeout == NULL -> endless timeout
 * *timeout is decremented after every _expired function call
 * maximum timeout is equal to MAX_OF(clock_t)
 */

void clocktimeout_init(clock_t *start, clock_t *timeout);
bool clocktimeout_expired(clock_t *start, clock_t *timeout);

#endif /* SRC_CLOCKTIMEOUT_H_ */
