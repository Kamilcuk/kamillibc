/*
 * tm.h
 *
 *  Created on: 13 kwi 2018
 *      Author: kamil
 */

#ifndef SRC_LIB_TIME_EX_TM_H_
#define SRC_LIB_TIME_EX_TM_H_

#include <time.h>
#include <string.h>

static inline void tm_normalize(struct tm *tm) {
	(void)mktime(tm);
}
static inline void tm_add_tm(struct tm *tm, struct tm tm2) {
	tm->tm_sec += tm2.tm_sec;
	tm->tm_min += tm2.tm_min;
	tm->tm_hour += tm2.tm_hour;
	tm->tm_mday += tm2.tm_mday;
	tm->tm_mon += tm2.tm_mon;
	tm->tm_year += tm2.tm_year;
	tm_normalize(tm);
}
static inline void tm_add_time_t(struct tm *tm, time_t t) {
	struct tm tm2;
#ifdef __USE_POSIX
	(void)gmtime_r(&t, &tm2);
#else
	(void)memcpy(&tm2, gmtime(&t), sizeof(tm2));
#endif
	tm_add_tm(tm, tm2);
}

#endif /* SRC_LIB_TIME_EX_TM_H_ */
