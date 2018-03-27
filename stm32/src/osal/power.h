/*
 * power.h
 *
 *  Created on: 28 mar 2018
 *      Author: kamil
 */

#ifndef SRC_STM32CLIBS_OSAL_POWER_H_
#define SRC_STM32CLIBS_OSAL_POWER_H_

#include <osal/types.h>

#define pwr_CRITICALSECTION() \
	for(int _tOdO = (UNI_CriticalSectionEnter(), 1); _tOdO; _tOdO = (UNI_CriticalSectionExit(), 0) )

#define pwr_CRITICALSECTION_for(pre, condition, action, post) do{ \
	for(;;) { \
		(void)(pre); \
		pwr_CriticalSectionEnter(); \
		const bool _tOdO = (bool)(condition); \
		if (_tOdO) { \
			(void)(action); \
		} \
		pwr_CriticalSectionExit(); \
		(void)(post); \
		if (!_tOdO) \
			break; \
	} \
}while(0)

#define pwr_CRITICAL_SECTION_while(condition)  POWER_STATE_for(0, condition, 0, 0);

#define pwr_mode_enter_on_cond(mode, condition) \
		pwr_CRITICALSECTION_for( \
				pwr_mode_enter_pre(mode), \
				pwr_mode_enter(mode), \
				pwr_mode_enter_post(mode), \
				pwr_mode_restore(mode))

void pwr_CriticalSectionEnter();
void pwr_CriticalSectionExit();

#ifndef __pwrmode_t_declared
#define __pwrmode_t_declared
typedef int pwrmode_t;
#endif

void pwr_mode_enter_pre(pwrmode_t mode);
void pwr_mode_enter(pwrmode_t mode);
void pwr_mode_enter_post(pwrmode_t mode);
void pwr_mode_restore(pwrmode_t from_mode);

#endif /* SRC_STM32CLIBS_OSAL_POWER_H_ */
