/*
 * curb.h
 *
 *  Created on: 19 mar 2018
 *      Author: kamil
 */

#ifndef SRC_CURB_H_
#define SRC_CURB_H_

#include <stddef.h>
#include <stdbool.h>

/* Private Macros ---------------------------------------------------- */

#define _curb_STRING(x)   #x
#define _curb_XSTRING(x)  _curb_STRING(x)

/* Exported Macros ---------------------------------------------------- */

/**
 * /def curb_STATIC
 * Compile time check if expression is true.
 * @param severity must be __warning__ or __error__ (gcc will take __deprecated__ also)
 * @param expr expression to evaluate.
 * @param msg a string literal to display with error message
 */
void curb_STATIC(int severity, bool expr, const char msg[]);
#define curb_STATIC(severity, expr, msg)  \
	__extension__({ \
		if (__builtin_constant_p(expr)) { \
			if (!(expr)) { \
				extern __attribute__(( severity ( \
					"\n" \
					__FILE__ ":" _curb_XSTRING(__LINE__) ": " \
					#severity ": curb \"" #expr "\" will fail" msg \
				))) \
				void curb_issue_warning (void); \
				curb_issue_warning (); \
			} \
		} \
	})

/**
 * /def curb_ex(expr, msg, error)
 * @param expr If expression is false, then call curb handler.
 * @param msg Optional message to print
 * @param error Optional error value to print
 */
void curb_ex(bool expr, const char msg[], int error);
#define curb_ex(expr, msg, error) \
	(__builtin_expect((expr), 1) ? 1 : (curb_STATIC(__warning__, expr, ""), \
		_curb_failed_func(msg, error, #expr, __FILE__, __LINE__, __func__), 0))

/**
 * /def curb(expr)
 * @param expr If expression is false, then call curb handler.
 */
void curb(bool expr);
#define curb(expr)  curb_ex(expr, NULL, 0)

/**
 * /def curb_exret(expr, msg, error, retval)
 */
int curb_exret(bool expr, const char msg[], int error, int retval);
#define curb_exret(expr, msg, error, ...)  do{ \
		if (__builtin_expect(!(expr), 0)) { \
			curb_STATIC(__warning__, expr, ""); \
			_curb_failed_func(msg, error, #expr, __FILE__, __LINE__, __func__); \
			return __VA_ARGS__; \
		} \
	} while(0)

/**
 * /def curb_ret(expr, retval)
 * See curb_exret
 */
int curb_ret(bool expr, int retval);
#define curb_ret(expr, ...)  curb_exret(expr, NULL, 0, ##__VA_ARGS__)

/* Exported Types ----------------------------------------------------- */

typedef void (*curb_handler_t)(const char * restrict msg, int error,
		const char * restrict assertion, const char * restrict file,
		unsigned int line, const char * restrict function);

/* Private Functions -------------------------------------------------- */

void _curb_failed_func(const char * restrict msg, int error,
		const char * restrict assertion, const char * restrict file,
		unsigned int line, const char * restrict function);

/* Exported Functions ------------------------------------------------ */

/**
 * Prints information message and then calls abort()
 */
void curb_printf_handler(const char * restrict msg, int error,
		const char * restrict assertion, const char * restrict file,
		unsigned int line, const char * restrict function);
/**
 * Calls abort()
 */
void curb_abort_handler(const char * restrict msg, int error,
		const char * restrict assertion, const char * restrict file,
		unsigned int line, const char * restrict function);
/**
 * Does nothing.
 */
void curb_ignore_handler(const char * restrict msg, int error,
		const char * restrict assertion, const char * restrict file,
		unsigned int line, const char * restrict function);
/**
 * Gets constraing handler. This function never returns NULL.
 * @return
 */
curb_handler_t curb_get_handler(void);
/**
 * Sets constaring handler to handler. Returns previous handler.
 * @param handler
 * @return
 */
curb_handler_t curb_set_handler(curb_handler_t handler);

/* ----------------------------------------------------------------- */

#endif /* SRC_CURB_H_ */
