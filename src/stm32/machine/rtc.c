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

#include <cdefs.h>
#include <time.h>
#include <sys/time.h>

#define HW_RTC_DBG(fmt, ...) //printf(fmt, ##__VA_ARGS__)

time_t HW_RTC_ticks_to_s(RTC_HandleTypeDef *hrtc, HW_RTC_ticks_t ticks)
{
	return HW_RTC_ticks_to_time_t_Callback(hrtc, ticks);
}

HW_RTC_ticks_t HW_RTC_ticks_from_s(RTC_HandleTypeDef *hrtc, time_t s)
{
	return HW_RTC_time_t_to_ticks_Callback(hrtc, s);
}

time_ms_t HW_RTC_ticks_to_ms(RTC_HandleTypeDef *hrtc, HW_RTC_ticks_t ticks)
{
	return HW_RTC_ticks_to_time_t_Callback(hrtc, ticks * (HW_RTC_ticks_t)1000);
}

HW_RTC_ticks_t HW_RTC_ticks_from_ms(RTC_HandleTypeDef *hrtc, time_ms_t ms)
{
	return HW_RTC_time_t_to_ticks_Callback(hrtc, ms) / (HW_RTC_ticks_t)1000;
}

time_us_t HW_RTC_ticks_to_us(RTC_HandleTypeDef *hrtc, HW_RTC_ticks_t ticks)
{
	return HW_RTC_ticks_to_time_t_Callback(hrtc, ticks * (HW_RTC_ticks_t)1000000);
}

HW_RTC_ticks_t HW_RTC_ticks_from_us(RTC_HandleTypeDef *hrtc, time_us_t us)
{
	return HW_RTC_time_t_to_ticks_Callback(hrtc, us) / (HW_RTC_ticks_t)1000000;
}

struct HW_RTC_DateTime_s HW_RTC_getDateTime(RTC_HandleTypeDef *hrtc)
{
	struct HW_RTC_DateTime_s dt;
	HAL_try(HAL_RTC_GetTime(hrtc, &dt.time, RTC_FORMAT_BIN));
	HAL_try(HAL_RTC_GetDate(hrtc, &dt.date, RTC_FORMAT_BIN));
	return dt;
}

HW_RTC_ticks_t HW_RTC_clock(RTC_HandleTypeDef *hrtc)
{
	const struct HW_RTC_DateTime_s now = HW_RTC_getDateTime(hrtc);
	const HW_RTC_ticks_t ticks = HW_RTC_DateTime_to_time_t(now);
	return ticks;
}

time_t HW_RTC_time_s(RTC_HandleTypeDef *hrtc)
{
	return HW_RTC_ticks_to_s(hrtc, HW_RTC_clock(hrtc));
}

time_ms_t HW_RTC_time_ms(RTC_HandleTypeDef *hrtc)
{
	return HW_RTC_ticks_to_ms(hrtc, HW_RTC_clock(hrtc));
}

time_us_t HW_RTC_time_us(RTC_HandleTypeDef *hrtc)
{
	return HW_RTC_ticks_to_us(hrtc, HW_RTC_clock(hrtc));
}

static inline RTC_AlarmTypeDef HW_RTC_nowPlusTicksToAlarmType_noAlarmDate(RTC_HandleTypeDef *hrtc,
		uint32_t Alarm, HW_RTC_ticks_t delayticks)
{
	assert(0ull < delayticks
		&& delayticks < HW_RTC_ticks_from_ms(hrtc, 24ull * 3600ull * 1000ull));
	RTC_TimeTypeDef time;
	HAL_try(HAL_RTC_GetTime(hrtc, &time, RTC_FORMAT_BIN));
	const HW_RTC_ticks_t nowticks = time.Hours * 3600 + time.Minutes * 60 + time.Seconds;
	const HW_RTC_ticks_t arm = nowticks + delayticks != 0 ? delayticks : 1;
	const HW_RTC_ticks_t arm_to_day = arm % (24*3600);
	const RTC_TimeTypeDef armtime = HW_RTC_ticks_to_TimeTypeDef(arm_to_day);
	HW_RTC_DBG("delay:%ld delayticks:%ld "
			"now= %d:%d:%d "
			"armtime= %d:%d:%d\n",
			delay, delayticks,
			time.Hours, time.Minutes, time.Seconds,
			armtime.Hours, armtime.Minutes, armtime.Seconds);
	const RTC_AlarmTypeDef sAlarm = {
			.Alarm = Alarm,
			.AlarmTime = armtime,
	};
	return sAlarm;
}

static inline RTC_AlarmTypeDef HW_RTC_nowPlusTicksToAlarmType(RTC_HandleTypeDef *hrtc,
		uint32_t Alarm, HW_RTC_ticks_t delayticks)
{
#ifndef RTC_ALARMDATEWEEKDAYSEL_DATE
	return HW_RTC_nowPlusTicksToAlarmType_noAlarmDate(hrtc, Alarm, delayticks);
#else
	// TODO: support delay longer then 24 hours on STM32Lx series and support subseconds
	RTC_AlarmTypeDef sAlarm = HW_RTC_nowPlusTicksToAlarmType_noAlarmDate(hrtc, Alarm, delayticks);
	sAlarm.AlarmMask = RTC_ALARMMASK_HOURS;
	sAlarm.AlarmDateWeekDaySel = RTC_ALARMDATEWEEKDAYSEL_DATE;
	sAlarm.AlarmDateWeekDay = RTC_WEEKDAY_MONDAY;
	sAlarm.AlarmSubSecondMask = 0;
	sAlarm.AlarmTime.SubSeconds = 0;
	return sAlarm;
#endif
}

void HW_RTC_alarm_set_ticks(RTC_HandleTypeDef *hrtc, uint32_t Alarm, HW_RTC_ticks_t delay)
{
	if (delay == 0) {
		HAL_try(HAL_RTC_DeactivateAlarm(hrtc, Alarm));
		return;
	}
	RTC_AlarmTypeDef sAlarm = HW_RTC_nowPlusTicksToAlarmType(hrtc, Alarm, delay);
	HAL_try(HAL_RTC_SetAlarm_IT(hrtc, &sAlarm, RTC_FORMAT_BIN));
}

void HW_RTC_alarm_set_s(RTC_HandleTypeDef *hrtc, uint32_t Alarm, time_t delay)
{
	HW_RTC_alarm_set_ms(hrtc, Alarm, delay * 1000);
}

void HW_RTC_alarm_set_ms(RTC_HandleTypeDef *hrtc, uint32_t Alarm, time_ms_t delay)
{
	if (delay == 0) {
		HW_RTC_alarm_set_ticks(hrtc, Alarm, 0);
	} else {
		const HW_RTC_ticks_t delayticks = HW_RTC_ticks_from_ms(hrtc, delay);
		assert("delay must at least result in one tick" && delayticks != 0);
		HW_RTC_alarm_set_ticks(hrtc, Alarm, delayticks);
	}
}

void HW_RTC_alarm_set_us(RTC_HandleTypeDef *hrtc, uint32_t Alarm, time_us_t delay)
{
	if (delay == 0) {
		HW_RTC_alarm_set_ticks(hrtc, Alarm, 0);
	} else {
		const HW_RTC_ticks_t delayticks = HW_RTC_ticks_from_us(hrtc, delay);
		assert("delay must at least result in one tick" && delayticks != 0);
		HW_RTC_alarm_set_ticks(hrtc, Alarm, delayticks);
	}
}

#endif // HAL_RTC_MODULE_ENABLED
