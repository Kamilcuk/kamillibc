/*
 * criticalsection.c
 *
 *  Created on: 7 kwi 2018
 *      Author: kamil
 */
#include <uni/criticalsection.h>

#include <assert.h>

static unsigned int criticalsection_cnt = 0;

__attribute__((__weak__))
void criticalsection_enter_callback() {

}
__attribute__((__weak__))
void criticalsection_exit_callback() {

}

void criticalsection_enter()
{
	if (++criticalsection_cnt == 1) {
		criticalsection_enter_callback();
	}
}

void criticalsection_exit()
{
	assert(criticalsection_cnt > 0);
	if (--criticalsection_cnt == 0) {
		criticalsection_exit_callback();
	}
}
