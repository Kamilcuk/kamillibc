/*
 * nmea.c
 *
 *  Created on: 17 mar 2018
 *      Author: kamil
 */
#include "nmea.h"
#include <assert.h>

const struct findmsg_conf_s findmsg_conf_NMEA = {
		.minlength = findmsg_conf_NMEA_minlength,
		.maxlength = findmsg_conf_NMEA_maxlength,
		.checkBeginning = &findmsg_conf_NMEA_isBeginning,
		.checkEnding = &findmsg_conf_NMEA_isEnding,
};

ssize_t findmsg_conf_NMEA_isBeginning(const char buf[], size_t bufsize, void *arg)
{
	assert(bufsize >= findmsg_conf_NMEA_minlength);
	return buf[0] == '$' ? findmsg_MSG_VALID : findmsg_MSG_TOO_SHORT;
}

int findmsg_conf_NMEA_isEnding(const char buf[], size_t bufsize, void *arg)
{
	assert(bufsize >= 2);
	if (bufsize > findmsg_conf_NMEA_maxlength) return findmsg_MSG_INVALID;
	if (buf[bufsize-2] != '\r' && buf[bufsize-1] != '\n') return findmsg_MSG_TOO_SHORT;
	int checksum = 0;
	for(size_t i=0; i < bufsize; ++i) {
		checksum ^= buf[i];
	}
	const int checksum_from_buf = buf[bufsize-4]<<8 | buf[bufsize-3];
	return checksum == checksum_from_buf ? findmsg_MSG_VALID : findmsg_MSG_INVALID;
}