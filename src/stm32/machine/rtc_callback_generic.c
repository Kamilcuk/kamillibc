/*
 * rtc_callback_generic.c
 *
 *  Created on: 3 cze 2018
 *      Author: kamil
 */
#include <machine/rtc.h>

#ifdef HAL_RTC_MODULE_ENABLED

static uint32_t HW_RTC_ticks_to_time_getFreq(void)
{
#ifdef RCC_PERIPHCLK_RTC
	static uint32_t HW_RTC_ticks_to_time_t_freq = 0;
	if (HW_RTC_ticks_to_time_t_freq == 0) {
		HW_RTC_ticks_to_time_t_freq = HAL_RCCEx_GetPeriphCLKFreq(RCC_PERIPHCLK_RTC);
	}
	assert(HW_RTC_ticks_to_time_t_freq != 0);
	return HW_RTC_ticks_to_time_t_freq;
#else
	assert(0);
	return (uint32_t)-1;
#endif
}

static uint32_t HW_RTC_ticks_to_time_getPresc(RTC_HandleTypeDef *hrtc)
{
	assert(hrtc);
	return
#ifdef IS_RTC_ASYNCH_PREDIV
			(hrtc->Init.AsynchPrediv + 1)
#endif
#ifdef IS_RTC_SYNCH_PREDIV
			*
			(hrtc->Init.SynchPrediv + 1)
#endif
			*
			1;
}

__attribute__((__weak__))
time_t HW_RTC_ticks_to_time_t_Callback(RTC_HandleTypeDef *hrtc, time_t t)
{
	return t * HW_RTC_ticks_to_time_getPresc(hrtc) / HW_RTC_ticks_to_time_getFreq();
}

__attribute__((__weak__))
time_t HW_RTC_time_t_to_ticks_Callback(RTC_HandleTypeDef *hrtc, time_t t)
{
	return t * HW_RTC_ticks_to_time_getFreq() / HW_RTC_ticks_to_time_getPresc(hrtc);
}

#endif
