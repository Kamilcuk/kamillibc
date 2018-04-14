/*
 * stm32hal-switch.h
 *
 *  Created on: 22 mar 2018
 *      Author: kamil
 */

#ifndef STM32_SRC_HAL_STM32_SWITCH_H_
#define STM32_SRC_HAL_STM32_SWITCH_H_


#if defined(STM32F0)
# include "stm32f0xx.h"
# include "stm32f0xx_hal.h"
#elif defined(STM32F1)
# include "stm32f1xx.h"
# include "stm32f1xx_hal.h"
#elif defined(STM32F2)
# include "stm32f2xx.h"
# include "stm32f2xx_hal.h"
#elif defined(STM32F4)
# include "stm32f4xx.h"
# include "stm32f4xx_hal.h"
#elif defined(STM32L0)
# include "stm32l0xx.h"
# include "stm32l0xx_hal.h"
#elif defined(STM32L1)
# include "stm32l1xx.h"
# include "stm32l1xx_hal.h"
#elif defined(STM32L4)
# include "stm32l4xx.h"
# include "stm32l4xx_hal.h"
#endif

#endif /* STM32_SRC_HAL_STM32_SWITCH_H_ */

