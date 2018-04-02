/*
 * uni.c
 *
 *  Created on: 02.04.2018
 *      Author: michal
 */

#include <uni/uni.h>

#if 0 // this is just an example file

#include <stdio.h>

static char stdout_buffer[256];
static char stderr_buffer[256];

__attribute__((__constructor__))
void hw_init()
{
	if ( setvbuf(stdout, stdout_buffer, _IOLBF, sizeof(stdout_buffer)) != 0 )
		assert(0);
	if ( setvbuf(stderr, stderr_buffer, _IOLBF, sizeof(stderr_buffer)) != 0 )
		assert(0);

	HAL_Init();
	MX_Init();
}

#endif
