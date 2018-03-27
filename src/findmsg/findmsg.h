/*
 * findmsg.h
 *
 *  Created on: 16 mar 2018
 *      Author: kamil
 */

#ifndef SRC_findmsg_H_
#define SRC_findmsg_H_

#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <stdbool.h>
#include <stdint.h>
#include <time.h>

/* Exported constants -------------------------------------------- */

enum findmsg_MSG_e {
	findmsg_MSG_TOO_SHORT  =      0,
	findmsg_MSG_VALID      =      1,
	findmsg_MSG_INVALID    =      2,
};

/* Exported types --------------------------------------------------- */

struct findmsg_s
{
	// private:
	int fd;
	char * buf;
	size_t size;
	size_t pos;
	size_t msgReceivedSize;
};

struct findmsg_conf_s
{
	size_t minlength;
	size_t maxlength;
	ssize_t (*checkBeginning)(const char buf[], size_t bufsize, void *arg);
	int (*checkEnding)(const char buf[], size_t bufsize, void *arg);
};

/* Weak functions --------------------------------------------------------- */

ssize_t findmsg_readtimeout(int fd, char buf[], size_t size, clock_t *timeout);

/* Exported Functions ----------------------------------------------------- */

struct findmsg_s * findmsg_new(int fd, size_t bufsize);
void findmsg_free(struct findmsg_s **t);

#define findmsg_INIT(_fd, _buf, _size)     {.fd = (_fd), .buf = (_buf), .size = (_size)}
#define findmsg_INIT_ON_STACK(_fd, _size)  findmsg_INIT(_fd, ((char[_size]){}), _size)
void findmsg_init(struct findmsg_s *t, int fd, char buf[], size_t size);

void findmsg_next(struct findmsg_s *t);

ssize_t findmsg_beginning(struct findmsg_s *t, size_t minlen,
		ssize_t (*checkBeginning)(const char buf[], size_t minlen, void *arg), void *arg,
		clock_t *timeout);

ssize_t findmsg_ending(struct findmsg_s *t, size_t startlen, size_t maxlen,
		int (*checkEnding)(const char buf[], size_t len, void *arg), void *arg,
		clock_t *timeout);

ssize_t findmsg(struct findmsg_s *t,
		const struct findmsg_conf_s *conf, void *arg,
		clock_t *timeout,
		/*out*/ const char **msg);

ssize_t findmsg_get(struct findmsg_s *t,
		const struct findmsg_conf_s *conf, void *arg,
		clock_t *timeout,
		/*out*/ char buf[], size_t bufsize);

#endif /* SRC_findmsg_H_ */
