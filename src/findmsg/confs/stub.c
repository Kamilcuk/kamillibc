/*
 * stub.c
 *
 *  Created on: 17 mar 2018
 *      Author: kamil
 */

#include "stub.h"
#include <string.h>

const struct findmsg_conf_s findmsg_conf_stub = {
		.minlength = 0,
		.maxlength = SIZE_MAX,
		.checkBeginning = &findmsg_stub_checkBeginning,
		.checkEnding = &findmsg_stub_checkEnding,
};

ssize_t findmsg_stub_checkBeginning(const char buf[], size_t bufsize, void *arg)
{
	return findmsg_MSG_VALID;
}

int findmsg_stub_checkEnding(const char buf[], size_t bufsize, void *arg)
{
	return findmsg_MSG_VALID;
}
