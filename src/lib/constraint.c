/*
 * constraint.c
 *
 *  Created on: 19 mar 2018
 *      Author: kamil
 */

#include <constraint.h>

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#if 0 && __STDC_VERSION__ >= 201112L && !defined(__STDC_NO_THREADS__)
#include <threads.h>
#endif

/* Private Variables ---------------------------------------------------------- */

#if 0 && __STDC_VERSION__ >= 201112L && !defined(__STDC_NO_THREADS__)
thread_local
#endif
#if __GNUC__
__thread
#endif
constraint_handler_t constraint_handler;

/* Exported Functions --------------------------------------------------------- */

void constraint_printf_handler(const char * restrict msg, int error,
		const char * restrict assertion, const char * restrict file,
		unsigned int line, const char * restrict function)
{
#if defined(__GNUC__) && _POSIX_C_SOURCE >= 200112L
	char errnostr[128];
	if (errno != 0)
		(void)strerror_r(errno, errnostr, sizeof(errnostr));
#elif __STDC_VERSION__ >= 201112L && defined(__STDC_LIB_EXT1__) && __STDC_WANT_LIB_EXT1__ == 1
	char errnostr[128];
	if (errno != 0)
		(void)strerror_s(errnostr, sizeof(errnostr), errno);
#else
	const char *errnostr;
	if (errno != 0)
		errnostr = strerror(errno);
#endif
	(void)fprintf(stderr,
			"%s:%u: %s%sConstraint `%s' failed.\n"
			"%s%s%s\n"
			,
			file, line,
			function != NULL ? function : "", function != NULL ? ": " : "",
			assertion,
			msg != NULL ? msg : "",
			msg != NULL && error != 0 ? ". " : "",
			error != 0 ? errnostr : ""
	);
}

void constraint_abort_handler(const char * restrict msg, int error,
		const char * restrict assertion, const char * restrict file,
		unsigned int line, const char * restrict function)
{
	constraint_printf_handler(msg, error, assertion, file, line, function);
	abort();
}

void constraint_ignore_handler(const char * restrict msg, int error,
		const char * restrict assertion, const char * restrict file,
		unsigned int line, const char * restrict function)
{
}

constraint_handler_t constraint_set_handler(constraint_handler_t handler)
{
	constraint_handler_t ret = constraint_handler;
	constraint_handler = handler;
	return ret;
}

constraint_handler_t contraint_get_handler()
{
	return constraint_handler != NULL ? constraint_handler : constraint_abort_handler;
}

void _constraint_failed(const char * restrict msg, int error,
		const char * restrict assertion, const char * restrict file,
		unsigned int line, const char * restrict function)
{
	contraint_get_handler()(msg, error, assertion, file, line, function);
}
