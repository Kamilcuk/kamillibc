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

time_t HW_RTC_ticks_to_time_t_Callback(RTC_HandleTypeDef *hrtc, time_t ms);
time_t HW_RTC_time_t_to_ticks_Callback(RTC_HandleTypeDef *hrtc, time_t ms);
struct HW_RTC_DateTime_s HW_RTC_getDateTime(RTC_HandleTypeDef *hrtc);
time_t HW_RTC_clock(RTC_HandleTypeDef *hrtc);
time_ms_t HW_RTC_time_ms(RTC_HandleTypeDef *hrtc);
void HW_RTC_alarm_set_ms(RTC_HandleTypeDef *hrtc, uint32_t Alarm, time_ms_t delay);

#endif /* SRC_MACHINE_UNI_RTC_H_ */
