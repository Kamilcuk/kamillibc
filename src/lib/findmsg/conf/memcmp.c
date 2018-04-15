/*
 * memcmp.c
 *
 *  Created on: 17 mar 2018
 *      Author: kamil
 */
#include "../conf/memcmp.h"

#include <string.h>

ssize_t findmsg_conf_memcmp_checkBeginning(const char buf[], size_t bufsize, void *arg)
{
	const char * const carg = arg;
	return buf[0] == carg[0] ? findmsg_MSG_VALID : findmsg_MSG_TOO_SHORT;
}

int findmsg_conf_memcmp_checkEnding(const char buf[], size_t bufsize, void *arg)
{
	const char * const carg = arg;
	return !memcmp(buf, carg, bufsize) ? findmsg_MSG_INVALID : findmsg_MSG_VALID;
}
