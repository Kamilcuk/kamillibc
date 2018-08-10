/*
 * msgstream.c
 *
 *  Created on: 16 mar 2018
 *      Author: kamil
 */
#include <findmsg/findmsg.h>

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
		if (pollret <= 0) return pollret;
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

static void findmsg_flushN(struct findmsg_s *t, size_t N)
{
	assert(N <= t->pos);
	t->pos -= N;
	memmove(&t->buf[0], &t->buf[N], t->pos);
}

static inline bool findmsg_isValid(const struct findmsg_s *t)
{
	//fprintf(stderr, "%d %d %d %d\n", t->buf, t->size, t->pos, t->msgReceivedSize);
	return t != NULL &&
			t->buf != NULL &&
			t->size > 0 &&
			0 <= t->pos && t->pos <= t->size &&
			0 <= t->msgReceivedSize && t->msgReceivedSize <= t->pos;
}

static inline bool findmsg_conf_isValid(const struct findmsg_conf_s *c)
{
	return c != NULL &&
			c->minlength > 0 &&
			c->maxlength > 0 &&
			c->minlength <= c->maxlength &&
			c->checkBeginning != NULL &&
			c->checkEnding != NULL;
}

/* Exported Functions --------------------------------------------------------- */

struct findmsg_s * findmsg_new(int fd, size_t size)
{
	struct findmsg_s * const t = malloc(sizeof(*t));
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
	assert(t != NULL);
	assert(*t != NULL);
	free((*t)->buf);
	free(*t);
	*t = NULL;
}

void findmsg_init(struct findmsg_s *t, int fd, char buf[], size_t size)
{
	assert(t != NULL);
	struct findmsg_s set = findmsg_INIT(fd, buf, size);
	assert(findmsg_isValid(&set));
	*t = set;
}

ssize_t findmsg_findmsg(struct findmsg_s *t,
		const struct findmsg_conf_s *c, void *arg,
		struct timespec *timeout)
{
	assert(findmsg_isValid(t));
	assert(findmsg_conf_isValid(c));
	findmsg_next(t);
	for (;; findmsg_flushN(t, 1)) {

		const ssize_t expectedMsgLen = findmsg_beginning(t, c, arg, timeout);
		if (expectedMsgLen <= 0) {
			return expectedMsgLen;
		}
		if ((size_t)expectedMsgLen > t->size) {
			continue;
		}

		const ssize_t ret = findmsg_ending(t, c, arg, expectedMsgLen, timeout);
		if (ret == findmsg_END_MSG_INVALID || ret == -ENOBUFS) {
			continue;
		}
		if (ret <= 0) {
			return ret;
		}

		t->msgReceivedSize = ret;
		break;
	}
	return t->msgReceivedSize;
}

void *findmsg_msgpnt(struct findmsg_s *t)
{
	assert(findmsg_isValid(t));
	return t->msgReceivedSize != 0 ? t->buf : NULL;
}

size_t findmsg_msglen(struct findmsg_s *t)
{
	assert(findmsg_isValid(t));
	return t->msgReceivedSize;
}

void findmsg_next(struct findmsg_s *t)
{
	assert(findmsg_isValid(t));
	if (t->msgReceivedSize > 0) {
		findmsg_flushN(t, t->msgReceivedSize);
		t->msgReceivedSize = 0;
	}
}

ssize_t findmsg_beginning(struct findmsg_s *t,
		const struct findmsg_conf_s *c, void *arg,
		struct timespec *timeout)
{
	assert(findmsg_isValid(t));
	assert(findmsg_conf_isValid(c));
	int ret;
	while (t->pos >= c->minlength || (ret = findmsg_readAtLeastChars(t, c->minlength, timeout)) > 0) {
		// check every minlength characters starting from t->buf[minlength ... t->pos-1] for beginning
		const size_t maxidx = t->pos - c->minlength;
		for (size_t i = 0; i <= maxidx; ++i) {
			const ssize_t retcheckBeginning = c->checkBeginning(&t->buf[i], c->minlength, arg);
			if (retcheckBeginning != 0) { // error in checkBeginning or beggining found
				if (retcheckBeginning > 0 && i > 0) {
					findmsg_flushN(t, i);
				}
				return retcheckBeginning;
			}
		}
		findmsg_flushN(t, maxidx + 1);
	}
	return ret;
}

ssize_t findmsg_ending(struct findmsg_s *t,
		const struct findmsg_conf_s *c, void *arg,
		size_t startlen, struct timespec *timeout)
{
	assert(findmsg_isValid(t));
	assert(findmsg_conf_isValid(c));
	assert(startlen <= c->maxlength);
	int ret;
	for (; t->pos >= startlen || (ret = findmsg_readAtLeastChars(t, startlen, timeout)) > 0; ++startlen) {
		const int retcheckEnding = c->checkEnding(t->buf, startlen, arg);
		if (retcheckEnding < 0 || retcheckEnding == findmsg_END_MSG_INVALID) {
			return retcheckEnding;
		}
		if (retcheckEnding == findmsg_END_MSG_VALID) {
			return startlen;
		}
		if (retcheckEnding != findmsg_END_MSG_TOO_SHORT) {
			assert(!"checkEnding function returned bad value");
		}
		if (startlen >= c->maxlength) {
			// buffer too short
			return -ENOBUFS;
		}
	}
	return ret;
}

ssize_t findmsg_get(struct findmsg_s *t,
		const struct findmsg_conf_s *c, void *arg,
		struct timespec *timeout,
		/*out*/ char buf[], size_t bufsize)
{
	assert(buf != NULL);
	ssize_t msgsize;
	if ((msgsize = findmsg_msglen(t)) == 0) {
		msgsize = findmsg_findmsg(t, c, arg, timeout);
		if (msgsize <= 0) return msgsize;
	}
	if (bufsize < msgsize) return -ENOBUFS;
	const char *msg = findmsg_msgpnt(t);
	assert(msg != NULL);
	memcpy(buf, msg, msgsize);
	findmsg_next(t);
	return msgsize;
}
