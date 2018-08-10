#ifndef TYPECHECK_H_INCLUDED
#define TYPECHECK_H_INCLUDED

#include <build_bug.h>
#define typecheck(x, y)  (!BUILD_BUG_ON_ZERO(!__builtin_types_compatible_p(__typeof__(x), __typeof__(y))))

#endif		/* TYPECHECK_H_INCLUDED */
