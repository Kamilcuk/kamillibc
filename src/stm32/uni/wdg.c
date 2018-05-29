/*
 * wdg.c
 *
 *  Created on: 7 kwi 2018
 *      Author: kamil
 */
#include <uni/wdg.h>

#include <machine/hal.h>

#ifdef HAL_IWDG_MODULE_ENABLED

static bool wdg_status;

void uni_wdg_init(void)
{
	wdg_status = __HAL_RCC_GET_FLAG(RCC_FLAG_IWDGRST);
}

bool wdg_get(void)
{
	return wdg_status;
}

void wdg_refresh(void)
{
	extern IWDG_HandleTypeDef hiwdg;
	if (hiwdg.Instance != NULL)
		HAL_try(HAL_IWDG_Refresh(&hiwdg));
}

#else

void uni_wdg_init(void) {
}

bool wdg_get(void) {
	return false;
}

void wdg_refresh(void) {
}

#endif
