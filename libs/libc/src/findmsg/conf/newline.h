/*
 * newline.h
 *
 *  Created on: 17 mar 2018
 *      Author: kamil
 */

#ifndef SRC_findmsg_NEWLINE_H_
#define SRC_findmsg_NEWLINE_H_

#include <findmsg/findmsg.h>

extern const struct findmsg_conf_s findmsg_conf_newline;

int findmsg_conf_newline_checkEnding(const char buf[], size_t size, void *arg);

static inline
ssize_t findmsg_newline(struct findmsg_s *t, struct timespec *timeout)
{
	return findmsg_findmsg(t, &findmsg_conf_newline, NULL, timeout);
}

#endif /* SRC_findmsg_NEWLINE_H_ */
