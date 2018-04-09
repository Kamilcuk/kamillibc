/*
 * uni.c
 *
 *  Created on: 02.04.2018
 *      Author: michal
 */

#include <uni/hw.h>

#include <uni/types.h>

#include <stdio.h>
#include <assert.h>

#ifndef STDOUT_BUFFER_SIZE
#define STDOUT_BUFFER_SIZE 256
#endif
#ifndef STDERR_BUFFER_SIZE
#define STDERR_BUFFER_SIZE 256
#endif

void _hw_init_std()
{
	static char stdout_buffer[STDOUT_BUFFER_SIZE];
	static char stderr_buffer[STDERR_BUFFER_SIZE];
	if ( setvbuf(stdout, stdout_buffer, _IOLBF, sizeof(stdout_buffer)) != 0 )
		assert(0);
	if ( setvbuf(stderr, stderr_buffer, _IOLBF, sizeof(stderr_buffer)) != 0 )
		assert(0);
}
