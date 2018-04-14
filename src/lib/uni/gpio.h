/*
 * gpio.h
 *
 *  Created on: 27 mar 2018
 *      Author: kamil
 */

#ifndef STM32_SRC_SYS_GPIO_H_
#define STM32_SRC_SYS_GPIO_H_

#include <uni/types.h>

#include <stdbool.h>

#ifndef __gpio_t_defined
typedef int gpio_t;
#define __gpio_t_defined 1
#endif

void gpio_set(gpio_t t, bool value);
bool gpio_get(gpio_t t);
void gpio_toggle(gpio_t t);

#endif /* STM32_SRC_SYS_GPIO_H_ */
