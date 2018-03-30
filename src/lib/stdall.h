
#ifndef __STDC_VERSION__
#error "Please define STDC_VERSION for this file. Och, and btw, your compiler is broken."
#endif

#if defined(__STDC_LIB_EXT1__)
#if __STDC_LIB_EXT1__ >= 201112L
#define __STDC_WANT_LIB_EXT1__ 1
#endif
#endif


/* Conditionally compiled macro that compares its argument to zero */
#include <assert.h>

#if __STDC_VERSION__ >= 199901L && !defined(__STDC_NO_COMPLEX__)
/* Complex number arithmetic */
#include <complex.h>
#endif

/* Functions to determine the type contained in character data */
#include <ctype.h>

/* Macros reporting error conditions */
#include <errno.h>

#if __STDC_VERSION__ >= 199901L
/* Floating-point environment */
#include <fenv.h>
#endif

/* Limits of float types */
#include <float.h>

#if __STDC_VERSION__ >= 199901L
/* Format conversion of integer types */
#include <inttypes.h>
#endif

#if __STDC_VERSION__ >= 199409L
/* Alternative operator spellings */
#include <iso646.h>
#endif

/* Sizes of basic types */
#include <limits.h>

/* Localization utilities  */
#include <locale.h>

/* Common mathematics functions */
#include <math.h>

/* Nonlocal jumps  */
#include <setjmp.h>

/* Signal handling  */
#include <signal.h>

#if __STDC_VERSION__ >= 201112L
/* alignas and alignof convenience macros */
#include <stdalign.h>
#endif

/* Variable arguments  */
#include <stdarg.h>

#if __STDC_VERSION__ >= 201112L && !defined(__STDC_NO_ATOMICS__)
/* Atomic types */
#include <stdatomic.h>
#endif

#if __STDC_VERSION__ >= 199901L
/* Boolean type */
#include <stdbool.h>
#endif

/* Common macro definitions */
#include <stddef.h>

#if __STDC_VERSION__ >= 199901L
/* Fixed-width integer types */
#include <stdint.h>
#endif

/* Input/output   */
#include <stdio.h>

/* General utilities: memory management, program utilities, string conversions, random numbers */
#include <stdlib.h>

#if __STDC_VERSION__ >= 201112L
/* noreturn convenience macros */
#include <stdnoreturn.h>
#endif

/* String handling  */
#include <string.h>

#if __STDC_VERSION__ >= 199901L
/* Type-generic math (macros wrapping math.h and complex.h) */
#include <tgmath.h>
#endif

#if __STDC_VERSION__ >= 201112L && !defined(__STDC_NO_THREADS__)
/* Thread library */
#include <threads.h>
#endif

/* Time/date utilities  */
#include <time.h>

#if __STDC_VERSION__ >= 201112L
/* UTF-16 and UTF-32 character utilities */
#include <uchar.h>
#endif

#if __STDC_VERSION__ >= 199409L
/* Extended multibyte and wide character utilities */
#include <wchar.h>
#endif

#if __STDC_VERSION__ >= 199409L
/* Functions to determine the type contained in wide character data */
#include <wctype.h>
#endif

