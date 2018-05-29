/*
 * rtc.c
 *
 *  Created on: 4 mar 2018
 *      Author: kamil
 */
#include <machine/hal.h>
#ifdef HAL_RTC_MODULE_ENABLED

#include "rtc.h"
#include "rtc_time.h"

#include <try.h>

#include <cdefs.h>
#include <time.h>
#include <sys/time.h>

#define HW_RTC_DBG(fmt, ...) //printf(fmt, ##__VA_ARGS__)

__weak
time_t HW_RTC_ticks_to_time_t_Callback(RTC_HandleTypeDef *hrtc, time_t t)
{
	assert(HAL_RCCEx_GetPeriphCLKFreq(RCC_PERIPHCLK_RTC) == LSE_VALUE);
	const uint32_t div = (hrtc->Init.AsynchPrediv+1);
	assert(div == 128);
	return t * 128 / LSE_VALUE;
}

__weak
time_t HW_RTC_time_t_to_ticks_Callback(RTC_HandleTypeDef *hrtc, time_t t)
{
	assert(HAL_RCCEx_GetPeriphCLKFreq(RCC_PERIPHCLK_RTC) == LSE_VALUE);
	const uint32_t div = (hrtc->Init.AsynchPrediv+1);
	assert(div == 128);
	return t * LSE_VALUE / 128;
}

static inline time_t HW_RTC_ticks_to_ms(RTC_HandleTypeDef *hrtc, time_t ticks)
{
	return HW_RTC_ticks_to_time_t_Callback(hrtc, ticks * 1000);
}

static inline time_t HW_RTC_ms_to_ticks(RTC_HandleTypeDef *hrtc, time_t ms)
{
	return HW_RTC_time_t_to_ticks_Callback(hrtc, ms) / 1000;
}


struct HW_RTC_DateTime_s HW_RTC_getDateTime(RTC_HandleTypeDef *hrtc)
{
	struct HW_RTC_DateTime_s dt;
	try( HAL_RTC_GetTime(hrtc, &dt.time, RTC_FORMAT_BIN) == HAL_OK );
	try( HAL_RTC_GetDate(hrtc, &dt.date, RTC_FORMAT_BIN) == HAL_OK );
	return dt;
}

time_t HW_RTC_clock(RTC_HandleTypeDef *hrtc)
{
	const struct HW_RTC_DateTime_s now = HW_RTC_getDateTime(hrtc);
	const time_t ticks = HW_RTC_DateTime_to_time_t(now);
	return ticks;
}

time_t HW_RTC_time_ms(RTC_HandleTypeDef *hrtc)
{
	return HW_RTC_ticks_to_ms(hrtc, HW_RTC_clock(hrtc));
}

static inline RTC_TimeTypeDef HW_RTC_now_plus_interval_to_AlarmTime(RTC_HandleTypeDef *hrtc, time_ms_t delay)
{
	RTC_TimeTypeDef time;
	try( HAL_RTC_GetTime(hrtc, &time, RTC_FORMAT_BIN) == HAL_OK );
	const time_t nowticks = time.Hours * 3600 + time.Minutes * 60 + time.Seconds;
	const time_t delayticks = HW_RTC_ms_to_ticks(hrtc, delay);
	const time_t arm = nowticks + delayticks;
	const time_t arm_to_day = arm % (24*3600);
	const RTC_TimeTypeDef armtime = HW_RTC_time_t_to_TimeTypeDef(arm_to_day);
	HW_RTC_DBG("delay:%ld delayticks:%ld "
			"now= %d:%d:%d "
			"armtime= %d:%d:%d\n",
			delay, delayticks,
			time.Hours, time.Minutes, time.Seconds,
			armtime.Hours, armtime.Minutes, armtime.Seconds);
	return armtime;
}

void HW_RTC_alarm_set_ms(RTC_HandleTypeDef *hrtc, uint32_t Alarm, time_ms_t delay)
{
	assert(delay < 24*3600);
	if (delay != 0) {
		RTC_AlarmTypeDef sAlarm = {
				.AlarmTime = HW_RTC_now_plus_interval_to_AlarmTime(hrtc, delay),
				.Alarm = Alarm,
		};
		HAL_try( HAL_RTC_SetAlarm_IT(hrtc, &sAlarm, RTC_FORMAT_BIN) );
	} else {
		HAL_try( HAL_RTC_DeactivateAlarm(hrtc, Alarm) );
	}
}

#endif // HAL_RTC_MODULE_ENABLED
