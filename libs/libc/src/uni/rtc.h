/*
 * rtc.h
 *
 *  Created on: 27 mar 2018
 *      Author: kamil
 */

#ifndef SRC_STM32CLIBS_OSAL_RTC_H_
#define SRC_STM32CLIBS_OSAL_RTC_H_

#include <uni/types.h>

#include <time_ex.h> // time_ms_t time_us_t

#include <time.h>

#ifndef __rtc_t_defined
typedef int rtc_t;
#define __rtc_t_defined
#endif

#ifndef __rtc_alarm_t_defined
typedef int rtc_alarm_t;
#define __rtc_alarm_t_defined
#endif

#ifndef __rtc_clock_t_defined
typedef clock_t rtc_clock_t;
#define __rtc_clock_t_defined
#endif

rtc_clock_t rtc_clock(rtc_t t);
time_ms_t rtc_time_ms(rtc_t t);
time_us_t rtc_time_us(rtc_t t);

void rtc_setAlarm(rtc_t t, rtc_alarm_t a, time_ms_t timeout_ms);

void rtc_alarm_Callback(rtc_t t, rtc_alarm_t a);

#endif /* SRC_STM32CLIBS_OSAL_RTC_H_ */
