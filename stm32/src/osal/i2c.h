/*
 * i2c.h
 *
 *  Created on: 27 mar 2018
 *      Author: kamil
 */

#ifndef STM32_SRC_SYS_I2C_H_
#define STM32_SRC_SYS_I2C_H_

#include <osal/types.h>
#include <stddef.h>
#include <time.h>

#ifndef __i2c_t_defined
typedef int i2c_t;
#define __i2c_t_defined
#endif

void i2c_transmit(i2c_t t, const char txbuf[], size_t txbytes, char rxbuf[], size_t rxbytes, clock_t timeout);
void i2c_receive(i2c_t t, char rxbuf[], size_t rxsize, clock_t timeout);


#endif /* STM32_SRC_SYS_I2C_H_ */
