/*
 * wdg.h
 *
 *  Created on: 27 mar 2018
 *      Author: kamil
 */

#ifndef SRC_STM32CLIBS_OSAL_WDG_H_
#define SRC_STM32CLIBS_OSAL_WDG_H_

#include <stdbool.h>

void wdg_init();
bool wdg_get();
void wdg_refresh();

#endif /* SRC_STM32CLIBS_OSAL_WDG_H_ */
