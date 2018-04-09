/*
 * breakpoint.h
 *
 *  Created on: 7 kwi 2018
 *      Author: kamil
 */

#ifndef SRC_STM32_BREAKPOINT_H_
#define SRC_STM32_BREAKPOINT_H_

#include <machine/hal.h>

static inline void breakpoint() {
#ifndef NDEBUG
	if (CoreDebug->DHCSR & CoreDebug_DHCSR_C_DEBUGEN_Msk)
		__BKPT(0);
#endif
}

#endif /* SRC_STM32_BREAKPOINT_H_ */
