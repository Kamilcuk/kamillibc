/*
 * minmaxof.h
 *
 *  Created on: 17 mar 2018
 *      Author: kamil
 */

#ifndef SRC_MINMAXOF_H_
#define SRC_MINMAXOF_H_

#include <limits.h>

#define IS_SIGNED(t)  (((t)(-1)) < ((t)0))

#define MAX_OF(t)     (IS_SIGNED(t) ? (t)((1LLU<<((sizeof(t)*CHAR_BIT)-1))-1LLU) : (t)(~0))

#define MIN_OF(t)     (IS_SIGNED(t) ? (t)((~0LLU)-((1LLU<<((sizeof(t)*CHAR_BIT)-1))-1LLU)) : (t)(0))

#endif /* SRC_MINMAXOF_H_ */
