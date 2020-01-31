/*
 * hd44780.c - communicate with hd44780 compatible controllers
 * look in hd44780-config.h for more information!
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
 * 
 * 
 * 
 */

#include "hd44780.h"

#include "hd44780-inst-def.h"

#include <unistd.h> //udelay
#include <assert.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

/* private configuration --------------------------------------------------------------- */

#define HD44780_CHECK_RAWCURPOS               1

#define hd44780_rawcurpos_check(rawcurpos)    \
		( ( ((rawcurpos)&0x3F) < HD44780_DISP_COLS ) && ( ((rawcurpos)&0x80) == 0 ) )

/* private macros && macros sanity checks ----------------------------------------------- */

#ifndef HD44780_PDEBUG
	///#define HD44780_PDEBUG( printf_args ) do{}while(0)
	#define HD44780_PDEBUG( printf_args ) do{ ( printf printf_args ); }while(0)
#endif

#ifndef HD44780_PERR
	//#define HD44780_PERR( printf_args ) do{}while(0)
	#define HD44780_PERR( printf_args ) do{ printf("HD44780 PERR(%d):", __LINE__); ( printf printf_args ); }while(0)
#endif

#ifndef HD44780_USE_ASSERT
	//#define HD44780_ASSERT(expr) do{}while(0)
	#define HD44780_ASSERT(expr) assert(expr)
#endif

/* wrappers around user functions callbacks */

#if HD44780_READ_ENABLED && !defined(HD44780_ADDRESS_SET_READ)
	#ifndef HD44780_ADDRESS_SET_READ_CALLBACK
		#error HD44780_ADDRESS_SET_READ_CALLBACK not defined
	#endif // HD44780_ADDRESS_SET_READ_CALLBACK
	#ifndef HD44780_ADDRESS_SET_UP_TIME
		#error HD44780_ADDRESS_SET_UP_TIME not defined
	#endif // HD44780_ADDRESS_SET_UP_TIME
	#define HD44780_ADDRESS_SET_READ(FLAG_RW, ctrl, FLAG_RS) do{ \
		HD44780_ADDRESS_SET_READ_CALLBACK(FLAG_RW, ctrl, FLAG_RS); \
		HD44780_ADDRESS_SET_UP_TIME(); \
	}while(0)
#endif // HD44780_ADDRESS_SET_READ

#ifndef HD44780_ADDRESS_SET_WRITE
	#ifndef HD44780_ADDRESS_SET_WRITE_CALLBACK
		#error HD44780_ADDRESS_SET_WRITE_CALLBACK not defined
	#endif // HD44780_ADDRESS_SET_WRITE_CALLBACK
	#ifndef HD44780_ADDRESS_SET_UP_TIME
		#error HD44780_ADDRESS_SET_UP_TIME not defined
	#endif // HD44780_ADDRESS_SET_UP_TIME
	#define HD44780_ADDRESS_SET_WRITE(FLAG_RW, ctrl, FLAG_RS) do{ \
		HD44780_ADDRESS_SET_WRITE_CALLBACK(FLAG_RW, ctrl, FLAG_RS); \
		HD44780_ADDRESS_SET_UP_TIME(); \
	}while(0)
#endif // HD44780_ADDRESS_SET_WRITE

#ifndef HD44780_DATA_SET
	#ifndef HD44780_DATA_SET_CALLBACK
		#error HD44780_DATA_SET_CALLBACK not defined
	#endif // HD44780_DATA_SET_CALLBACK
	#ifndef HD44780_DATA_SET_UP_TIME
		#error HD44780_DATA_SET_UP_TIME not defined
	#endif // HD44780_DATA_SET_UP_TIME
	#define HD44780_DATA_SET(FLAG_RW, ctrl, FLAG_RS, data) do{ \
		HD44780_DATA_SET_CALLBACK(FLAG_RW, ctrl, FLAG_RS, data); \
		HD44780_DATA_SET_UP_TIME(); \
	}while(0)
#endif

#ifndef HD44780_ENABLE
	#ifndef HD44780_ENABLE_CALLBACK
		#error HD44780_ENABLE_CALLBACK not defined
	#endif // HD44780_ENABLE_CALLBACK
	#ifndef HD44780_ENABLE_PULSE_WIDTH
		#error HD44780_ENABLE_PULSE_WIDTH not defined
	#endif // HD44780_ENABLE_PULSE_WIDTH
	#define HD44780_ENABLE(FLAG_RW, ctrl, FLAG_RS) do{ \
		HD44780_ENABLE_CALLBACK(FLAG_RW, ctrl, FLAG_RS); \
		HD44780_ENABLE_PULSE_WIDTH(); \
	}while(0)
#endif // HD44780_ENABLE

#ifndef HD44780_DISABLE_ALL
	#ifndef HD44780_DISABLE_ALL_CALLBACK
		#error HD44780_DISABLE_ALL_CALLBACK not defined
	#endif // HD44780_DISABLE_ALL_CALLBACK
	#ifndef HD44780_DATA_HOLD_TIME
		#error HD44780_DATA_HOLD_TIME not defined
	#endif // HD44780_DATA_HOLD_TIME
	#define HD44780_DISABLE_ALL(FLAG_RW, FLAG_RS) do{ \
		HD44780_DISABLE_ALL_CALLBACK(FLAG_RW, FLAG_RS); \
		HD44780_DATA_HOLD_TIME(); \
	}while(0)
#endif // HD44780_DISABLE_ALL

/* ---------------- wrappers around flags --------------- */
#if HD44780_BF_CHECKING_ENABLED
	#ifndef HD44780_READ_ENABLED
		#error if checking busyflag, then reading from hd44780 must be enabled
	#endif
#endif

/* ------------------ check user defines ---------- */
#ifndef HD44780_LONG_DELAY
	#error HD44780_LONG_DELAY not defined
#endif // HD44780_LONG_DELAY
#ifndef HD44780_NORMAL_DELAY
	#error HD44780_NORMAL_DELAY not defined
#endif // HD44780_NORMAL_DELAY
#ifndef HD44780_INIT_CALLBACK
	#error HD44780_INIT_CALLBACK not defined
#endif // HD44780_INIT_CALLBACK
#if HD44780_READ_ENABLED
	#ifndef HD44780_DATA_GET_CALLBACK
		#error To read from displays you need to define HD44780_DATA_GET_CALLBACK function
	#endif
#endif

/* static variables ---------------------------------------------------------------------------------- */

/**
 * our struct
 */
struct hd44780_s hd44780 = {0};

/* low-level functions ----------------------------------------------------------------------------- */

/**
 * waits for hd44780 to complete operation
 */
#define _hd44780_sleep_on_write_(FLAG_RW, FLAG_RS, command) do{ \
	if ( FLAG_RS == HD44780_FLAG_RS_INST && HD44780_INST_IS_RETURNHOME(command) ) { \
		HD44780_LONG_DELAY(); \
	} else { \
		HD44780_NORMAL_DELAY(); \
	} \
}while(0)

/* checking busyflag --------------------------------------------------------------------------------- */

#if HD44780_BF_CHECKING_ENABLED
/** _hd44780_wait_for_busyflag_
 * wait for busyflag to go low on ctrl_num, mutex is locked
 */
static void _hd44780_wait_for_ready_(uint8_t ctrl)
{
	uint8_t readbuff;

	HD44780_PDEBUG(("hd wait_for_ready(%d)\n", ctrl));

#if HD44780_BF_CHECKING_OPTIONAL
	if ( HD44780_BFCHECK_IS_DISABLED(ctrl) ) {
		return;
	}
#endif // HD44780_BF_CHECKING_OPTIONAL

	HD44780_ADDRESS_SET_READ(HD44780_FLAG_RW_READ, ctrl, HD44780_FLAG_RS_INST);

#if HD44780_BF_CHECKING_COUNT_MAX
	uint16_t count;
	for ( count = HD44780_BF_CHECKING_COUNT_MAX; count; --count )
#else // HD44780_BF_CHECKING_COUNT_MAX
	for (;;)
#endif // HD44780_BF_CHECKING_COUNT_MAX
	{

		HD44780_EXTRA_ENABLE_CYCLE_TIME();
		HD44780_ENABLE(HD44780_FLAG_RW_READ, ctrl, HD44780_FLAG_RS_INST);
		readbuff = HD44780_DATA_GET_CALLBACK(HD44780_FLAG_RW_READ, ctrl, HD44780_FLAG_RS_INST);
		HD44780_DISABLE_ALL(HD44780_FLAG_RW_READ, HD44780_FLAG_RS_INST);

#if HD44780_MODE_4_BIT
		HD44780_EXTRA_ENABLE_CYCLE_TIME();
		HD44780_ENABLE(HD44780_FLAG_RW_READ, ctrl, HD44780_FLAG_RS_INST);
		/* ignore second byte */
		HD44780_DISABLE_ALL(HD44780_FLAG_RW_READ, HD44780_FLAG_RS_INST);

		if ( (readbuff&0x08) == 0x00 ) {
			break;
		}
#else // HD44780_MODE_4_BIT
		if ( (readbuff&0x80) == 0x00 ) {
			break;
		}
#endif // HD44780_MODE_4_BIT


	}

#if HD44780_BF_CHECKING_COUNT_MAX

#if HD44780_CHECKER_ENABLED
	hd44780.bfchecks_count[ctrl] += ( HD44780_BF_CHECKING_COUNT_MAX - count );
#endif // HD44780_CHECKER_ENABLED

	if ( !count ) {
		HD44780_PERR(("hd wait_for_ready err (%d) = %d\n", ctrl, readbuff));
#if HD44780_BF_CHECKING_OPTIONAL
		HD44780_BFCHECK_DISABLE(ctrl);
#endif // HD44780_BF_CHECKING_OPTIONAL
	}
#endif // HD44780_BF_CHECKING_COUNT_MAX

}
#endif // HD44780_BF_CHECKING_ENABLED

/* read operation ---------------------------------------------------------------------------- */

#if HD44780_READ_ENABLED
uint8_t hd44780_read(const uint8_t ctrl, const enum HD44780_FLAG_RS_e flag_rs)
{
	HD44780_ASSERT( ctrl < HD44780_NUM_CONTROLLERS );
	HD44780_ASSERT( flag_rs == HD44780_FLAG_RS_DATA || flag_rs == HD44780_FLAG_RS_INST );

	uint8_t data;

#if HD44780_BF_CHECKING_ENABLED
	_hd44780_wait_for_ready_(ctrl);
#endif

	// prepare read data
	HD44780_ADDRESS_SET_READ(HD44780_FLAG_RW_READ, ctrl, flag_rs);

#if HD44780_MODE_4_BIT
	for(uint8_t upper = 0; upper < 2; ++upper)
#endif
	{
		// read data
		HD44780_ENABLE(HD44780_FLAG_RW_READ, ctrl, flag_rs);
#if HD44780_MODE_4_BIT
		HD44780_EXTRA_ENABLE_CYCLE_TIME();
		if ( upper == 0 ) {
			data = HD44780_DATA_GET_CALLBACK(HD44780_FLAG_RW_READ, ctrl, flag_rs);
		} else {
			data = (data<<4) | HD44780_DATA_GET_CALLBACK(HD44780_FLAG_RW_READ, ctrl, flag_rs);
		}
		HD44780_PDEBUG(("hd_4read(%x,%x)=%x upper=%x\n", ctrl, flag_rs, data, upper));
#else // HD44780_MODE_4_BIT
		data = HD44780_DATA_GET_CALLBACK(HD44780_FLAG_RW_READ, ctrl, flag_rs);
		HD44780_PDEBUG(("hd_read(%x,%x)=%x\n", ctrl, flag_rs, data));
#endif // HD44780_MODE_4_BIT
		HD44780_DISABLE_ALL(HD44780_FLAG_RW_READ, flag_rs);
	}

#if HD44780_COUNTERS_ENABLED
	++hd44780.read_count[ctrl];
#endif // HD44780_COUNTERS_ENABLED

#if HD44780_BF_CHECKING_ENABLED
#if HD44780_BF_CHECKING_OPTIONAL
	if ( HD44780_BFCHECK_IS_DISABLED(ctrl) )
#endif // HD44780_BF_CHECKING_OPTIONAL
	{
		HD44780_NORMAL_DELAY();
	}
#endif // HD44780_BF_CHECKING_ENABLED

#if HD44780_CHECK_RAWCURPOS
	// if we have read cursor position, we can check if its ok
	if ( flag_rs == HD44780_FLAG_RS_INST &&
			( data & HD44780_DDRAM_ADDRESS ) &&
			!hd44780_rawcurpos_check(data)
		) {
		HD44780_PERR(("rawcurpos_check data=%x flag_rs=%x ret=%x col=%x\n",
				data, flag_rs, hd44780_rawcurpos_check(data), hd44780_col_from_rawcurpos(data)
		));
	}
#endif // HD44780_CHECK_RAWCURPOS

	return data;
}
#endif // HD44780_READ_ENABLED

/* write operation ------------------------------------------------------------------------ */

/**
 * Low-lever write function - writes specified data to controller(-s).
 * @param ctrl
 * @param flag_rs
 * @param data
 */
void hd44780_write_unsafe(const uint8_t ctrl, const enum HD44780_FLAG_RS_e flag_rs, const uint8_t data)
{
	HD44780_ASSERT( ctrl < HD44780_NUM_CONTROLLERS );
	HD44780_ASSERT( flag_rs == HD44780_FLAG_RS_DATA || flag_rs == HD44780_FLAG_RS_INST );

#if HD44780_CHECK_RAWCURPOS
	// if this is a command to set cursor position && if the rawcursor position is outside the window
	if ( flag_rs == HD44780_FLAG_RS_INST &&
			HD44780_INST_IS_DDRAM_ADDRESS(data) &&
			!hd44780_rawcurpos_check( HD44780_INST_GET_DDRAM_ADDRESS(data) )
		) {
		HD44780_PERR(("rawcurpos_check flag_rs=%x flag_rs=%x ret=%x col=%x\n",
				data, flag_rs, hd44780_rawcurpos_check(data), hd44780_col_from_rawcurpos(data)
		));
	}
#endif

#if HD44780_BF_CHECKING_ENABLED
	_hd44780_wait_for_ready_(ctrl);
#endif // HD44780_BF_CHECKING_ENABLED

	HD44780_ADDRESS_SET_WRITE(HD44780_FLAG_RW_WRITE, ctrl, flag_rs);

#if HD44780_MODE_4_BIT
	for(uint_fast8_t upper = 2; upper; --upper)
#endif
	{
		/** write data */
#if HD44780_MODE_4_BIT
		HD44780_EXTRA_ENABLE_CYCLE_TIME();
		HD44780_PDEBUG(("hd_4write(%x,%x,%x) upper=%x\n", ctrl, flag_rs, data, upper));
		if ( upper ) {
			HD44780_DATA_SET(HD44780_FLAG_RW_WRITE, ctrl, flag_rs, ( HIGH_HALFBYTE(data) ) );
		} else {
			HD44780_DATA_SET(HD44780_FLAG_RW_WRITE, ctrl, flag_rs, ( LOW_HALFBYTE(data) ) );
		}
#else // HD44780_MODE_4_BIT
		HD44780_PDEBUG(("hd_write(%x,%x,%x)\n", ctrl, flag_rs, data));
		HD44780_DATA_SET(HD44780_FLAG_RW_WRITE, ctrl, flag_rs, data);
#endif // HD44780_MODE_4_BIT
		HD44780_ENABLE(HD44780_FLAG_RW_WRITE, ctrl, flag_rs);
		HD44780_DISABLE_ALL(HD44780_FLAG_RW_WRITE, flag_rs);
	}

#if HD44780_COUNTERS_ENABLED
	++hd44780.write_count[ctrl];
#endif //HD44780_COUNTERS_ENABLED

#if HD44780_BF_CHECKING_ENABLED
#if HD44780_BF_CHECKING_OPTIONAL
	if ( HD44780_BFCHECK_IS_DISABLED(ctrl) )
#endif //  HD44780_BF_CHECKING_OPTIONAL
	{
		_hd44780_sleep_on_write_(HD44780_FLAG_RW_WRITE, flag_rs, data);
	}
#endif // HD44780_BF_CHECKING_ENABLED
}

#if HD44780_MODE_4_BIT
/**
 * Write to all controllers in 8-bit mode once, for doing initialization.
 * The lower bits of command are ignored.
 * The body of this function should be synchronized with hd44780_write
 * Gets called by hd44780_initializa_by_instruction();
 */
static inline void hd44780_write_8bit_op(const uint8_t ctrl, const enum HD44780_FLAG_RS_e flag_rs, const uint8_t command)
{
	HD44780_PDEBUG(("hd44780_write_8bit_op(%x,%x,%x)\n", ctrl, flag_rs, command));

	/** write data */
	HD44780_ADDRESS_SET_WRITE(HD44780_FLAG_RW_WRITE, ctrl, flag_rs);
	HD44780_DATA_SET(HD44780_FLAG_RW_WRITE, ctrl, flag_rs, ( HIGH_HALFBYTE(command) ) );
	HD44780_ENABLE(HD44780_FLAG_RW_WRITE, ctrl, flag_rs);
	HD44780_DISABLE_ALL(HD44780_FLAG_RW_WRITE, flag_rs);
}

#else // !HD44780_MODE_4_BIT
#define hd44780_write_8bit_op hd44780_write
#endif // !HD44780_MODE_4_BIT

/* --------------------------- medium-level functions --------------------- */

void hd44780_write(const uint8_t RAM_and_ctrl, const enum HD44780_FLAG_RS_e flag_rs, const uint8_t data)
{
	static_assert( HD44780_NUM_CONTROLLERS <= 0x7F ,"");
	const uint8_t ctrl = (RAM_and_ctrl&0x7F);
	HD44780_ASSERT( ctrl < HD44780_NUM_CONTROLLERS );
	HD44780_ASSERT( flag_rs == HD44780_FLAG_RS_DATA || flag_rs == HD44780_FLAG_RS_INST );

	// save cursor position
	uint8_t inst_set_rawcurpossave; // instruction to set saved rawcursor pos
	if ( flag_rs == HD44780_FLAG_RS_DATA ) {
		inst_set_rawcurpossave =
				( (RAM_and_ctrl&0x80) ? HD44780_CGRAM_ADDRESS : HD44780_DDRAM_ADDRESS )
				| hd44780_read_rawcurpos(ctrl);
	}
	uint_fast8_t check_every_write_times;

	for(check_every_write_times = HD44780_CHECK_EVERY_WRITE_MAX;
			check_every_write_times;
			--check_every_write_times) {

		// do the write
		hd44780_write_unsafe(ctrl, flag_rs, data);

		// check if the write was successful depending on the data writed
		if ( flag_rs == HD44780_FLAG_RS_DATA ) {
			// writing characters
			// restore cursor position
			hd44780_write_unsafe(ctrl, HD44780_FLAG_RS_INST, inst_set_rawcurpossave);
			// read character, compare to command
			if ( hd44780_read(ctrl, HD44780_FLAG_RS_DATA) == data ) {
				// write succeded, may continue
				break;
			}
			// restore cursor position
			hd44780_write_unsafe(ctrl, HD44780_FLAG_RS_INST, inst_set_rawcurpossave);
		} else { // flag_rs == FLAG_RS_INST
			if ( HD44780_INST_IS_DDRAM_ADDRESS(data) ) { // writing DDGRAM addresss
				if ( HD44780_INST_GET_DDRAM_ADDRESS( hd44780_read(ctrl, HD44780_FLAG_RS_INST) ) ==
						HD44780_INST_GET_DDRAM_ADDRESS(data) ) {
					break; // writing address was successfull
				}
			} else if ( HD44780_INST_IS_CGRAM_ADDRESS(data) ) { // writing DDGRAM addresss
				if ( HD44780_INST_GET_CGRAM_ADDRESS( hd44780_read(ctrl, HD44780_FLAG_RS_INST) ) ==
						HD44780_INST_GET_CGRAM_ADDRESS(data) ) {
					break; // writing address was successfull
				}
			} else {
				break; // success
			}
		}

		// write unsuccessful
		HD44780_PERR(("HD check_every_write goto write_again\n"));
	}

	if ( check_every_write_times == 0 ) {
		HD44780_PERR(("HD MAX_TIMES_CHECK_EVERY_WRITE times=%u MAX=%u\n",
				check_every_write_times, HD44780_CHECK_EVERY_WRITE_MAX));
		// meh, errors happen - we should probably implement reseting display here
	}

#ifdef HD44780_COUNTERS_ENABLED
	// increment errors count
	hd44780.errors_count[ctrl] += HD44780_CHECK_EVERY_WRITE_MAX - check_every_write_times;
#endif // HD44780_COUNTERS_ENABLED
}

/* high level functions ---------------------------------------------------------------------------------------- */

void hd44780_initialize_by_instruction(const uint8_t ctrl)
{
	HD44780_PDEBUG(("hd_initialize_by_instruction(%u) in\n", ctrl));

#if HD44780_BF_CHECKING_ENABLED && HD44780_BF_CHECKING_OPTIONAL
	/* no BF checking here */
	HD44780_BFCHECK_DISABLE_ALL();
#endif // HD44780_BF_CHECKING_OPTIONAL

	/* Wait for more then 15 ms after Vcc raises to 4.5V */
	usleep(15*1000);
	/* Function set (Interface is 8 bits long.) */
	hd44780_write_8bit_op(ctrl, HD44780_FLAG_RS_INST, HD44780_FS_DATAWIDTH8BIT);
	/* Wait for more then 4.1 ms */
	usleep(5*1000);
	/* Function set (Interface is 8 bits long.) */
	hd44780_write_8bit_op(ctrl, HD44780_FLAG_RS_INST, HD44780_FS_DATAWIDTH8BIT);
	/* Wait for more then 100 us */
	usleep(200);
	/* Function set (Interface is 8 bits long.) */
	hd44780_write_8bit_op(ctrl, HD44780_FLAG_RS_INST, HD44780_FS_DATAWIDTH8BIT);
	HD44780_LONG_DELAY();

#if HD44780_MODE_4_BIT
	/* Function set (Interface is 8 bits long.)
	 * Set Interface to be 4 bits long*/
	hd44780_write_8bit_op(ctrl, HD44780_FLAG_RS_INST, HD44780_FS_DATAWIDTH4BIT);
	HD44780_LONG_DELAY();
#endif

	/* initialize user-specified configuration
	 * done twice, to be sure */
	for(uint_fast8_t i = 0; i < 2; ++i) {

#if HD44780_MODE_4_BIT
		hd44780_write_inst(ctrl,
			HD44780_FS_DATAWIDTH4BIT |
			HD44780_FS_TWOLINES |
			HD44780_FS_5x8DOTS);
#else // HD44780_MODE_4_BIT
		hd44780_write_inst(ctrl,
			HD44780_FS_DATAWIDTH8BIT |
			HD44780_FS_TWOLINES |
			HD44780_FS_5x8DOTS);
#endif // HD44780_MODE_4_BIT
		HD44780_LONG_DELAY();

		hd44780_write_inst(ctrl,
			HD44780_DOOC_DISPLAYON |
			HD44780_DOOC_CURSORON |
			HD44780_DOOC_CURSORNOBLINK);
		HD44780_LONG_DELAY();

		hd44780_write_inst(ctrl,
			HD44780_CLRDISP);
		HD44780_LONG_DELAY();

		hd44780_write_inst(ctrl,
			HD44780_EMS_INC |
			HD44780_EMS_NOSHIFT);
		HD44780_LONG_DELAY();

#if HD44780_BF_CHECKING_ENABLED && HD44780_BF_CHECKING_OPTIONAL
		/* re-allow BFchecking on all displays */
		HD44780_BFCHECK_ENABLE_ALL();
		/* now bf checking is set on all hd-s,
		 * autodetection and disabling busyflag detection will
		 * happen automagically in _hd44780_wait_for_ready_ */
#endif

	}

	HD44780_PDEBUG(("hd_initialize_by_instruction(%u) out\n", ctrl));
}

void hd44780_init(void) 
{
	HD44780_PDEBUG(("HD44780_INIT_CALLBACK()\n"));
	HD44780_INIT_CALLBACK();

	for(uint_fast8_t i = 0; i < HD44780_NUM_CONTROLLERS; i++) {
		hd44780_initialize_by_instruction(i);
		hd44780_write_inst(i, HD44780_CLRDISP);
	}
}

void hd44780_write_cgram(const uint8_t ctrl, const uint8_t pos, const uint8_t cgram[8])
{
	assert( pos < 8 );
	const uint8_t rawcurpos = hd44780_read_rawcurpos(ctrl);
	hd44780_write_inst(ctrl, HD44780_CGRAM_ADDRESS | (pos<<3) );
	for(uint_fast8_t i = 8; i; --i) {
		hd44780_write(0x80 | ctrl, HD44780_FLAG_RS_DATA, *++cgram);
	}
	hd44780_write_rawcurpos(ctrl, rawcurpos);
}

void hd44780_write_cgram_all(const uint8_t pos, const uint8_t cgram[8])
{
	hd44780_foreach(i) {
		hd44780_write_cgram(i, pos, cgram);
	}
}

/* unit tests ------------------------------------------------------------------------------ */

static inline void hd44780_unittest_curpos()
{
	static_assert( hd44780_rawcurpos_check( 0x80 ) == false, "");
	static_assert( hd44780_rawcurpos_check( HD44780_DISP_COLS ) == false,"");
	static_assert( hd44780_rawcurpos_check( (HD44780_DISP_COLS-1) ) == true,"");
#if HD44780_DISP_ROWS >= 2
	static_assert( hd44780_rawcurpos_check( 0x40 | HD44780_DISP_COLS ) == false,"");
	static_assert( hd44780_rawcurpos_check( 0x40 | (HD44780_DISP_COLS-1) ) == true,"");
#endif
	struct {
		uint8_t raw;
		uint8_t curpos;
		uint8_t row;
		uint8_t col;
	} t[] = {
			{ 0x1, 0x1, 0, 1, },
			{ HD44780_DISP_COLS-1, HD44780_DISP_COLS-1, 0, HD44780_DISP_COLS-1 },
#if HD44780_DISP_ROWS >= 2
			{ 0x41, HD44780_DISP_COLS*1 + 1, 1, 1, },
			{ 0x40 | (HD44780_DISP_COLS-1), HD44780_DISP_COLS+HD44780_DISP_COLS-1, 1, HD44780_DISP_COLS-1 },
#endif
	};
	for(uint_fast8_t i = 0; i < sizeof(t)/sizeof(t[0]); ++i) {
		assert( hd44780_curpos_from_raw(t[i].raw) == t[i].curpos );
		assert( hd44780_curpos_to_raw(t[i].curpos) == t[i].raw );
		assert( hd44780_rowcol_to_rawcurpos(t[i].row, t[i].col) == t[i].raw );
		assert( hd44780_row_from_rawcurpos(t[i].raw) == t[i].row );
		assert( hd44780_col_from_rawcurpos(t[i].raw) == t[i].col );
		assert( hd44780_rawcurpos_check(t[i].raw) == true );
	}
}

void hd44780_unittest()
{
	hd44780_unittest_curpos();
}

void hd44780_assemblytest()
{
	//hd44780_init();

	hd44780_write_inst(0, HD44780_CLRDISP);

	uint8_t pos[] = { 0, 10, HD44780_DISP_COLS-1, HD44780_DISP_COLS+10, };
	for(uint8_t j = 0; j < 4; j++) {
		hd44780_write_curpos(0, pos[j]);
		hd44780_foreach(c) {
			for(uint8_t i=0;i<20;++i) {
				hd44780_write_data(c, 100+i);
			}
		}
		hd44780_foreach(c) {
			for(uint8_t i=0;i<20;++i) {
				assert( hd44780_read_data(c) == 100+i );
			}
		}
	}

	const uint8_t dark[8] = { 0xff };
	hd44780_write_cgram_all(0, dark);
	hd44780_foreach(c) {
		for(uint8_t i=0;i<HD44780_DISP_SIZE;++i) {
			hd44780_write_data(c, 0);
		}
	}
}
