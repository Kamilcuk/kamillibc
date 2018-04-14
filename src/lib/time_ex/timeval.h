/*
 * timeval.h
 *
 *  Created on: 13 kwi 2018
 *      Author: kamil
 */

#ifndef SRC_LIB_TIME_EX_TIMEVAL_H_
#define SRC_LIB_TIME_EX_TIMEVAL_H_

#include <time_ex/internal.h>
#include <time.h>

#define TIMEVAL_TO_INT_RATIO_SEC_USEC(sec, usec, numerator, denominator) \
	( EXTIME_SEC_TO_INT_RATIO((sec),(numerator),(denominator)) + \
			EXTIME_SUBSEC_TO_INT_RATIO((usec),1000000L,(numerator),(denominator)))

#define TIMEVAL_FROM_INT_RATIO_SEC(value, numerator, denominator) \
	EXTIME_SEC_FROM_INT_RATIO(value, numerator, denominator)

#define TIMEVAL_FROM_INT_RATIO_USEC(value, numerator, denominator) \
	EXTIME_SUBSEC_FROM_INT_RATIO((value),1000000L,(numerator),(denominator))

#define TIMEVAL_TO_INT_RATIO(timeval, numerator, denominator) \
	TIMEVAL_TO_INT_RATIO_SEC_USEC((timeval).tv_sec, (timeval).tv_usec, numerator, denominator)

#define TIMEVAL_FROM_INT_RATIO(value, numerator, denominator) (struct timeval){ \
	.tv_sec =  TIMEVAL_FROM_INT_RATIO_SEC(value, numerator, denominator), \
	.tv_usec = TIMEVAL_FROM_INT_RATIO_USEC(value, numerator, denominator), \
	}

static inline clock_t timeval_to_clock(struct timeval tv) {
	return TIMEVAL_TO_INT_RATIO(tv, 1, CLOCKS_PER_SEC);
}
static inline struct timeval timeval_from_clock(clock_t v) {
	return TIMEVAL_FROM_INT_RATIO(v, 1, CLOCKS_PER_SEC);
}


#endif /* SRC_LIB_TIME_EX_TIMEVAL_H_ */
