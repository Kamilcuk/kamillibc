/*
 * rtc.h
 *
 *  Created on: 4 mar 2018
 *      Author: kamil
 */

#ifndef SRC_MACHINE_UNI_RTC_H_
#define SRC_MACHINE_UNI_RTC_H_

#include <machine/hal.h>
#include <machine/rtc_time.h>
#include <time_ex.h>

#ifdef HAL_RTC_MODULE_ENABLED

time_t HW_RTC_ticks_to_s(RTC_HandleTypeDef *hrtc, HW_RTC_ticks_t ticks);
HW_RTC_ticks_t HW_RTC_ticks_from_s(RTC_HandleTypeDef *hrtc, time_t s);
time_ms_t HW_RTC_ticks_to_ms(RTC_HandleTypeDef *hrtc, HW_RTC_ticks_t ticks);
HW_RTC_ticks_t HW_RTC_ticks_from_ms(RTC_HandleTypeDef *hrtc, time_ms_t ms);
time_us_t HW_RTC_ticks_to_us(RTC_HandleTypeDef *hrtc, HW_RTC_ticks_t ticks);
HW_RTC_ticks_t HW_RTC_ticks_from_us(RTC_HandleTypeDef *hrtc, time_us_t us);

struct HW_RTC_DateTime_s HW_RTC_getDateTime(RTC_HandleTypeDef *hrtc);

HW_RTC_ticks_t HW_RTC_clock(RTC_HandleTypeDef *hrtc);
time_t HW_RTC_time_s(RTC_HandleTypeDef *hrtc);
time_ms_t HW_RTC_time_ms(RTC_HandleTypeDef *hrtc);
time_us_t HW_RTC_time_us(RTC_HandleTypeDef *hrtc);

void HW_RTC_alarm_set_ticks(RTC_HandleTypeDef *hrtc, uint32_t Alarm, HW_RTC_ticks_t delay);
void HW_RTC_alarm_set_s(RTC_HandleTypeDef *hrtc, uint32_t Alarm, time_t delay);
void HW_RTC_alarm_set_ms(RTC_HandleTypeDef *hrtc, uint32_t Alarm, time_ms_t delay);
void HW_RTC_alarm_set_us(RTC_HandleTypeDef *hrtc, uint32_t Alarm, time_us_t delay);

// weak functions(!)
time_t HW_RTC_ticks_to_time_t_Callback(RTC_HandleTypeDef *hrtc, time_t ms);
time_t HW_RTC_time_t_to_ticks_Callback(RTC_HandleTypeDef *hrtc, time_t ms);

#endif

#endif /* SRC_MACHINE_UNI_RTC_H_ */
