/*
 * printmacro.h
 *
 *  Created on: 27 mar 2018
 *      Author: kamil
 */

#ifndef MACROPRINT_H_
#define MACROPRINT_H_

#if defined(__GNUC__)

#define _MACROPRINT_DO_PRAGMA(x)    _Pragma(#x)
#define _MACROPRINT_STRING(...)     #__VA_ARGS__
#define MACROPRINT(VAR)             _MACROPRINT_DO_PRAGMA(message ("\n" #VAR "=" _MACROPRINT_STRING(VAR)))

#else

#define MACROPRINT(VAR) error: MACROPRINT is not defined for this compiler!

#endif

#endif /* MACROPRINT_H_ */
