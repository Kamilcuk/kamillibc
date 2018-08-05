/*
 * nmea.h
 *
 *  Created on: 17 mar 2018
 *      Author: kamil
 */

#ifndef SRC_FINDMSG_NMEA_H_
#define SRC_FINDMSG_NMEA_H_

#include <findmsg/findmsg.h>

enum {
	findmsg_conf_nmea_minlength = sizeof("$,99\r\n") - 1,
	findmsg_conf_nmea_maxlength = 82,
};

extern const struct findmsg_conf_s findmsg_conf_nmea;

ssize_t findmsg_conf_nmea_isBeginning(const char buf[], size_t bufsize, void *arg);

int findmsg_conf_nmea_isEnding(const char buf[], size_t bufsize, void *arg);

static inline
ssize_t findmsg_nmea(struct findmsg_s *t, struct timespec *timeout)
{
	return findmsg_findmsg(t, &findmsg_conf_nmea, NULL, timeout);
}

#endif /* SRC_FINDMSG_NMEA_H_ */
