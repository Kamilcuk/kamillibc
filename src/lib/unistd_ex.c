/*
 * unistd_ex.c
 *
 *  Created on: 10 kwi 2018
 *      Author: kamil
 */
#define _POSIX_TIMERS

#include <unistd_ex.h>

#include <cdefs.h>

#include <time.h>
#include <assert.h>
#include <errno.h>

__weak
void sdelay(time_t seconds) {
	nanodelay(seconds, 0);
}
__weak
void mdelay(time_ms_t ms) {
	const time_ms_t tausend = 1000;
	udelay(ms*tausend);
}
__weak
void udelay(time_us_t us) {
	const time_us_t tausend = 1000;
	ndelay(us*tausend);
}
__weak
void ndelay(time_ns_t ns) {
	nanodelay(0, ns);
}
__weak
void nanodelay(time_t seconds, time_ns_t nanoseconds) {
	struct timespec rem = {seconds, nanoseconds};
	while(nanosleep(&rem, &rem) == -1 && errno == EINTR);
}
__weak
int poll_in(struct pollfd fds[], nfds_t nfds, int timeout) {
	for(nfds_t i = 0; i < nfds; ++i) {
		fds[i].events = POLLIN;
	}
	return poll(fds, nfds, timeout);
}
__weak
int poll_out(struct pollfd fds[], nfds_t nfds, int timeout) {
	for(nfds_t i = 0; i < nfds; ++i) {
		fds[i].events = POLLOUT;
	}
	return poll(fds, nfds, timeout);
}


