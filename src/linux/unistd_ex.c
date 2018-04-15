/*
 * unistd_ex.c
 *
 *  Created on: 10 kwi 2018
 *      Author: kamil
 */
#define _POSIX_TIMERS

#include <unistd_ex.h>

#include <uni/cdefs.h>

#include <time.h>
#include <assert.h>
#include <errno.h>

#if __GNUC__
#define INT_MUL_OVERFLOW_P(a, b) \
   __builtin_add_overflow_p (a, b, (__typeof__ ((a) + (b))) 0)
#else
#define INT_ADD_OVERFLOW_P(a, b) do{}while(0);
#endif


void sdelay(time_t seconds) {
	nanodelay(seconds, 0);
}

void mdelay(time_ms_t ms) {
	const time_ms_t tausend = 1000;
	assert(!INT_MUL_OVERFLOW_P(ms,tausend));
	udelay(ms*tausend);
}

void udelay(time_us_t us) {
	const time_us_t tausend = 1000;
	assert(!INT_MUL_OVERFLOW_P(us,tausend));
	ndelay(us*tausend);
}

void ndelay(time_ns_t ns) {
	nanodelay(0, ns);
}

void nanodelay(time_t seconds, time_ns_t nanoseconds) {
	struct timespec rem = {seconds, nanoseconds};
	while(nanosleep(&rem, &rem) == -1 && errno == EINTR);
}


int poll_in(struct pollfd fds[], nfds_t nfds, int timeout) {
	for(nfds_t i = 0; i < nfds; ++i) {
		fds[i].events = POLLIN;
	}
	return poll(fds, nfds, timeout);
}

int poll_out(struct pollfd fds[], nfds_t nfds, int timeout) {
	for(nfds_t i = 0; i < nfds; ++i) {
		fds[i].events = POLLOUT;
	}
	return poll(fds, nfds, timeout);
}


