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

#ifndef __pwrmode_t_declared
#define __pwrmode_t_declared
typedef int pwrmode_t;
#endif

#define pwrmode_enter_while(mode, condition) do{ \
		pwr_CRITICALSECTION_for( \
				pwrmode_enter_pre(mode), \
				(condition), \
				pwrmode_enter(mode), \
				pwrmode_enter_post(mode)); \
		pwrmode_restore(mode); \
	}while(0)

void pwrmode_enter_pre(pwrmode_t mode);
void pwrmode_enter(pwrmode_t mode);
void pwrmode_enter_post(pwrmode_t mode);
void pwrmode_restore(pwrmode_t mode);

#endif /* SRC_STM32CLIBS_OSAL_POWER_H_ */
