/*
 * ublox.h
 *
 *  Created on: 17 mar 2018
 *      Author: kamil
 */

#ifndef SRC_FINDMSG_CONFS_UBLOX_H_
#define SRC_FINDMSG_CONFS_UBLOX_H_

#include <findmsg/findmsg.h>

enum {
	findmsg_ublox_minlength = 8,
	findmsg_ublox_maxlength = 8 + 512,
};

extern const struct findmsg_conf_s findmsg_conf_ublox;

unsigned int findmsg_ublox_crc(const char buf[], size_t len);
ssize_t findmsg_ublox_checkBeginning(const char buf[], size_t size, void *arg);
int findmsg_ublox_checkEnding(const char buf[], size_t size, void *arg);

static inline
ssize_t findmsg_ublox(struct findmsg_s *t, struct timespec *timeout)
{
	return findmsg_findmsg(t, &findmsg_conf_ublox, NULL, timeout);
}

#endif /* SRC_FINDMSG_CONFS_UBLOX_H_ */
