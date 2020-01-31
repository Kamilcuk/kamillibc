/*
 * gpio.c
 *
 *  Created on: 13 kwi 2018
 *      Author: kamil
 */
#include <uni/gpio.h>

#if __UNI_USE_GPIO_DEFAULT_IMPLEMENTATION == 1

void gpio_set(gpio_t t, bool value) {
	__gpio_set(t, value);
}

bool gpio_get(gpio_t t) {
	return __gpio_get(t);
}

void gpio_toggle(gpio_t t) {
	__gpio_toggle(t);
}

#endif
