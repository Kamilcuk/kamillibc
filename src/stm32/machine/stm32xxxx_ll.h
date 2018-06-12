/*
 * stm32xxxx_ll_adc.h
 *
 *  Created on: 13 kwi 2018
 *      Author: kamil
 */

#ifndef SRC_STM32_MACHINE_STM32XXXX_LL_H_
#define SRC_STM32_MACHINE_STM32XXXX_LL_H_

#include <machine/hal.h>

#if defined(STM32F0)
#error
#elif defined(STM32F1)
#include <stm32f1xx_ll_adc.h>
#include <stm32f1xx_ll_bus.h>
#include <stm32f1xx_ll_cortex.h>
#include <stm32f1xx_ll_crc.h>
#include <stm32f1xx_ll_dac.h>
#include <stm32f1xx_ll_dma.h>
#include <stm32f1xx_ll_exti.h>
#include <stm32f1xx_ll_fsmc.h>
#include <stm32f1xx_ll_gpio.h>
#include <stm32f1xx_ll_i2c.h>
#include <stm32f1xx_ll_iwdg.h>
#include <stm32f1xx_ll_pwr.h>
#include <stm32f1xx_ll_rcc.h>
#include <stm32f1xx_ll_rtc.h>
#include <stm32f1xx_ll_sdmmc.h>
#include <stm32f1xx_ll_spi.h>
#include <stm32f1xx_ll_system.h>
#include <stm32f1xx_ll_tim.h>
#include <stm32f1xx_ll_usart.h>
#include <stm32f1xx_ll_usb.h>
#include <stm32f1xx_ll_utils.h>
#include <stm32f1xx_ll_wwdg.h>
#elif defined(STM32F2)
#error
#elif defined(STM32F4)
#error
#elif defined(STM32L0)
#include <stm32l0xx_hal_firewall.h>
#include <stm32l0xx_ll_adc.h>
#include <stm32l0xx_ll_bus.h>
#include <stm32l0xx_ll_comp.h>
#include <stm32l0xx_ll_cortex.h>
#include <stm32l0xx_ll_crc.h>
#include <stm32l0xx_ll_crs.h>
#include <stm32l0xx_ll_dac.h>
#include <stm32l0xx_ll_dma.h>
#include <stm32l0xx_ll_exti.h>
#include <stm32l0xx_ll_gpio.h>
#include <stm32l0xx_ll_i2c.h>
#include <stm32l0xx_ll_iwdg.h>
#include <stm32l0xx_ll_lptim.h>
#include <stm32l0xx_ll_lpuart.h>
#include <stm32l0xx_ll_pwr.h>
#include <stm32l0xx_ll_rcc.h>
#include <stm32l0xx_ll_rng.h>
#include <stm32l0xx_ll_rtc.h>
#include <stm32l0xx_ll_spi.h>
#include <stm32l0xx_ll_system.h>
#include <stm32l0xx_ll_tim.h>
#include <stm32l0xx_ll_usart.h>
#include <stm32l0xx_ll_utils.h>
#include <stm32l0xx_ll_wwdg.h>
#elif defined(STM32L1)
#include <stm32l1xx_ll_adc.h>
#include <stm32l1xx_ll_bus.h>
#include <stm32l1xx_ll_cortex.h>
#include <stm32l1xx_ll_crc.h>
#include <stm32l1xx_ll_dac.h>
#include <stm32l1xx_ll_dma.h>
#include <stm32l1xx_ll_exti.h>
#include <stm32l1xx_ll_fsmc.h>
#include <stm32l1xx_ll_gpio.h>
#include <stm32l1xx_ll_i2c.h>
#include <stm32l1xx_ll_iwdg.h>
#include <stm32l1xx_ll_pwr.h>
#include <stm32l1xx_ll_rcc.h>
#include <stm32l1xx_ll_rtc.h>
#include <stm32l1xx_ll_sdmmc.h>
#include <stm32l1xx_ll_spi.h>
#include <stm32l1xx_ll_system.h>
#include <stm32l1xx_ll_tim.h>
#include <stm32l1xx_ll_usart.h>
#include <stm32l1xx_ll_utils.h>
#include <stm32l1xx_ll_wwdg.h>
#elif defined(STM32L4)
#error
#endif


#endif /* SRC_STM32_MACHINE_STM32XXXX_LL_H_ */
