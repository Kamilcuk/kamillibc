/*
 * uart_DMAhwfifo.c
 *
 *  Created on: 14 kwi 2018
 *      Author: kamil
 */
/*
 * uart_DMAhwfifo.c
 *
 *  Created on: 16.02.2018
 *      Author: kamil
 */

#include "uart_dmarx.h"

#include <machine/hal.h>
#ifdef HAL_UART_MODULE_ENABLED

#include <string.h>
#include <assert.h>
#include <stddef.h>

//#define DBGLN(fmt, ...)  printf("%s" fmt "\n", __func__, ##__VA_ARGS__)
#define DBGLN(fmt, ...)  do{}while(0)

static inline bool HAL_DMA_isCircular(DMA_HandleTypeDef * restrict hdma)
{
	assert(hdma && hdma->Instance);
	return hdma->Instance->CCR & DMA_CCR_CIRC;
}

static inline uint32_t HW_UART_dmarx_get_cnt(struct HW_UART_dmarx_s * restrict t)
{
	assert(t && t->conf.huart && t->conf.huart->hdmarx);
	// get DMA counter position reading uint32_t is atomic
	const uint32_t cndtr = __HAL_DMA_GET_COUNTER(t->conf.huart->hdmarx);
	assert(cndtr <= t->conf.receiversize);
	const uint32_t rxcnt = t->conf.receiversize - cndtr; // count of transfered data
	return rxcnt;
}

void HW_UART_dmarx_start(struct HW_UART_dmarx_s * restrict t)
{
	assert(t && t->conf.huart && t->conf.huart->hdmarx);
	if (!HAL_UART_IsReceiveReady(t->conf.huart)) return;
	HW_UART_dmarx_flush(t);
	assert(HAL_DMA_isCircular(t->conf.huart->hdmarx));
	NVIC_DisableIRQlist(t->conf.IRQlist);
	HAL_try( HAL_UART_Receive_DMA(t->conf.huart, t->conf.receiver, t->conf.receiversize) );
	NVIC_EnableIRQlist(t->conf.IRQlist);
}

size_t HW_UART_dmarx_read(struct HW_UART_dmarx_s * restrict t, uint8_t buf[restrict], size_t nbyte)
{
	assert(t && buf);
	HW_UART_dmarx_start(t);
	const size_t rxcnt = HW_UART_dmarx_get_cnt(t);
	const bool rollover = rxcnt < t->receiverpos;
	const size_t datalen =
			rollover
			? t->conf.receiversize - t->receiverpos
			: rxcnt - t->receiverpos;
	if (nbyte > datalen)
		nbyte = datalen;
	memcpy(buf, &t->conf.receiver[t->receiverpos], nbyte);
	t->receiverpos += nbyte;
	assert(t->receiverpos <= t->conf.receiversize);
	if (t->receiverpos == t->conf.receiversize)
		t->receiverpos = 0;
	return nbyte;
}

bool HW_UART_dmarx_poll_in(struct HW_UART_dmarx_s * restrict t)
{
	HW_UART_dmarx_start(t);
	return t->receiverpos != HW_UART_dmarx_get_cnt(t);
}

void HW_UART_dmarx_flush(struct HW_UART_dmarx_s * restrict t)
{
	t->receiverpos = HW_UART_dmarx_get_cnt(t);
}

#endif
