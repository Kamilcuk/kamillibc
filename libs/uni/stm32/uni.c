/*
 * uni.c
 *
 *  Created on: 02.04.2018
 *      Author: michal
 */
#include <uni/uni.h>
#include <cdefs.h>

#include <stdio.h>
#include <assert.h>

#ifndef STDOUT_BUFFER_SIZE
#define STDOUT_BUFFER_SIZE 256
#endif

#ifndef STDERR_BUFFER_SIZE
#define STDERR_BUFFER_SIZE 256
#endif

__attribute__((__constructor__, __weak__))
void uni_init_std(void)
{
	static char stdout_buffer[STDOUT_BUFFER_SIZE];
	if ( setvbuf(stdout, stdout_buffer, _IOLBF, sizeof(stdout_buffer)) != 0 )
		assert(0);
	static char stderr_buffer[STDERR_BUFFER_SIZE];
	if ( setvbuf(stderr, stderr_buffer, _IOLBF, sizeof(stderr_buffer)) != 0 )
		assert(0);
}
