/*
 * result.h
 *
 *  Created on: 30 sty 2018
 *      Author: Kamil Cukrowski
 */

#ifndef resULT_H_
#define resULT_H_

#include <stdbool.h>
#include <stddef.h>

#define res_struct(ok_type, error_type) { \
	/* boolean variable setting if theres an error */ \
	bool isErr; \
	union { \
		/* If isErr is false, then this memb contains the ok return value */ \
		ok_type ok; \
		/* If isErr is true, then this memb contains the error message or code */ \
		error_type err; \
	}; \
}

#define res_initOk(...)   { .isErr = false, .ok = __VA_ARGS__ }
#define res_initErr(...)  { .isErr = true, .err = __VA_ARGS__ }

#define res_setOk(result, ...)   ( (result).isErr = false, (result).ok  = __VA_ARGS__ )
#define res_setErr(result, ...)  ( (result).isErr = true , (result).err = __VA_ARGS__ )

#define res_isOk(result)   ((result).isErr == false)
#define res_isErr(result)  ((result).isErr == true )

#define res_getOk(result, ok_var)    ( res_isOk(result)  ? ((ok_var ) = (result).ok , true) : false )
#define res_getErr(result, err_var)  ( res_isErr(result) ? ((err_var) = (result).err, true) : false )

typedef struct res_struct(int, int) res_int_int_t;
typedef struct res_struct(size_t, int) res_size_int_t;
typedef struct res_struct(char *, int) res_charpnt_int_t;

int _res_unittest(void);

#endif /* resULT_H_ */
