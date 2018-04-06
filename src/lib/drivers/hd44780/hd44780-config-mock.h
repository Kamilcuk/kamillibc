/*
 * hd44780-config-mock.h
 *
 *  Created on: 23 lip 2017
 *      Author: Kamil Cukrowski
 * ------------------- The MIT Expat License + Beerware License ----------------
 * Copyright (c) 2017 Kamil Cukrowski
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 * 
 * If we meet some day, and you think this stuff is worth it, you can buy me a
 * beer in return.
 * -----------------------------------------------------------------------------
 */

#ifndef HD44780_CONFIG_MOCK_H_
#define HD44780_CONFIG_MOCK_H_

#include "hd44780-config-default.h"

#include <stdint.h>

#undef HD44780_INIT_CALLBACK
#define HD44780_INIT_CALLBACK() hd44780_mock_init()

#undef HD44780_DATA_SET_CALLBACK
#define HD44780_DATA_SET_CALLBACK(FLAG_RW, ctrl, FLAG_RS, data) \
	hd44780_mock_data_set_callback(FLAG_RW, ctrl, FLAG_RS, data)

#undef HD44780_DATA_GET_CALLBACK
#define HD44780_DATA_GET_CALLBACK(FLAG_RW, ctrl, FLAG_RS) \
	hd44780_mock_data_get_callback(FLAG_RW, ctrl, FLAG_RS)

void hd44780_mock_init();

void hd44780_mock_data_set_callback(uint8_t flag_rw, uint8_t ctrl, uint8_t flag_rs, uint8_t data);

uint8_t hd44780_mock_data_get_callback(uint8_t flag_rw, uint8_t ctrl, uint8_t flag_rs);

#endif /* HD44780_CONFIG_MOCK_H_ */
