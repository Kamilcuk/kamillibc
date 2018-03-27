/*
 * stm32hal-switch.h
 *
 *  Created on: 22 mar 2018
 *      Author: kamil
 */

#ifndef STM32_SRC_HAL_STM32_SWITCH_H_
#define STM32_SRC_HAL_STM32_SWITCH_H_


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


#endif /* STM32_SRC_HAL_STM32_SWITCH_H_ */

