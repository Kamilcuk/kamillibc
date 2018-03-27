/*
 * printmacro.h
 *
 *  Created on: 27 mar 2018
 *      Author: kamil
 */

#ifndef SRC_PRINTMACRO_H_M4_
#define SRC_PRINTMACRO_H_M4_

#if defined(__GNUC__)

#define _PRINTMACRO_DO_PRAGMA(x)    _Pragma(#x)
#define _PRINTMACRO_STRING(x...)    #x
#define PRINTMACRO(VAR)             _PRINTMACRO_DO_PRAGMA(message ("\n"#VAR"="_PRINTMACRO_STRING(VAR)""))

#endif

#endif /* SRC_PRINTMACRO_H_M4_ */
