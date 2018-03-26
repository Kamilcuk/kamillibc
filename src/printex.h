/*
 * printuint64.h
 *
 *  Created on: 23 mar 2018
 *      Author: kamil
 */

#ifndef SRC_CLIBS_PRINTEX_H_
#define SRC_CLIBS_PRINTEX_H_

#include <stdint.h>

char *printbinary_r(int width, uint64_t v, char result[width+1]);
#define PRINTBINARY(width, v)  printbinary_r((width), (v), (char[(width)+1]){})

char *printuint64_r(uint64_t v, char result[21]);
char *printuint64(uint64_t v);
#define PRINTUINT64(v)  printuint64_r((v), (char[21]){})

char *printint64_r(int64_t v, char result[22]);
char *printint64(int64_t v);
#define PRINTINT64(v)  printint64_r((v), (char[22]){})

int print_unittest();

#endif /* SRC_CLIBS_PRINTEX_H_ */
