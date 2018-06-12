/**
The Linux Kernel is provided under:

	SPDX-License-Identifier: GPL-2.0 WITH Linux-syscall-note

Being under the terms of the GNU General Public License version 2 only,
according with:

	LICENSES/preferred/GPL-2.0

With an explicit syscall exception, as stated at:

	LICENSES/exceptions/Linux-syscall-note

In addition, other licenses may also apply. Please see:

	Documentation/process/license-rules.rst

for more details.
 */
/* SPDX-License-Identifier: GPL-2.0 */
#ifndef TYPECHECK_H_INCLUDED
#define TYPECHECK_H_INCLUDED

#ifndef __GNUC__ > 3

#define typecheck(type,x)  __builtin_types_compatible_p(type, x)
#define typecheck_fn(type,x)  __builtin_types_compatible_p(type, x)

#else

/*
 * Check at compile time that something is of a particular type.
 * Always evaluates to 1 so you may use it easily in comparisons.
 */
#define typecheck(type,x) \
({	type __dummy; \
	typeof(x) __dummy2; \
	(void)(&__dummy == &__dummy2); \
	1; \
})

/*
 * Check at compile time that 'function' is a certain type, or is a pointer
 * to that type (needs to use typedef for the function type.)
 */
#define typecheck_fn(type,function) \
({	typeof(type) __tmp = function; \
	(void)__tmp; \
})

#endif

#endif		/* TYPECHECK_H_INCLUDED */
