/*
 * result.h
 *
 *  Created on: 30 sty 2018
 *      Author: kamil
 */

#ifndef RESULT_H_
#define RESULT_H_

#include "errno_t.h"

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#define RESULT_struct(T,E) { \
	/* boolean variable setting if theres an error */ \
	bool isErr; \
	union { \
		/* If isErr is true, then this memb contains the error message or code */ \
		E err; \
		/* If isErr is false, then this memb contains the ok return value */ \
		T ok; \
	}; \
}
#define RESULT_typedef(T,E)            typedef struct RESULT_struct(T,E)

static inline void RESULT___assert(
		const char * restrict file,
		int line,
		const char * restrict assertion) {
	(void)fprintf(stderr, "%s:%d: result assertion %s failed\n", file, line, assertion);
	abort();
}
#define RESULT_assert(expr) ((expr)?(void)0:RESULT___assert(__FILE__, __LINE__, #expr))

#define RESULT_init_ok(...)             { .isErr = false, .ok = __VA_ARGS__ }
#define RESULT_init_err(...)            { .isErr = true, .err = __VA_ARGS__ }

#define RESULT_set_ok(Var, ...)         ( (Var).isErr = false, (Var).ok = __VA_ARGS__ )
#define RESULT_set_err(Var, ...)        ( (Var).isErr = true, (Var).err = __VA_ARGS__ )

#ifdef __GNUC__
#define RESULT_tryOk(Var) __extension__({ \
	__typeof__(Var) _RESULT_var = (Var); \
	RESULT_assert( _RESULT_var.isErr == false ); \
	_RESULT_var.ok; \
})
#else
#define RESULT_tryOk(Var)               ( RESULT_assert( (Var).isErr == false ), (Var).ok )
#endif

#ifdef __GNUC__
#define RESULT_tryErr(Var) __extension__({ \
	__typeof__(Var) _RESULT_var = (Var); \
	RESULT_assert( _RESULT_var.isErr == true ); \
	_RESULT_var.err; \
})
#else
#define RESULT_tryErr(Var)              ( RESULT_assert( (Var).isErr == true ), (Var).err )
#endif

#ifdef __GNUC__
#define RESULT_tryOkElse(Var, ...) __extension__({ \
	__typeof__((Var).ok) _RESULT_var_ok; \
	if (!(Var).isErr) { \
		_RESULT_var_ok = (Var).ok; \
	} else { \
		_RESULT_var_ok = ( __VA_ARGS__ ); \
	} \
	_RESULT_var_ok; \
})
#else
#define RESULT_tryOkElse(Var,...)       ( (!(Var).isErr)?((Var).ok ):( __VA_ARGS__ ) )
#endif

#ifdef __GNUC__
#define RESULT_tryErrElse(Var, ...) __extension__({ \
	__typeof__((Var).err) _RESULT_var_err; \
	if ( (Var).isErr) { \
		_RESULT_var_err = (Var).err; \
	} else { \
		_RESULT_var_err = ( __VA_ARGS__ ); \
	} \
	_RESULT_var_err; \
})
#else
#define RESULT_tryErrElse(Var,...)      ( ( (Var).isErr)?((Var).err):( __VA_ARGS__ ) )
#endif

#ifdef __GNUC__

#define RESULT_TRY __extension__({ \
	__auto_type _RESULT_var = (
#define RESULT_TRYEND \
	); \
	__typeof__(_RESULT_var.ok) _RESULT_var##_ok = RESULT_tryOk(_RESULT_var); \
	_RESULT_var##_ok; \
})
#define RESULT_TRYELSE \
	); \
	__typeof__(_RESULT_var.ok) _RESULT_var##_ok; \
	if ( (!_RESULT_var.isErr) ) { \
		_RESULT_var##_ok = _RESULT_var.ok; \
	} else { \
		_RESULT_var##_ok = __extension__({
#define RESULT_TRYELSEEND \
		; }); \
	} \
	_RESULT_var##_ok; \
})

#endif // __GNUC__

// typedefs for the most commonly used types

RESULT_typedef(int, errno_t) res_int_err_t;
RESULT_typedef(size_t, errno_t) res_size_err_t;
RESULT_typedef(char *, errno_t) res_charpnt_err_t;

#endif /* RESULT_H_ */
