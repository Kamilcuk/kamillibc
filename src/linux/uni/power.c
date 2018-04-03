/*
 * power.c
 *
 *  Created on: 30 mar 2018
 *      Author: Kamil Cukrowski
 *     License: All rights reserved. Copyright by Netemera(C).
 */

#include <uni/power.h>

#include <signal.h>
#include <stddef.h>
#include <assert.h>
#if _XOPEN_SOURCE >= 500

static long CriticalSectionCnt = 0;
static sigset_t CriticalSectionMask = {0};

void pwr_CriticalSectionEnter()
{
	if (++CriticalSectionCnt == 1) {
		sigset_t mask;
		sigfillset(&mask);
		sigprocmask(SIG_SETMASK, &mask, &CriticalSectionMask);
	}
}

void pwr_CriticalSectionExit()
{
	assert(CriticalSectionCnt > 0);
	if (--CriticalSectionCnt == 0) {
		sigprocmask(SIG_SETMASK, &CriticalSectionMask, NULL);
	}
}

#endif
