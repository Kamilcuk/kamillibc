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

__attribute__((__weak__))
void sdelay(time_t seconds) {
	nanodelay(seconds, 0);
}

__attribute__((__weak__))
void mdelay(time_ms_t ms) {
	nanodelay(ms / 1000, (ms % 1000) * 1000000);
}

__attribute__((__weak__))
void udelay(time_us_t us) {
	nanodelay(us / 1000000, (us % 1000000) * 1000);
}

__attribute__((__weak__))
void ndelay(time_ns_t ns) {
	nanodelay(0, ns);
}

__attribute__((__weak__))
void nanodelay(time_t seconds, time_ns_t nanoseconds) {
	struct timespec rem = {seconds, nanoseconds};
	while(nanosleep(&rem, &rem) == -1 && errno == EINTR);
}

__attribute__((__weak__))
int poll_in(struct pollfd fds[], nfds_t nfds, int timeout) {
	for(nfds_t i = 0; i < nfds; ++i) {
		fds[i].events = POLLIN;
	}
	return poll(fds, nfds, timeout);
}

__attribute__((__weak__))
int poll_out(struct pollfd fds[], nfds_t nfds, int timeout) {
	for(nfds_t i = 0; i < nfds; ++i) {
		fds[i].events = POLLOUT;
	}
	return poll(fds, nfds, timeout);
}



