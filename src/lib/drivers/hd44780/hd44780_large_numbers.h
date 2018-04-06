/*
 *        File: hd44780_large_numbers.h
 *  Created on: 29.06.2017
 *      Author: Kamil Cukrowski
 * ----------------------------------------------------------------------------
 *  "THE BEER-WARE LICENSE" (Revision 42):
 *  Kamil Cukrowski wrote this file. As long as you retain this notice you
 *  can do whatever you want with this stuff. If we meet some day, and you think
 *  this stuff is worth it, you can buy me a beer in return.
 * ----------------------------------------------------------------------------
 */
// based on https://gist.github.com/ronivaldo/5587355
#ifndef HD44780_LARGE_NUMBERS_H_
#define HD44780_LARGE_NUMBERS_H_

#include <stdint.h>

/**
 * write to CGRAM values for big numbers
 */
void hd44780_large_numbers_setup(void);

/**
 * Write num on controller number ctrl starting from column col
 * @param ctrl
 * @param num
 * @param col
 */
void printNumber(uint8_t ctrl, uint8_t num, uint8_t col);

#endif /* HD44780_LARGE_NUMBERS_H_ */
