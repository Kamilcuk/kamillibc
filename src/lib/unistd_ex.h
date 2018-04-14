/*
 * unistd_ex.h
 *
 *  Created on: 10 kwi 2018
 *      Author: Kamil Cukrowski
 *     License: Jointly under MIT License and Beerware License
 *
 * This file served as an extension to missing posix functions.
 * All functions here may be implemented with posix functions.
 */
#ifndef _UNISTD_EX_H_
#define _UNISTD_EX_H_

#include <time_ex.h>
#include <sys/time.h>
#include <unistd.h>
#include <time.h>
#include <poll.h>

// same as usleep, but with miliseconds
int msleep(time_ms_t ms);

// delay function work same as sleep, but ignore signals (run in a loop)
void sdelay(time_t seconds);
void mdelay(time_ms_t ms);
void udelay(time_us_t us);
void ndelay(time_ns_t ns);
void nanodelay(time_t seconds, time_ns_t nanoseconds);

// polls only for POLL* event flag, only fds[*].fd needs to be filled, events are set to POLL* flag
int poll_in(struct pollfd fds[], nfds_t nfds, int timeout);
int poll_out(struct pollfd fds[], nfds_t nfds, int timeout);

#endif /* _UNISTD_EX_H_ */
