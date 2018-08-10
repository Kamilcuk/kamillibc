/*
 * clocktimeout.c
 *
 *  Created on: 11 sie 2018
 *      Author: kamil
 */
#include <clocktimeout.h>

int main()
{
	const clock_t meas_start = clock();
	const clock_t timeout = 100;
	clock_t statetimeout = timeout;
	clock_t statenow = clocktimeout_start(&statetimeout);
	while (!clocktimeout_expired(&statenow, &statetimeout));
	const clock_t meas_stop = clock();
	//printf("%d %d %d %d \n", meas_stop, meas_start, timeout, statetimeout);
	bool ret = (meas_stop - meas_start) > timeout && statetimeout == 0;
	return !ret;
}
