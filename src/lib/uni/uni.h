/*
 * uni.h
 *
 *  Created on: 02.04.2018
 *      Author: michal
 */

#include <uni/types.h>
#include <cdefs.h>

void uni_init(void) __constructor;
void uni_init_std(void) __constructor;
void uni_deinit(void);
