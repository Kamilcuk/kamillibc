/*
 * constraint.c
 *
 *  Created on: 19 mar 2018
 *      Author: kamil
 */

#include <constraint.h>

#include <cdefs.h> // __weak_function

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

/* Private Variables ---------------------------------------------------------- */

#if __STDC_VERSION__ >= 201112L && !defined(__STDC_NO_THREADS__)
thread_local
#endif
constraint_handler_t constraint_handler;

/* Exported Functions --------------------------------------------------------- */

void constraint_abort_handler(const char *msg, int error,
		const char *assertion, const char *file, unsigned int line, const char *function)
{
#ifdef __GNUC__
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
	(void)fprintf(stderr, "%s:%u: %s%sConstraint `%s' failed.\n",
			file, line,
			function != NULL ? function : "", function != NULL ? ": " : "",
			assertion);
	perror(msg);
	abort();
}

void constraint_ignore_handler(const char *msg, int error,
		const char *assertion, const char *file, unsigned int line, const char *function)
{

}

void constraint_set_handler(constraint_handler_t handler)
{
	constraint_handler = handler;
}


void constraint_failed(const char *msg, int error,
		const char *assertion, const char *file, unsigned int line, const char *function)
{
	(
		constraint_handler != NULL ? constraint_handler : constraint_ignore_handler
	)(msg, error, assertion, file, line, function);
}
