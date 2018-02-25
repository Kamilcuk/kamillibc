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

#if defined(STM32F0) || defined(STM32F0) || defined(STM32F0)
# ifndef STM32F0
#  define STM32F0 1
# endif
# ifndef STM32f0xx
#  define STM32f0xx 1
# endif
# ifndef STM32F0XX
#  define STM32F0XX 1
# endif
# include "stm32f0xx.h"
# include "stm32f0xx_hal.h"
#endif

#if defined(STM32F1) || defined(STM32F1) || defined(STM32F1)
# ifndef STM32F1
#  define STM32F1 1
# endif
# ifndef STM32f1xx
#  define STM32f1xx 1
# endif
# ifndef STM32F1XX
#  define STM32F1XX 1
# endif
# include "stm32f1xx.h"
# include "stm32f1xx_hal.h"
#endif

#if defined(STM32F2) || defined(STM32F2) || defined(STM32F2)
# ifndef STM32F2
#  define STM32F2 1
# endif
# ifndef STM32f2xx
#  define STM32f2xx 1
# endif
# ifndef STM32F2XX
#  define STM32F2XX 1
# endif
# include "stm32f2xx.h"
# include "stm32f2xx_hal.h"
#endif

#if defined(STM32F4) || defined(STM32F4) || defined(STM32F4)
# ifndef STM32F4
#  define STM32F4 1
# endif
# ifndef STM32f4xx
#  define STM32f4xx 1
# endif
# ifndef STM32F4XX
#  define STM32F4XX 1
# endif
# include "stm32f4xx.h"
# include "stm32f4xx_hal.h"
#endif

#if defined(STM32L0) || defined(STM32L0) || defined(STM32L0)
# ifndef STM32L0
#  define STM32L0 1
# endif
# ifndef STM32l0xx
#  define STM32l0xx 1
# endif
# ifndef STM32L0XX
#  define STM32L0XX 1
# endif
# include "stm32l0xx.h"
# include "stm32l0xx_hal.h"
#endif

#if defined(STM32L1) || defined(STM32L1) || defined(STM32L1)
# ifndef STM32L1
#  define STM32L1 1
# endif
# ifndef STM32l1xx
#  define STM32l1xx 1
# endif
# ifndef STM32L1XX
#  define STM32L1XX 1
# endif
# include "stm32l1xx.h"
# include "stm32l1xx_hal.h"
#endif

#if defined(STM32L4) || defined(STM32L4) || defined(STM32L4)
# ifndef STM32L4
#  define STM32L4 1
# endif
# ifndef STM32l4xx
#  define STM32l4xx 1
# endif
# ifndef STM32L4XX
#  define STM32L4XX 1
# endif
# include "stm32l4xx.h"
# include "stm32l4xx_hal.h"
#endif

#include <stdbool.h>
#include <assert.h>

__STATIC_INLINE void HAL_try(HAL_StatusTypeDef status)
{
#ifndef NDEBUG
	volatile HAL_StatusTypeDef _hal_status = status;
#endif
	assert( status == HAL_STATUS_OK );
}

#ifdef HAL_SPI_MODULE_ENABLED

__STATIC_INLINE bool HAL_SPI_IsReady(const SPI_HandleTypeDef *hspi)
{
	assert(hspi);
	assert(hspi->gState != HAL_SPI_STATE_RESET);
	return hspi->gState == HAL_SPI_STATE_READY;
}

#endif // HAL_SPI_MODULE_ENABLED

#ifdef HAL_UART_MODULE_ENABLED

__STATIC_INLINE bool HAL_UART_IsReceiveReady(const UART_HandleTypeDef *huart)
{
	assert(huart);
	assert(huart->gState != HAL_UART_STATE_RESET);
	return huart->gState == HAL_UART_STATE_READY;
}

__STATIC_INLINE bool HAL_UART_IsTransmitReady(const UART_HandleTypeDef *huart)
{
	assert(huart);
	assert(huart->rxState != HAL_UART_STATE_RESET);
	return huart->rxState == HAL_UART_STATE_READY;
}

#endif // HAL_UART_MODULE_ENABLED

#endif /* STM32_STM32HAL_H_ */
