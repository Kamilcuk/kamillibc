/*
 * result.h
 *
 *  Created on: 30 sty 2018
 *      Author: kamil
 */

#ifndef CONTRACT_RESULT_H_
#define CONTRACT_RESULT_H_

#include "errno_t.h"
#include "require.h"

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#define CONTRACT_Result_s(T,E)                  { bool isErr; union { E err; T ok; }; }
#define CONTRACT_Result_ok(Symbol, ok_, ...)    ((Symbol){ .isErr = false, .ok = (ok_, ##__VA_ARGS__), })
#define CONTRACT_Result_err(Symbol, err_, ...)  ((Symbol){ .isErr = true, .err = (err_, ##__VA_ARGS__), })
#define CONTRACT_Result_is_ok(Var)              (!(Var)->isErr)
#define CONTRACT_Result_is_err(Var)             ( (Var)->isErr)

#define CONTRACT_Result_tryOk(Var)                ( CONTRACT_assert(!(Var).isErr), (Var).ok )
#define CONTRACT_Result_tryErr(Var)               ( CONTRACT_assert( (Var).isErr), (Var).err )

#define CONTRACT_Result_tryOkElse(Var,Else,...)   ({ \
	typeof((Var).ok) _ok; \
	if (!(Var).isErr) { \
		_ok = ({ Else, ##__VA_ARGS__ ; }) ; \
	} else { \
		_ok = (Var).ok ; \
	} \
	_ok; \
})
#define CONTRACT_Result_tryErrElse(Var,Else,...)   ({ \
	typeof((Var).err) _err; \
	if (!(Var).isErr) { \
		_err = ({ Else, ##__VA_ARGS__ ; }) ; \
	} else { \
		_err = (Var).err ; \
	} \
	_err; \
})

#define CONTRACT_Result_typedef(T,E)        typedef struct CONTRACT_Result_s(T,E)

#define CONTRACT_TRYv(_ret)              ({ __auto_type (_ret) = (
#define CONTRACT_TRYENDv(_ret)           ) ; assert(!(_ret).isErr); (_ret).ok; })
#define CONTRACT_TRYELSEv(_ret)          ) ; typeof((_ret).ok) _ok; if ( (_ret).isErr ) { _ok = ({
#define CONTRACT_TRYELSEENDv(_ret)       ; }); } else { _ok = (_ret).ok ; } _ok ; })

#define CONTRACT_TRY         CONTRACT_TRYv(_ret)
#define CONTRACT_TRYELSE     CONTRACT_TRYELSEv(_ret)
#define CONTRACT_TRYELSEEND  CONTRACT_TRYELSEENDv(_ret)
#define CONTRACT_TRYEND      CONTRACT_TRYENDv(_ret)

CONTRACT_Result_typedef(char *, errno_t) result_charpnt_errno_t_t;

CONTRACT_Result_typedef(char, errno_t) result_char_errno_t_t;
CONTRACT_Result_typedef(signed char, errno_t) result_schar_errno_t_t;
CONTRACT_Result_typedef(unsigned char, errno_t) result_uchar_errno_t_t;

CONTRACT_Result_typedef(int, errno_t) result_int_errno_t_t;
CONTRACT_Result_typedef(signed int, errno_t) result_sint_errno_t_t;
CONTRACT_Result_typedef(unsigned int, errno_t) result_uint_errno_t_t;

CONTRACT_Result_typedef(long, errno_t) result_long_errno_t_t;
CONTRACT_Result_typedef(signed long, errno_t) result_slong_errno_t_t;
CONTRACT_Result_typedef(unsigned long, errno_t) result_ulong_errno_t_t;

CONTRACT_Result_typedef(long long, errno_t) result_llong_errno_t_t;
CONTRACT_Result_typedef(signed long long, errno_t) result_sllong_errno_t_t;
CONTRACT_Result_typedef(unsigned long long, errno_t) result_ullong_errno_t_t;

CONTRACT_Result_typedef(float, errno_t) result_float_errno_t_t;
CONTRACT_Result_typedef(double, errno_t) result_double_errno_t_t;
CONTRACT_Result_typedef(double, errno_t) result_ldouble_errno_t_t;

CONTRACT_Result_typedef(int8_t, errno_t) result_int8_t_errno_t_t;
CONTRACT_Result_typedef(int16_t, errno_t) result_int16_t_errno_t_t;
CONTRACT_Result_typedef(int32_t, errno_t) result_int32_t_errno_t_t;
CONTRACT_Result_typedef(int64_t, errno_t) result_int64_t_errno_t_t;

CONTRACT_Result_typedef(int_fast8_t, errno_t) result_int_fast8_t_errno_t_t;
CONTRACT_Result_typedef(int_fast16_t, errno_t) result_int_fast16_t_errno_t_t;
CONTRACT_Result_typedef(int_fast32_t, errno_t) result_int_fast32_t_errno_t_t;
CONTRACT_Result_typedef(int_fast64_t, errno_t) result_int_fast64_t_errno_t_t;

CONTRACT_Result_typedef(int_least8_t, errno_t) result_int_least8_t_errno_t_t;
CONTRACT_Result_typedef(int_least16_t, errno_t) result_int_least16_t_errno_t_t;
CONTRACT_Result_typedef(int_least32_t, errno_t) result_int_least32_t_errno_t_t;
CONTRACT_Result_typedef(int_least64_t, errno_t) result_int_least64_t_errno_t_t;

CONTRACT_Result_typedef(intmax_t, errno_t) result_intmax_t_errno_t_t;
CONTRACT_Result_typedef(intptr_t, errno_t) result_intptr_t_errno_t_t;

CONTRACT_Result_typedef(uint8_t, errno_t) result_uint8_t_errno_t_t;
CONTRACT_Result_typedef(uint16_t, errno_t) result_uint16_t_errno_t_t;
CONTRACT_Result_typedef(uint32_t, errno_t) result_uint32_t_errno_t_t;
CONTRACT_Result_typedef(uint64_t, errno_t) result_uint64_t_errno_t_t;

CONTRACT_Result_typedef(uint_fast8_t, errno_t) result_uint_fast8_t_errno_t_t;
CONTRACT_Result_typedef(uint_fast16_t, errno_t) result_uint_fast16_t_errno_t_t;
CONTRACT_Result_typedef(uint_fast32_t, errno_t) result_uint_fast32_t_errno_t_t;
CONTRACT_Result_typedef(uint_fast64_t, errno_t) result_uint_fast64_t_errno_t_t;

CONTRACT_Result_typedef(uint_least8_t, errno_t) result_uint_least8_t_errno_t_t;
CONTRACT_Result_typedef(uint_least16_t, errno_t) result_uint_least16_t_errno_t_t;
CONTRACT_Result_typedef(uint_least32_t, errno_t) result_uint_least32_t_errno_t_t;
CONTRACT_Result_typedef(uint_least64_t, errno_t) result_uint_least64_t_errno_t_t;

CONTRACT_Result_typedef(uintmax_t, errno_t) result_uintmax_t_errno_t_t;
CONTRACT_Result_typedef(uintptr_t, errno_t) result_uintptr_t_errno_t_t;
CONTRACT_Result_typedef(size_t, errno_t) result_size_t_errno_t_t;

#endif /* CONTRACT_RESULT_H_ */
