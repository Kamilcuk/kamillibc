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
#define breakpoint() do{ \
	if (CoreDebug->DHCSR & CoreDebug_DHCSR_C_DEBUGEN_Msk) { \
		_breakpoint(__LINE__); \
	} \
}while(0)
#else
#define breakpoint() do{}while(0)
#endif

#endif /* SRC_STM32_BREAKPOINT_H_ */
