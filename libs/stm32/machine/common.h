/*
 * hw.h
 *
 *  Created on: 13 kwi 2018
 *      Author: kamil
 */

#ifndef SRC_STM32_MACHINE_COMMON_H_
#define SRC_STM32_MACHINE_COMMON_H_

#include <stdint.h>
#include <stddef.h>

unsigned int HW_srand_seed();
void HW_unique_id(uint8_t *pnt, size_t pntsize);

#endif /* SRC_STM32_MACHINE_COMMON_H_ */
