/*
 * msgstream.c
 *
 *  Created on: 16 mar 2018
 *      Author: kamil
 */
#include <findmsg/findmsg.h>

#include <minmax.h>
#include <clocktimeout.h>
#include <cdefs.h>

#include <poll.h>
#include <sys/cdefs.h>
#include <time.h>
#include <limits.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>

/* Macros ----------------------------------------------------------- */

#define PRINTERR(str, ...)  fprintf(stderr, str, ##__VA_ARGS__)

/* Weak function ------------------------------------------------------ */

__weak_symbol
ssize_t findmsg_readtimeout(int fd, char buf[], size_t size, clock_t *timeout)
{
	struct pollfd pollfd = {
			.fd = fd,
			.events = POLLIN,
	};
	int timeout_poll_int;
	if (timeout == NULL) {
		timeout_poll_int = -1;
	} else {
		const clock_t timeout_poll = *timeout * 1000 / CLOCKS_PER_SEC;
		timeout_poll_int = MIN(timeout_poll, INT_MAX);
	}
	const int pollret = poll(&pollfd, 1, timeout_poll_int);
	assert(pollfd.revents&POLLIN);
	if(pollret <= 0) return pollret;
	assert(pollret == 1);
	return read(fd, buf, size);
}

/* Private functions ----------------------------------------------------------- */

static ssize_t findmsg_readAtLeastChars(struct findmsg_s *t, size_t N, clock_t *timeout)
{
	if (t->pos <= N) return 0;
	const size_t toread = N - t->pos;
	const ssize_t ret = findmsg_readtimeout(t->fd, &t->buf[t->pos], toread, timeout);
	if (ret <= 0) return ret;
	assert(ret <= toread);
#if 0
	printf("READ %u %u ", t->pos, ret);
	for(size_t i=0;i<ret;++i) printf("%02x", t->buf[t->pos+i]);
	printf("\n");
#endif
	t->pos += ret;
	return ret;
}

static void findmsg_flushN(struct findmsg_s *t, size_t N)
{
	assert(N <= t->pos);
	memmove(&t->buf[0], &t->buf[N], t->pos - N);
	t->pos -= N;
	t->msgReceivedSize = 0;
}

/* Exported Functions --------------------------------------------------------- */

int findmsg_new(struct findmsg_s *t, int fd, size_t size)
{
	assert(t);
	char *buf = malloc(size);
	if (buf == NULL) return errno;
	findmsg_init(t, fd, buf, size);
	return 0;
}

void findmsg_free(struct findmsg_s *t)
{
	assert(t);
	free(t->buf);
	*t = (struct findmsg_s){0};
}

void findmsg_init(struct findmsg_s *t, int fd, char buf[], size_t size)
{
	assert(t != NULL);
	assert(buf != NULL);
	*t = (struct findmsg_s)findmsg_INIT(fd, buf, size);
}

/**
 * Prepare receive buffer to receive next message
 * @param t
 * @param msg
 * @param size
 */
void findmsg_next(struct findmsg_s *t)
{
	if(t->msgReceivedSize > 0) {
		findmsg_flushN(t, t->msgReceivedSize);
	}
}

/**
 * Calls checkBeggining for every position in receive buffer if it is at least minlen long in specified timeout.
 * @param t
 * @param minlength
 * @param checkBeginning  same function as described in (uartbufrx_findmsgconf_s)
 * @param arg
 * @param timeout
 * @return checkBeginning returned value
 *         -ETIMEDOUT - on timeout
 */
ssize_t findmsg_beginning(struct findmsg_s *t, size_t minlength,
		ssize_t (*checkBeginning)(const char buf[], size_t minlength, void *arg), void *arg,
		clock_t *timeout)
{
	assert(t);
	assert(minlength);
	assert(checkBeginning);
	assert(minlength < t->size);
	clock_t start;
	clocktimeout_init(&start, timeout);
	while( findmsg_readAtLeastChars(t, minlength, timeout) >= 0 && t->pos >= minlength ) {
		// check every minlength characters starting from t->buf[minlength ... t->pos-1] for beginning
		const size_t maxidx = t->pos - minlength;
		for(size_t i = 0; i <= maxidx; ++i) {
			const ssize_t retcheckBeginning = checkBeginning(&t->buf[i], minlength, arg);
			if ( retcheckBeginning != 0 ) { // error in checkEnding or beggining found
				if ( retcheckBeginning > 0 && i > 0 ) {
					findmsg_flushN(t, i);
				}
				return retcheckBeginning;
			}
		}
		findmsg_flushN(t, maxidx + 1);

		if (clocktimeout_expired(&start, timeout)) { // timeout
			break;
		}
	}
	return -ETIMEDOUT;
}


/**
 * Wait for more characters in buffer up until maxlen until checkending returns != 0 in specified timeout
 * @param t
 * @param startlen
 * @param maxlen maximum length of chars in buffer
 * @param checkEnding same function as described in (uartbufrx_findmsgconf_s)->checkEnding
 * @param arg
 * @param Timeout
 * @return checkEnding returned value
 *         -ENOBUFS - no more space in buffer
 *         -ENOTIMEOUT - on timeout
 */
ssize_t findmsg_ending(struct findmsg_s *t, size_t startlen, size_t maxlength,
		int (*checkEnding)(const char buf[], size_t len, void *arg), void *arg,
		clock_t *timeout)
{
	assert(t);
	assert(checkEnding);
	clock_t start;
	clocktimeout_init(&start, timeout);
	if (maxlength == 0 || maxlength > t->size ) {
		maxlength = t->size;
	}
	for(; findmsg_readAtLeastChars(t, startlen, timeout) >= 0 && t->pos >= startlen; ++startlen ) {
		const int retcheckEnding = checkEnding(t->buf, startlen, arg);
		if ( retcheckEnding < 0 ) { // error in checkEnding
			return retcheckEnding;
		}
		if ( retcheckEnding > 0 ) { // ending found!
			return startlen;
		}
		if ( startlen >= maxlength ) { // buffer too short
			return -ENOBUFS;
		}
		if(clocktimeout_expired(&start, timeout)) { // timeout
			break;
		}
	}
	return -ETIMEDOUT;
}

/**
 * Find message in receive buffer
 * @param t
 * @param msg filled with pointer to message beginning
 * @param conf specifies functions for message finding
 * @param arg argument passed to conf-> functions
 * @param Timeout
 * @return < 0 on error, otherwise message length
 */
ssize_t findmsg(struct findmsg_s *t,
		const struct findmsg_conf_s *conf, void *arg,
		clock_t *timeout,
		const char *msg[])
{
	clock_t start;
	clocktimeout_init(&start, timeout);
	findmsg_next(t);
	do {

		const ssize_t expectedMsgLen =
				findmsg_beginning(t, conf->minlength, conf->checkBeginning, arg, timeout);
		if ( expectedMsgLen <= 0 ) {
			continue;
		}
		// message beggining found

		// expectedMsgLen is longer then our buffer can hold
		if ( expectedMsgLen > t->size ) {
			// try again
			PRINTERR("expected %zu rxbufsize %zu\n", expectedMsgLen, t->size);
			continue;
		}

		const int ret =
				findmsg_ending(t, expectedMsgLen, conf->maxlength, conf->checkEnding, arg, timeout);
		if ( ret < 0 ) {
			continue;
		}

		if ( msg != NULL ) *msg = t->buf;
		return t->msgReceivedSize = ret;

	} while( findmsg_flushN(t, 1), !clocktimeout_expired(&start, timeout) );
	return -ETIMEDOUT;
}

ssize_t findmsg_get(struct findmsg_s *t,
		const struct findmsg_conf_s *conf, void *arg,
		clock_t *timeout,
		/*out*/ char buf[], size_t bufsize)
{
	assert(buf != NULL);
	assert(conf->minlength <= bufsize);
	const char *msg;
	const ssize_t msgsize = findmsg(t, conf, arg, timeout, &msg);
	if (msgsize) return msgsize;
	const size_t cpysize = MIN(bufsize, msgsize);
	memcpy(buf, msg, cpysize);
	findmsg_next(t);
	return msgsize;
}
