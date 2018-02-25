/*
 * contract.h
 *
 *  Created on: 30 sty 2018
 *      Author: kamil
 */

#ifndef QQ_CONTRACT_H_
#define QQ_CONTRACT_H_

#include <assert.h>

#define CONTRACT_assert(expr)         assert(expr)
#define CONTRACT_assert_nonnull(ptr)  CONTRACT_assert( (ptr) != NULL )

#define CONTRACT_require(expr)                 /* statically analyze if possible if expr is true */
#define CONTRACT_require_return(expr,retval)   /* if expr is false, return retval */
#define CONTRACT_require_nonnull(...)          __attribute__((__nonnull__(__VA_ARGS__)))
#define CONTRACT_require_nonnull_return(ptr,retval)

#endif /* QQ_CONTRACT_H_ */
