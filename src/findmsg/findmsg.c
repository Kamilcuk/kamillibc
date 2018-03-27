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

#include <sys/cdefs.h>
#include <poll.h>
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
	// try to read from fd
	const ssize_t readret = read(fd, buf, size);
	if (readret != 0) return readret;
	// read returned 0 chars - fd is empty, poll for new characters
	struct pollfd pollfd = {
			.fd = fd,
			.events = POLLIN,
	};
	const int poll_timeout = clocktimeout_timeout_to_polltimeout(timeout);
	const int pollret = poll(&pollfd, 1, poll_timeout);
	if(pollret <= 0) return pollret;
	assert(pollfd.revents&POLLIN);
	assert(pollret == 1);
	// there are new characters in fd, read them
	return read(fd, buf, size);
}

/* Private functions ----------------------------------------------------------- */

static ssize_t findmsg_readAtLeastChars(struct findmsg_s *t, size_t N, clock_t *start, clock_t *timeout)
{
	if (t->pos >= N) return 0;
	const size_t toread = N - t->pos;
	const ssize_t ret = findmsg_readtimeout(t->fd, &t->buf[t->pos], toread, timeout);
	clocktimeout_update(start, timeout);
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

static ssize_t findmsg_beginning_started(struct findmsg_s *t, size_t minlength,
		ssize_t (*checkBeginning)(const char buf[], size_t minlength, void *arg), void *arg,
		clock_t *start, clock_t *timeout)
{
	assert(t != NULL);
	if (minlength == 0) {
		minlength = 1;
	}
	assert(minlength < t->size);
	assert(checkBeginning != NULL);
	int ret;
	while( (ret = findmsg_readAtLeastChars(t, minlength, start, timeout)) >= 0 && t->pos >= minlength ) {
		// check every minlength characters starting from t->buf[minlength ... t->pos-1] for beginning
		const size_t maxidx = t->pos - minlength;
		for(size_t i = 0; i <= maxidx; ++i) {
			const ssize_t retcheckBeginning = checkBeginning(&t->buf[i], minlength, arg);
			if ( retcheckBeginning != 0 ) { // error in checkBeginning or beggining found
				if ( retcheckBeginning > 0 && i > 0 ) {
					findmsg_flushN(t, i);
				}
				return retcheckBeginning;
			}
		}
		findmsg_flushN(t, maxidx + 1);
	}
	return ret;
}

ssize_t findmsg_ending_started(struct findmsg_s *t, size_t startlen, size_t maxlength,
		int (*checkEnding)(const char buf[], size_t len, void *arg), void *arg,
		clock_t *start, clock_t *timeout)
{
	assert(t != NULL);
	assert(checkEnding != NULL);
	if (startlen == 0) {
		startlen = 1;
	}
	if (maxlength == 0 || maxlength > t->size ) {
		maxlength = t->size;
	}
	assert(startlen < maxlength);
	int ret;
	for(; (ret = findmsg_readAtLeastChars(t, startlen, start, timeout)) >= 0 && t->pos >= startlen; ++startlen ) {
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
	}
	return ret;
}

/* Exported Functions --------------------------------------------------------- */

struct findmsg_s * findmsg_new(int fd, size_t size)
{
	struct findmsg_s * t = malloc(sizeof(*t));
	if (t == NULL) goto ERROR_MALLOC_T;
	char * const buf = malloc(size);
	if (buf == NULL) goto ERROR_MALLOC_BUF;
	findmsg_init(t, fd, buf, size);
	return t;
ERROR_MALLOC_BUF:
	free(t);
ERROR_MALLOC_T:
	return NULL;
}

void findmsg_free(struct findmsg_s **t)
{
	assert(t);
	free((*t)->buf);
	free(*t);
	*t = NULL;
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
	assert(t != NULL);
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
 * @return negative value on error, zero on timeout or message length
 *
 */
ssize_t findmsg_beginning(struct findmsg_s *t, size_t minlen,
		ssize_t (*checkBeginning)(const char buf[], size_t minlen, void *arg), void *arg,
		clock_t *timeout)
{
	clock_t start;
	clocktimeout_init(&start, timeout);
	return findmsg_beginning_started(t, minlen, checkBeginning, arg, &start, timeout);
}


/**
 * Wait for more characters in buffer up until maxlen until checkending returns != 0 in specified timeout
 * @param t
 * @param startlen
 * @param maxlen maximum length of chars in buffer
 * @param checkEnding same function as described in (uartbufrx_findmsgconf_s)->checkEnding
 * @param arg
 * @param Timeout
 * @return negative value on error, zero on timeout or message length
 */
ssize_t findmsg_ending(struct findmsg_s *t, size_t startlen, size_t maxlen,
		int (*checkEnding)(const char buf[], size_t len, void *arg), void *arg,
		clock_t *timeout)
{
	clock_t start;
	clocktimeout_init(&start, timeout);
	return findmsg_ending_started(t, startlen, maxlen, checkEnding, arg, &start, timeout);
}

/**
 * Find message in receive buffer
 * @param t
 * @param msg filled with pointer to message beginning
 * @param conf specifies functions for message finding
 * @param arg argument passed to conf-> functions
 * @param Timeout
 * @return negative value on error, zero on timeout or message length
 */
ssize_t findmsg(struct findmsg_s *t,
		const struct findmsg_conf_s *conf, void *arg,
		clock_t *timeout,
		const char *msg[])
{
	assert(conf != NULL);
	clock_t start;
	clocktimeout_init(&start, timeout);
	findmsg_next(t);
	do {

		const ssize_t expectedMsgLen =
				findmsg_beginning_started(t, conf->minlength, conf->checkBeginning, arg, &start, timeout);
		if ( expectedMsgLen <= 0 ) {
			return expectedMsgLen;
		}
		if ( expectedMsgLen > t->size ) {
			findmsg_flushN(t, 1);
			PRINTERR("expected %zu rxbufsize %zu\n", expectedMsgLen, t->size);
			continue;
		}

		const int ret =
				findmsg_ending_started(t, expectedMsgLen, conf->maxlength, conf->checkEnding, arg, &start, timeout);
		if ( ret == findmsg_MSG_INVALID || ret == -ENOBUFS ) {
			findmsg_flushN(t, 1);
			continue;
		}
		if ( ret <= 0 ) {
			return ret;
		}
		assert(ret != 0);

		if ( msg != NULL ) *msg = t->buf;
		t->msgReceivedSize = ret;
	} while( t->msgReceivedSize == 0 );
	return t->msgReceivedSize;
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
