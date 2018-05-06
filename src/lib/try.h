/*
 * try.h
 *
 *  Created on: 13 mar 2018
 *      Author: Kamil Cukrowski
 *     License: All rights reserved. Copyright by Netemera(C).
 */

#ifndef SRC_TRY_H_
#define SRC_TRY_H_

#include <stdio.h>

#ifndef _TRY_FAILED
#include <stdlib.h>
#include <assert.h>
#ifdef __GNUC__
#include <cdefs.h>
#define _TRY_FAILED(expr)  __extension__({ \
		if (__builtin_constant_p(expr)) { \
			__attribute__((__warning__( \
					"try \"" #expr "\" will fail in file " __FILE__ ", line " __XSTRING(__LINE__) \
			))) void __try_issue_warning(void) {} \
			__try_issue_warning(); \
		} \
		assert(expr), \
		(void)fprintf(stderr, "Try %s failed\n", #expr), \
		abort(); \
	})
#else

#endif
#endif

#define try(expr)                ((expr)?((void)0):_TRY_FAILED(expr))
#define try_msg(expr, msg, ...)  ((expr)?((void)0):(fprintf(stderr,(msg),##__VA_ARGS__),_TRY_FAILED(expr)))

#endif /* SRC_TRY_H_ */
