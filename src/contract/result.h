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

#define RESULT_struct(T,E)             { bool isErr; union { E err; T ok; }; }
#define RESULT_typedef(T,E)            typedef struct RESULT_struct(T,E)

#define RESULT_init_ok(...)             { .isErr = false, .ok = __VA_ARGS__ }
#define RESULT_init_err(...)            { .isErr = true, .err = __VA_ARGS__ }

#define RESULT_set_ok(Var, ...)         ( (Var).isErr = false, (Var).ok = __VA_ARGS__ )
#define RESULT_set_err(Var, ...)        ( (Var).isErr = true, (Var).err = __VA_ARGS__ )

#ifndef __GNUC__

#define RESULT_tryOk(Var)               ( assert( (Var).isErr == false ), (Var).ok )
#define RESULT_tryErr(Var)              ( assert( (Var).isErr == true ), (Var).err )

#define RESULT_tryOkElse(Var,...)       ( (!(Var).isErr)?((Var).ok ):( __VA_ARGS__ ) )
#define RESULT_tryErrElse(Var,...)      ( ( (Var).isErr)?((Var).err):( __VA_ARGS__ ) )

#else // __GNUC__

#define RESULT_tryOk(Var) ({ \
	__typeof__(Var) _RESULT_var = (Var); \
	assert( _RESULT_var.isErr == false ); \
	_RESULT_var.ok; \
})
#define RESULT_tryErr(Var) ({ \
	__typeof__(Var) _RESULT_var = (Var); \
	assert( _RESULT_var.isErr == true ); \
	_RESULT_var.err; \
})

#define RESULT_tryOkElse(Var, ...) ({ \
	__typeof__((Var).ok) _RESULT_var_ok; \
	if (!(Var).isErr) { \
		_RESULT_var_ok = (Var).ok; \
	} else { \
		_RESULT_var_ok = __VA_ARGS__ ; \
	} \
	_RESULT_var_ok; \
})
#define RESULT_tryErrElse(Var, ...) ({ \
	__typeof__((Var).err) _RESULT_var_err; \
	if ( (Var).isErr) { \
		_RESULT_var_err = (Var).err; \
	} else { \
		_RESULT_var_err = __VA_ARGS__ ; \
	} \
	_RESULT_var_err; \
})

#define RESULT_TRY ({ \
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
		_RESULT_var##_ok = ({
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
