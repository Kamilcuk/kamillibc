/*
 * check_overflow.h
 *
 *  Created on: 13 kwi 2018
 *      Author: kamil
 */

#ifndef SRC_LIB_TIME_EX_CHECK_OVERFLOW_H_
#define SRC_LIB_TIME_EX_CHECK_OVERFLOW_H_

#include <assert.h>

#ifdef __GNUC__
#ifndef EXTIME_ASSERT_NO_MUL_OVERFLOW
#define EXTIME_ASSERT_NO_MUL_OVERFLOW(a,b) \
		( assert( !__builtin_mul_overflow_p(a, b, (__typeof__((a)+(b))) 0) ) )
#else
#define EXTIME_ASSERT_NO_MUL_OVERFLOW(a,b) (assert((a)<((a)*(b))&&(b)<((a)*(b))))
#endif
#endif

#endif /* SRC_LIB_TIME_EX_CHECK_OVERFLOW_H_ */
