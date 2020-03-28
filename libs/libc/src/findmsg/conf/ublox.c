/*
 * ublox.c
 *
 *  Created on: 17 mar 2018
 *      Author: kamil
 */

#include "../conf/ublox.h"

#include <assert.h>

const struct findmsg_conf_s findmsg_conf_ublox = {
		.minlength = findmsg_ublox_minlength,
		.maxlength = findmsg_ublox_maxlength,
		.checkBeginning = &findmsg_ublox_checkBeginning,
		.checkEnding = &findmsg_ublox_checkEnding,
};

unsigned int findmsg_ublox_crc(const char buf[], size_t len)
{
	assert(buf != NULL);
	assert(len >= 4);
	buf += 2;
	len -= 4;
	unsigned int crc_a = 0;
	unsigned int crc_b = 0;
	while (len--) {
		crc_a += *buf++;
		crc_b += crc_a;
	}
	crc_a &= 0xff;
	crc_b &= 0xff;
	return crc_a << 8 | crc_b;
}

ssize_t findmsg_ublox_checkBeginning(const char buf[], size_t size, void *arg)
{
	const unsigned char *bufu = (void*)buf;
	assert(size >= findmsg_ublox_minlength);
	if (!(bufu[0] == 0xB5 && bufu[1] == 0x62)) {
		return findmsg_BEG_NOT_MSG_BEGNNING;
	}
	const unsigned int len = (bufu[4] << 8 | bufu[5]);
	if (!(findmsg_ublox_minlength <= len && len <= findmsg_ublox_maxlength)) {
		return findmsg_BEG_NOT_MSG_BEGNNING;
	}
	return len;
}

int findmsg_ublox_checkEnding(const char buf[], size_t size, void *arg)
{
	const unsigned char *bufu = (void*)buf;
	assert(size >= 6);
	assert(size >= ((unsigned)bufu[4] << 8 | (unsigned)bufu[5]));
	// check crc
	if (((unsigned)bufu[size - 2] << 8 | (unsigned)bufu[size - 1]) != findmsg_ublox_crc(buf, size)) {
		return findmsg_END_MSG_INVALID;
	}
	return findmsg_END_MSG_VALID;
}
