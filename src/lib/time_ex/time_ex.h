/*
 * time-ex.h
 *
 *  Created on: 4 mar 2018
 *      Author: kamil
 */

#ifndef TIME_EX_TIME_EX_H_
#define TIME_EX_TIME_EX_H_

#include <time_ex/types.h>
#include <time_ex/timespec.h>
#include <time_ex/timeval.h>
#include <time_ex/clock.h>
#include <time_ex/tm.h>
#include <time.h>

/* Exported Functions ------------------------------------------------------ */

#if _POSIX_C_SOURCE >= 199309L

time_ns_t timer_gettime_ns(timer_t timerid, time_ns_t *interval);
int timer_settime_ns(timer_t timerid, time_ns_t value, time_ns_t interval);
time_ms_t timer_gettime_ms(timer_t timerid, time_ms_t *interval);
int timer_settime_ms(timer_t timerid, time_ms_t value, time_ms_t interval);

#endif

int _time_ex_unittest();

#endif /* TIME_EX_TIME_EX_H_ */
