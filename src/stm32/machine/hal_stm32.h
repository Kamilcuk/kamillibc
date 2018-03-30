/*
 * stm32hal.h
 *
 *  Created on: 09.10.2017
 *      Author: Kamil Cukrowski
 *     License: Jointly under MIT License and Beerware License.
 */
#ifndef STM32_STM32HAL_H_
#define STM32_STM32HAL_H_

#ifndef USE_HAL_DRIVER
# define USE_HAL_DRIVER 1
#endif

#ifndef NDEBUG
# define USE_FULL_ASSERT 1
#endif

#include "hal_stm32_switch.h"

#include <stdbool.h>
#include <assert.h>
#include <stdnoreturn.h>

noreturn __STATIC_INLINE void NVIC_SystemReset(void);

__STATIC_INLINE bool NVIC_IsInInterrupt()
{
    return (SCB->ICSR & SCB_ICSR_VECTACTIVE_Msk) != 0 ;
}

__STATIC_INLINE void HAL_try(HAL_StatusTypeDef status)
{
#ifdef DEBUG
	volatile HAL_StatusTypeDef debug_HAL_StatusTypeDef = status;
	UNUSED(debug_HAL_StatusTypeDef);
#endif
	assert(status == HAL_OK);
}

#ifdef HAL_CRYP_MODULE_ENABLED

static inline bool HAL_CRYP_IsReady(const CRYP_HandleTypeDef *hcryp)
{
	assert(hcryp->State != HAL_CRYP_STATE_RESET);
	return hcryp->State == HAL_CRYP_STATE_READY;
}

#endif

#ifdef HAL_SPI_MODULE_ENABLED

static inline bool HAL_SPI_IsTransmitReceiveReady(const SPI_HandleTypeDef *hspi)
{
	assert(hspi->State != HAL_SPI_STATE_RESET);
	return hspi->State == HAL_SPI_STATE_READY;
}

#endif

#ifdef HAL_UART_MODULE_ENABLED

static inline bool HAL_UART_IsReceiveReady(const UART_HandleTypeDef *huart)
{
	assert(huart->gState != HAL_UART_STATE_RESET);
	return huart->RxState == HAL_UART_STATE_READY;
}

static inline bool HAL_UART_IsTransmitReady(const UART_HandleTypeDef *huart)
{
	assert(huart->gState != HAL_UART_STATE_RESET);
	return huart->gState == HAL_UART_STATE_READY;
}

#endif

#endif /* STM32_STM32HAL_H_ */
