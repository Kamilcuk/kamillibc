/*
 * minmax.h
 *
 *  Created on: 21 lut 2018
 *      Author: kamil
 */

#ifndef SRC_MINMAX_H_
#define SRC_MINMAX_H_

#ifdef __GNUC__
# define MIN(a,b)  ({ __typeof__ (a) _a = (a); __typeof__ (b) _b = (b); _a < _b ? _a : _b; })
#else
# define MIN(a,b)  ((a)<(b)?(a):(b))
#endif

#ifdef __GNUC__
# define MAX(a,b)  ({ __typeof__ (a) _a = (a); __typeof__ (b) _b = (b); _a > _b ? _a : _b; })
#else
# define MAX(a,b)  ((a)>(b)?(a):(b))
#endif

#ifdef __GNUC__
# define MIN3(a,b,c)  ({ __typeof__ (a) _a = (a); __typeof__ (b) _b = (b); _a < _b ? MIN(_a,c) : MIN(_b,c); })
#else
# define MIN3(a,b,c)  ((a)<(b)?MIN((a),(c)):MIN((b),(c)))
#endif

#ifdef __GNUC__
# define MAX3(a,b,c)  ({ __typeof__ (a) _a = (a); __typeof__ (b) _b = (b); _a > _b ? MAX(_a,c) : MAX(_b,c); })
#else
# define MAX3(a,b,c)  ((a)>(b)?MAX((a),(c)):MAX((b),(c)))
#endif

#endif /* SRC_MINMAX_H_ */
