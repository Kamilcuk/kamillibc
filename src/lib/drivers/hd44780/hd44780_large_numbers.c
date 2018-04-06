/*
 *        File: hd44780_large_numbers.c
 *  Created on: 29.06.2017
 *      Author: Kamil Cukrowski
 * ----------------------------------------------------------------------------
 *  "THE BEER-WARE LICENSE" (Revision 42):
 *  Kamil Cukrowski wrote this file. As long as you retain this notice you
 *  can do whatever you want with this stuff. If we meet some day, and you think
 *  this stuff is worth it, you can buy me a beer in return.
 * ----------------------------------------------------------------------------
 */

#include "hd44780_large_numbers.h"

#include "hd44780.h"

#include <stdint.h>

/* ------------------------------------------------------------------------------- */

// the 8 arrays that form each segment of the custom numbers
static uint8_t bar1[8] =
{
		0b00011100,
		0b00011110,
		0b00011110,
		0b00011110,
		0b00011110,
		0b00011110,
		0b00011110,
		0b00011100
};
static uint8_t bar2[8] =
{
		0b00000111,
		0b00001111,
		0b00001111,
		0b00001111,
		0b00001111,
		0b00001111,
		0b00001111,
		0b00000111
};
static uint8_t bar3[8] =
{
		0b00011111,
		0b00011111,
		0b00000000,
		0b00000000,
		0b00000000,
		0b00000000,
		0b00011111,
		0b00011111
};
static uint8_t bar4[8] =
{
		0b00011110,
		0b00011100,
		0b00000000,
		0b00000000,
		0b00000000,
		0b00000000,
		0b00011000,
		0b00011100
};
static uint8_t bar5[8] =
{
		0b00001111,
		0b00000111,
		0b00000000,
		0b00000000,
		0b00000000,
		0b00000000,
		0b00000011,
		0b00000111
};
static uint8_t bar6[8] =
{
		0b00000000,
		0b00000000,
		0b00000000,
		0b00000000,
		0b00000000,
		0b00000000,
		0b00011111,
		0b00011111
};
static uint8_t bar7[8] =
{
		0b00000000,
		0b00000000,
		0b00000000,
		0b00000000,
		0b00000000,
		0b00000000,
		0b00000111,
		0b00001111
};
static uint8_t bar8[8] =
{
		0b00011111,
		0b00011111,
		0b00000000,
		0b00000000,
		0b00000000,
		0b00000000,
		0b00000000,
		0b00000000
};

/* ------------------------------------------------------------------------ */

void hd44780_large_numbers_setup(void)
{
	hd44780_write_cgram(HD44780_NUM_CONTROLLERS, 1, bar1);
	hd44780_write_cgram(HD44780_NUM_CONTROLLERS, 2, bar2);
	hd44780_write_cgram(HD44780_NUM_CONTROLLERS, 3, bar3);
	hd44780_write_cgram(HD44780_NUM_CONTROLLERS, 4, bar4);
	hd44780_write_cgram(HD44780_NUM_CONTROLLERS, 5, bar5);
	hd44780_write_cgram(HD44780_NUM_CONTROLLERS, 6, bar6);
	hd44780_write_cgram(HD44780_NUM_CONTROLLERS, 7, bar7);
	hd44780_write_cgram(HD44780_NUM_CONTROLLERS, 8, bar8);
}

static void custom0(uint8_t ctrl, uint8_t col)
{
	hd44780_write_cursor_rowcol(ctrl, col, 0);
	hd44780_write_data(ctrl, 2);
	hd44780_write_data(ctrl, 8);
	hd44780_write_data(ctrl, 1);
	hd44780_write_cursor_rowcol(ctrl, col, 1);
	hd44780_write_data(ctrl, 2);
	hd44780_write_data(ctrl, 6);
	hd44780_write_data(ctrl, 1);
}

static void custom1(uint8_t ctrl, uint8_t col)
{
	hd44780_write_cursor_rowcol(ctrl, col, 0);
	hd44780_write_data(ctrl, 32);
	hd44780_write_data(ctrl, 32);
	hd44780_write_data(ctrl, 1);
	hd44780_write_cursor_rowcol(ctrl, col, 1);
	hd44780_write_data(ctrl, 32);
	hd44780_write_data(ctrl, 32);
	hd44780_write_data(ctrl, 1);
}

static void custom2(uint8_t ctrl, uint8_t col)
{
	hd44780_write_cursor_rowcol(ctrl, col, 0);
	hd44780_write_data(ctrl, 5);
	hd44780_write_data(ctrl, 3);
	hd44780_write_data(ctrl, 1);
	hd44780_write_cursor_rowcol(ctrl, col, 1);
	hd44780_write_data(ctrl, 2);
	hd44780_write_data(ctrl, 6);
	hd44780_write_data(ctrl, 6);
}

static void custom3(uint8_t ctrl, uint8_t col)
{
	hd44780_write_cursor_rowcol(ctrl, col,0);
	hd44780_write_data(ctrl, 5);
	hd44780_write_data(ctrl, 3);
	hd44780_write_data(ctrl, 1);
	hd44780_write_cursor_rowcol(ctrl, col, 1);
	hd44780_write_data(ctrl, 7);
	hd44780_write_data(ctrl, 6);
	hd44780_write_data(ctrl, 1);
}

static void custom4(uint8_t ctrl, uint8_t col)
{
	hd44780_write_cursor_rowcol(ctrl, col, 0);
	hd44780_write_data(ctrl, 2);
	hd44780_write_data(ctrl, 6);
	hd44780_write_data(ctrl, 1);
	hd44780_write_cursor_rowcol(ctrl, col, 1);
	hd44780_write_data(ctrl, 32);
	hd44780_write_data(ctrl, 32);
	hd44780_write_data(ctrl, 1);
}

static void custom5(uint8_t ctrl, uint8_t col)
{
	hd44780_write_cursor_rowcol(ctrl, col, 0);
	hd44780_write_data(ctrl, 2);
	hd44780_write_data(ctrl, 3);
	hd44780_write_data(ctrl, 4);
	hd44780_write_cursor_rowcol(ctrl, col, 1);
	hd44780_write_data(ctrl, 7);
	hd44780_write_data(ctrl, 6);
	hd44780_write_data(ctrl, 1);
}

static void custom6(uint8_t ctrl, uint8_t col)
{
	hd44780_write_cursor_rowcol(ctrl, col,0);
	hd44780_write_data(ctrl, 2);
	hd44780_write_data(ctrl, 3);
	hd44780_write_data(ctrl, 4);
	hd44780_write_cursor_rowcol(ctrl, col, 1);
	hd44780_write_data(ctrl, 2);
	hd44780_write_data(ctrl, 6);
	hd44780_write_data(ctrl, 1);
}

static void custom7(uint8_t ctrl, uint8_t col)
{
	hd44780_write_cursor_rowcol(ctrl, col,0);
	hd44780_write_data(ctrl, 2);
	hd44780_write_data(ctrl, 8);
	hd44780_write_data(ctrl, 1);
	hd44780_write_cursor_rowcol(ctrl, col, 1);
	hd44780_write_data(ctrl, 32);
	hd44780_write_data(ctrl, 32);
	hd44780_write_data(ctrl, 1);
}

static void custom8(uint8_t ctrl, uint8_t col)
{
	hd44780_write_cursor_rowcol(ctrl, col, 0);
	hd44780_write_data(ctrl, 2);
	hd44780_write_data(ctrl, 3);
	hd44780_write_data(ctrl, 1);
	hd44780_write_cursor_rowcol(ctrl, col, 1);
	hd44780_write_data(ctrl, 2);
	hd44780_write_data(ctrl, 6);
	hd44780_write_data(ctrl, 1);
}

static void custom9(uint8_t ctrl, uint8_t col)
{
	hd44780_write_cursor_rowcol(ctrl, col, 0);
	hd44780_write_data(ctrl, 2);
	hd44780_write_data(ctrl, 3);
	hd44780_write_data(ctrl, 1);
	hd44780_write_cursor_rowcol(ctrl, col, 1);
	hd44780_write_data(ctrl, 7);
	hd44780_write_data(ctrl, 6);
	hd44780_write_data(ctrl, 1);
}

void printNumber(uint8_t ctrl, uint8_t num, uint8_t col)
{
	switch(num) {
	case 0: custom0(ctrl, col); break;
	case 1: custom1(ctrl, col); break;
	case 2: custom2(ctrl, col); break;
	case 3: custom3(ctrl, col); break;
	case 4: custom4(ctrl, col); break;
	case 5: custom5(ctrl, col); break;
	case 6: custom6(ctrl, col); break;
	case 7: custom7(ctrl, col); break;
	case 8: custom8(ctrl, col); break;
	case 9: custom9(ctrl, col); break;
	}
}

