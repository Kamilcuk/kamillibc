/*
 * nonnull.h
 *
 *  Created on: 30 sty 2018
 *      Author: kamil
 */

#ifndef QQ_NONNULL_H_
#define QQ_NONNULL_H_

#include <assert.h>
#include <stddef.h>

#define CONTRACT_nonnull(ptr)  ( assert(ptr != NULL), ptr )

#endif /* QQ_NONNULL_H_ */
