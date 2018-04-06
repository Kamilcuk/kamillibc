/*
 * morse.h
 *
 *  Created on: 6 kwi 2018
 *      Author: kamil
 */

#ifndef SRC_LIB_MORSE_H_
#define SRC_LIB_MORSE_H_

void morse_send_char(void *arg, char c);

void morse_send_string(void *arg, const char *str);

#endif /* SRC_LIB_MORSE_H_ */
