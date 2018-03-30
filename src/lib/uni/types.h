/*
 * types.h
 *
 *  Created on: 27 mar 2018
 *      Author: kamil
 */

#ifndef STM32_SRC_SYS_TYPES_H_
#define STM32_SRC_SYS_TYPES_H_

typedef enum {
	PWR_SLEEP,
	PWR_SLEEP_NOSYSTICK,
	PWR_STOP,
	PWR_STOP_NOSYSTICK,
	PWR_STANDBY,
} pwrmode_t;
#define __pwrmode_t_declared

#endif /* STM32_SRC_SYS_TYPES_H_ */
