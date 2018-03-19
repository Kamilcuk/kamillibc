/*
 * constraint.h
 *
 *  Created on: 19 mar 2018
 *      Author: kamil
 */

#ifndef SRC_CONSTRAINT_H_
#define SRC_CONSTRAINT_H_

#include <stddef.h>

typedef void (*constraint_handler_t)(const char *msg, int errno,
		const char *assertion, const char *file, unsigned int line, const char *function);

void constraint_abort_handler(const char *msg, int error,
		const char *assertion, const char *file, unsigned int line, const char *function);

void constraint_ignore_handler(const char *msg, int error,
		const char *assertion, const char *file, unsigned int line, const char *function);

void constraint_set_handler(constraint_handler_t handler);

void constraint_failed(const char *msg, int error,
		const char *assertion, const char *file, unsigned int line, const char *function);

#define _contraint_failed(expr, msg, error)  constraint_failed(msg, error, #expr, __FILE__, __LINE__, __func__)
#define _constraint_1(expr)                  _constraint_2(expr, NULL)
#define _constraint_2(expr, msg)             _constraint_3(expr, msg, 0)
#define _constraint_3(expr, msg, error)      ((expr)?(void)0:_contraint_failed(expr, msg, error))
#define _constraint_N(_1, _2, _3, N, ...)    _constraint_##N
#define constraint(expr, ...)                _constraint_N(expr, ##__VA_ARGS__, 3, 2, 1)(expr, ##__VA_ARGS__)

#endif /* SRC_CONSTRAINT_H_ */
