/*
 * assert_param.h
 *
 *  Created on: 25 lut 2018
 *      Author: kamil
 */

#ifndef STM32_ASSERT_PARAM_H_
#define STM32_ASSERT_PARAM_H_

#include "machine/stm32xxxx.h"

#ifdef  USE_FULL_ASSERT
#ifndef assert_param
#define assert_param(expr) ((expr) ? (void)0 : assert_failed((uint8_t *)__FILE__, __LINE__))
void assert_failed(uint8_t* file, uint32_t line);
#else
#define assert_param(expr) ((void)0)
#endif // assert_param
#endif /* USE_FULL_ASSERT */

#endif /* STM32_ASSERT_PARAM_H_ */
