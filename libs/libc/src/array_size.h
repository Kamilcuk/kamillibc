/*
 * array_size.h
 *
 *  Created on: 10 sie 2018
 *      Author: kamil
 */

#ifndef SRC_LIB_ARRAY_SIZE_H_
#define SRC_LIB_ARRAY_SIZE_H_

#include <build_bug.h>

#define ARRAY_SIZE(x)  \
	((sizeof(x) / sizeof(x[0])) \
			+ BUILD_BUG_ON_ZERO(__builtin_types_compatible_p(__typeof__(x), __typeof__(&x[0]))))

#endif /* SRC_LIB_ARRAY_SIZE_H_ */
