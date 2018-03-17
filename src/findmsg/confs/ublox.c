/*
 * ublox.c
 *
 *  Created on: 17 mar 2018
 *      Author: kamil
 */

#include "findmsg/confs/ublox.h"

#include <assert.h>
#include <sys/cdefs.h>

const struct findmsg_conf_s findmsg_conf_ublox = {
		.minlength = findmsg_ublox_minlength,
		.maxlength = findmsg_ublox_maxlength,
		.checkBeginning = &findmsg_ublox_checkBeginning,
		.checkEnding = &findmsg_ublox_checkEnding,
};

__attribute__((__weak__))
bool findmsg_ublox_isValid(const char buf[], size_t len)
{
	return true;
}

ssize_t findmsg_ublox_checkBeginning(const char buf[], size_t size, void *arg)
{
	assert(size >= 6);
	return ( buf[0] == 0xB5 && buf[1] == 0x62 ) ? (buf[4]<<8 | buf[5]) : findmsg_MSG_TOO_SHORT;
}

int findmsg_ublox_checkEnding(const char buf[], size_t size, void *arg)
{
	assert(size >= 6);
	const unsigned int len = buf[4]<<8 | buf[5];
	if ( size < len ) return findmsg_MSG_TOO_SHORT;
	const bool ret = findmsg_ublox_isValid(buf, size);
	return ret == false ? findmsg_MSG_INVALID : findmsg_MSG_VALID;
}
