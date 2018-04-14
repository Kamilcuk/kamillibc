/*
 * HW_RTC_time.h
 *
 *  Created on: 4 mar 2018
 *      Author: kamil
 */

#ifndef SRC_MACHINE_UNI_HW_RTC_TIME_H_
#define SRC_MACHINE_UNI_HW_RTC_TIME_H_

#include <machine/hal.h>
#ifdef HAL_RTC_MODULE_ENABLED

#include <stdbool.h>
#include <time.h>

/* Exported Types ------------------------------------------------------- */

/**
 * Represents time elapsed since Epoch
 * 1970-01-01 00:00:00 +0000 (UTC)
 */
struct HW_RTC_DateTime_s {
	RTC_DateTypeDef date;
	RTC_TimeTypeDef time;
};

/* Exported Functions --------------------------------------------------- */

bool HW_RTC_IsLeapYear(unsigned int nYear);
RTC_TimeTypeDef HW_RTC_time_t_to_TimeTypeDef(time_t seconds);
time_t HW_RTC_DateTime_to_time_t_from_wiki(struct HW_RTC_DateTime_s dt);
time_t HW_RTC_DateTime_to_time_t_from_net(struct HW_RTC_DateTime_s dt);
struct HW_RTC_DateTime_s HW_RTC_time_t_to_DateTime_from_hal(time_t timev);
struct HW_RTC_DateTime_s HW_RTC_time_t_to_DateTime_from_net(time_t timev);
struct tm HW_RTC_DateTime_to_tm(struct HW_RTC_DateTime_s dt);
struct HW_RTC_DateTime_s HW_RTC_tm_to_DateTime(struct tm t);
bool HW_RTC_DateTime_isValid(const struct HW_RTC_DateTime_s *dt);
char *HW_RTC_DateTime_print_r(const struct HW_RTC_DateTime_s *dt, char result[26]);
#define HW_RTC_DateTime_print(dt)  HW_RTC_DateTime_print_r((dt)(char[26]){0})

int HW_RTC_time_unittest();

static inline time_t HW_RTC_DateTime_to_time_t(struct HW_RTC_DateTime_s t)
{
	return HW_RTC_DateTime_to_time_t_from_net(t);
}

static inline struct HW_RTC_DateTime_s HW_RTC_time_t_to_DateTime(time_t t)
{
	return HW_RTC_time_t_to_DateTime_from_net(t);
}

#endif // HAL_RTC_MODULE_ENABLED
#endif /* SRC_MACHINE_UNI_HW_RTC_TIME_H_ */
