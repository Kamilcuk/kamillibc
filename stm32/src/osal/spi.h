/*
 * spi.h
 *
 *  Created on: 27 mar 2018
 *      Author: kamil
 */

#ifndef STM32_SRC_SYS_SPI_H_
#define STM32_SRC_SYS_SPI_H_

#include <osal/types.h>
#include <stdbool.h>

#ifndef __spi_t_defined
typedef int spi_t;
#define __spi_t_defined
#endif

void spi_select(spi_t spip);
void spi_unselect(spi_t spip);
void spi_send(spi_t spip, size_t n, const void *txbuf);
void spi_receive(spi_t spip, size_t n, void *rxbuf);
void spi_exchange(spi_t spip, size_t n, const void *txbuf, void *rxbuf);

#endif /* STM32_SRC_SYS_SPI_H_ */
