/*
 * time-ex.h
 *
 *  Created on: 4 mar 2018
 *      Author: kamil
 */

#ifndef SRC_TIME_EX_H_
#define SRC_TIME_EX_H_

#include <time.h>
#include <stdint.h>
#include <assert.h>

#define CLOKS_PER_MILISEC   CLOCKS_PER_SEC
#define CLOKS_PER_MICROSEC  CLOCKS_PER_SEC
#define CLOKS_PER_NANOSEC   CLOCKS_PER_SEC

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
	(void)gmtime_r(&t, &tm2);
	tm_add_tm(tm, tm2);
}

#define TIMESPEC_TO_INT(ts, ns)  ( (ts).tv_sec * 1000000000ull / (ns) + (ts).tv_nsec / (ns) )
#define INT_TO_TIMESPEC(v, ns)   { \
	.tv_sec =   (v) / ( 1000000000ull * ns ), \
	.tv_nsec = ((v) % ( 1000000000ull * ns )) * ns , \
	}

static inline uint64_t timespec_to_ns(struct timespec ts)
{
	return TIMESPEC_TO_INT(ts, 1);
}

static inline struct timespec ns_to_timespec(uint64_t v)
{
	return (struct timespec)INT_TO_TIMESPEC(v, 1);
}

static inline uint64_t timespec_to_us(struct timespec ts)
{
	return TIMESPEC_TO_INT(ts, 1000ull);
}

static inline struct timespec us_to_timespec(uint64_t v)
{
	return (struct timespec)INT_TO_TIMESPEC(v, 1000ull);
}

static inline uint64_t timespec_to_ms(struct timespec ts)
{
	return TIMESPEC_TO_INT(ts, 1000000ull);
}

static inline struct timespec ms_to_timespec(uint64_t v)
{
	return (struct timespec)INT_TO_TIMESPEC(v, 1000000ull);
}

static inline clock_t timespec_to_clock(struct timespec ts)
{
	return ts.tv_sec * CLOCKS_PER_SEC
			+ ts.tv_nsec * CLOCKS_PER_SEC / 1000000000ull;
}

static inline struct timespec clock_to_timespec(clock_t v)
{
	return (struct timespec){
		.tv_sec = v / CLOCKS_PER_SEC,
		.tv_nsec = (v % CLOCKS_PER_SEC) * 1000000000ull / CLOCKS_PER_SEC,
	};
}

static inline void timespec_normalize(struct timespec *ts)
{
	while(ts->tv_nsec < 0) {
		ts->tv_nsec += 1000000000ll;
		if (ts->tv_sec == 0) assert(0);
		--ts->tv_sec;
	}
	while(ts->tv_nsec >= 1000000000ll) {
		ts->tv_nsec -= 1000000000ll;
		++ts->tv_sec;
	}
}

#ifdef _POSIX_TIMERS

static inline uint64_t timer_gettime_ns(timer_t timerid, uint64_t *interval)
{
	struct itimerspec value;
	if ( timer_gettime(timerid, &value) ) assert(0);
	if (interval) *interval = timespec_to_ns(value.it_interval);
	return timespec_to_ns(value.it_value);
}

static inline int timer_settime_ns(timer_t timerid, uint64_t value, uint64_t interval)
{
	struct itimerspec it = {
			.it_value = ns_to_timespec(value),
			.it_interval = ns_to_timespec(interval),
	};
	return timer_settime(timerid, 0, &it, NULL);
}

static inline int timer_settime_ms(timer_t timerid, uint32_t value, uint32_t interval)
{
	return timer_settime_ns(timerid, (uint64_t)value * 1000000ull, (uint64_t)interval * 1000000ull);
}

#endif //_POSIX_TIMERS

static inline int _time_ex_unittest()
{
	assert(TIMESPEC_TO_INT(((struct timespec){.tv_sec=1, .tv_nsec=0}), 1) == 1000000000ull);
	assert(TIMESPEC_TO_INT(((struct timespec){.tv_sec=0, .tv_nsec=1}), 1) == 1);
	assert(TIMESPEC_TO_INT(((struct timespec){.tv_sec=1, .tv_nsec=0}), 1000) == 1000000);
	assert(TIMESPEC_TO_INT(((struct timespec){.tv_sec=0, .tv_nsec=1000}), 1000) == 1);
	assert(((struct timespec)INT_TO_TIMESPEC(1000000000ull, 1)).tv_sec == 1);
	assert(((struct timespec)INT_TO_TIMESPEC(1000000000ull, 1)).tv_nsec == 0);
	assert(((struct timespec)INT_TO_TIMESPEC(1000000, 1000)).tv_sec == 1);
	assert(((struct timespec)INT_TO_TIMESPEC(1000, 1000)).tv_nsec == 1000);
	return 0;
}

#endif /* SRC_TIME_EX_H_ */
