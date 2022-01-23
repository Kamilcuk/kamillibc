/*
 * spi.c
 *
 *  Created on: 30 mar 2018
 *      Author: Kamil Cukrowski
 *     License: All rights reserved. Copyright by Netemera(C).
 */
#include <uni/spi.h>

#include <curb.h>

#include <stddef.h>
#include <stropts.h>
#include <linux/ioctl.h>
#include <linux/spi/spidev.h>

void spi_select(spi_t fd)
{
	struct spi_ioc_transfer transfer = { .cs_change = 1, };
	const int ret = ioctl(fd, SPI_IOC_MESSAGE(1), &transfer);
	curb_exit(ret != 0);
}
void spi_unselect(spi_t fd)
{
	struct spi_ioc_transfer transfer = { .cs_change = 1, };
	const int ret = ioctl(fd, SPI_IOC_MESSAGE(1), &transfer);
	curb_exit(ret != 0);
}
void spi_send(spi_t fd, const void *tx_buf, size_t len)
{
	spi_exchange(fd, tx_buf, NULL, len);
}
void spi_receive(spi_t fd, void *rx_buf, size_t len)
{
	spi_exchange(fd, NULL, rx_buf, len);
}
void spi_exchange(spi_t fd, const void *tx_buf, void *rx_buf, size_t len)
{
	struct spi_ioc_transfer transfer = {
			.tx_buf = (__u64)tx_buf,
			.rx_buf = (__u64)rx_buf,
			.len = len,
	};
	const int ret = ioctl(fd, SPI_IOC_MESSAGE(1), &transfer);
	curb_exit(ret != 0);
}
