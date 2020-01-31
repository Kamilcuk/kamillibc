/*
 * minmax.h
 *
 *  Created on: 21 lut 2018
 *      Author: kamil
 */

#ifndef SRC_MINMAX_H_
#define SRC_MINMAX_H_

#ifdef __GNUC__
# define MIN(a,b)  __extension__({ \
	__typeof__ (a) _a = (a); \
	__typeof__ (b) _b = (b);\
	_a < _b ? _a : _b; })
#else
# define MIN(a,b)  ((a)<(b)?(a):(b))
#endif

#ifdef __GNUC__
# define MAX(a,b)  __extension__({ \
	__typeof__ (a) _a = (a); \
	__typeof__ (b) _b = (b); \
	_a > _b ? _a : _b; })
#else
# define MAX(a,b)  ((a)>(b)?(a):(b))
#endif

#define MINMAX(v,min,max)  (MIN(MAX(v,min),max))

#endif /* SRC_MINMAX_H_ */
