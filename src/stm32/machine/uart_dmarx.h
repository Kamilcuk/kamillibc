/*
 * uart_dma_hwfifo.h
 *
 *  Created on: 16.02.2018
 *      Author: kamil
 */

#ifndef SRC_SYSTEM_UNI_UART_DMAHWFIFO_H_
#define SRC_SYSTEM_UNI_UART_DMAHWFIFO_H_

#include <machine/hal.h>
#ifdef HAL_UART_MODULE_ENABLED

#include <uni/cdefs.h>
#include <stddef.h>
#include <stdint.h>

struct HW_UART_dmarx_conf_s {
	UART_HandleTypeDef *huart;
	struct IRQlist_s IRQlist;
	uint8_t *receiver; // where we receive data
	size_t receiversize;
};

struct HW_UART_dmarx_s {
	//public:
	const struct HW_UART_dmarx_conf_s conf;
	//private:
	size_t receiverpos; // read position in receiver
};

void HW_UART_dmarx_start(struct HW_UART_dmarx_s * restrict t) __nonnull((1));
size_t HW_UART_dmarx_read(struct HW_UART_dmarx_s * restrict t, uint8_t buf[restrict], size_t nbyte)  __nonnull((1,2));
bool HW_UART_dmarx_poll_in(struct HW_UART_dmarx_s * restrict t)  __nonnull((1));
void HW_UART_dmarx_flush(struct HW_UART_dmarx_s * restrict t)  __nonnull((1));

#endif

#endif /* SRC_SYSTEM_UNI_UART_DMAHWFIFO_H_ */
