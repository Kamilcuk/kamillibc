/*
 * hd44780-inst-def.h
 * 
 * 2012 Kamil Cukrowski <kamil@dyzio.pl>
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
 
#ifndef _hd44780_inst_def_
#define _hd44780_inst_def_

/* hd44780 instructions definitions */
#define HD44780_CLRDISP             0x01
#define HD44780_RETURNHOME          0x02
/* EMS - Entry Mode Set */
#define HD44780_EMS_INC             0x06
#define HD44780_EMS_DEC             0x04
#define HD44780_EMS_SHIFT           0x05
#define HD44780_EMS_NOSHIFT         0x04
/* DOOC - Display On/Off Control */
#define HD44780_DOOC_DISPLAYON      0x0c
#define HD44780_DOOC_DISPLAYOFF     0x08
#define HD44780_DOOC_CURSORON       0x0a
#define HD44780_DOOC_CURSOROFF      0x08
#define HD44780_DOOC_CURSORBLINK    0x09
#define HD44780_DOOC_CURSORNOBLINK  0x08
/* CDS - Cursor and Display Shift */
#define HD44780_CDS_DISPSHIFT       0x18
#define HD44780_CDS_CURSORSHIFT     0x10
#define HD44780_CDS_SHIFTRIGHT      0x14
#define HD44780_CDS_SHIFTLEFT       0x10
/* FS - Function Set */
#define HD44780_FS_DATAWIDTH8BIT    0x30
#define HD44780_FS_DATAWIDTH4BIT    0x20
#define HD44780_FS_TWOLINES         0x28
#define HD44780_FS_ONELINE          0x20
#define HD44780_FS_5x10DOTS         0x24
#define HD44780_FS_5x8DOTS          0x20
/* cgram ddram adress */
#define HD44780_CGRAM_ADDRESS       0x40
#define HD44780_DDRAM_ADDRESS       0x80

#define HD44780_INST_IS_CLRDISP(x)             (!((x) & 0xFE) && ((x) & 0x01))
#define HD44780_INST_IS_RETURNHOME(x)          (!((x) & 0xFC) && ((x) & 0x02))
#define HD44780_INST_IS_EMS(x)                 (!((x) & 0xF8) && ((x) & 0x04))
#define HD44780_INST_IS_DOOC(x)                (!((x) & 0xF0) && ((x) & 0x08))
#define HD44780_INST_IS_CDS(x)                 (!((x) & 0xE0) && ((x) & 0x10))
#define HD44780_INST_IS_FS(x)                  (!((x) & 0xC0) && ((x) & 0x20))
#define HD44780_INST_IS_CGRAM_ADDRESS(x)       (!((x) & 0x80) && ((x) & 0x40))
#define HD44780_INST_IS_DDRAM_ADDRESS(x)         ((x) & 0x80)

#define HD44780_INST_GET_EMS_COMMAND(x)        ((x) & 0x03)
#define HD44780_INST_GET_DOOC_COMMAND(x)       ((x) & 0x07)
#define HD44780_INST_GET_CDS_COMMAND(x)        ((x) & 0x0F)
#define HD44780_INST_GET_FS_COMMAND(x)         ((x) & 0x1F)
#define HD44780_INST_GET_CGRAM_ADDRESS(x)      ((x) & 0x3F)
#define HD44780_INST_GET_DDRAM_ADDRESS(x)      ((x) & 0x7F)

#endif /* _hd44780_inst_def_ */
