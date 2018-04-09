/*
 * time_ex.c
 *
 *  Created on: 25 mar 2018
 *      Author: kamil
 */
#define _POSIX_TIMERS

#include <time_ex.h>

#include <uni/cdefs.h>

#include <assert.h>

#if _POSIX_C_SOURCE >= 199309L

#include <time.h>

__weak_symbol
uint64_t timer_gettime_ns(timer_t timerid, uint64_t *interval)
{
	struct itimerspec value;
	if (timer_gettime(timerid, &value) != 0) { assert(0); }
	if (interval) *interval = timespec_to_ns(value.it_interval);
	return timespec_to_ns(value.it_value);
}

__weak_symbol
int timer_settime_ns(timer_t timerid, uint64_t value, uint64_t interval)
{
	struct itimerspec it = {
			.it_value = timespec_from_ns(value),
			.it_interval = timespec_from_ns(interval),
	};
	return timer_settime(timerid, 0, &it, NULL);
}

__weak_symbol
uint32_t timer_gettime_ms(timer_t timerid, uint32_t *interval)
{
	struct itimerspec value;
	if (timer_gettime(timerid, &value) != 0) { assert(0); }
	if (interval != NULL ) *interval  = timespec_to_ms(value.it_interval);
	return timespec_to_ms(value.it_value);
}

__weak_symbol
int timer_settime_ms(timer_t timerid, uint32_t value, uint32_t interval)
{
	struct itimerspec it = {
			.it_value = timespec_from_ms(value),
			.it_interval = timespec_from_ms(interval),
	};
	return timer_settime(timerid, 0, &it, NULL);
}

#endif

int _time_ex_unittest()
{
#define STEST(expr) static_assert(expr, #expr);
	STEST(TIMESPEC_TO_INT_RATIO_SEC_NSEC(5,0,1,1) == 5);
	STEST(TIMESPEC_TO_INT_RATIO_SEC_NSEC(0,5,1,1000000000ull) == 5);
	STEST(TIMESPEC_TO_INT_RATIO_SEC_NSEC(5,5,1,1000000000ull) == 5000000005ull);
	STEST(TIMESPEC_TO_INT_RATIO_SEC_NSEC(10,0,3,2) == 6);
	STEST(TIMESPEC_FROM_INT_RATIO_SEC(5000000005ull,1,1000000000ull) == 5);
	STEST(TIMESPEC_FROM_INT_RATIO_NSEC(5000000005ull,1,1000000000ull) == 5);
	STEST(TIMESPEC_FROM_INT_RATIO_SEC(10*1000,2,3000) == 6);
	STEST(TIMESPEC_FROM_INT_RATIO_NSEC(10*1000,2,3000) == 666666666);
	return 0;
}

