/*
 * criticalsection.h
 *
 *  Created on: 7 kwi 2018
 *      Author: kamil
 */

#ifndef SRC_KAMILLIBC_UNI_CRITICALSECTION_H_
#define SRC_KAMILLIBC_UNI_CRITICALSECTION_H_

#include <uni/types.h>
#include <stdbool.h>

#define CRITICALSECTION() \
	for(bool _tOdO = (criticalsection_enter(), 1); _tOdO; _tOdO = (criticalsection_exit(), 0))

#define CRITICALSECTION_WHILE(condition) \
	for(; criticalsection_enter(), !!(condition); criticalsection_exit())

void criticalsection_enter(void);
void criticalsection_exit(void);

void criticalsection_enter_callback(void);
void criticalsection_exit_callback(void);

#endif /* SRC_KAMILLIBC_UNI_CRITICALSECTION_H_ */
