/*
 * spi.h
 *
 *  Created on: 27 mar 2018
 *      Author: kamil
 */

#ifndef STM32_SRC_SYS_SPI_H_
#define STM32_SRC_SYS_SPI_H_

#include <uni/types.h>

#include <stdbool.h>
#include <stddef.h>

#ifndef __spi_t_defined
typedef int spi_t;
#define __spi_t_defined
#endif

void spi_select(spi_t t);
void spi_unselect(spi_t t);
void spi_send(spi_t t, const void *tx_buf, size_t len);
void spi_receive(spi_t t, void *rx_buf, size_t len);
void spi_exchange(spi_t t, const void *tx_buf, void *rx_buf, size_t len);

#endif /* STM32_SRC_SYS_SPI_H_ */
