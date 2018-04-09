/*
 * time-ex.h
 *
 *  Created on: 4 mar 2018
 *      Author: kamil
 */

#ifndef SRC_TIME_EX_H_
#define SRC_TIME_EX_H_

#include <sys/types.h>
#include <sys/time.h>

#include <assert.h>
#include <time.h>
#include <stdint.h>
#include <string.h>

/* Private Macro Functions --------------------------------------------------------------- */

#define EXTIME_SEC_SUBSEC_TO_INT_RATIO(sec, subsec, subsecdenominator, numerator, denominator) \
	( (sec)  * (denominator) / (numerator) + (subsec) * (denominator) / (numerator) / (subsecdenominator) )

#define EXTIME_SEC_FROM_INT_RATIO(value, numerator, denominator) \
	( (value) / (denominator) * (numerator) )

#define EXTIME_SUBSEC_FROM_INT_RATIO(subsecdenominator, value, numerator, denominator) \
	( (value) % (denominator) * (numerator) * (subsecdenominator) / (denominator) )

/* Exported Macro Function TIMESPEC --------------------------------------------------------- */

/**
 * Converts seconds number (sec) and nanoseconds number (nsec)
 * into value expressed in (numerator)/(denominator) seconds
 */
#define TIMESPEC_TO_INT_RATIO_SEC_NSEC(sec, nsec, numerator, denominator) \
	EXTIME_SEC_SUBSEC_TO_INT_RATIO(sec, nsec, 1000000000ul, numerator, denominator)

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
	EXTIME_SUBSEC_FROM_INT_RATIO(1000000000ull, value, numerator, denominator)

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
#define TIMESPEC_FROM_INT_RATIO(value, numerator, denominator) (struct timespec){ \
	.tv_sec =  TIMESPEC_FROM_INT_RATIO_SEC(value, numerator, denominator), \
	.tv_nsec = TIMESPEC_FROM_INT_RATIO_NSEC(value, numerator, denominator), \
	}

/* Exported Macro Function TIMEVAL --------------------------------------------------------- */

#define TIMEVAL_TO_INT_RATIO_SEC_NSEC(sec, nsec, numerator, denominator) \
	EXTIME_SEC_SUBSEC_TO_INT_RATIO(sec, nsec, 1000000ul, numerator, denominator)

#define TIMEVAL_FROM_INT_RATIO_SEC(value, numerator, denominator) \
	EXTIME_SEC_FROM_INT_RATIO(value, numerator, denominator)

#define TIMEVAL_FROM_INT_RATIO_NSEC(value, numerator, denominator) \
	EXTIME_SUBSEC_FROM_INT_RATIO(1000000ul, value, numerator, denominator)

#define TIMEVAL_TO_INT_RATIO(timeval, numerator, denominator) \
	TIMEVAL_TO_INT_RATIO_SEC_NSEC((timeval).tv_sec, (timeval).tv_usec, numerator, denominator)

#define TIMEVAL_FROM_INT_RATIO(value, numerator, denominator) (struct timeval){ \
	.tv_sec =  TIMEVAL_FROM_INT_RATIO_SEC(value, numerator, denominator), \
	.tv_usec = TIMEVAL_FROM_INT_RATIO_NSEC(value, numerator, denominator), \
	}

/* Exported Types ------------------------------------------------------------------- */

#ifndef __time_ms_t_defined
typedef time_t time_ms_t;
#define __time_ms_t_defined 1
#endif

#ifndef __time_us_t_defined
typedef time_t time_us_t;
#define __time_us_t_defined 1
#endif

#ifndef __time_ns_t_defined
typedef unsigned long long time_ns_t;
#define __time_ns_t_defined 1
#endif

/* Exported static inline functions ------------------------------------------------- */

static inline void tm_normalize(struct tm *tm)
{
	(void)mktime(tm);
}

static inline void tm_add_tm(struct tm *tm, struct tm tm2)
{
	tm->tm_sec += tm2.tm_sec;
	tm->tm_min += tm2.tm_min;
	tm->tm_hour += tm2.tm_hour;
	tm->tm_mday += tm2.tm_mday;
	tm->tm_mon += tm2.tm_mon;
	tm->tm_year += tm2.tm_year;
	tm_normalize(tm);
}

static inline void tm_add_time_t(struct tm *tm, time_t t)
{
	struct tm tm2;
#ifdef __USE_POSIX
	(void)gmtime_r(&t, &tm2);
#else
	(void)memcpy(&tm2, gmtime(&t), sizeof(tm2));
#endif
	tm_add_tm(tm, tm2);
}

static inline uint64_t timespec_to_ns(struct timespec ts) {
	return TIMESPEC_TO_INT_RATIO(ts, 1ul, 1000000000ul);
}
static inline struct timespec timespec_from_ns(uint64_t v) {
	return TIMESPEC_FROM_INT_RATIO(v, 1ul, 1000000000ul);
}
static inline uint64_t timespec_to_us(struct timespec ts) {
	return TIMESPEC_TO_INT_RATIO(ts, 1ull, 1000000ull);
}
static inline struct timespec timespec_from_us(uint64_t v) {
	return TIMESPEC_FROM_INT_RATIO(v, 1ul, 1000000ul);
}
static inline uint64_t timespec_to_ms(struct timespec ts) {
	return TIMESPEC_TO_INT_RATIO(ts, 1u, 1000u);
}
static inline struct timespec timespec_from_ms(uint64_t v) {
	return TIMESPEC_FROM_INT_RATIO(v, 1u, 1000u);
}
static inline clock_t timespec_to_clock(struct timespec ts) {
	return ts.tv_sec * CLOCKS_PER_SEC
			+ ts.tv_nsec * CLOCKS_PER_SEC / 1000000000u;;
}
static inline struct timespec timespec_from_clock(clock_t v) {
	return (struct timespec){
		.tv_sec = v / CLOCKS_PER_SEC,
		.tv_nsec = (v % CLOCKS_PER_SEC) * 1000000000ul / CLOCKS_PER_SEC,
	};
}

static inline void timespec_normalize(struct timespec *ts)
{
	while(ts->tv_nsec < 0) {
		ts->tv_nsec += 1000000000l;
		if (ts->tv_sec == 0) assert(0);
		--ts->tv_sec;
	}
	while(ts->tv_nsec >= 1000000000l) {
		ts->tv_nsec -= 1000000000l;
		++ts->tv_sec;
	}
}

static inline clock_t timeval_to_clock(struct timeval tv) {
	return TIMEVAL_TO_INT_RATIO(tv, CLOCKS_PER_SEC, 1);
}
static inline struct timeval timeval_from_clock(clock_t v) {
	return TIMEVAL_FROM_INT_RATIO(v, CLOCKS_PER_SEC, 1);
}

/* Exported Functions ------------------------------------------------------ */

#if _POSIX_C_SOURCE >= 199309L
uint64_t timer_gettime_ns(timer_t timerid, uint64_t *interval);
int timer_settime_ns(timer_t timerid, uint64_t value, uint64_t interval);
uint32_t timer_gettime_ms(timer_t timerid, uint32_t *interval);
int timer_settime_ms(timer_t timerid, uint32_t value, uint32_t interval);
#endif

int _time_ex_unittest();

#endif /* SRC_TIME_EX_H_ */
