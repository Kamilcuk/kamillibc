/*
 * clock.h
 *
 *  Created on: 13 kwi 2018
 *      Author: kamil
 */

#ifndef SRC_LIB_TIME_EX_CLOCK_H_
#define SRC_LIB_TIME_EX_CLOCK_H_

#include <time_ex/internal.h>
#include <time_ex/types.h>
#include <time.h>

#define CLOCK_TO_INT_RATIO(clock_value, numerator, denominator) \
	(EXTIME_SEC_FROM_INT_RATIO((clock_value) * (CLOCKS_PER_SEC), (numerator), (denominator)))

#define CLOCK_FROM_INT_RATIO(value, numerator, denominator) \
	(EXTIME_SEC_TO_INT_RATIO((value), (numerator), (denominator)) / (CLOCKS_PER_SEC))

static inline clock_t clock_from_ms(time_ms_t ms) __pure2;
static inline clock_t clock_from_ms(time_ms_t ms) {
	return CLOCK_TO_INT_RATIO(ms, 1, 1000);
}
static inline time_ms_t clock_to_ms(clock_t t) __pure2;
static inline time_ms_t clock_to_ms(clock_t t) {
	return CLOCK_FROM_INT_RATIO(t, 1, 1000);
}


#endif /* SRC_LIB_TIME_EX_CLOCK_H_ */
