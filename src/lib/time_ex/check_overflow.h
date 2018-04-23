/*
 * check_overflow.h
 *
 *  Created on: 13 kwi 2018
 *      Author: kamil
 */

#ifndef SRC_LIB_TIME_EX_CHECK_OVERFLOW_H_
#define SRC_LIB_TIME_EX_CHECK_OVERFLOW_H_

#include <assert.h>

#ifndef EXTIME_ASSERT_NO_MUL_OVERFLOW
# ifdef __GNUC__
#  if __GNUC__ >= 7
#   define EXTIME_ASSERT_NO_MUL_OVERFLOW(a,b) \
		( assert( !__builtin_mul_overflow_p(a, b, (__typeof__((a)+(b))) 0) ) )
#  else
#   define EXTIME_ASSERT_NO_MUL_OVERFLOW(a,b) \
		( assert( !__builtin_umulll_overflow((unsigned long long)a, (unsigned long long)b, NULL ) )
#  endif
# else
#  define EXTIME_ASSERT_NO_MUL_OVERFLOW(a,b) (assert((a)<((a)*(b))&&(b)<((a)*(b))))
# endif
#endif

#endif /* SRC_LIB_TIME_EX_CHECK_OVERFLOW_H_ */
