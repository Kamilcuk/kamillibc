/*
 * internal.h
 *
 *  Created on: 13 kwi 2018
 *      Author: kamil
 */

#ifndef SRC_LIB_TIME_EX_INTERNAL_H_
#define SRC_LIB_TIME_EX_INTERNAL_H_

#include <time_ex/types.h>
#include <assert.h>
#include <limits.h>
#include <minmaxof.h>

/* Conversion from RATIO to seconds */

#define EXTIME_ASSERT_MUL(a, b) \
	assert( b > 0 && a > maxof(b) / b )

#define EXTIME_SEC_TO_INT_RATIO(sec, numerator, denominator) \
	( (sec) * (denominator) / (numerator) )

#define EXTIME_SEC_FROM_INT_RATIO(sec, numerator, denominator) \
	( (sec) * (numerator) / (denominator) )

#define EXTIME_SUBSEC_TO_INT_RATIO(subsec, subsecdenominator, numerator, denominator) \
	( EXTIME_SEC_TO_INT_RATIO((subsec), (numerator), (denominator)) / (subsecdenominator) )

#define EXTIME_SUBSEC_FROM_INT_RATIO(value, subsecdenominator, numerator, denominator) \
	( (value) % (denominator) * (numerator) * (subsecdenominator) / (denominator) )

/* Just a generic template, made for fun, unused */

#define EXTIME_DECLARE(SUFFIX,TYPE) \
	\
	static inline TYPE EXTIME_SEC_TO_INT_RATIO_##SUFFIX \
		(TYPE sec, TYPE numerator, TYPE denominator) \
	{ \
		EXTIME_ASSERT_NO_MUL_OVERFLOW(sec, denominator); \
		return EXTIME_SEC_TO_INT_RATIO(sec, numerator, denominator); \
	} \
		\
	static inline TYPE EXTIME_SEC_FROM_INT_RATIO_##SUFFIX \
		(TYPE sec, TYPE numerator, TYPE denominator) \
	{ \
		EXTIME_ASSERT_NO_MUL_OVERFLOW(sec, numerator); \
		return EXTIME_SEC_FROM_INT_RATIO(sec, numerator, denominator); \
	} \
		\
	static inline TYPE EXTIME_SUBSEC_TO_INT_RATIO_##SUFFIX \
		(TYPE subsec, TYPE subsecdenominator, TYPE numerator, TYPE denominator) \
	{ \
		return EXTIME_SEC_TO_INT_RATIO_##SUFFIX(subsec, numerator, denominator) / subsecdenominator; \
	} \
		\
	static inline TYPE EXTIME_SUBSEC_FROM_INT_RATIO_##SUFFIX \
		(TYPE value, TYPE subsecdenominator, TYPE numerator, TYPE denominator) \
	{ \
		const TYPE temp = value % denominator; \
		EXTIME_ASSERT_NO_MUL_OVERFLOW(temp, numerator); \
		const TYPE temp2 = temp * numerator; \
		EXTIME_ASSERT_NO_MUL_OVERFLOW(temp2, subsecdenominator); \
		const TYPE temp3 = temp2 * subsecdenominator; \
		return temp3 / denominator; \
	}

#endif /* SRC_LIB_TIME_EX_INTERNAL_H_ */
