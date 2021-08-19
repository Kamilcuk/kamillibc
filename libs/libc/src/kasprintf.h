/*
 * @file
 * @author Kamil Cukrowski <kamil@leonidas>
 * @date 2021-08-19
 * SPDX-License-Identifier: MIT AND Beerware
 */
#ifndef KASPRINTF_H_
#define KASPRINTF_H_
#ifdef __cplusplus
extern "C" {
#endif

#ifdef __GNUC__
#define _kasprintf_attr __attribute__((__format__(__printf__, 2, 3)))
#else
#define _kasprintf_attr
#endif

#include <stdarg.h>

/// Like asprintf.
_kasprintf_attr
int kasprintf(char **strp, const char *fmt, ...);
int kvasprintf(char **strp, const char *fmt, va_list ap);

/// Appends on the end of the buffer.
_kasprintf_attr 
int kreasprintf(char **strp, const char *fmt, ...);
int kvreasprintf(char **strp, const char *fmt, va_list ap);

#ifdef __cplusplus
}
#endif
#endif /* KASPRINTF_H_ */
