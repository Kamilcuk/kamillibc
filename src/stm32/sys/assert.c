/*
 * assert.c
 *
 *  Created on: 22 mar 2018
 *      Author: kamil
 */
#include <machine/hal.h>
#include <breakpoint.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

void __assert_func (const char *file, int line,
			   const char *func, const char *failedexpr)
{

	breakpoint();
	fiprintf(stderr,
			"assertion \"%s\" failed: file \"%s\", line %d%s%s\n",
			failedexpr, file, line,
			func ? ", function: " : "", func ? func : "");
	abort();
}
