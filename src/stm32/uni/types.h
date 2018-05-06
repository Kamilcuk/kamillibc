/*
 * types.h
 *
 *  Created on: 13 kwi 2018
 *      Author: kamil
 */
#ifndef _UNI_TYPES_H_
#define _UNI_TYPES_H_

#include <uni/_types.h>

#ifndef __pwrmode_t_defined
typedef enum {
	PWRMODE_RUN,
	PWRMODE_LOW_POWER_RUN,
	PWRMODE_SLEEP,
	PWRMODE_SLEEP_NOSYSTICK,
	PWRMODE_STOP,
	PWRMODE_STOP_NOSYSTICK,
	PWRMODE_STANDBY,
} pwrmode_t;
#define __pwrmode_t_defined 1
#define __UNI_PWRMODE_DEFAULT_IMPLEMENTATION_ENABLED 1
#endif

#ifndef __gpio_t_defined
#include <uni/__gpio.h>
#define GPIO_INIT(GPIOx,GPIO_Pin)  __GPIO_INIT(GPIOx,GPIO_Pin)
typedef __gpio_t gpio_t;
#define __gpio_t_defined 1
#define __UNI_USE_GPIO_DEFAULT_IMPLEMENTATION 1
#endif

#include <uni/system_types.h>

#endif /* _UNI_TYPES_H_ */
