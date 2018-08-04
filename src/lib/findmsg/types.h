/*
 * types.h
 *
 *  Created on: 15 kwi 2018
 *      Author: kamil
 */

#ifndef SRC_LIB_FINDMSG_TYPES_H_
#define SRC_LIB_FINDMSG_TYPES_H_

#include <stdlib.h>
#include <unistd.h>

/* Exported constants -------------------------------------------- */

enum findmsg_BEGINNING_e {
	findmsg_NOT_MSG_BEGNNING = 0,
	findmsg_MSG_BEGINNING    = 1,
};

enum findmsg_MSG_e {
	findmsg_MSG_TOO_SHORT  = 0,
	findmsg_MSG_VALID      = 1,
	findmsg_MSG_INVALID    = 2,
};

/* Exported types --------------------------------------------------- */

struct findmsg_s {
	// private:
	int fd;
	char * buf;
	size_t size;
	size_t pos;
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

#endif /* SRC_LIB_FINDMSG_TYPES_H_ */
