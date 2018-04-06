/*
 * hd44780-config-mock.c
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

#include "hd44780.h"

#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>

#define _(x) \
	static uint8_t ddgram_ ## x [ HD44780_DISP_SIZE ]; \
	static uint8_t cgram_ ## x[ 8 * 10 ]; /* 8 characters * 10 dot character patterns */

_(1)
_(2)
_(3)
_(4)
_(5)
#undef _

static struct disp_s {
	uint8_t *ddgram;
	uint8_t *cgram;

	bool cgram_enabled;
	bool busy; // TODO

	uint8_t ddgram_pos;

	uint8_t cgram_pos;
};


#if HD44780_NUM_CONTROLLERS > 5
#error
#endif

#define _(x) { ddgram_ ## x , cgram_ ## x , }
static struct disp_s disp[5] = {
		_(1),
		_(2),
		_(3),
		_(4),
		_(5),
};
#undef _

void hd44780_mock_init()
{
}

static uint8_t hd44780_mock_next_cgram_address(uint8_t curpos)
{
	assert( curpos < 8 * 10  );
	if ( curpos == 8 * 10 - 1 ) {
		return 0;
	} else {
		return curpos + 1;
	}
}

static uint8_t hd44780_mock_next_ddgram_address(uint8_t curpos)
{
	assert( curpos < HD44780_DISP_SIZE );
	if ( curpos == (HD44780_DISP_SIZE-1) ) {
		return 0;
	} else {
		return curpos + 1;
	}
}

void hd44780_mock_data_set_callback(uint8_t flag_rw, uint8_t ctrl, uint8_t flag_rs, uint8_t data)
{
	struct disp_s * const d = &disp[ctrl];
	switch(flag_rs) {
	case HD44780_FLAG_RS_DATA:
		if ( d->cgram_enabled ) {
			d->cgram[ d->cgram_pos ] = data;
			d->cgram_pos = hd44780_mock_next_cgram_address( d->cgram_pos );
		} else {
			d->ddgram[ d->ddgram_pos ] = data;
			d->ddgram_pos = hd44780_mock_next_ddgram_address( d->ddgram_pos );
		}
		break;
	case HD44780_FLAG_RS_INST:
		if ( HD44780_INST_IS_CLRDISP(data) ) {
			d->cgram_enabled = false;
			d->ddgram_pos = 0;
			memset(d->ddgram, 0x00, HD44780_DISP_SIZE);
		} else if ( HD44780_INST_IS_RETURNHOME(data) ) {
			d->cgram_enabled = false;
			d->ddgram_pos = 0;
		} else if ( HD44780_INST_IS_EMS(data) ) {

		} else if ( HD44780_INST_IS_DOOC(data) ) {

		} else if ( HD44780_INST_IS_CDS(data) ) {

		} else if ( HD44780_INST_IS_FS(data) ) {

		} else if ( HD44780_INST_IS_CGRAM_ADDRESS(data) ) {
			d->cgram_pos = HD44780_INST_GET_CGRAM_ADDRESS(data);
			d->cgram_enabled = true;
		} else if ( HD44780_INST_IS_DDRAM_ADDRESS(data) ) {
			d->ddgram_pos = hd44780_curpos_from_raw( HD44780_INST_GET_DDRAM_ADDRESS(data) );
			d->cgram_enabled = false;
		}
		break;
	}
}

uint8_t hd44780_mock_data_get_callback(uint8_t flag_rw, uint8_t ctrl, uint8_t flag_rs)
{
	uint8_t ret = 0;
	struct disp_s * const d = &disp[ctrl];
	switch(flag_rs) {
	case HD44780_FLAG_RS_DATA:
		if ( d->cgram_enabled ) {
			ret = d->cgram[ d->cgram_pos ];
			d->cgram_pos = hd44780_mock_next_cgram_address( d->cgram_pos );
		} else {
			ret = d->ddgram[ d->ddgram_pos ];
			d->ddgram_pos = hd44780_mock_next_ddgram_address( d->ddgram_pos );
		}
		return ret;
	case HD44780_FLAG_RS_INST:
		return d->busy | hd44780_curpos_to_raw( d->ddgram_pos );
	}
	return 0;
}
