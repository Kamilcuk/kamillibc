/*
 * gpio_t.h
 *
 *  Created on: 13 kwi 2018
 *      Author: kamil
 */

#ifndef SRC_STM32_UNI_GPIO_T_H_
#define SRC_STM32_UNI_GPIO_T_H_

#include <machine/hal.h>

typedef struct {
	GPIO_TypeDef *GPIOx;
	uint32_t GPIO_Pin;
} __gpio_t;
#define ___gpio_t_defined 1

#define __GPIO_INIT(GPIOx, GPIO_Pin)  {(GPIOx),(GPIO_Pin)}

static inline void __gpio_set(__gpio_t t, bool value) {
	HAL_GPIO_WritePin(t.GPIOx, t.GPIO_Pin, value);
}
static inline bool __gpio_get(__gpio_t t) {
	return HAL_GPIO_ReadPin(t.GPIOx, t.GPIO_Pin);
}
static inline void __gpio_toggle(__gpio_t t) {
	HAL_GPIO_TogglePin(t.GPIOx, t.GPIO_Pin);
}

#endif /* SRC_STM32_UNI_GPIO_T_H_ */
