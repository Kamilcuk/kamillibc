/*
 * debug.c
 *
 *  Created on: 02.04.2018
 *      Author: michal
 */
#include <debug.h>

#include <minmax.h>

#include <stdlib.h>
#include <limits.h>
#include <stdarg.h>

int dbg_get_lvl_from_env()
{
#if DBG_ENV_ENABLED
	const char * const val = getenv("DEBUG");
	if (val != NULL) {
		const long lvl = strtoll(val, NULL, 10);
		if (INT_MIN <= lvl && lvl <= INT_MAX) {
			return lvl;
		}
	}
#endif
	return 0;
}
