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

#include "stm32xxxx.h"
#include "nvic.h"

#include <stdbool.h>
#include <assert.h>
#include <stddef.h>
#include <stdnoreturn.h>

void HAL_try(HAL_StatusTypeDef status);
#define HAL_try(status)  do{ \
	volatile HAL_StatusTypeDef HAL_try_status = (status); \
	assert(HAL_try_status == HAL_OK); \
}while(0)

__STATIC_INLINE HAL_StatusTypeDef HAL_try_ok_or_timeout(HAL_StatusTypeDef status)
{
#ifdef DEBUG
	static volatile HAL_StatusTypeDef HAL_try_status = HAL_OK;
	HAL_try_status = status;
	UNUSED(HAL_try_status);
#endif
	assert(status == HAL_OK || status == HAL_TIMEOUT);
	return status;
}

#ifdef HAL_CRYP_MODULE_ENABLED

static inline bool HAL_CRYP_IsReady(const CRYP_HandleTypeDef *hcryp)
{
	assert(hcryp != NULL && hcryp->State != HAL_CRYP_STATE_RESET);
	return hcryp->State == HAL_CRYP_STATE_READY;
}

#endif

#ifdef HAL_SPI_MODULE_ENABLED

static inline bool HAL_SPI_IsTransmitReceiveReady(const SPI_HandleTypeDef *hspi)
{
	assert(hspi != NULL && hspi->State != HAL_SPI_STATE_RESET);
	return hspi->State == HAL_SPI_STATE_READY;
}

#endif

#ifdef HAL_UART_MODULE_ENABLED

static inline bool HAL_UART_IsReceiveReady(const UART_HandleTypeDef *huart)
{
	assert(huart != NULL);
#if defined(STM32L1)
	assert(huart->State != HAL_UART_STATE_RESET);
	return huart->State == HAL_UART_STATE_READY;
#else
	assert(huart->gState != HAL_UART_STATE_RESET);
	return huart->RxState == HAL_UART_STATE_READY;
#endif
}

static inline bool HAL_UART_IsTransmitReady(const UART_HandleTypeDef *huart)
{
	assert(huart != NULL);
#if defined(STM32L1)
	assert(huart->State != HAL_UART_STATE_RESET);
	return huart->State == HAL_UART_STATE_READY;
#else
	assert(huart->gState != HAL_UART_STATE_RESET);
	return huart->gState == HAL_UART_STATE_READY;
#endif
}

#endif

static inline bool HAL_IWDG_test()
{
	return __HAL_RCC_GET_FLAG(RCC_FLAG_IWDGRST);
}

double HW_IWDG_calc_period(uint32_t Prescaler, uint32_t Period, double *min, double *max);

#endif /* STM32_STM32HAL_H_ */
