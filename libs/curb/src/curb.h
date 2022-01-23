/**
 * @file
 * @copyright Kamil Cukrowski
 */
#ifndef SRC_CURB_H_
#define SRC_CURB_H_
#ifdef __cplusplus
extern "C" {
#endif

/* configuration ------------------------------------------------------------------------- */

#ifndef CURB_SEVERITY
/**
 * Severity passed to __attribute__( CURB_SEVERITY GCC extension when generating error.
 */
#define CURB_SEVERITY  error
#endif

/* ------------------------------------------------------------------------- */

#ifdef __GNUC__
#define _curb_noreturn   __attribute__((__noreturn__))
#elif !defined(__cplusplus) && __STDC_VERSION__ == 201112L
#define _curb_noreturn   _Noreturn
#elif __cplusplus >= 201103L
#define _curb_noreturn   [[noreturn]]
#else
#define _curb_noreturn
#endif

#ifdef __GNUC__
#define _curb_nonnull(...)    __attribute__((__nonnull__(__VA_ARGS__)))
#else
#define _curb_nonnull(...)
#endif

#define _curb_STRING(x)   #x
#define _curb_XSTRING(x)  _curb_STRING(x)

#define _curb_CONCAT(a, b)   a##b
#define _curb_XCONCAT(a, b)  _curb_CONCAT(a, b)

/* ------------------------------------------------------------------------- */

#if !( defined(__GNUC__) && defined(__OPTIMIZE__) )
#define _curb_2(line, expr, msg)   (!!(expr))
#else
#define _curb_2(line, expr, msg) \
	(__extension__({ \
		if (__builtin_constant_p(expr)) { \
			if (expr) {} else { \
				__attribute__((__noinline__)) \
				__attribute__(( CURB_SEVERITY ( \
					"\n" \
					__FILE__ ":" _curb_STRING(line) ": " \
					_curb_XSTRING(CURB_SEVERITY) \
					": curb(" #expr ") will fail: " msg \
				))) \
				void _curb_fail##line() { \
					/* that way the function is not inlined */ \
					__asm__(""); \
				} \
				_curb_fail##line(); \
			} \
		} \
		__builtin_expect(!!(expr), 1); \
	}))
#endif

#define _curb_1(line, expr)        _curb_2(line, expr, "")
#define _curb_N(_2, _1, N, ...)  _curb_##N
/**
 * @macro curb(expression, [message])
 * @param expression Expression to check
 * @param message A string literal to display when the expression failed.
 * @return Result of expr converted to values 1 or 0.
 *
 * If the expression can be checked at compile-time and the expression fails,
 * causes a compile-time error and a message to be displayed.
 * If the expression can't be checked at compile-time just returns the result
 * of passed expression.
 */
#define curb(...)            _curb_N(__VA_ARGS__, 2, 1)(__LINE__, __VA_ARGS__)

#define _curbh_3(line, handler, expr, msg, error)  \
		(_curb_2(line, expr, msg) ? 1 : (_curb_failed_func(handler, msg, #expr, __FILE__, __func__, line, error), 0))
#define _curbh_2(line, handler, expr, msg)  _curbh_3(line, handler, expr, "", 0)
#define _curbh_1(line, handler, expr)       _curbh_2(line, handler, expr, "")
#define _curbh_N(_3, _2, _1, N, ...)        _curbh_##N
/**
 * @macro curbh(handler, [expression, [message, [error]]])
 * @param handler Handler to call when the expression fails at runtime.
 * @param expression Expression to check.
 * @param message A string literal to display when the expression failed.
 * @param error If non-zero, strerror is called on this value and is displayed. Meant for errno.
 * @return Result of expression convert to values 1 or 0.
 *
 * If the expression can be checked at compile-time, a error message is displayed that includes
 * a custom message if given.
 * If the expression can't be checked at compile-time, it is checked at runtime. If the expression
 * fails at runtime, handler is called with prepared arguments with message and error passed.
 * Then the function returns with the result of expression.
 */
#define curbh(handler, ...)                 _curbh_N(__VA_ARGS__, 3, 2, 1)(__LINE__, handler, __VA_ARGS__)

/**
 * If NDEBUG is defined, check at compile time or does nothing.
 * When NDEBUG is not defined, checks at compile time or calls curbh_abort at runtime.
 */
#define curb_assert(...)  curbh(curb_assert, __VA_ARGS__)

/**
 * The same as curbh(curbh_exitlong
 */
#define curb_exit(...)    curbh(curbh_exitlong, __VA_ARGS__)

/* ------------------------------------------------------------------------- */

struct curbh_ctx {
	/// Custom user message or NULL.
	const char *msg;
	/// The stringified assertion.
	const char *assertion;
	/// The file where assertiong fired.
	const char *file;
	/// The function where assertion fired.
	const char *function;
	/// The line in the file where assertion fired.
	unsigned int line;
	/// The value of errno.
	int error;
};

_curb_nonnull()
static inline
void _curb_failed_func(void (*handler)(struct curbh_ctx ctx),
		const char *msg,
		const char *assertion,
		const char *file,
		const char *function,
		unsigned int line,
		int error) {
	const struct curbh_ctx ctx = {
			msg,
			assertion,
			file,
			function,
			line,
			error,
	};
	(*handler)(ctx);
}

/**
 * Works just like assert.
 */
#ifndef NDEBUG
#define curbh_assert  curb_abort
#else
#define curbh_assert  curbh_ignore
#endif

/**
 * Prints a information message like assert and continues execution.
 */
void curbh_print(struct curbh_ctx ctx);

/**
 * Calls curbh_print() and then calls abort().
 */
void curbh_abort(struct curbh_ctx ctx);

/**
 * Calls curbh_print() and then calls exit(EXIT_FAILURE).
 */
_curb_noreturn
void curbh_exit(struct curbh_ctx ctx);

/**
 * Prints a longer information message like on cppreference:
 * https://en.cppreference.com/w/c/error/set_constraint_handler_s
 */
void curbh_printlong(struct curbh_ctx ctx);

/**
 * Similar to curbh_abort, but calls curbh_printlong().
 */
_curb_noreturn
void curbh_abortlong(struct curbh_ctx ctx);

/**
 * Similar to curbh_exit, but calls curbh_print_long().
 */
_curb_noreturn
void curbh_exitlong(struct curbh_ctx ctx);

/**
 * Does nothing.
 */
static inline void curbh_ignore(struct curbh_ctx ctx) {
	(void)ctx;
}

/* ------------------------------------------------------------------------- */

#ifdef __cplusplus
}
#endif
#endif /* SRC_CURB_H_ */
