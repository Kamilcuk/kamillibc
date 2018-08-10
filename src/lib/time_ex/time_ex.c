/*
 * time_ex.c
 *
 *  Created on: 25 mar 2018
 *      Author: kamil
 */
#define _POSIX_TIMERS

#include <time_ex.h>

#include <cdefs.h>

#include <assert.h>
#include <time.h>
#include <stdio.h>

#if _POSIX_C_SOURCE >= 199309L

__attribute__((__weak__))
time_ns_t timer_gettime_ns(timer_t timerid, time_ns_t *interval)
{
	struct itimerspec value;
	if (timer_gettime(timerid, &value) != 0) { assert(0); }
	if (interval) *interval = timespec_to_ns(value.it_interval);
	return timespec_to_ns(value.it_value);
}

__attribute__((__weak__))
int timer_settime_ns(timer_t timerid, time_ns_t value, time_ns_t interval)
{
	struct itimerspec it = {
			.it_value = timespec_from_ns(value),
			.it_interval = timespec_from_ns(interval),
	};
	return timer_settime(timerid, 0, &it, NULL);
}

__attribute__((__weak__))
time_ms_t timer_gettime_ms(timer_t timerid, time_ms_t *interval)
{
	struct itimerspec value;
	if (timer_gettime(timerid, &value) != 0) { assert(0); }
	if (interval != NULL ) *interval  = timespec_to_ms(value.it_interval);
	return timespec_to_ms(value.it_value);
}

__attribute__((__weak__))
int timer_settime_ms(timer_t timerid, time_ms_t value, time_ms_t interval)
{
	struct itimerspec it = {
			.it_value = timespec_from_ms(value),
			.it_interval = timespec_from_ms(interval),
	};
	return timer_settime(timerid, 0, &it, NULL);
}

#endif

#define TEST(expr) do { \
	if (!(expr)) { \
		fprintf(stderr, "error: test %s failed\n", #expr); \
		return -__LINE__; \
	} \
} while(0)

#define STEST(expr) static_assert(expr, #expr)

#define TIMESPEC_TEST(func, sec, nsec) do { \
	struct timespec _tmp = (func); \
	if (_tmp.tv_sec != sec || _tmp.tv_nsec != nsec) { \
		fprintf(stderr, \
			"Error: computing %s = {%ld, %ld} != (struct timespec){ %s = %ld, %s = %ld }\n", \
			#func, _tmp.tv_sec, _tmp.tv_nsec, #sec, (long)sec, #nsec, (long)nsec); \
		return -__LINE__; \
	} \
} while(0)

int _time_ex_unittest()
{
	STEST(TIMESPEC_TO_INT_RATIO_SEC_NSEC(5,0,1,1) == 5);
	STEST(TIMESPEC_TO_INT_RATIO_SEC_NSEC(0,5,1,1000000000ull) == 5);
	STEST(TIMESPEC_TO_INT_RATIO_SEC_NSEC(5,5,1,1000000000ull) == 5000000005ull);
	STEST(TIMESPEC_TO_INT_RATIO_SEC_NSEC(10,0,3,2) == 6);
	STEST(TIMESPEC_FROM_INT_RATIO_SEC(5000000005ull,1,1000000000ull) == 5);
	STEST(TIMESPEC_FROM_INT_RATIO_NSEC(5000000005ull,1,1000000000ull) == 5);
	STEST(TIMESPEC_FROM_INT_RATIO_SEC(10*1000,2,3000) == 6);
	STEST(TIMESPEC_FROM_INT_RATIO_NSEC(10*1000,2,3000) == 666666666);

	struct timespec a = { 10, 10 };
	struct timespec b = { 20, 20 };
	struct timespec c = { 0, 1000000000 / 2 };
	struct timespec d = { 3, 0 };
	struct timespec one_and_half = {1, 1000000000 / 2};
	TIMESPEC_TEST(timespec_add(a, b), a.tv_sec + b.tv_sec, a.tv_nsec + b.tv_nsec);
	TIMESPEC_TEST(timespec_sub(b, a), b.tv_sec - a.tv_sec, b.tv_nsec - a.tv_nsec);
	TIMESPEC_TEST(timespec_mul(a, 2), a.tv_sec * 2, a.tv_nsec * 2);
	TIMESPEC_TEST(timespec_mul(c, 2), 1, 0);
	TIMESPEC_TEST(timespec_mul(c, 3), 1, 500000000);
	TIMESPEC_TEST(timespec_div(a, 2), a.tv_sec / 2, a.tv_nsec / 2);
	TIMESPEC_TEST(timespec_div(d, 2), 1, 500000000);
	TIMESPEC_TEST(timespec_mul_d(b, 1.5), 30, 30);
	TIMESPEC_TEST(timespec_mul_d(c, 2), 1, 0);
	TIMESPEC_TEST(timespec_mul_d(c, 2.5), 1, 250000000);
	TIMESPEC_TEST(timespec_div_d(d, 2), 1, 500000000);
	TIMESPEC_TEST(timespec_div_d(d, 1.5), 2, 0);
	TIMESPEC_TEST(timespec_div_d(d, 2.5), 1, 199999999);

	TEST(timespec_to_s_d(one_and_half) == 1.5);
	TIMESPEC_TEST(timespec_from_s_d(1.5), 1, 1000000000 / 2);

	TEST(timespec_to_clock(one_and_half) == 1.5 * CLOCKS_PER_SEC);
	TIMESPEC_TEST(timespec_from_clock(1.5 * CLOCKS_PER_SEC), 1, 1000000000 / 2);

	struct timespec e = timespec_normalize((struct timespec){0, UINT_MAX});
	TEST(-1 <= e.tv_nsec && e.tv_nsec <= 1000000000);
	struct timespec f = timespec_normalize((struct timespec){UINT_MAX, INT_MIN});
	TEST(-1 <= f.tv_nsec && f.tv_nsec <= 1000000000);

	return 0;
}

