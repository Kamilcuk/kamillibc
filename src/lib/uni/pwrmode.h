/*
 * power.h
 *
 *  Created on: 28 mar 2018
 *      Author: kamil
 */

#ifndef SRC_STM32CLIBS_OSAL_POWER_H_
#define SRC_STM32CLIBS_OSAL_POWER_H_

#include <uni/types.h>
#include <uni/criticalsection.h>

#ifndef __pwrmode_t_defined
typedef int pwrmode_t;
#define __pwrmode_t_defined 1
#endif

#define PWRMODE_WHILE(mode, condition) \
	do{ \
		CRITICALSECTION_FOR((condition),pwrmode_enter(mode)); \
		pwrmode_restore(mode); \
	}while(0)

void pwrmode_enter(pwrmode_t mode);
void pwrmode_restore(pwrmode_t mode);

#endif /* SRC_STM32CLIBS_OSAL_POWER_H_ */
