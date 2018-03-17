/*
 * ublox.h
 *
 *  Created on: 17 mar 2018
 *      Author: kamil
 */

#ifndef SRC_FINDMSG_CONFS_UBLOX_H_
#define SRC_FINDMSG_CONFS_UBLOX_H_

#include <findmsg/findmsg.h>

#include <stdbool.h>

enum {
	findmsg_ublox_minlength = sizeof("ub######")-1,
	findmsg_ublox_maxlength = 512,
};

extern const struct findmsg_conf_s findmsg_conf_ublox;

ssize_t findmsg_ublox_checkBeginning(const char buf[], size_t size, void *arg);
int findmsg_ublox_checkEnding(const char buf[], size_t size, void *arg);

// isValid should be defined in different transaction unit
bool findmsg_ublox_isValid(const char buf[], size_t len);

#endif /* SRC_FINDMSG_CONFS_UBLOX_H_ */