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
#undef static_assert
#define _static_assert_2(expr, msg)       _Static_assert(expr, msg)
#define _static_assert_1(expr)            _static_assert_2(expr, "")
#define _static_assert_N(_1, _2, N, ...)  _static_assert_##N
#define static_assert(expr, ...)          _static_assert_N(expr, ##__VA_ARGS__, 2, 1)(expr, ##__VA_ARGS__)
#else
#define static_assert(expr, ...)  ((void)sizeof(char[1 - 2*!!(expr)]))
#endif

/**
 * Compile time assertion, same as __CTASSERT in cdefs.h
 */
#define STATIC_ASSERT(condition)  typedef char[1 - 2*!!(condition)] __CONCAT(__static_assert, __LINE__)

/**
 * Used in static initializers
 */
#define STATIC_ASSERT_IF(condition,true,false)  (sizeof(char[(condition)?1:-1])?(true):(false))

/**
 * Check if pnt is declared as pointer or as array
 * Usage:
 * int a[5], *b = &a;
 * STATIC_ASSERT_STATIC_ARRAY(a); // ok
 * STATIC_ASSERT_STATIC_ARRAY(b); // error
 */
#define STATIC_ASSERT_STATIC_ARRAY(pnt)  STATIC_ASSERT((void*)&(pnt)[0] == (void*)&(pnt)))

#endif /* SRC_STATIC_ASSERT_H_ */
