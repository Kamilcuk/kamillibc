/*
 * morse.c
 *
 *  Created on: 6 kwi 2018
 *      Author: kamil
 */
#include <morse.h>

#include <morse_config.h>

#ifndef MORSE_ON
#error MORSE_ON not defined
#define MORSE_ON(arg)  do { PORTA = PORTB = PORTC = 0xff; }while(0)
#endif

#ifndef MORSE_OFF
#error MORSE_OFF not defined
#define MORSE_OFF(arg) do { PORTA = PORTB = PORTC = 0x00; }while(0)
#endif

#ifndef MORSE_DELAY
#error MORSE_DELAY not defined
#define MORSE_DELAY(arg) __delay_ms(150)
#endif

// Lookup table for Morse code.  Index is calculated from LCD char
// value.  Reading from least to most sig. bit:
//
//   Filler zeros (not sent)
//   Start bit (not sent)
//   Bits to be sent, 1 = dit, 0 = dah (reverse order)
const unsigned char CODE_TABLE[54] = {
        0x01,  // Flag to send blank
        0x86,  // '
        0x94,  // (
        0x4a,  // )
        0x00,  // * - not used
        0xac,  // + or AR
        0x32,  // comma
        0x7a,  // minus or long break
        0x56,  // period
        0xb4,  // slash
        0x04,  // 0
        0x0c,  // 1
        0x1c,  // 2
        0x3c,  // 3
        0x7c,  // 4
        0xfc,  // 5
        0xf4,  // 6
        0xe4,  // 7
        0xc4,  // 8
        0x84,  // 9
        0xe2,  // colon
        0xaa,  // semicolon
        0x00,  // less than - not used
        0x74,  // equals or short break BT
        0x5e,  // grtr than = SK
        0xce,  // question mark
        0xa6,  // @ symbol
        0x60,  // A
        0xe8,  // B
        0xa8,  // C
        0xd0,  // D
        0xc0,  // E
        0xb8,  // F
        0x90,  // G
        0xf8,  // H
        0xe0,  // I
        0x18,  // J
        0x50,  // K
        0xd8,  // L
        0x20,  // M
        0xa0,  // N
        0x10,  // O
        0x98,  // P
        0x48,  // Q
        0xb0,  // R
        0xf0,  // S
        0x40,  // T
        0x70,  // U
        0x78,  // V
        0x30,  // W
        0x68,  // X
        0x28,  // Y
        0xc8,  // Z
        0x00}; // Null - send nothing

void morse_send_char(void *arg, char c)
{
	unsigned char k, j, n_elts;
	unsigned char code;

	// conert ASCII to Morse Code
	if (c == 32 || c == 0x0A ) { // 32 = space,' ', 0x0A = newline,'\n'
		k = 0; // Blank
	} else if((c > 96) && (c < 123)) { // islower(c)
		k = (c - 32) - 38; // toupper(c) - 38
	} else if((c > 38) && (c < 91)) {
		k = c - 38; // All others
	} else {
		k = 53; // Send nothing
	}
	code = CODE_TABLE[k];

	if (code == 0x01) {
		// For additional space btwn words
		MORSE_DELAY(arg);
		MORSE_DELAY(arg);
		MORSE_DELAY(arg);
		MORSE_DELAY(arg);
	} else if (code != 0x00) {
		// Send Morse character
		for (j = 0; j < 8; j++) { // Find start bit
			if ( (code & 0x01) == 0x01 ) {
				break;
			}
			code = code >> 1;
		}
		n_elts = 7 - j;    // Number of character elements

		for (j = 0; j < n_elts; j++) { // Send character elements
			// Shift out start bit and/or Shift to next element
			code = code >> 1;
			MORSE_ON(arg);
			MORSE_DELAY(arg);  // Delay for one Morse element (dit)
			if (!(code & 1)) {
				MORSE_DELAY(arg);  // Delay for 3 Morse elements (dah)
				MORSE_DELAY(arg);
			}
			MORSE_OFF(arg);
			MORSE_DELAY(arg);
		}
		// For additional space btwn char
		MORSE_DELAY(arg);
		MORSE_DELAY(arg);
	}
}


void morse_send_string(void *arg, const char *str)
{
	for(char c; (c = *str) != '\0'; ++str) {
		morse_send_char(arg, c);
	}
}
