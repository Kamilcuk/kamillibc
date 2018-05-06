/*
 * wdg.h
 *
 *  Created on: 27 mar 2018
 *      Author: kamil
 */

#ifndef SRC_STM32CLIBS_OSAL_WDG_H_
#define SRC_STM32CLIBS_OSAL_WDG_H_

#include <uni/types.h>
#include <stdbool.h>

#ifndef __wdg_t_defined
typedef int wdg_t;
#define __wdg_t_defined 1
#endif

bool wdg_get(void);
void wdg_refresh(void);

bool wdgn_get(wdg_t wdg);
void wdgn_refresh(wdg_t wdg);

#endif /* SRC_STM32CLIBS_OSAL_WDG_H_ */
