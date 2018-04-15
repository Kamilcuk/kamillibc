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

#endif /* SRC_LIB_FINDMSG_TYPES_H_ */
