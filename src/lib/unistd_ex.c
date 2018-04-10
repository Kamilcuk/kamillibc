/*
 * unistd_ex.c
 *
 *  Created on: 10 kwi 2018
 *      Author: kamil
 */
#include <unistd_ex.h>

#include <uni/cdefs.h>

#include <assert.h>
#include <errno.h>

#if __GNUC__
#define INT_MUL_OVERFLOW_P(a, b) \
   __builtin_add_overflow_p (a, b, (__typeof__ ((a) + (b))) 0)
#else
#define INT_ADD_OVERFLOW_P(a, b) do{}while(0);
#endif

__weak_symbol
int msleep(mseconds_t ms) {
	const mseconds_t tausend = 1000;
	assert(!INT_MUL_OVERFLOW_P(ms,tausend));
	return usleep(ms*tausend);
}

__weak_symbol
void sdelay(time_t seconds) {
	nanodelay(seconds, 0);
}
__weak_symbol
void mdelay(mseconds_t ms) {
	const mseconds_t tausend = 1000;
	assert(!INT_MUL_OVERFLOW_P(ms,tausend));
	udelay(ms*tausend);
}
__weak_symbol
void udelay(useconds_t us) {
	const useconds_t tausend = 1000;
	assert(!INT_MUL_OVERFLOW_P(us,tausend));
	ndelay(us*tausend);
}
__weak_symbol
void ndelay(nseconds_t ns) {
	nanodelay(0, ns);
}
__weak_symbol
void nanodelay(time_t seconds, suseconds_t nanoseconds) {
	struct timespec rem = {seconds, nanoseconds};
	while(nanosleep(&rem, &rem) == -1 && errno == EINTR);
}

__weak_symbol
int poll_in(struct pollfd fds[], nfds_t nfds, int timeout) {
	for(nfds_t i = 0; i < nfds; ++i) {
		fds[i].events = POLLIN;
	}
	return poll(fds, nfds, timeout);
}
__weak_symbol
int poll_out(struct pollfd fds[], nfds_t nfds, int timeout) {
	for(nfds_t i = 0; i < nfds; ++i) {
		fds[i].events = POLLOUT;
	}
	return poll(fds, nfds, timeout);
}


