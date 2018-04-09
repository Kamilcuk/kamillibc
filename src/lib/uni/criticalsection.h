/*
 * criticalsection.h
 *
 *  Created on: 7 kwi 2018
 *      Author: kamil
 */

#ifndef SRC_KAMILLIBC_UNI_CRITICALSECTION_H_
#define SRC_KAMILLIBC_UNI_CRITICALSECTION_H_

#include <uni/types.h>
#include <stdbool.h>

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

#define pwr_CRITICALSECTION_while(condition)  POWER_STATE_for(0, condition, 0, 0);

void pwr_CriticalSectionEnter();
void pwr_CriticalSectionExit();

void pwr_CriticalSectionEnter_callback();
void pwr_CriticalSectionExit_callback();

#endif /* SRC_KAMILLIBC_UNI_CRITICALSECTION_H_ */
