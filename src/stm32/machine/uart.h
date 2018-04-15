/*
 * uart.h
 *
 *  Created on: 14 kwi 2018
 *      Author: kamil
 */

#ifndef SRC_STM32_MACHINE_UART_H_
#define SRC_STM32_MACHINE_UART_H_

#include <machine/hal.h>

#ifdef HAL_UART_MODULE_ENABLED
#include <machine/uart_dmarx.h>
#include <machine/nvic.h>
#include <ringbuffer.h>
#include <uni/cdefs.h>

struct HW_UART_dmatxrb_conf_s {
	UART_HandleTypeDef * restrict huart;
	struct IRQlist_s IRQlist;
};
struct HW_UART_dmatxrb_s {
	const struct HW_UART_dmatxrb_conf_s conf;
	RingBuffer_t rb;
};

size_t HW_UART_dmatxrb_write(struct HW_UART_dmatxrb_s * restrict t,
		const uint8_t buf[restrict], size_t nbyte) __nonnull((1,2));
void HW_UART_dmatxrb_TxCplt_IRQHandler(struct HW_UART_dmatxrb_s * restrict t) __nonnull((1));

void HW_UART_ErrorCheckPrintf(UART_HandleTypeDef * restrict huart) __nonnull((1));
void HW_UART_Transmit_fsync(UART_HandleTypeDef * restrict huart) __nonnull((1));
void HW_UART_noirq_rb_flush(RingBuffer_t * restrict rb, struct IRQlist_s IRQlist) __nonnull((1));

void HW_UART_IdleCallback(UART_HandleTypeDef * restrict huart) __nonnull((1));
void HW_UART_IDLE_IRQHandler(UART_HandleTypeDef * restrict huart) __nonnull((1));

#endif

#endif /* SRC_STM32_MACHINE_UART_H_ */
