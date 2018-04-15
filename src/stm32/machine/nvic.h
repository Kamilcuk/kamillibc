/*
 * nvic.h
 *
 *  Created on: 15 kwi 2018
 *      Author: kamil
 */

#ifndef SRC_STM32_MACHINE_NVIC_H_
#define SRC_STM32_MACHINE_NVIC_H_

#include "stm32xxxx.h"

#include <stdbool.h>
#include <stdnoreturn.h>

noreturn __STATIC_INLINE void NVIC_SystemReset(void);

__STATIC_INLINE bool NVIC_IsInInterrupt()
{
	return (SCB->ICSR & SCB_ICSR_VECTACTIVE_Msk) != 0 ;
}

struct IRQlist_s {
	const IRQn_Type * restrict list;
	size_t num;
};

__STATIC_INLINE void NVIC_DisableIRQn(const IRQn_Type IRQns[], size_t IRQnn) {
	while(IRQnn--) {
		NVIC_DisableIRQ(IRQns++[0]);
	}
}

__STATIC_INLINE void NVIC_EnableIRQn(const IRQn_Type IRQns[], size_t IRQnn) {
	IRQns += IRQnn-1;
	while(IRQnn--) {
		NVIC_EnableIRQ(IRQns--[0]);
	}
}
__STATIC_INLINE void NVIC_DisableIRQlist(struct IRQlist_s IRQlist) {
	NVIC_DisableIRQn(IRQlist.list, IRQlist.num);
}

__STATIC_INLINE void NVIC_EnableIRQlist(const struct IRQlist_s IRQlist) {
	NVIC_EnableIRQn(IRQlist.list, IRQlist.num);
}

#endif /* SRC_STM32_MACHINE_NVIC_H_ */
