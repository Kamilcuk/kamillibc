/*
 * uart.c
 *
 *  Created on: 07.02.2018
 *      Author: kamil
 */
#include <machine/hal.h>
#ifdef HAL_UART_MODULE_ENABLED

#include <machine/uart.h>

#include <inttypes.h>
#include <string.h>
#include <stdio.h>

/* Private Macros --------------------------------------------------------------------------- */
/* Private Functions ------------------------------------------------------------------------ */

static inline void HW_UART_dmatxrb_arm(UART_HandleTypeDef * restrict huart, RingBuffer_t *  restrict rb)
{
	size_t size;
	const uint8_t * const pnt = (const uint8_t*)rb_read_pointer(rb, 0, &size);
	if (pnt != NULL && size != 0) {
		HAL_try( HAL_UART_Transmit_DMA(huart, (uint8_t*)pnt, size) );
	}
}

static inline void _UART_Transmit_DMArb_move_cndtr(struct HW_UART_dmatxrb_s * restrict t)
{
	fprintf(stderr, "%s The following code does not currently work, do not use it...\n", __func__);
	assert(0);
	const uint8_t * const pTxBuffPtr_end = &t->conf.huart->pTxBuffPtr[t->conf.huart->TxXferSize];
	const uint8_t * const rb_data_end = (uint8_t*)&t->rb.data[t->rb.size];
	if ( pTxBuffPtr_end == rb_data_end ) {
		// current DMA transmission ends on ring buffer ending
		// arm transmission only if it completed
		if ( HAL_UART_IsTransmitReady(t->conf.huart) ) {
			HW_UART_dmatxrb_arm(t->conf.huart, &t->rb);
		}
		// otherwise TxCplt_IRQHandler will be called on DMA transmission ending
		// which will re-arm DMA
	} else {
		// we copied new data onto the end of ring buffer
		// we can notify DMA of new data, do hdmatx->cndtr += nbyte
		if ( !HAL_UART_IsTransmitReady(t->conf.huart) ) {
			// DMA is working - stop it
#if defined(STM32L1)
			HAL_DMA_Abort_IT(t->conf.huart->hdmatx);
#else
			HAL_UART_AbortTransmit(t->conf.huart);
#endif

			NVIC_EnableIRQlist(t->conf.IRQlist);

			// commit number of bytes already transferred
			assert(t->conf.huart->hdmatx);
			const uint32_t cndtr = __HAL_DMA_GET_COUNTER(t->conf.huart->hdmatx);
			const uint32_t TxXferSize = t->conf.huart->TxXferSize;
			assert(TxXferSize >= cndtr);
			const uint32_t transferred = TxXferSize - cndtr;

			NVIC_DisableIRQlist(t->conf.IRQlist);

			rb_read_commit(&t->rb, transferred);
		}

		// re-arm transmission with new buffer length
		HW_UART_dmatxrb_arm(t->conf.huart, &t->rb);
	}
}


/* Exported Functions ------------------------------------------------------------------------- */

size_t HW_UART_dmatxrb_write(struct HW_UART_dmatxrb_s * restrict t,
		const uint8_t buf[restrict], size_t nbyte)
{
	assert(t && buf && nbyte);

	NVIC_DisableIRQlist(t->conf.IRQlist);
	size_t tocopy;
	uint8_t * const pnt = (uint8_t*)rb_write_pointer(&t->rb, &tocopy);
	if (tocopy > nbyte)
		tocopy = nbyte;
	NVIC_EnableIRQlist(t->conf.IRQlist);
	if (tocopy != 0) {
		memcpy(pnt, buf, tocopy);
		NVIC_DisableIRQlist(t->conf.IRQlist);
		rb_write_commit(&t->rb, tocopy);
		if ( HAL_UART_IsTransmitReady(t->conf.huart) ) {
			HW_UART_dmatxrb_arm(t->conf.huart, &t->rb);
		}
		NVIC_EnableIRQlist(t->conf.IRQlist);
	}

	return tocopy;
}

void HW_UART_dmatxrb_TxCplt_IRQHandler(struct HW_UART_dmatxrb_s * restrict t)
{
	assert(t && t->conf.huart && t->conf.huart->TxXferCount == 0);
	rb_read_commit(&t->rb, t->conf.huart->TxXferSize);
	HW_UART_dmatxrb_arm(t->conf.huart, &t->rb);
}

/* --------------------------------------------------------------------------- */

void HW_UART_noirq_rb_flush(RingBuffer_t * restrict rb, struct IRQlist_s IRQlist)
{
	NVIC_DisableIRQlist(IRQlist);
	rb_flush(rb);
	NVIC_EnableIRQlist(IRQlist);
}

void HW_UART_IDLE_IRQHandler(UART_HandleTypeDef * restrict huart)
{
#if defined(STM32L0)
	/* handle UART IDLE Interrupt */
	const uint32_t isrflags   = READ_REG(huart->Instance->ISR);
	const uint32_t cr1its     = READ_REG(huart->Instance->CR1);
	// idle is enabled
	if(((isrflags & USART_ISR_IDLE) != RESET) && ((cr1its & USART_CR1_IDLEIE) != RESET)) {
		__HAL_UART_CLEAR_IT(huart, UART_CLEAR_IDLEF);
		HW_UART_IdleCallback(huart);
	}
#elif defined(STM32F1) || defined(STM32L1)
	if (__HAL_UART_GET_FLAG(huart, UART_FLAG_IDLE) && __HAL_UART_GET_IT_SOURCE(huart, UART_IT_IDLE)) {
		__HAL_UART_CLEAR_IDLEFLAG(huart);
		HW_UART_IdleCallback(huart);
	}
#else
#error Implement HW_UART_IDLE_IRQHandler to use it
#endif
}

__weak_symbol
void HW_UART_IdleCallback(UART_HandleTypeDef * restrict huart)
{

}

#endif
