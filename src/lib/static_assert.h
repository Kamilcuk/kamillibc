/*
 * static_assert.h
 *
 *  Created on: 17 mar 2018
 *      Author: kamil
 */

#ifndef SRC_STATIC_ASSERT_H_
#define SRC_STATIC_ASSERT_H_

#include <assert.h>

/**
 * Compile time assertion, same as __CTASSERT in cdefs.h
 */
#define STATIC_ASSERT(condition)  typedef char[1 - 2*!!(condition)] __CONCAT(__static_assert, __LINE__)

/**
 * Used in static initializers
 */
#define STATIC_ASSERT_IF(condition,true,false)  (sizeof(char[(condition)?1:-1])?(true):(false))

#endif /* SRC_STATIC_ASSERT_H_ */
