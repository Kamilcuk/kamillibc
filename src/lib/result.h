/*
 * result.h
 *
 *  Created on: 30 sty 2018
 *      Author: Kamil Cukrowski
 */

#ifndef RESULT_H_
#define RESULT_H_

#include <stdbool.h>
#include <stddef.h>

#define RES_struct(ok_type, error_type) { \
	/* boolean variable setting if theres an error */ \
	bool isErr; \
	union { \
		/* If isErr is false, then this memb contains the ok return value */ \
		ok_type ok; \
		/* If isErr is true, then this memb contains the error message or code */ \
		error_type err; \
	}; \
}

#define RES_initOk(...)   { .isErr = false, .ok = __VA_ARGS__ }
#define RES_initErr(...)  { .isErr = true, .err = __VA_ARGS__ }

#define RES_setOk(result, ...)   ( (result).isErr = false, (result).ok  = __VA_ARGS__ )
#define RES_setErr(result, ...)  ( (result).isErr = true , (result).err = __VA_ARGS__ )

#define RES_isOk(result)   ((result).isErr == false)
#define RES_isErr(result)  ((result).isErr == true )

#define RES_getOk(result, ok_var)    ( RES_isOk(result)  ? ((ok_var ) = (result).ok , true) : false )
#define RES_getErr(result, err_var)  ( RES_isErr(result) ? ((err_var) = (result).err, true) : false )

typedef struct RES_struct(int, int) res_int_int_t;
typedef struct RES_struct(size_t, int) res_size_int_t;
typedef struct RES_struct(char *, int) res_charpnt_int_t;

int _res_unittest(void);

#endif /* RESULT_H_ */
