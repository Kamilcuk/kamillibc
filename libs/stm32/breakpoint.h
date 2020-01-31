/*
 * breakpoint.h
 *
 *  Created on: 7 kwi 2018
 *      Author: kamil
 */

#ifndef SRC_STM32_BREAKPOINT_H_
#define SRC_STM32_BREAKPOINT_H_

#include <machine/hal.h>
#include <cdefs.h>

#ifndef NDEBUG
#define _breakpoint(x) __BKPT(x)
#ifdef Coredebug
#define _breakpoint_isDebuggerConnected()  (CoreDebug->DHCSR & CoreDebug_DHCSR_C_DEBUGEN_Msk)
#else
#define _breakpoint_isDebuggerConnected()  (1)
#endif
#define breakpoint() do{ \
	if (_breakpoint_isDebuggerConnected()) { \
		_breakpoint(__LINE__); \
	} \
}while(0)
#else
#define breakpoint()  ((void)0)
#endif

#endif /* SRC_STM32_BREAKPOINT_H_ */
