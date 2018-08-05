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
#include <time.h>
#include <limits.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>

/* Macros ----------------------------------------------------------- */

/* Weak function ------------------------------------------------------ */

__attribute__((__weak__))
ssize_t findmsg_readtimeout(int fd, char buf[], size_t size,
		struct timespec *timeout)
{
	{
		// try to read from fd
		const ssize_t ret = read(fd, buf, size);
		if (ret != 0) {
			return ret;
		}
		// read returned 0 chars - fd is empty, poll for new characters
	}
	{
		fd_set fds[1];
		FD_ZERO(&fds[0]);
		FD_SET(fd, &fds[0]);
		const int ret = pselect(1, fds, NULL, NULL, timeout, NULL);
		if (ret <= 0) return ret;
		assert(FD_ISSET(fd, &fds[0]));
	}
#if 0 || USE_POLL
	{
		struct pollfd pollfd = {
				.fd = fd,
				.events = POLLIN,
		};
		const int poll_timeout = timeout->tv_sec * 1000 + timeout->tv_usec / 1000;
		const int pollret = poll(&pollfd, 1, poll_timeout);
		if(pollret <= 0) return pollret;
		assert(pollfd.revents&POLLIN);
		assert(pollret == 1);
	}
#endif
	// there are new characters in fd, read them
	return read(fd, buf, size);
}

/* Private functions ----------------------------------------------------------- */

static ssize_t findmsg_readAtLeastChars(struct findmsg_s *t, size_t N, struct timespec *timeout)
{
	if (t->pos >= N) return 0;
	const size_t toread = N - t->pos;
	const ssize_t ret = findmsg_readtimeout(t->fd, &t->buf[t->pos], toread, timeout);
	if (ret <= 0) return ret;
	assert((size_t)ret <= toread);
#if 0
	printf("READ %u %u ", t->pos, ret);
	for(size_t i=0;i<ret;++i) printf("%02x", t->buf[t->pos+i]);
	printf("\n");
#endif
	t->pos += ret;
	return ret;
}

static void findmsg_flushN(struct findmsg_s * restrict t, size_t N)
{
	assert(t != NULL && N <= t->pos);
	memmove(&t->buf[0], &t->buf[N], t->pos - N);
	t->pos -= N;
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
	struct findmsg_s set = findmsg_INIT(fd, buf, size);
	*t = set;
}

ssize_t findmsg_findmsg(struct findmsg_s *t,
		const struct findmsg_conf_s *conf, void *arg,
		struct timespec *timeout)
{
	assert(conf != NULL);
	findmsg_next(t);
	for(;; findmsg_flushN(t, 1) ) {

		const ssize_t expectedMsgLen =
				findmsg_beginning(t, conf->minlength, conf->checkBeginning, arg, timeout);
		if ( expectedMsgLen <= 0 ) {
			return expectedMsgLen;
		}
		if ( (size_t)expectedMsgLen > t->size ) {
			continue;
		}

		const int ret =
				findmsg_ending(t, expectedMsgLen, conf->maxlength, conf->checkEnding, arg, timeout);
		if ( ret == findmsg_END_MSG_INVALID || ret == -ENOBUFS ) {
			continue;
		}
		if ( ret <= 0 ) {
			return ret;
		}
		assert(ret != 0);

		t->msgReceivedSize = ret;
		break;
	}
	return t->msgReceivedSize;
}

void *findmsg_msgpnt(struct findmsg_s *t)
{
	assert(t != NULL);
	assert(t->msgReceivedSize > 0);
	return t->msgReceivedSize == 0 ? NULL : t->buf;
}

void findmsg_next(struct findmsg_s *t)
{
	assert(t != NULL);
	if (t->msgReceivedSize > 0) {
		findmsg_flushN(t, t->msgReceivedSize);
		t->msgReceivedSize = 0;
	}
}

ssize_t findmsg_beginning(struct findmsg_s *t, size_t minlen,
		ssize_t (*checkBeginning)(const char buf[], size_t minlen, void *arg), void *arg,
		struct timespec *timeout)
{
	assert(t != NULL);
	assert(1 <= minlen);
	assert(minlen < t->size);
	assert(checkBeginning != NULL);
	int ret;
	while( (ret = findmsg_readAtLeastChars(t, minlen, timeout)) >= 0 && t->pos >= minlen ) {
		// check every minlength characters starting from t->buf[minlength ... t->pos-1] for beginning
		const size_t maxidx = t->pos - minlen;
		for(size_t i = 0; i <= maxidx; ++i) {
			const ssize_t retcheckBeginning = checkBeginning(&t->buf[i], minlen, arg);
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

ssize_t findmsg_ending(struct findmsg_s *t, size_t startlen, size_t maxlen,
		int (*checkEnding)(const char buf[], size_t len, void *arg), void *arg,
		struct timespec *timeout)
{
	assert(t != NULL);
	assert(checkEnding != NULL);
	assert(startlen);
	assert(maxlen);
	if (maxlen > t->size) {
		maxlen = t->size;
	}
	assert(startlen <= maxlen);
	int ret;
	for(; (ret = findmsg_readAtLeastChars(t, startlen, timeout)) >= 0 && t->pos >= startlen; ++startlen) {
		const int retcheckEnding = checkEnding(t->buf, startlen, arg);
		if (retcheckEnding < 0 || retcheckEnding == findmsg_END_MSG_INVALID) {
			return retcheckEnding;
		}
		if (retcheckEnding == findmsg_END_MSG_VALID) {
			return startlen;
		}
		if (retcheckEnding != 0) {
			assert(!"checkEnding function returned bad value");
		}
		if (startlen >= maxlen) {
			// buffer too short
			return -ENOBUFS;
		}
	}
	return ret;
}

ssize_t findmsg_get(struct findmsg_s *t,
		const struct findmsg_conf_s *conf, void *arg,
		struct timespec *timeout,
		/*out*/ char buf[], size_t bufsize)
{
	assert(buf != NULL);
	const ssize_t msgsize = findmsg_findmsg(t, conf, arg, timeout);
	if (msgsize <= 0) {
		return msgsize;
	}
	const char *msg = findmsg_msgpnt(t);
	assert(msg != NULL);
	const size_t cpysize = MIN(bufsize, (size_t)msgsize);
	memcpy(buf, msg, cpysize);
	findmsg_next(t);
	return msgsize;
}
