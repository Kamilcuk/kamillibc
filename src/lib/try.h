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
#define _TRY_FAILED(expr)  ( \
		assert(expr), \
		(void)fprintf(stderr, "Try %s failed\n", #expr), \
		abort() \
		)
#endif

#define try(expr)                ((expr)?((void)0):_TRY_FAILED(expr))
#define try_msg(expr, msg, ...)  ((expr)?((void)0):(fprintf(stderr, msg, ##__VA_ARGS__),_TRY_FAILED(expr)))
#define try_ret(expr, val)       do{ if(!(expr)) { return(val); } }while(0)

#endif /* SRC_TRY_H_ */
