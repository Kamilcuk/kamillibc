/*
 * wdg.c
 *
 *  Created on: 7 kwi 2018
 *      Author: kamil
 */
#include <uni/wdg.h>

#include <machine/hal.h>

#ifdef HAL_IWDG_MODULE_ENABLED

extern IWDG_HandleTypeDef hiwdg;

static bool wdg_status;

void wdg_init() {
	wdg_status = __HAL_RCC_GET_FLAG(RCC_FLAG_IWDGRST);
}

bool wdg_get() {
	return wdg_status;
}

void wdg_refresh() {
	HAL_try( HAL_IWDG_Refresh(&hiwdg) );
}

#else

void wdg_init() {}
bool wdg_get() { return false; }
void wdg_refresh() {}

#endif
