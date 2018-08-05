/*
 * nmea.c
 *
 *  Created on: 17 mar 2018
 *      Author: kamil
 */
#include "nmea.h"

#include <assert.h>

const struct findmsg_conf_s findmsg_conf_nmea = {
		.minlength = findmsg_conf_nmea_minlength,
		.maxlength = findmsg_conf_nmea_maxlength,
		.checkBeginning = &findmsg_conf_nmea_isBeginning,
		.checkEnding = &findmsg_conf_nmea_isEnding,
};

ssize_t findmsg_conf_nmea_isBeginning(const char buf[], size_t bufsize, void *arg)
{
	assert(bufsize == findmsg_conf_nmea_minlength);
	return buf[0] == '$' ? findmsg_BEG_MSG_BEGINNING : findmsg_BEG_NOT_MSG_BEGNNING;
}

int findmsg_conf_nmea_isEnding(const char buf[], size_t bufsize, void *arg)
{
	assert(bufsize >= findmsg_conf_nmea_minlength);
	if (bufsize > findmsg_conf_nmea_maxlength) return findmsg_END_MSG_INVALID;
	if (buf[bufsize-2] != '\r' && buf[bufsize-1] != '\n') return findmsg_END_MSG_TOO_SHORT;
	int checksum = 0;
	for(size_t i=0; i < bufsize; ++i) {
		checksum ^= buf[i];
	}
	const int checksum_from_buf = buf[bufsize-4]<<8 | buf[bufsize-3];
	return checksum == checksum_from_buf ? findmsg_END_MSG_VALID : findmsg_END_MSG_INVALID;
}
