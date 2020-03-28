/*
 * curb.h
 *
 *  Created on: 19 mar 2018
 *      Author: kamil
 */

#ifndef SRC_CURB_H_
#define SRC_CURB_H_
#ifdef __cplusplus
extern "C" {
#define _curb_restrict  /**/
#else
#define _curb_restrict  restrict
#endif

#if __CDT_PARSER__
#define _curb_thread_local  /**/
#elif __STDC_VERSION__ >= 201112L && !defined(__STDC_NO_THREADS__)
#define _curb_thread_local  _Thread_local
#elif __GNUC__
#define _curb_thread_local  __thread
#endif

/* Exported Macros ---------------------------------------------------- */

#ifndef CURB_SEVERITY
// #define CURB_SEVERITY  warning
#define CURB_SEVERITY  error
#endif

#ifndef CURB_DEFAULT_HANDLER
#define CURB_DEFAULT_HANDLER  curb_handler_abort
#endif

#define curb_ex(expr, exprstr, error, msg) \
		_curb_PRE_CURB_FAIL(expr, exprstr, ": " #error, ": " msg) \
		_curb_fail(); \
		_curb_POST_CURB_FAIL(expr, exprstr, error, msg)

#define curb_msgerrno(expr, msg, error) \
		_curb_PRE_CURB_FAIL(expr, #expr, ": " #error, ": " msg) \
		_curb_fail(); \
		_curb_POST_CURB_FAIL(expr, #expr, error, msg)

#define curb_msg(expr, msg) \
		_curb_PRE_CURB_FAIL(expr, #expr, "", ": " msg) \
		_curb_fail(); \
		_curb_POST_CURB_FAIL(expr, #expr, 0, msg)

#define curb_errno(expr, error) \
		_curb_PRE_CURB_FAIL(expr, #expr, ": " #error, "") \
		_curb_fail(); \
		_curb_POST_CURB_FAIL(expr, #expr, error, "")

#define curb(expr) \
		_curb_PRE_CURB_FAIL(expr, #expr, "", "") \
		_curb_fail(); \
		_curb_POST_CURB_FAIL(expr, #expr, 0, "")

/* Exported Types ----------------------------------------------------- */

struct curb_handler_ctx_s {
	// Custom user message or NULL.
	const char * _curb_restrict msg;
	// The stringified assertion.
	const char * _curb_restrict assertion;
	// The file where assertiong fired.
	const char * _curb_restrict file;
	// The function where assertion fired.
	const char * _curb_restrict function;
	// The line in the file where assertion fired.
	int line;
	// A custom errno message can be included.
	int error;
};

typedef void (*curb_handler_t)(struct curb_handler_ctx_s ctx);

/* Exported Functions ------------------------------------------------ */

/**
 * Prints a information message like assert.
 */
void curb_handler_print(struct curb_handler_ctx_s ctx);
/**
 * Calls curb_handler_printf() and then calls abort().
 * This is the default handler.
 */
__attribute__((__noreturn__))
void curb_handler_abort(struct curb_handler_ctx_s ctx);
/**
 * Calls curb_handler_printf() and then calls exit(EXIT_FAILURE).
 */
__attribute__((__noreturn__))
void curb_handler_exit(struct curb_handler_ctx_s ctx);
/**
 * Prints a longer information message like on cppreference.
 */
void curb_handler_print_long(struct curb_handler_ctx_s ctx);
/**
 * Similar to curb_handler_abort, but calls curb_handler_print_long().
 */
__attribute__((__noreturn__))
void curb_handler_abort_long(struct curb_handler_ctx_s ctx);
/**
 * Similar to curb_handler_exit, but calls curb_handler_print_long().
 */
__attribute__((__noreturn__))
void curb_handler_exit_long(struct curb_handler_ctx_s ctx);
/**
 * Does nothing.
 */
void curb_handler_ignore(struct curb_handler_ctx_s ctx);

/**
 * Gets constraning handler. This function never returns NULL.
 * @return
 */
curb_handler_t curb_get_handler(void);

/**
 * Sets constaring handler to handler. Returns previous handler.
 * @param handler
 * @return
 */
curb_handler_t curb_set_handler(curb_handler_t handler);

/* Private Macros ---------------------------------------------------- */

#define _curb_STRING(x)   #x
#define _curb_XSTRING(x)  _curb_STRING(x)

#define _curb_PRE_CURB_FAIL(expr, exprstr, errorstr, msg) \
	(__builtin_expect((expr), 1) ? 1 : \
		__extension__({ \
			if (__builtin_constant_p(expr)) { \
				if (!(expr)) { \
					__attribute__((__noinline__)) \
					__attribute__(( CURB_SEVERITY ( \
						"\n" \
						__FILE__ ":" _curb_XSTRING(__LINE__) ": " \
						_curb_XSTRING(CURB_SEVERITY) \
							": curb(" exprstr ") will fail" msg errorstr \
					))) \
					void _curb_fail(void) { __asm__(""); }

#define _curb_POST_CURB_FAIL(expr, exprstr, error, msg) \
				} \
			} \
			_curb_failed_func(msg, error, exprstr, __FILE__, __LINE__, __func__); \
			0; \
		}) \
	)

/* Private Exported Variables ---------------------------------------------- */

extern _curb_thread_local curb_handler_t _curb_handler;

/* Private Functions -------------------------------------------------- */

__attribute__((__nonnull__))
static inline
void _curb_failed_func(const char msg[_curb_restrict], int error,
		const char assertion[_curb_restrict], const char file[_curb_restrict],
		unsigned int line, const char function[_curb_restrict])
{
	const struct curb_handler_ctx_s ctx = {
			.msg = msg,
			.assertion = assertion,
			.file = file,
			.line = line,
			.function = function,
			.error = error,
	};
	curb_get_handler()(ctx);
}

/* ----------------------------------------------------------------------- */

#ifdef __cplusplus
}
#endif
#endif /* SRC_CURB_H_ */
