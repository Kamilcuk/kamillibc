/*
 * cyclecnt.h
 *
 *  Created on: 8 maj 2018
 *      Author: kamil
 */

#ifndef SRC_KAMILLIBCSTM32_MACHINE_CYCLECNT_H_
#define SRC_KAMILLIBCSTM32_MACHINE_CYCLECNT_H_

#include <machine/hal.h>

static inline void cyclecnt_reset(void)
{
	DWT->CYCCNT = 0;
}

static inline uint32_t cyclecnt_get(void)
{
	return DWT->CYCCNT;
}

static inline uint32_t cyclecnt_ex_get(void)
{
	return DWT->CYCCNT - DWT->CPICNT - DWT->EXCCNT - DWT->SLEEPCNT - DWT->LSUCNT + DWT->FOLDCNT;
}

static inline double cyclecnt_get_s_d(void)
{
	return (double)cyclecnt_get() / SystemCoreClock;
}

static inline uint32_t cyclecnt_get_ns(void)
{
	return cyclecnt_get() * 1000000UL / SystemCoreClock;
}

static inline void cyclecnt_start()
{
	// enable DWT debug block
	CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
	// enable CYCCNT counter
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
    // reset counter
    cyclecnt_single_reset();
}

static inline uint32_t cyclecnt_stop()
{
	uint32_t ret = cyclecnt_single_get();
	DWT->CTRL &= ~DWT_CTRL_CYCCNTENA_Msk;
	return ret;
}

#endif /* SRC_KAMILLIBCSTM32_MACHINE_CYCLECNT_H_ */
