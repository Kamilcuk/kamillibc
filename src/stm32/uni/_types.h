/*
 * types.h
 *
 *  Created on: 27 mar 2018
 *      Author: kamil
 */

#ifndef STM32_SRC_SYS_TYPES_H_
#define STM32_SRC_SYS_TYPES_H_

typedef enum {
	PWRMODE_RUN,
	PWRMODE_LOW_POWER_RUN,
	PWRMODE_SLEEP,
	PWRMODE_SLEEP_NOSYSTICK,
	PWRMODE_STOP,
	PWRMODE_STOP_NOSYSTICK,
	PWRMODE_STANDBY,
} pwrmode_t;
#define __pwrmode_t_declared

#endif /* STM32_SRC_SYS_TYPES_H_ */
