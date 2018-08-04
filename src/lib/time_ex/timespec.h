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
#include <time.h>
#include <assert.h>

/**
 * Converts seconds number (sec) and nanoseconds number (nsec)
 * into value expressed in (numerator)/(denominator) seconds
 */
#define TIMESPEC_TO_INT_RATIO_SEC_NSEC(sec, nsec, numerator, denominator) \
	( EXTIME_SEC_TO_INT_RATIO((sec),(numerator),(denominator)) + \
			EXTIME_SUBSEC_TO_INT_RATIO((nsec),1000000000LL,(numerator),(denominator)))

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
	EXTIME_SUBSEC_FROM_INT_RATIO((value),1000000000LL,(numerator),(denominator))

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


static inline time_ns_t timespec_to_ns(struct timespec ts) {
	return TIMESPEC_TO_INT_RATIO(ts, 1LL, 1000000000LL);
}

static inline struct timespec timespec_from_ns(time_ns_t v) {
	return (struct timespec)TIMESPEC_FROM_INT_RATIO(v, 1, 1000000000l);
}

static inline time_us_t timespec_to_us(struct timespec ts) {
	return TIMESPEC_TO_INT_RATIO(ts, 1, 1000000l);
}

static inline struct timespec timespec_from_us(time_us_t v) {
	return (struct timespec)TIMESPEC_FROM_INT_RATIO(v, 1, 1000000l);
}

static inline time_ms_t timespec_to_ms(struct timespec ts) {
	return TIMESPEC_TO_INT_RATIO(ts, 1, 1000);
}

static inline struct timespec timespec_from_ms(time_ms_t v) {
	return (struct timespec)TIMESPEC_FROM_INT_RATIO(v, 1, 1000);
}

static inline clock_t timespec_to_clock(struct timespec ts) {
	return TIMESPEC_TO_INT_RATIO(ts, 1, CLOCKS_PER_SEC);
}

static inline struct timespec timespec_from_clock(clock_t v) {
	return (struct timespec)TIMESPEC_FROM_INT_RATIO(v, 1, CLOCKS_PER_SEC);
}

static inline struct timespec timespec_normalize(struct timespec ts)
{
	while (ts.tv_nsec < 0) {
		ts.tv_nsec += 1000000000LL;
		--ts.tv_sec;
	}
	while (ts.tv_nsec >= 1000000000LL) {
		ts.tv_nsec -= 1000000000LL;
		++ts.tv_sec;
	}
	return ts;
}

#endif /* SRC_LIB_TIME_EX_TIMESPEC_H_ */
