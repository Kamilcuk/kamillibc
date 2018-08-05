/*
 * findmsg.h
 *
 *  Created on: 16 mar 2018
 *      Author: kamil
 */

#ifndef SRC_findmsg_H_
#define SRC_findmsg_H_

#include <sys/types.h>
#include <stdbool.h>
#include <stdint.h>
#include <time.h>

/* Exported constants -------------------------------------------- */

/**
 * Returned by checkBeginning functions
 */
enum findmsg_BEG_e {
	findmsg_BEG_NOT_MSG_BEGNNING = 0,
	findmsg_BEG_MSG_BEGINNING    = 1,
};

/**
 * Returned by checkEnding functions
 */
enum findmsg_END_e {
	findmsg_END_MSG_TOO_SHORT = 0,
	findmsg_END_MSG_VALID     = 1,
	findmsg_END_MSG_INVALID   = 2,
};

/* Exported types --------------------------------------------------- */

struct findmsg_s {
	// private:
	/**
	 * File descriptor to read from
	 */
	int fd;
	/**
	 * Pointer to allocated buffer of size size
	 */
	char * buf;
	/**
	 * Size of buf
	 */
	size_t size;
	/**
	 * Current position inside buf
	 */
	size_t pos;
	/**
	 * Then length of the last received message or zero
	 */
	size_t msgReceivedSize;
};

struct findmsg_conf_s {
	/**
	 * Minimum length of the message.
	 */
	size_t minlength;
	/**
	 * Maximum length of the message
	 */
	size_t maxlength;
	/**
	 * Checks if the bytes pointed by buf contain message beginning
	 * @param buf
	 * @param bufsize is equal to minlength (always)
	 * @param arg pointer to context variable
	 * @return positive value that is presumed length of the message or 1, when
	 * 	          a message starts in bytes pointed to by buf
	 *         0 or findmsg_NOT_MSG_BEGNNING when message beginning is not in buf
	 *         negative value on error
	 */
	ssize_t (*checkBeginning)(const char buf[], size_t bufsize, void *arg);
	/**
	 * Checks
	 * @param buf
	 * @param bufsize
	 * @param arg
	 * @return findmsg_MSG_VALID if buf contains valid message with the length bufsize
	 *         findmgs_MSG_TOO_SHOFT if bufsize is too small
	 *         findmsg_MSG_INVALID if buf contains an invalid message (for ex. CRC failed)
	 *         negative value on internal error
	 */
	int (*checkEnding)(const char buf[], size_t bufsize, void *arg);
};

/* Weak functions --------------------------------------------------------- */

/**
 * Read from fd maximum size characters in timeout time
 * @param fd
 * @param buf
 * @param size
 * @param timeout
 * @return number of read characters or negative on error
 */
ssize_t findmsg_readtimeout(int fd, char buf[], size_t size, struct timespec *timeout);

/* Exported Functions ----------------------------------------------------- */

/**
 * Initialize findmsg object with buffer allocated with malloc
 * @param fd
 * @param bufsize
 * @return pointer to valid findmsg object or NULL
 */
struct findmsg_s * findmsg_new(int fd, size_t bufsize);

/**
 * Free findmsg object
 * @param t
 */
void findmsg_free(struct findmsg_s **t);

/**
 * \def findmsg_INIT(_fd, _buf, _size)
 * initialize findmsg object
 */
#define findmsg_INIT(_fd, _buf, _size)     { .fd = (_fd), .buf = (_buf), .size = (_size) }

/**
 * \def findmsg_INIT(_fd, _size)
 * initialize findmsg object with buffer residing on stack
 */
#define findmsg_INIT_ON_STACK(_fd, _size)  findmsg_INIT(_fd, ((char[_size]){0}), _size)

/**
 * Initialize findmsg object
 * @param t
 * @param fd
 * @param buf
 * @param size
 */
void findmsg_init(struct findmsg_s *t, int fd, char buf[], size_t size);

/**
 * Find message in receive buffer
 * @param t
 * @param conf specifies functions for message finding
 * @param arg argument passed to conf-> functions
 * @param Timeout
 * @param msg filled with pointer to message beginning
 * @return negative value on error, zero on timeout, positive value is the message length on valid message
 *         the pointer to the message may be obtained with findmsg_msgpng(t)
 */
ssize_t findmsg_findmsg(struct findmsg_s *t,
		const struct findmsg_conf_s *conf, void *arg,
		struct timespec *timeout);

/**
 * After a message has been successfully found (ie. findmsg_findmsg returned 1)
 * @param t
 * @return
 */
void *findmsg_msgpnt(struct findmsg_s *t);

/**
 * Prepare receive buffer to receive next message
 * This function must be called before next findmsg_findmsg function to receive next message
 * @param t
 * @param msg
 * @param size
 */
void findmsg_next(struct findmsg_s *t);

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
		struct timespec *timeout);

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
		struct timespec *timeout);

ssize_t findmsg_get(struct findmsg_s *t,
		const struct findmsg_conf_s *conf, void *arg,
		struct timespec *timeout,
		/*out*/ char buf[], size_t bufsize);

#endif /* SRC_findmsg_H_ */
