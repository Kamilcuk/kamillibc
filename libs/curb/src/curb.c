/**
 * @file
 * @copyright Kamil Cukrowski
 */
#define _GNU_SOURCE  1
#define _POSIX_C_SOURCE  200112L
#define __STDC_WANT_LIB_EXT1__  1
#include "curb.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

/* Private Functions ---------------------------------------------------------- */

#if ((_POSIX_C_SOURCE >= 200112L || _XOPEN_SOURCE >= 600) && ! _GNU_SOURCE) || \
		(__STDC_VERSION__ >= 201112L && defined(__STDC_LIB_EXT1__))

static inline const char *_curb_strerror_in(char errnostr[128], int error) {
	if (error != 0) {
		(void)strerror_r(error, errnostr, 128);
	}
	return errnostr;
}

#define _curb_strerror(error)  _curb_strerror_in((char[128]){0}, error)

#else

static inline const char *_curb_strerror(int error) {
	return error ? strerror(error) : NULL;
}

#endif

/* Exported Functions --------------------------------------------------------- */

void curbh_print(struct curbh_ctx ctx) {
	const char *errnostr = _curb_strerror(ctx.error);
	(void)fprintf(stderr,
#if HAS_PROGRAM_INVOCATION_SHORT_NAME
			"%s: "
#endif
			"%s:%u: %s: constraint `%s' failed%s%s%s%s\n"
			,
#if HAS_PROGRAM_INVOCATION_SHORT_NAME
			program_invocation_short_name,
#endif
			ctx.file,
			ctx.line,
			ctx.function,
			ctx.assertion,
			(ctx.msg == NULL || ctx.msg[0] == '\0' ? "" : ": "),
			(ctx.msg == NULL || ctx.msg[0] == '\0' ? "" : ctx.msg),
			(ctx.error == 0 ? "" : ": "),
			(ctx.error == 0 ? "" : errnostr)
	);
}

void curbh_abort(struct curbh_ctx ctx) {
	curbh_print(ctx);
	abort();
}

void curbh_exit(struct curbh_ctx ctx) {
	curbh_print(ctx);
	exit(EXIT_FAILURE);
}

void _curbh_printlong_in(struct curbh_ctx ctx, const char *func) {
	const char *errnostr = _curb_strerror(ctx.error);
	(void)fprintf(stderr,
			"%s was called in response to a runtime-constraint violation.\n"
			"\n"
			"The runtime-constraint violation was caused by the following expression in %s:\n"
			"%s (in %s:%d)\n"
			"%s%s"
			"%s%s"
			"\n"
			,
			func,
			ctx.function,
			ctx.assertion,
			ctx.file,
			ctx.line,
			(ctx.msg == NULL || ctx.msg[0] == '\0' ? "" : ctx.msg),
			(ctx.msg == NULL || ctx.msg[0] == '\0' ? "" : "\n"),
			(ctx.error == 0 ? "" : errnostr),
			(ctx.error == 0 ? "" : ".\n")
	);
}

void curbh_printlong(struct curbh_ctx ctx) {
	_curbh_printlong_in(ctx, __func__);
}

static inline
void _curbh_terminatednote(void) {
	(void)fprintf(stderr,
			"Note to end users: This program was terminated as a result\n"
			"of a bug present in the software. Please reach out to your\n"
			"software's vendor to get more help.\n"
	);
}

void curbh_abortlong(struct curbh_ctx ctx) {
	_curbh_printlong_in(ctx, __func__);
	_curbh_terminatednote();
	abort();
}

void curbh_exitlong(struct curbh_ctx ctx) {
	_curbh_printlong_in(ctx, __func__);
	_curbh_terminatednote();
	exit(EXIT_FAILURE);
}

