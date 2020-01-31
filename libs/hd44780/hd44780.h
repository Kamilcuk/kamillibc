/*
 * hd44780.h
 * 
 * 2013 Kamil Cukrowski <kamil@dyzio.pl>
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
#ifndef _HD44780_H_
#define _HD44780_H_ 1

#include <hd44780-config.h> // all configuration(!) and customization

#include "hd44780-inst-def.h"

#include <stdint.h>

/* private macros ------------------------------------------------------------------ */

#ifndef LOW_HALFBYTE
#define LOW_HALFBYTE(x)  ((x)&0x0f)
#endif
#ifndef HIGH_HALFBYTE
#define HIGH_HALFBYTE(x) ((x)>>4)
#endif

#if HD44780_NUM_CONTROLLERS     <=  8
typedef uint_least8_t HD44780_NUM_CONTROLLERS_BITMAP_TYPE;
#elif HD44780_NUM_CONTROLLERS   <= 16
typedef uint_least16_t HD44780_NUM_CONTROLLERS_BITMAP_TYPE;
#elif HD44780_NUM_CONTROLLERS   <= 32
typedef uint_least32_t HD44780_NUM_CONTROLLERS_BITMAP_TYPE;
#elif HD44780_NUM_CONTROLLERS   <= 64
typedef uint_least64_t HD44780_NUM_CONTROLLERS_BITMAP_TYPE;
#else // HD44780_NUM_CONTROLLERS
# error redefine hd44780.nobfcheck member in struct hd44780_s with bigger size
#endif // HD44780_NUM_CONTROLLERS

/*
 * constants
 */
#define HD44780_DISP_SIZE   ((HD44780_DISP_COLS  * HD44780_DISP_ROWS))        /* one display size */
#define HD44780_DISPS_COLS  ((HD44780_DISP_COLS))                             /* all displays columns */
#define HD44780_DISPS_ROWS  ((HD44780_DISP_ROWS  * HD44780_NUM_CONTROLLERS))  /* all displays rows */
#define HD44780_DISPS_SIZE  ((HD44780_DISPS_COLS * HD44780_DISPS_ROWS))       /* all displays display size */

/* ---------------------------------------------------------------------------------- */

/**
 * Interface for cursor position on 2-line and 1-line displays
 * rawcurpos is "raw cursor position" - it is DDGRAM position as returned from hd44780 display.
 * rawcurpos is in format: xxCCRRRR , where  C - column number, R - row number,, x - don't care
 * These macros convert rawcurpos to tuple (col,row).
 * curpos is cursor position as if we had one long line.
 * ex.
 * on a HD44780 with 2 lines and 40 rows, when position in CGRAM is equal 0x46=70, then:
 * rawcurpos = 70
 * curpos = 6 + 40 = 46
 * (row,col) = (1,6)
 */
#if HD44780_DISP_ROWS == 1
# define hd44780_curpos_from_raw(curpos)       ( curpos )
# define hd44780_curpos_to_raw(curpos)         ( curpos )
# define hd44780_rowcol_to_rawcurpos(row,col)  ( col )
# define hd44780_row_from_rawcurpos(rawcurpos) ( 0 )
# define hd44780_col_from_rawcurpos(rawcurpos) ( rawcurpos )
#elif HD44780_DISP_ROWS == 2
# define hd44780_curpos_from_raw(rawcurpos)    ( ( ((rawcurpos)&0x40) ? HD44780_DISP_COLS : 0 ) + ((rawcurpos)&0x3F) )
# define hd44780_curpos_to_raw(curpos)         ( ((curpos) >= HD44780_DISP_COLS) ? (0x40 | (curpos - HD44780_DISP_COLS)) : (curpos) )
# define hd44780_rowcol_to_rawcurpos(row,col)  ( ( (row) << 6 ) | (col) )
# define hd44780_row_from_rawcurpos(rawcurpos) ( ((rawcurpos)&0x40) ? 1 : 0 )
# define hd44780_col_from_rawcurpos(rawcurpos) ( (rawcurpos)&0x3F )
#else
#error HD44780 only supports up to 2 rows. However there is HD66712, be sure to check it out
#endif // HD44780_DISP_ROWS == 1

/**
 * User code may want to enable/disable busyflag checking manually
 */
#if HD44780_BF_CHECKING_ENABLED && HD44780_BF_CHECKING_OPTIONAL
# define HD44780_BFCHECK_DISABLE(ctrl)         (  hd44780.nobfcheck |= 1<<(ctrl) )
# define HD44780_BFCHECK_ENABLE(ctrl)          (  hd44780.nobfcheck &= ~(1<<(ctrl)) )
# define HD44780_BFCHECK_IS_DISABLED(ctrl)     (  hd44780.nobfcheck & (1<<(ctrl)) )
# define HD44780_BFCHECK_IS_ENABLED(ctrl)      (!(hd44780.nobfcheck & (1<<(ctrl)) )
# define HD44780_BFCHECK_IS_ANY_DISABLED()            ( hd44780.nobfcheck !=  0 )
# define HD44780_BFCHECK_IS_ANY_ENABLED()             ( hd44780.nobfcheck != ~0 )
# define HD44780_BFCHECK_ENABLE_ALL()                 ( hd44780.nobfcheck =   0 )
# define HD44780_BFCHECK_DISABLE_ALL()                ( hd44780.nobfcheck =  ~0 )
#endif // HD44780_BF_CHECKING_OPTIONAL

/* ---------------------------------------------------------------------------------- */

/**
 * Internal struct state definition
 */
struct hd44780_s {

#if HD44780_BF_CHECKING_ENABLED && HD44780_BF_CHECKING_OPTIONAL
	/**
	 * This is a bitmap.
	 * If bit number X is set, that means that display number X is probably malfunctioning.
	 * If bit number X is set, we don't check busyflag on this display, we only do wait's.
	 */
	 HD44780_NUM_CONTROLLERS_BITMAP_TYPE nobfcheck;
#endif // HD44780_BF_CHECKING_OPTIONAL

#ifdef HD44780_COUNTERS_ENABLED

	/**
	 * Count times hd44780 controller were written to
	 */
	HD44780_COUNTERS_TYPE write_count[HD44780_NUM_CONTROLLERS];

	/**
	 * Count times hd44780 was read from.
	 */
	HD44780_COUNTERS_TYPE read_count[HD44780_NUM_CONTROLLERS];

#if HD44780_BF_CHECKING_ENABLED
	/**
	 * count times we checked busyflag on this controller.
	 */
	HD44780_COUNTERS_TYPE bfchecks_count[HD44780_NUM_CONTROLLERS];
#endif // HD44780_BF_CHECKING

#ifdef HD44780_CHECK_EVERY_WRITE
	/**
	 * count times we corrected display on controller.
	 */
	HD44780_COUNTERS_TYPE errors_count[HD44780_NUM_CONTROLLERS];
#endif // HD44780_CHECK_EVERY_WRITE

#endif // HD44780_COUNTERS

};

/**
 * hd44780 state structure declaration
 */
extern struct hd44780_s hd44780;

/* ---------------------------------------------------------------------------------- */

/**
 * initialize hd44780 subsystem
 */
void hd44780_init();

/**
 * hd44780_initialize_by_instruction
 * hd44780 documentation page 45
 * If the power supply conditions for correctly operating the internal reset circuit are not met, initialization by
 * instructions becomes necessary.
 */
void hd44780_initialize_by_instruction(const uint8_t ctrl);

/**
 * Write data to controller.
 * @param ctrl Controller number to write to. Must be lower then HD44780_NUM_CONTROLLERS.
 * @param flag_rs Either HD44780_FLAG_RS_INST or HD44780_FLAG_RS_DATA
 * @param data to write to.
 */
void hd44780_write_unsafe(const uint8_t ctrl, const enum HD44780_FLAG_RS_e flag_rs, const uint8_t data);


/**
 * Write data to the controller.
 * If writing RS_INST instruction, the functions checks if the CGRAM or DDRAM address is correctly written
 * If writing RS_DATA data to CGRAM or DDRAM address,
 *   when bit RAM_and_ctrl&0x80 is set, function expects to be writing CGRAM address
 *   when bit RAM_and_ctrl&0x80 is unset, function expects to be writing DDRAM address
 * @param RAM_and_ctrl Controller number to write to. The value of (RAM_and_ctrl&0x7F) must be lower then HD44780_NUM_CONTROLLERS.
 * @param flag_rs Either HD44780_FLAG_RS_INST or HD44780_FLAG_RS_DATA
 * @param data to write to.
 */
void hd44780_write(const uint8_t RAM_and_ctrl, const enum HD44780_FLAG_RS_e flag_rs, const uint8_t data);

/**
 *
 * @param ctrl Controller number to read from. Must be lower then HD44780_NUM_CONTROLLERS.
 * @param flag_rs Either HD44780_FLAG_RS_INST or HD44780_FLAG_RS_DATA
 * @return data read
 */
uint8_t hd44780_read(const uint8_t ctrl, const enum HD44780_FLAG_RS_e flag_rs);

/**
 * Write to CGRAM to ctrl at CGRAM position pos
 * @param ctrl
 * @param pos must be lower then 8
 * @param cgram is at least 8 chars long
 */
void hd44780_write_cgram(const uint8_t ctrl, const uint8_t pos, const uint8_t cgram[8]);

/**
 * Write CGRAM to all controllers
 * @param pos
 * @param cgram
 */
void hd44780_write_cgram_all(const uint8_t pos, const uint8_t cgram[8]);

/* user interface macros ------------------------------------------------------------------ */

static inline void hd44780_write_inst(uint8_t ctrl, uint8_t inst) {
	hd44780_write(ctrl, HD44780_FLAG_RS_INST, inst);
}
static inline void hd44780_write_data(uint8_t ctrl, uint8_t data) {
	hd44780_write(ctrl, HD44780_FLAG_RS_DATA, data);
}

#define hd44780_read_inst(ctrl)				 hd44780_read(ctrl, HD44780_FLAG_RS_INST)
#define hd44780_read_data(ctrl)				 hd44780_read(ctrl, HD44780_FLAG_RS_DATA)

#define hd44780_read_rawcurpos(ctrl) \
	HD44780_INST_GET_DDRAM_ADDRESS( hd44780_read_inst(ctrl) )

static inline void hd44780_write_rawcurpos(uint8_t ctrl, uint8_t rawcurpos) {
	hd44780_write_inst(ctrl, HD44780_DDRAM_ADDRESS | rawcurpos);
}
static inline void hd44780_write_curpos(uint8_t ctrl, uint8_t rawcurpos) {
	hd44780_write_rawcurpos(ctrl, hd44780_curpos_from_raw(rawcurpos) );
}
static inline void hd44780_write_cursor_rowcol(uint8_t ctrl, uint_fast8_t row,  uint_fast8_t col) {
	hd44780_write_rawcurpos(ctrl, hd44780_rowcol_to_rawcurpos(row, col) );
}

#define hd44780_foreach( iterator_name ) \
		for(uint_fast8_t iterator_name = 0; iterator_name < HD44780_NUM_CONTROLLERS ; ++iterator_name )

/* unit tests ------------------------------------------------------------------------------ */

/**
 * Unit tests - internal tests, done without connected hd44780
 */
void hd44780_unittest();

/**
 * Devive tests - tests connected device operation
 */
void hd44780_assemblytest();

#endif // _HD44780_H_
