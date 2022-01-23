/*
 * timespec.h
 *
 *  Created on: 13 kwi 2018
 *      Author: kamil
 */

#ifndef SRC_LIB_TIME_EX_TIMESPEC_H_
#define SRC_LIB_TIME_EX_TIMESPEC_H_

#include <time_ex/internal.h>
#include <time_ex/types.h>

#include <sys/time.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>
#include <assert.h>
#include <stdio.h>

/**
 * Converts seconds number (sec) and nanoseconds number (nsec)
 * into value expressed in (numerator)/(denominator) seconds
 */
#define TIMESPEC_TO_INT_RATIO_SEC_NSEC(sec, nsec, numerator, denominator) \
	( EXTIME_SEC_TO_INT_RATIO((sec), (numerator), (denominator)) + \
			EXTIME_SUBSEC_TO_INT_RATIO((nsec), 1000000000ll, (numerator), (denominator)))

/**
 * Converts (value) expressed in (numerator)/(denominator) seconds
 * into seconds
 */
#define TIMESPEC_FROM_INT_RATIO_SEC(value, numerator, denominator) \
	EXTIME_SEC_FROM_INT_RATIO(value, numerator, denominator)

/**
 * Converts (value) expressed in (numerator)/(denominator) seconds
 * into nanoseconds
 */
#define TIMESPEC_FROM_INT_RATIO_NSEC(value, numerator, denominator) \
	EXTIME_SUBSEC_FROM_INT_RATIO((value), 1000000000ll, (numerator), (denominator))

/**
 * Converts timespec structure
 * into value expressed in (numerator)/(denominator) seconds
 */
#define TIMESPEC_TO_INT_RATIO(timespec, numerator, denominator) \
	TIMESPEC_TO_INT_RATIO_SEC_NSEC((timespec).tv_sec, (timespec).tv_nsec, numerator, denominator)

/**
 * Creates a timespec structure declaration
 * from (value) expressed in (numerator)/(denominator) seconds
 */
#define TIMESPEC_FROM_INT_RATIO(value, numerator, denominator) { \
	.tv_sec =  TIMESPEC_FROM_INT_RATIO_SEC(value, numerator, denominator), \
	.tv_nsec = TIMESPEC_FROM_INT_RATIO_NSEC(value, numerator, denominator), \
	}

static inline bool timespec_isValid(struct timespec ts)
{
	return ts.tv_sec >= 0 && (0 <= ts.tv_nsec || ts.tv_nsec <= 1000000000);
}

static inline struct timespec timespec_normalize(struct timespec ts)
{
	while (ts.tv_nsec < 0) {
		ts.tv_nsec += 1000000000ll;
		--ts.tv_sec;
	}
	while (ts.tv_nsec >= 1000000000ll) {
		ts.tv_nsec -= 1000000000ll;
		++ts.tv_sec;
	}
	return ts;
}

static inline int timespec_print(struct timespec t)
{
	return printf("%ld.%09ld", (long)t.tv_sec, (long)t.tv_nsec);
}

static inline int timespec_println(struct timespec t)
{
	for (int ret = 0; (ret = timespec_print(t)) < 0;) return ret;
	return puts("");
}


static inline time_ns_t timespec_to_ns(struct timespec ts) {
	return TIMESPEC_TO_INT_RATIO(ts, 1ll, 1000000000ll);
}

static inline struct timespec timespec_from_ns(time_ns_t v) {
	struct timespec ret = TIMESPEC_FROM_INT_RATIO(v, 1, 1000000000l);
	return ret;
}

static inline time_us_t timespec_to_us(struct timespec ts) {
	return TIMESPEC_TO_INT_RATIO(ts, 1, 1000000l);
}

static inline struct timespec timespec_from_us(time_us_t v) {
	struct timespec ret = TIMESPEC_FROM_INT_RATIO(v, 1, 1000000l);
	return ret;
}

static inline time_ms_t timespec_to_ms(struct timespec ts) {
	return TIMESPEC_TO_INT_RATIO(ts, 1, 1000);
}

static inline struct timespec timespec_from_ms(time_ms_t v) {
	struct timespec ret = TIMESPEC_FROM_INT_RATIO(v, 1, 1000);
	return ret;
}

static inline clock_t timespec_to_clock(struct timespec ts) {
	return TIMESPEC_TO_INT_RATIO(ts, 1, CLOCKS_PER_SEC);
}

static inline struct timespec timespec_from_clock(clock_t v) {
	struct timespec ret = TIMESPEC_FROM_INT_RATIO(v, 1, CLOCKS_PER_SEC);
	return ret;
}

static inline double timespec_to_s_d(struct timespec ts)
{
	return (double)ts.tv_sec + ((double)ts.tv_nsec / 1e9);
}

static inline struct timespec timespec_from_s_d(double s)
{
	struct timespec ret = { .tv_sec = s };
	ret.tv_nsec = (s - ret.tv_sec) * 1E9;
	return timespec_normalize(ret);
}

static inline struct timespec timespec_init(time_t tv_sec, long int tv_nsec)
{
	assert(0 <= tv_nsec && tv_nsec <= 1000000000);
	struct timespec ret = { tv_sec, tv_nsec };
	return ret;
}

static inline struct timespec timespec_add(struct timespec a, struct timespec b)
{
	a.tv_sec += b.tv_sec;
	a.tv_nsec += b.tv_nsec;
	if (a.tv_nsec >= 1000000000) {
		a.tv_nsec -= 1000000000;
		++a.tv_sec;
	}
	return a;
}


static inline bool timespec_ckd_add(struct timespec *r, struct timespec a, struct timespec b)
{
	assert(a.tv_nsec < 1000000000);
	assert(b.tv_nsec < 1000000000);
	a.tv_sec += b.tv_sec;
	a.tv_nsec += b.tv_nsec;
	if (a.tv_nsec >= 1000000000) {
		a.tv_nsec -= 1000000000;
		if (ckd_add(&a.tv_sec, a.tv_sec, 1)) {
			return 1;
		}
	}
	*r = a;
	return 0;
}
static inline struct timespec timespec_sub(struct timespec a, struct timespec b)
{
	assert(timespec_isValid(a));
	assert(timespec_isValid(b));
	assert(a.tv_sec > b.tv_sec);
	a.tv_sec -= b.tv_sec;
	if (a.tv_nsec < b.tv_nsec) {
		assert(a.tv_sec > 0);
		--a.tv_sec;
		a.tv_nsec += 1000000000;
	}
	a.tv_nsec -= b.tv_nsec;
	return a;
}

static inline struct timespec timespec_mul(struct timespec a, int i)
{
	struct timespec in = a;
	bool minus = i < 0;
	i = i < 0 ? -i : i;
	while (--i) {
		a = timespec_add(a, in);
	}
	if (minus) {
		a.tv_sec *= -1;
		a.tv_nsec *= -1;
	}
	return a;
}

static inline struct timespec timespec_div(struct timespec a, int i)
{
	assert(i);
	a.tv_nsec += (a.tv_sec % i) * 1000000000;
	a.tv_sec /= i;
	a.tv_nsec /= i;
	return timespec_normalize(a);
}

static inline struct timespec timespec_mul_d(struct timespec a, double i)
{
	struct timespec in = a;
	a = timespec_mul(a, (int)i);
	i = i - (int)i;
	i = i < 0 ? -i : i;
	a.tv_sec += in.tv_sec * i;
	a.tv_nsec += in.tv_nsec * i;
	return timespec_normalize(a);
}

static inline struct timespec timespec_div_d(struct timespec a, double i)
{
	double tmp;
	tmp = (double)a.tv_sec / i;
	a.tv_sec = tmp;
	a.tv_nsec /= i;
	a.tv_nsec += (tmp - a.tv_sec) * 1e9;
	return timespec_normalize(a);
}

static inline bool timespec_cmp_eq(struct timespec a, struct timespec b)
{
	return a.tv_sec == b.tv_sec && a.tv_nsec == b.tv_nsec;
}

static inline bool timespec_cmp_ge(struct timespec a, struct timespec b)
{
	return a.tv_sec > b.tv_sec || (a.tv_sec == b.tv_sec && a.tv_nsec >= b.tv_nsec);
}

static inline bool timespec_cmp_gt(struct timespec a, struct timespec b)
{
	return a.tv_sec > b.tv_sec || (a.tv_sec == b.tv_sec && a.tv_nsec > b.tv_nsec);
}

static inline bool timespec_cmp_le(struct timespec a, struct timespec b)
{
	return a.tv_sec < b.tv_sec || (a.tv_sec == b.tv_sec && a.tv_nsec <= b.tv_nsec);
}

static inline bool timespec_cmp_lt(struct timespec a, struct timespec b)
{
	return a.tv_sec < b.tv_sec || (a.tv_sec == b.tv_sec && a.tv_nsec < b.tv_nsec);
}

static inline bool timespec_cmp_ne(struct timespec a, struct timespec b)
{
	return a.tv_sec != b.tv_sec && a.tv_nsec != b.tv_nsec;
}

#endif /* SRC_LIB_TIME_EX_TIMESPEC_H_ */
