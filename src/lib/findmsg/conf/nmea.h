/*
 * nmea.h
 *
 *  Created on: 17 mar 2018
 *      Author: kamil
 */

#ifndef SRC_findmsg_NMEA_H_
#define SRC_findmsg_NMEA_H_

#include <findmsg/types.h>

enum {
	findmsg_conf_NMEA_minlength  = (sizeof("$,99\r\n")-1),
	findmsg_conf_NMEA_maxlength = 82,
};

extern const struct findmsg_conf_s findmsg_conf_NMEA;

ssize_t findmsg_conf_NMEA_isBeginning(const char buf[], size_t bufsize, void *arg);

int findmsg_conf_NMEA_isEnding(const char buf[], size_t bufsize, void *arg);

#endif /* SRC_findmsg_NMEA_H_ */
