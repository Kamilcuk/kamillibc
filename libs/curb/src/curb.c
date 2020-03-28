/*
 * curb.c
 *
 *  Created on: 19 mar 2018
 *      Author: kamil
 */
#define _POSIX_C_SOURCE  200112L
#define __STDC_WANT_LIB_EXT1__  1
#include "curb.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* Private Variables ---------------------------------------------------------- */

_curb_thread_local curb_handler_t _curb_handler;

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
	const char *errnostr = NULL;
	if (error != 0) {
		errnostr = strerror(error);
	}
	return errnostr;
}

#endif

/* Exported Functions --------------------------------------------------------- */

void curb_handler_print(struct curb_handler_ctx_s ctx) {
	const char *errnostr = _curb_strerror(ctx.error);
	(void)fprintf(stderr,
			"%s:%u: %s: curb: constraint `%s' failed%s%s%s%s\n"
			,
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

void curb_handler_abort(struct curb_handler_ctx_s ctx) {
	curb_handler_print(ctx);
	abort();
}

void curb_handler_exit(struct curb_handler_ctx_s ctx) {
	curb_handler_print(ctx);
	exit(EXIT_FAILURE);
}

void _curb_handler_print_in(struct curb_handler_ctx_s ctx, const char *func) {
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

void curb_handler_print_long(struct curb_handler_ctx_s ctx) {
	_curb_handler_print_in(ctx, __func__);
}

static inline
void _curb_handler_terminatednode(void) {
	(void)fprintf(stderr,
			"Note to end users: This program was terminated as a result\n"
			"of a bug present in the software. Please reach out to your\n"
			"software's vendor to get more help.\n"
	);
}

void curb_handler_abort_long(struct curb_handler_ctx_s ctx) {
	_curb_handler_print_in(ctx, __func__);
	_curb_handler_terminatednode();
	abort();
}

void curb_handler_exit_long(struct curb_handler_ctx_s ctx) {
	_curb_handler_print_in(ctx, __func__);
	_curb_handler_terminatednode();
	exit(EXIT_FAILURE);
}

void curb_handler_ignore(struct curb_handler_ctx_s ctx) {
	(void)ctx;
}

curb_handler_t curb_get_handler(void)
{
	return _curb_handler != NULL ? _curb_handler : CURB_DEFAULT_HANDLER;
}

curb_handler_t curb_set_handler(curb_handler_t handler)
{
	const curb_handler_t ret = _curb_handler;
	_curb_handler = handler;
	return ret;
}
