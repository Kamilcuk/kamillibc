/*
 * static_assert.h
 *
 *  Created on: 17 mar 2018
 *      Author: kamil
 */

#ifndef SRC_STATIC_ASSERT_H_
#define SRC_STATIC_ASSERT_H_

#include <sys/cdefs.h>

#if __STDC_VERSION__ >= 201112L
#include <assert.h>
#else
#define static_assert(condition, msg)  ((void)sizeof(char[1 - 2*!!(condition)]))
#endif

#define STATIC_ASSERT(condition)  typedef char[1 - 2*!!(condition)] __CONCAT(__static_assert, __LINE__)

#define STATIC_ASSERT_IF(condition,true,false)  (sizeof(char[(condition)?1:-1])?(true):(false))

#endif /* SRC_STATIC_ASSERT_H_ */
